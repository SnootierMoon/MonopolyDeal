#pragma once

#include <netinet/in.h>
#include <stdbool.h>

typedef struct _player_data_struct player_data;

#define INPUT_BUFFER_SIZE (2048)
#define player_count (32)

struct _player_data_struct {
    int in_fd;
    int out_fd;

    char input_buffer[INPUT_BUFFER_SIZE + 1];
    unsigned input_buffer_position;

    struct sockaddr_in address;
    char * username;
    bool etc;
    bool ansi;
    bool status_print;
    bool playing;
    int owe;
};

extern player_data players[player_count];

void player_libinit();
void player_libfree();

void player_clear(int player, bool cleanup);

const char * player_ip(int player);
int player_port(int player);

int player_findby_ip(const char * ip);
int player_findby_username(const char * username);

void player_accept();
void player_disconnect(int player);
void player_handle_input(int player);
bool player_set_username(int player, const char * username);

void player_read(int player);

void player_write(int player, const char * message);
void player_writeln(int player, const char * message);
void player_fwrite(int player, const char * format, ...);
void player_fwriteln(int player, const char * format, ...);
void player_write_welcome(int player);
void player_write_alert(int player, const char * message);
void player_write_alert_broadcast(int access_bits, const char * message);

