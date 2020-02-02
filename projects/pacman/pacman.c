/******************************************************************************/
/*             Author: Eli Umansky || Reviewer: Igal Bogopolsky               */
/******************************************************************************/
/*                 Exercise: Packet Manager || Date: 01.02.20                 */
/******************************************************************************/

#include <stdlib.h> /* malloc */
#include <string.h> /* memset */
#include <assert.h> /* assert */
#include <time.h> /* srand */

#include "pacman.h" /* pac_man_t */
#include "hash.h" /* HashCreate */

#define TWO_BYTES_MAX_VAL (65536)
#define LAN_TBL (pac_man->lan_table)
#define CONNEC_TBL (pac_man->connection_table)
#define NAT_IP (pac_man->nat_ip)
#define SUCCESS (0)
#define MALLOC_FAIL (-1)
#define CONVERSION_FAIL (-2)

struct pac_man
{
	hash_table_t *connection_table;
	hash_table_t *lan_table;
	ip_t nat_ip;
};

typedef struct connection_data
{
	socket_t socket_key;
	port_t port_key;
} connection_data_t;

typedef struct lan_data
{
	socket_t socket_key;
	ip_t saved_src_ip;
	port_t saved_src_port;
} lan_data_t;

static size_t HashFunc(const void *key);
static int MatchConnection(const void *data, const void *param);
static int MatchLan(const void *data, const void *param);
static int FreeSockets(void *data, void *param);
static lan_data_t *InitAndInsertLanData(pac_man_t *pac_man, socket_t *socket);
static connection_data_t *InitAndInsertConnData(pac_man_t *pac_man, lan_data_t *lan_data,
                                                socket_t *socket);

pac_man_t *PacManCreate(ip_t nat_ip)
{
	pac_man_t *pac_man = NULL;

	pac_man = (pac_man_t*)malloc(sizeof(pac_man_t));
	if (NULL == pac_man)
	{
		return NULL;
	}

	pac_man->nat_ip = nat_ip;
	pac_man->connection_table = HashCreate(TWO_BYTES_MAX_VAL, &MatchConnection, &HashFunc);
	if (NULL == pac_man->connection_table)
	{
		memset(pac_man, 0, sizeof(pac_man_t));
		free(pac_man);
		return NULL;
	}

	pac_man->lan_table = HashCreate(TWO_BYTES_MAX_VAL, &MatchLan, &HashFunc);
	if (NULL == pac_man->lan_table)
	{
		HashDestroy(pac_man->connection_table);
		memset(pac_man, 0, sizeof(pac_man_t));
		free(pac_man);
		return NULL;
	}

	return pac_man;
}

void PacManDestroy(pac_man_t *pac_man)
{
	assert (NULL != pac_man);

	HashForEach(CONNEC_TBL, &FreeSockets, NULL);
	HashForEach(LAN_TBL, &FreeSockets, NULL);

	HashDestroy(CONNEC_TBL);
	HashDestroy(LAN_TBL);

	memset(pac_man, 0, sizeof(pac_man_t));
	free(pac_man);
}

int PacManConvertOut(pac_man_t *pac_man, socket_t *socket)
{
	connection_data_t *conn_data = NULL;
	lan_data_t *lan_data = NULL;

	assert (NULL != pac_man);
	assert (NULL != socket);

	conn_data = HashFind(CONNEC_TBL, socket);
	if (NULL != conn_data)
	{
		socket->src_ip = NAT_IP;
		socket->src_port = conn_data->port_key;
		return SUCCESS;
	}

	lan_data = InitAndInsertLanData(pac_man, socket);
	if (NULL == lan_data)
	{
		return MALLOC_FAIL;
	}

    conn_data = InitAndInsertConnData(pac_man, lan_data, socket);
	if (NULL == conn_data)
	{
		return MALLOC_FAIL;
	}

	socket->src_ip = NAT_IP;
	socket->src_port = conn_data->port_key;

	return SUCCESS;
}
static lan_data_t *InitAndInsertLanData(pac_man_t *pac_man, socket_t *socket)
{
    lan_data_t *new_lan_data = (lan_data_t*)malloc(sizeof(lan_data_t));
	if (NULL == new_lan_data)
	{
		return NULL;
	}

	srand(time(NULL));

	new_lan_data->socket_key.dest_ip = NAT_IP;
	new_lan_data->socket_key.dest_port = rand() % TWO_BYTES_MAX_VAL;
	new_lan_data->socket_key.src_ip = socket->dest_ip;
	new_lan_data->socket_key.src_port = socket->dest_port;
	new_lan_data->saved_src_ip = socket->src_ip;
	new_lan_data->saved_src_port = socket->src_port;

	while (HashFind(LAN_TBL, new_lan_data))
	{
		new_lan_data->socket_key.dest_port = rand() % TWO_BYTES_MAX_VAL;
	}

	if (SUCCESS != HashInsert(LAN_TBL, new_lan_data))
    {
        free(new_lan_data);
        return NULL;
    }

    return new_lan_data;
}

