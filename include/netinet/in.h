/*
Functions to convert multibyte value from host byte order to net byte 
order (big endian) and reverse
*/

#ifndef _NETINET_IN_H
#define _NETINET_IN_H

unsigned short htons (unsigned short c);
unsigned htonl (unsigned c);
unsigned short ntohs (unsigned short c);
unsigned ntohl (unsigned c);

#endif

