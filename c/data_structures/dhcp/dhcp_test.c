
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dhcp.h"

#define MIN_HOST (0x00000000)
#define MAX_HOST (0xffffffff)

enum num_of_children { LEFT, RIGHT, NUM_OF_CHILDREN };

typedef struct trie_node trie_node_t;

struct trie_node 
{
	int is_used;
	trie_node_t *child[NUM_OF_CHILDREN];
};

struct dhcp
{
	trie_node_t *root;
	size_t max_depth;
	ip_t network_ip;
};

void TestIpToStr();
void TestStrToIp();
void CreateDestroy();
void FreeSize();
void Alloc();
void Free();

static void test(int expected, int actual, const char* testName);
static void unstest(ip_t expected, ip_t actual, const char* testName);

int main()
{
	TestIpToStr();	
	TestStrToIp();
	CreateDestroy();
	FreeSize();
	Alloc();
	Free();

	return 0;
}

void TestIpToStr()
{
	char str[40];
	char *teststr = "255.255.255.2";
	ip_t ip = 0xffffff02;

	puts("Conversion tests");
	puts("----------------");

	DHCPIpToStr(str, ip);

	test(0, strcmp(str, teststr), "Checking if conversion's correct - Ip to str");
}

void TestStrToIp()
{
	char str[30] = "000.000.002.1";
    ip_t ret = DHCPStrToIp(str);

	unstest(513, ret, "Checking if conversion's correct - str to Ip\n");
}

void CreateDestroy()
{
	dhcp_t *dhcp = NULL;
	ip_t net_bits = 0xffffff02;

	puts("Create and destroy - no ip allocated");
	puts("------------------------------------");
	dhcp = DHCPCreate(28, net_bits);
	unstest(0xffffff00, dhcp->network_ip, "Checking network_ip\n");

	DHCPDestroy(dhcp);
}

void FreeSize()
{
	dhcp_t *dhcp = NULL;
	ip_t ip = 0xffffff02;

	puts("Free size");
	puts("---------");

	dhcp = DHCPCreate(28, ip);
	test(14, DHCPFreeSize(dhcp), "Checking initial free size - 14");

	DHCPAlloc(dhcp, 6);
	DHCPAlloc(dhcp, 1);
	DHCPAlloc(dhcp, 3);

	test(11, DHCPFreeSize(dhcp), "Checking free size after 3 allocs - 11\n");

	DHCPDestroy(dhcp);
}

void Alloc()
{
	dhcp_t *dhcp = NULL;
	ip_t ip = 0xffffff02;

	puts("Allocation tests");
	puts("----------------");

	dhcp = DHCPCreate(29, ip);

	unstest(0xffffff01, DHCPAlloc(dhcp, 1), "Simple allocation");
	unstest(0xffffff02, DHCPAlloc(dhcp, 0xffffff02), "Simple allocation");
	unstest(0xffffff03, DHCPAlloc(dhcp, 2), "Trying to allocate used");
	unstest(0xffffff04, DHCPAlloc(dhcp, MIN_HOST), "Trying to allocate min_host");
	unstest(0xffffff06, DHCPAlloc(dhcp, MAX_HOST), "Trying to allocate min_host");
	unstest(0xffffff05, DHCPAlloc(dhcp, 0xabcdef03), "Trying to allocate weird number");
	unstest(0, DHCPAlloc(dhcp, 4), "Trying to allocate when 0 free space\n");

	DHCPDestroy(dhcp);
}

void Free()
{
	dhcp_t *dhcp = NULL;
	ip_t ip = 0xffffff02;
	size_t i = 0;

	puts("Free tests");
	puts("----------");

	dhcp = DHCPCreate(29, ip);

	for (; i < 5; ++i)
	{
		DHCPAlloc(dhcp, i + 1);
	}

	test(1, DHCPFreeSize(dhcp), "Free size after 5 allocs - 1");

	DHCPFree(dhcp, 1);
	test(2, DHCPFreeSize(dhcp), "Free size after free - 2");
	DHCPFree(dhcp, 0xfffffffe);
	test(2, DHCPFreeSize(dhcp), "Free size after trying to free non-existing ip - 2");
	DHCPDestroy(dhcp);
}
static void test(int expected, int actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %d actual: %d\n", expected, actual);
    }
}

static void unstest(ip_t expected, ip_t actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %u actual: %u\n", expected, actual);
    }
}