static connection_data_t *InitAndInsertConnData(pac_man_t *pac_man, lan_data_t *lan_data,
                                                socket_t *socket)
{
    connection_data_t *new_conn_data = (connection_data_t*)malloc(sizeof(connection_data_t));
	if (NULL == new_conn_data)
	{
		free(lan_data);
		return NULL;
	}

	new_conn_data->socket_key.dest_ip = socket->dest_ip;
	new_conn_data->socket_key.dest_port = socket->dest_port;
	new_conn_data->socket_key.src_ip = socket->src_ip;
	new_conn_data->socket_key.src_port = socket->src_port;
	new_conn_data->port_key = lan_data->socket_key.dest_port;

	if (SUCCESS != HashInsert(CONNEC_TBL, new_conn_data))
    {
        free(lan_data);
        free(new_conn_data);
        return NULL;
    }

    return new_conn_data;
}

void PacManRelease(pac_man_t *pac_man, const socket_t *socket)
{
	connection_data_t *conn_to_remove = NULL;
	lan_data_t *lan_to_remove = NULL;
	socket_t search_lan = {0};

	assert (NULL != pac_man);
	assert (NULL != socket);

	conn_to_remove = HashFind(CONNEC_TBL, socket);
	if (NULL == conn_to_remove)
	{
		return;
	}

	search_lan.dest_ip = NAT_IP;
	search_lan.dest_port = conn_to_remove->port_key;
	search_lan.src_ip = conn_to_remove->socket_key.dest_ip;
	search_lan.src_port = conn_to_remove->socket_key.dest_port;

	lan_to_remove = HashFind(LAN_TBL, &search_lan);

	HashRemove(CONNEC_TBL, socket);
	HashRemove(LAN_TBL, &search_lan);

	free(conn_to_remove);
	free(lan_to_remove);
}

int PacManConvertIn(const pac_man_t *pac_man, socket_t *socket)
{
	lan_data_t *lan_data = NULL;

	assert (NULL != pac_man);
	assert (NULL != socket);

	lan_data = HashFind(LAN_TBL, socket);

	if (NULL == lan_data)
	{
		return CONVERSION_FAIL;
	}

	socket->dest_ip = lan_data->saved_src_ip;
	socket->dest_port = lan_data->saved_src_port;

	return SUCCESS;
}

static int MatchConnection(const void *data, const void *param)
{
	return
	(((socket_t*)data)->src_ip == ((connection_data_t*)param)->socket_key.src_ip) &&
	(((socket_t*)data)->dest_ip == ((connection_data_t*)param)->socket_key.dest_ip) &&
	(((socket_t*)data)->src_port == ((connection_data_t*)param)->socket_key.src_port) &&
	(((socket_t*)data)->dest_port == ((connection_data_t*)param)->socket_key.dest_port);
}

static int MatchLan(const void *data, const void *param)
{
	return
	(((socket_t*)data)->src_ip == ((lan_data_t*)param)->socket_key.src_ip) &&
	(((socket_t*)data)->dest_ip == ((lan_data_t*)param)->socket_key.dest_ip) &&
	(((socket_t*)data)->src_port == ((lan_data_t*)param)->socket_key.src_port) &&
	(((socket_t*)data)->dest_port == ((lan_data_t*)param)->socket_key.dest_port);
}

static size_t HashFunc(const void *key)
{
	return (((size_t)((socket_t*)key)->src_ip) * 59) ^
			(((size_t)((socket_t*)key)->dest_ip)) ^
			(((size_t)((socket_t*)key)->src_port) << 16) ^
			(((size_t)((socket_t*)key)->dest_port));
}

static int FreeSockets(void *data, void *param)
{
	(void)param;
	free(data);

	return 0;
}
