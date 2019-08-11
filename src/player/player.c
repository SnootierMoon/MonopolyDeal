#include "command/command_structure.h"
#include "game/game.h"
#include "main.h"
#include "message.h"
#include "player.h"
#include "util/net.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

player_data players[player_count];

char * DEFAULT_USERNAME = "Choosing Username...";

const unsigned minimum_username_length = 3;
const unsigned maximum_username_length = 30;

void player_libinit() {
    struct sockaddr_in listening_address = players->address;
    for (unsigned i = 0; i < player_count; i++) {
        player_clear(i, false);
    }
    players->address = listening_address;
    players->in_fd = STDIN_FILENO;
    players->out_fd = STDOUT_FILENO;
    player_writeln(0, "Client emulator:");
    player_write_welcome(0);
}

void player_libfree() {
    for (unsigned i = 0; i < player_count; i++) {
        player_clear(i, true);
    }
}

void player_clear(int player, bool cleanup) {
    if (cleanup) {
        message_playerdc(player);
        game_playerdc(player);
        close(players[player].in_fd);
        close(players[player].out_fd);
        if (players[player].username != DEFAULT_USERNAME) {
            free(players[player].username);
        }
    }
    memset(players + player, 0, sizeof(struct _player_data_struct));
    players[player].out_fd = -1;
    players[player].in_fd = -1;
    players[player].username = DEFAULT_USERNAME;
}

const char * player_ip(int player) {
    return inet_ntoa(players[player].address.sin_addr);
}

int player_port(int player) {
    return ntohs(players[player].address.sin_port);
}

int player_findby_ip(const char * ip) {
    for (unsigned i = 0; i < player_count; i++) {
        if ((players[i].in_fd != -1) && !strcmp(ip, inet_ntoa(players[i].address.sin_addr))) {
            return i;
        }
    }
    return -1;
}

int player_findby_username(const char * username) {
    if (!strcmp(username, DEFAULT_USERNAME)) {
        return -1;
    }
    for (unsigned i = 0; i < player_count; i++) {
        if (!strcmp(players[i].username, username)) {
            return i;
        }
    }
    return -1;
}

void player_accept() {
    for (unsigned player = 1; player < player_count; player++) {
        if (players[player].in_fd == -1) {
            net_accept(&players[player].in_fd, &players[player].address);
            if (players[player].in_fd == -1) {
                return player_clear(player, true);
            }
            players[player].out_fd = players[player].in_fd;
            player_write_welcome(player);
            return;
        }
    }
}

void player_disconnect(int player) {
    message_fwriteln(-1, ".System", "%s left.", players[player].username);
    if (!player) {
        server_closed = true;
        return;
    }
    player_clear(player, true);
}

void player_handle_input(int player) {
    const char * input = players[player].input_buffer;
    for (unsigned i = 0; i < strlen(input); i++) {
        if (!isprint(input[i]) && !isblank(input[i])) {
            input = 0;
            break;
        }
    }
    if (players[player].username == DEFAULT_USERNAME) {
        if (!input || !player_set_username(player, input)) {
            player_write(player, "Username: ");
            return;
        }
    } else {
        if (players[player].etc) {
            players[player].etc = false;
        } else {
            int status = input ? command_run(player, input) : COMMAND_FAILURE;
            if (players[player].status_print) {
                if (status == COMMAND_FAILURE) {
                    player_writeln(player, "(failed)");
                }
                if (status == COMMAND_SUCCESS) {
                    player_writeln(player, "(successful)");
                }
            }
        }
    }
    if (!server_closed && !players[player].etc) {
        player_write(player, ">>> ");
    }
}

bool player_set_username(int player, const char * username) {
    bool alnum_ = true;
    for (unsigned i = 0; i < strlen(username); i++) {
        if (!isalnum(username[i]) && username[i] != '_') {
            alnum_ = false;
        }
    }
    if (!alnum_) {
        player_writeln(player, "Invalid username. Must be alphanumeric/underscore.");
        return false;
    }
    for (unsigned i = 0; i < player_count; i++) {
        if (!strcmp(username, players[i].username)) {
            player_writeln(player, "Invalid username. Already taken.");
            return false;
        }
    }
    if (strlen(username) < minimum_username_length) {
        player_writeln(player, "Invalid username. Too short.");
        return false;
    }
    if (strlen(username) > maximum_username_length) {
        player_writeln(player, "Invalid username. Too long.");
        return false;
    }
    players[player].username = strdup(username);
    player_fwriteln(player, "Successfully claimed username \"%s\".", username);
    player_writeln(player, "Type \"help\" if you don't know what you're doing.");
    message_fwriteln(-1, ".System", "%s joined.", username);
    return true;
}

void player_read(int player) {
    int valread = read(players[player].in_fd, players->input_buffer, INPUT_BUFFER_SIZE);
    if (valread <= 0) {
        player_disconnect(player);
        return;
    }
    players->input_buffer[valread] = 0;
    valread = strlen(players->input_buffer);
    if (!player) {
        for (int i = 0; i < valread; i++) {
            if (players->input_buffer[i] == '\r' || players->input_buffer[i] == '\n') {
                players->input_buffer[i] = 0;
                break;
            }
        }
        player_handle_input(player);
    }
    for (int i = 0; i < valread; ) {
        if ((players[player].input_buffer_position >= INPUT_BUFFER_SIZE) || (players->input_buffer[i] == '\r') || (players->input_buffer[i] == '\n')) {
            players[player].input_buffer[players[player].input_buffer_position] = 0;
            player_handle_input(player);
            memset(players[player].input_buffer, 0, INPUT_BUFFER_SIZE + 1);
            players[player].input_buffer_position = 0;
            while ((i < valread) && ((players->input_buffer[i] == '\r') || (players->input_buffer[i] == '\n'))) {
                i++;
            }
        } else if ((players->input_buffer[i] == 127) || (players->input_buffer[i] == 8)) {
            if (players[player].input_buffer_position > 0) {
                players[player].input_buffer_position--;
            }
            i++;
        } else {
            players[player].input_buffer[players[player].input_buffer_position] = players->input_buffer[i];
            players[player].input_buffer_position++;
            i++;
        }
    }
}

void player_write(int player, const char * message) {
    dprintf(players[player].out_fd, "%s", message);
}

void player_writeln(int player, const char * message) {
    dprintf(players[player].out_fd, "%s\r\n", message);
}

void player_fwrite(int player, const char * format, ...) {
    va_list args;
    va_start(args, format);
    vdprintf(players[player].out_fd, format, args);
    va_end(args);
}

void player_fwriteln(int player, const char * format, ...) {
    va_list args;
    va_start(args, format);
    vdprintf(players[player].out_fd, format, args);
    dprintf(players[player].out_fd, "\r\n");
    va_end(args);
}

void player_write_welcome(int player) {
    player_writeln(player, "\tMONOPOLY DEAL 2.0");
    player_writeln(player, "Copyeet (C) 2019, Iced Out Blob.");
    player_writeln(player, "Welcome to the Monopoly Deal Server!");
    player_writeln(player, "");
    player_write(player, "Username: ");
}

void player_write_alert(int player, const char * message) {
    if (players[player].username != DEFAULT_USERNAME) {
        players[player].etc = true;
        player_fwriteln(player, "");
        player_fwriteln(player, "%s", message);
        player_writeln(player, "Press ENTER to continue...");
    }
}

void player_write_alert_broadcast(int access_bits, const char * message) {
    for (unsigned i = 0; i < player_count; i++) {
        if ((1 << i) & access_bits) {
            player_fwriteln(i, "%s", message);
        }
    }
}

