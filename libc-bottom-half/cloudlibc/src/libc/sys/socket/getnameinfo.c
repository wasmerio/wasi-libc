#include <netdb.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
// #include "lookup.h"

int sprintf(char *__restrict, const char *__restrict, ...);

#define PTR_MAX (64 + sizeof ".in-addr.arpa")
#define RR_PTR 12

static char *itoa(char *p, unsigned x) {
	p += 3*sizeof(int);
	*--p = 0;
	do {
		*--p = '0' + x % 10;
		x /= 10;
	} while (x);
	return p;
}

static void mkptr4(char *s, const unsigned char *ip)
{
	sprintf(s, "%d.%d.%d.%d.in-addr.arpa",
		ip[3], ip[2], ip[1], ip[0]);
}

static void mkptr6(char *s, const unsigned char *ip)
{
	static const char xdigits[] = "0123456789abcdef";
	int i;
	for (i=15; i>=0; i--) {
		*s++ = xdigits[ip[i]&15]; *s++ = '.';
		*s++ = xdigits[ip[i]>>4]; *s++ = '.';
	}
	strcpy(s, "ip6.arpa");
}

int getnameinfo(const struct sockaddr *restrict sa, socklen_t sl,
	char *restrict node, socklen_t nodelen,
	char *restrict serv, socklen_t servlen,
	int flags)
{
	char ptr[PTR_MAX];
	char buf[256], num[3*sizeof(int)+1];
	int af = sa->sa_family;
	unsigned char *a;
	unsigned scopeid;

	switch (af) {
	case AF_INET:
		a = (void *)&((struct sockaddr_in *)sa)->sin_addr;
		if (sl < sizeof(struct sockaddr_in)) return EAI_FAMILY;
		mkptr4(ptr, a);
		scopeid = 0;
		break;
	case AF_INET6:
		a = (void *)&((struct sockaddr_in6 *)sa)->sin6_addr;
		if (sl < sizeof(struct sockaddr_in6)) return EAI_FAMILY;
		if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12))
			mkptr6(ptr, a);
		else
			mkptr4(ptr, a+12);
		scopeid = ((struct sockaddr_in6 *)sa)->sin6_scope_id;
		break;
	default:
		return EAI_FAMILY;
	}

	if (node && nodelen) {
		buf[0] = 0;
		return EAI_NONAME;
	}

	if (serv && servlen) {
		return EAI_OVERFLOW;
	}

	return 0;
}
