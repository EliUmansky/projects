#include <stdlib.h>
#include <stdio.h>

#include "pacman.h"
#include "tests.h"

void Test_Create_Out();

int main()
{
	Test_Create_Out();

	return 0;
}

void Test_Create_Out()
{
	socket_t sockets[10] = {0};
	socket_t socket_to_remove = {0};
	pac_man_t *nat = PacManCreate(0x80000000); /* 2147483648 */
	TestSizeT("Test Size Lan Table", 0, LanSize(nat));
	TestSizeT("Test Size Connection Table", 0, ConnectionSize(nat));

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

	puts("Test ConvertOut");
	
	PacManConvertOut(nat, sockets);

	TestSizeT("Test Size After Insert lan", 1, LanSize(nat));
	TestSizeT("Test Size After Insert con", 1, ConnectionSize(nat));
	TestSizeT("Testing Dest IP after convert", 286331153, sockets[0].dest_ip);
	TestSizeT("Testing Dest Port after convert", 8080, sockets[0].dest_port);
	TestSizeT("Testing Src Ip after convert", 0x80000000, sockets[0].src_ip);
	printf("src port should not be 4443, src_port = %u\n", sockets[0].src_port);

	sockets[2].dest_port = sockets[0].src_port;

	puts("/*****************************************/");

	PacManConvertIn(nat, sockets + 2);
	TestSizeT("Testing Dest IP after convert", 3232235521, sockets[2].dest_ip);
	TestSizeT("Testing Dest Port after convert", 4443, sockets[2].dest_port);
	TestSizeT("Testing Src Ip after convert", 286331153, sockets[2].src_ip);
	TestSizeT("Testing Src Ip after convert", 8080, sockets[2].src_port);

	puts("/*****************************************/");	

	PacManConvertOut(nat, sockets + 1);
	TestSizeT("Test Size After Insert lan", 1, LanSize(nat));
	TestSizeT("Test Size After Insert con", 1, ConnectionSize(nat));
	TestSizeT("Testing Dest IP after convert", 286331153, sockets[1].dest_ip);
	TestSizeT("Testing Dest Port after convert", 8080, sockets[1].dest_port);
	TestSizeT("Testing Src Ip after convert", 0x80000000, sockets[1].src_ip);
	printf("src port should not be 4443, src_port = %u\n", sockets[1].src_port);
	
	PacManRelease(nat, &socket_to_remove);

	TestSizeT("Test Size After Insert lan", 0, LanSize(nat));
	TestSizeT("Test Size After Insert con", 0, ConnectionSize(nat));

	PacManDestroy(nat);
}




