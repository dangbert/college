/**
 * Declares userspace networking code.
 * Copyright(c) 2016-2017 Jason Tang <jtang@umbc.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define CS421NET_PORT 4210

/**
 * Initializes the network connection to the server.
 *
 * If unable to connect to the server, then display an error message
 * and abort the program.
 *
 * You do not need to modify this function.
 */
void cs421net_init(void);

/**
 * Send a message to the server.
 *
 * You do not need to modify this function.
 *
 * @param[in] buffer buffer containing data to send
 * @param[in] buffer_len number of bytes to send
 *
 * @return true if data successfully sent, false if not
 */
bool cs421net_send(const void *buffer, size_t buffer_len);
