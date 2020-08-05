/*************************
     Packet Manager
      Header File
    Author : OL80
      Date : 30/01/2020
*************************/

#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include <stddef.h> /* size_t */

typedef unsigned int ip_t;
typedef unsigned short port_t;
typedef struct pac_man pac_man_t;

typedef struct socket
{
	ip_t dest_ip;
	port_t dest_port;
	ip_t src_ip;
	port_t src_port;
} socket_t;

/* - returns a pointre to a new packet manager ------------------------------ */
/* - lan_ip must be a valid internet address -------------------------------- */
/* - time complexity O(1) --------------------------------------------------- */
pac_man_t *PacManCreate(ip_t nat_ip);

/* - destroys an existing packet manager ------------------------------------ */
/* - pac_man must not be NULL ----------------------------------------------- */
/* - time complexity O(n) --------------------------------------------------- */
void PacManDestroy(pac_man_t *pac_man);

/* - receives a socket from LAN and tranlates it for WWW -------------------- */
/* - returns success (0) upon successful completion ------------------------- */
/* - returns a different return value if the operation fails ---------------- */
/* - pac_man must not be NULL ----------------------------------------------- */
/* - socket_t must not be NULL ---------------------------------------------- */
/* - time complexity O(1) --------------------------------------------------- */
int PacManConvertOut(pac_man_t *pac_man, socket_t *socket);

/* - recives a socket from WWW and translates it for LAN -------------------- */
/* - pac_man must not be NULL ----------------------------------------------- */
/* - socket_t must not be NULL ---------------------------------------------- */
/* - time complexity O(1) --------------------------------------------------- */
int PacManConvertIn(const pac_man_t *pac_man, socket_t *socket);

/* - removes a specific socket from being tracked --------------------------- */
/* - pac_man must not be NULL ----------------------------------------------- */
/* - socket_t must not be NULL ---------------------------------------------- */
/* - time complexity O(1) --------------------------------------------------- */
void PacManRelease(pac_man_t *pac_man, const socket_t *socket);

#endif /* PACKET_MANAGER_H */

/* To be added to the pacman.c file: 

struct pac_man
{
	hash_table_t *connection_table;
	hash_table_t *lan_table;
	ip_t nat_ip;
};

typedef struct connection_data
{
	socket_t socket_key; *//* note that it's NOT a pointer *//*
	port_t port_key;
} connection_data_t;

typedef struct lan_data
{
	socket_t socket_key; *//* note that it's NOT a pointer *//*
	ip_t saved_src_ip;
	port_t saved_src_port;
} lan_data_t;


Hash Function 1:

static size_t hash_func_t(void *key)
{
	return((((size_t)((socket_t*)key)->src_ip) * 59) ^
			((((size_t)((socket_t*)key)->dest_ip)) ^
			((((size_t)((socket_t*)key)->src_port) << 16) ^
			((((size_t)((socket_t*)key)->dest_port));
}

Hash Function 2:

static size_t HashSocket(void *key)
{
	port_t index = 0;

	index ^= (((socket_t *)key)->dest_ip >> 16);
	index ^= (((socket_t *)key)->dest_ip & 0xffff);
	index ^= (((socket_t *)key)->src_ip >> 16);
	index ^= (((socket_t *)key)->src_ip & 0xffff);
	index ^= ((socket_t *)key)->dest_port;
	index ^= ((socket_t *)key)->src_port;

	return index;
}



*/

