/**
 * Defines userspace networking code.
 * Copyright(c) 2016-2018 Jason Tang <jtang@umbc.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "cs421net.h"

static int cs421net_socket = -1;

void cs421net_init(void)
{
	struct addrinfo hints, *result, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int ret = getaddrinfo("localhost", "4210", &hints, &result);
	if (ret) {
		fprintf(stderr, "Could not resolve localhost: %s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}

	for (p = result; p; p = p->ai_next) {
		cs421net_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (cs421net_socket < 0) {
			continue;
		}
		if (connect(cs421net_socket, p->ai_addr, p->ai_addrlen) >= 0) {
			break;
		}
		close(cs421net_socket);
		cs421net_socket = -1;
	}

	freeaddrinfo(result);
	if (cs421net_socket < 0) {
		fprintf(stderr, "Could not connect to server\n");
		exit(EXIT_FAILURE);
	}
}

bool cs421net_send(const void *buffer, size_t buffer_len)
{
	if (cs421net_socket < 0) {
		fprintf(stderr, "Not connected to server\n");
		return false;
	}

	ssize_t retval;
	do {
		retval = write(cs421net_socket, buffer, buffer_len);
	} while (retval < 0 && errno == EINTR);

	if (retval < 0) {
		perror("failed to send data");
		return false;
	}
	else if (retval != buffer_len) {
		fprintf(stderr, "unable to send %zu bytes (only sent %zu)\n", buffer_len, retval);
		return false;
	}
	sleep(1);
	return true;
}
