#include "net.h"
#include "player/player.h"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
//#include <linux/wireless.h>
#include <net/if.h>

int listener;
const int listener_options = 1;
int port = 54321;
fd_set set;
socklen_t sockaddr_in_length = sizeof(struct sockaddr_in);

void net_libinit() {
    players->address.sin_family = AF_INET;
    players->address.sin_addr.s_addr = INADDR_ANY;
    players->address.sin_port = htons(port);
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char *)&listener_options,  sizeof(listener_options)) == -1) {
        perror("setsockopt()");
        exit(EXIT_FAILURE);
    }
    if (bind(listener, (struct sockaddr *)&players->address, sockaddr_in_length) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
    if (listen(listener, 3) == -1) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }
    if (getsockname(listener, (struct sockaddr *)&players->address, &sockaddr_in_length) == -1) {
        perror("getsockname()");
        exit(EXIT_FAILURE);
    }
    net_print_ips();
    printf("Listening on port %d...\r\n", ntohs(players->address.sin_port));
    printf("To connect from this device, type \"netcat localhost %d\" in a new terminal.\r\n", ntohs(players->address.sin_port));
}

void net_libfree() {
    close(listener);
}

void net_accept(int * fd, struct sockaddr_in * address) {
    if ((*fd = accept(listener, (struct sockaddr *)address, &sockaddr_in_length)) == -1) {
        perror("accept()");
    }
}

bool net_has_connection() {
    return FD_ISSET(listener, &set);
}

bool net_has_input(int player) {
    return (players[player].in_fd != -1) && FD_ISSET(players[player].in_fd, &set);
}

void net_ignore_connection() {
    struct sockaddr_in address;
    int pooper;
    net_accept(&pooper, &address);
}

void net_print_ips() {
    struct ifaddrs * addrs;
    if (getifaddrs(&addrs) == -1) {
        perror("getifaddrs()");
    }
    printf("Interfaces   Addresses\r\n");
    for (struct ifaddrs * tmp = addrs; tmp; tmp = tmp->ifa_next) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            printf("%-13s%s\n", tmp->ifa_name, inet_ntoa(((struct sockaddr_in *)tmp->ifa_addr)->sin_addr));
        }
    }
    freeifaddrs(addrs);
}

void net_select() {
    int fd = listener, max_fd = fd;
    FD_ZERO(&set);
    FD_SET(fd, &set);
    for (unsigned i = 0; i < player_count; i++) {
        if ((fd = players[i].in_fd) == -1) {
            continue;
        }
        if (max_fd < fd) {
            max_fd = fd;
        }
        FD_SET(fd, &set);
    }
    if (select(max_fd + 1, &set, 0, 0, 0) == -1) {
        perror("select()");
        exit(EXIT_FAILURE);
    }
}

