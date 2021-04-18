#ifndef NET
#define NET

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <ifaddrs.h>

void client(int argc, char* argv[]);
void server(int argc, char* argv[]);

#endif