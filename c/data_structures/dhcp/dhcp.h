/*************************
          DHCP
      Header File
    Author : OL80
      Date : 27/01/2020
*************************/

#ifndef DHCP_H
#define DHCP_H

#include <stddef.h> /* size_t */

typedef struct dhcp dhcp_t;
typedef unsigned int ip_t;

/* - returns a pointer to a new dhcp ---------------------------------------- */ 
/* - net_bits is the number of bits reserved for the net -------------------- */
/* - net_bits must be less then 32 ------------------------------------------ */
/* - network_ip is the prefix to be applied to all ips created -------------- */
/* - time complexity O(log-n) ----------------------------------------------- */
dhcp_t *DHCPCreate(size_t net_bits, ip_t network_ip);

/* - destroys and existing dhcp --------------------------------------------- */
/* - dhcp must not be NULL -------------------------------------------------- */
/* - time complexity O(n) --------------------------------------------------- */
void DHCPDestroy(dhcp_t *dhcp);

/* - returns the requested ip if free and locks it into the dhcp ------------ */
/* - if the ip is taken, a different free ip is returned -------------------- */
/* - if the ip is 0 the returned ip will be the lowest available ------------ */
/* - if the dhcp is full, 0 is returned ------------------------------------- */
/* - dhcp must not be NULL -------------------------------------------------- */
/* - time complexity 0(log-n) ----------------------------------------------- */
ip_t DHCPAlloc(dhcp_t *dhcp, ip_t ip_to_alloc);

/* - frees an ip from the dhcp ---------------------------------------------- */
/* - ip 0 and ip MAX_HOST can not be freed ---------------------------------- */
/* - dhcp must not be NULL -------------------------------------------------- */
/* - time complexity 0(log-n) ----------------------------------------------- */
void DHCPFree(dhcp_t *dhcp, ip_t ip_to_free);

/* - returns the number of free addresses ----------------------------------- */
/* - dhcp must not be NULL -------------------------------------------------- */
/* - time complexity 0(n) --------------------------------------------------- */
size_t DHCPFreeSize(const dhcp_t *dhcp);

/* - converts an ip to str format (XXX.XXX.XXX.XXX) ------------------------- */
/* - dest must not be NULL and capable of holding 16 chars ------------------ */
/* - time complexity O(1) --------------------------------------------------- */
char *DHCPIpToStr(char *dest, ip_t ip);

/* - converts an str formatted ip into ip_t --------------------------------- */
/* - src must not be NULL use the following format: "0-255.0-255.0-255.0-255" */
/* - time complexity O(1) --------------------------------------------------- */
ip_t DHCPStrToIp(const char *src);

/* - returns 1 if ip is used and 0 if the ip is free ------------------------ */
/* - dhcp must not be NULL -------------------------------------------------- */
/* - time complexity O(log-n) ----------------------------------------------- */
int DHCPIsIpUsed(const dhcp_t *dhcp, ip_t ip);

#endif /* DHCP_H */


/* to be placed in dhcp.c:

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
*/
