/*
 * Common declarations for CS421 virtual network.
 *
 * Copyright(c) 2016-2018 Jason Tang <jtang@umbc.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef XT_CS421NET_H
#define XT_CS421NET_H

#define CS421NET_IRQ 6
void cs421net_enable(void);
void cs421net_disable(void);
char *cs421net_get_data(size_t * const);

#endif
