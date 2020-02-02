
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pacman.h"
#include "hash.h"

struct pac_man
{
	hash_table_t *connection_table;
	hash_table_t *lan_table;
	ip_t nat_ip;
};

void CreateDestroy();
void ConvertOut();
void ConvertIn();
void Release();

static void sizettest(const char* testName, size_t expected, size_t actual);

int main()
{
	CreateDestroy();
	ConvertOut();
	ConvertIn();
	Release();
	
	return 0;
}

void CreateDestroy()
{
	pac_man_t *pac_man = NULL;
	ip_t nat_ip = 0xfedcba98;

	puts("Create and destroy");
	puts("------------------");
	pac_man = PacManCreate(nat_ip);

	PacManDestroy(pac_man);
}

void ConvertOut()
{
	socket_t sockets[10] = {0};
	pac_man_t *nat = PacManCreate(0x80000000); /* 2147483648 */

	puts("Test ConvertOut");
	puts("---------------");

	sizettest("Test Size Lan Table", 0, HashSize(nat->lan_table));
	sizettest("Test Size Connection Table", 0, HashSize(nat->connection_table));

	sockets[0].dest_ip = 286331153; /*17.17.17.17*/
	sockets[0].dest_port = 8080;
	sockets[0].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[0].src_port = 4443;

	sockets[1].dest_ip = 286331153; /*17.17.17.17*/
	sockets[1].dest_port = 8080;
	sockets[1].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[1].src_port = 4443;

	sockets[2].dest_ip = 2147483648;
	sockets[2].src_ip = 286331153;
	sockets[2].src_port = 8080;

	PacManConvertOut(nat, sockets);

	sizettest("Test Size After Insert lan", 1, HashSize(nat->lan_table));
	sizettest("Test Size After Insert con", 1, HashSize(nat->connection_table));
	sizettest("Testing Dest IP after convert", 286331153, sockets[0].dest_ip);
	sizettest("Testing Dest Port after convert", 8080, sockets[0].dest_port);
	sizettest("Testing Src Ip after convert", 0x80000000, sockets[0].src_ip);
	printf("src port should not be 4443, src_port = %u\n\n", sockets[0].src_port);

	PacManDestroy(nat);
}

void ConvertIn()
{
	socket_t sockets[10] = {0};
	pac_man_t *nat = PacManCreate(0x80000000); /* 2147483648 */

	puts("Test ConvertIn");
	puts("---------------");

	sockets[0].dest_ip = 286331153; /*17.17.17.17*/
	sockets[0].dest_port = 8080;
	sockets[0].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[0].src_port = 4443;

	sockets[1].dest_ip = 286331153; /*17.17.17.17*/
	sockets[1].dest_port = 8080;
	sockets[1].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[1].src_port = 4443;

	sockets[2].dest_ip = 2147483648;
	sockets[2].src_ip = 286331153;
	sockets[2].src_port = 8080;

	PacManConvertOut(nat, sockets);
	sockets[2].dest_port = sockets[0].src_port;

	PacManConvertIn(nat, sockets + 2);
	sizettest("Testing Dest IP after convert", 3232235521, sockets[2].dest_ip);
	sizettest("Testing Dest Port after convert", 4443, sockets[2].dest_port);
	sizettest("Testing Src Ip after convert", 286331153, sockets[2].src_ip);
	sizettest("Testing Src Ip after convert\n", 8080, sockets[2].src_port);

	
	PacManDestroy(nat);
}

void Release()
{
	socket_t sockets[10] = {0};
	socket_t socket_to_remove = {0};
	pac_man_t *nat = PacManCreate(0x80000000); /* 2147483648 */

	puts("Test Release");
	puts("------------");

	sockets[0].dest_ip = 286331153; /*17.17.17.17*/
	sockets[0].dest_port = 8080;
	sockets[0].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[0].src_port = 4443;

	socket_to_remove.dest_ip = 286331153; /*17.17.17.17*/
	socket_to_remove.dest_port = 8080;
	socket_to_remove.src_ip = 3232235521; /* 192.168.0.1*/
	socket_to_remove.src_port = 4443;

	sockets[1].dest_ip = 286331153; /*17.17.17.17*/
	sockets[1].dest_port = 8080;
	sockets[1].src_ip = 3232235521; /* 192.168.0.1*/
	sockets[1].src_port = 4443;

	sockets[2].dest_ip = 2147483648;
	sockets[2].src_ip = 286331153;
	sockets[2].src_port = 8080;

	PacManConvertOut(nat, sockets);
	sockets[2].dest_port = sockets[0].src_port;

	PacManConvertIn(nat, sockets + 2);
	PacManConvertOut(nat, sockets + 1);	

	PacManRelease(nat, &socket_to_remove);

	sizettest("Test Size After Insert lan", 0, HashSize(nat->lan_table));
	sizettest("Test Size After Insert con", 0, HashSize(nat->connection_table));

	PacManDestroy(nat);
}

static void sizettest(const char* testName, size_t expected, size_t actual)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %lu actual: %lu\n", expected, actual);
    }
}
