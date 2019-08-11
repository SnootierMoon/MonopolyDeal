#pragma once

#include <stdbool.h>

struct sockaddr_in;

void net_libinit();
void net_libfree();

void net_accept(int * fd, struct sockaddr_in * address);
bool net_has_connection();
bool net_has_input(int player);
void net_ignore_connection();
void net_print_ips();
void net_select();

