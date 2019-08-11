#pragma once

#include <stdint.h>

typedef struct _message_data_struct message_data;

struct _message_data_struct {
    uint32_t access_bits;
    char * message;
    char * ansi_message;
};

message_data * messages;
unsigned message_count;

void message_libfree();
void message_libinit();

void message_playerdc(int player);

char * message_get(int player, unsigned index);

void message_writeln(uint32_t access_bits, const char * sender, const char * message);
void message_fwriteln(uint32_t access_bits, const char * sender, const char * format, ...);

