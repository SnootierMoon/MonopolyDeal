#define _GNU_SOURCE
#include "command/command_structure.h"
#include "message.h"
#include "player/player.h"
#include "util/token.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void message_libinit() {
    messages = 0;
    message_count = 0;
    message_fwriteln(-1, ".System", "Server starting...");
}

void message_libfree() {
    for (unsigned i = 0; i < message_count; i++) {
        free(messages[i].message);
    }
    free(messages);
}

void message_playerdc(int player) {
    for (unsigned i = 0; i < message_count; i++) {
        messages[i].access_bits &= (~(1 << player));
    }
}

char * message_get(int player, unsigned index) {
    if (messages[index].access_bits & (1 << player)) {
        return messages[index].message;
    }
    return 0;
}

void message_writeln(uint32_t access_bits, const char * sender, const char * message) {
    message_count++;
    messages = realloc(messages, sizeof(message_data) * message_count);
    asprintf(&messages[message_count - 1].message, "%s: %s\r\n", sender, message);
    messages[message_count - 1].access_bits = access_bits;
}

void message_fwriteln(uint32_t access_bits, const char * sender, const char * format, ...) {
    char * message;
    va_list args;
    va_start(args, format);
    vasprintf(&message, format, args);
    va_end(args);
    message_writeln(access_bits, sender, message);
    free(message);
}

int cmdfn_chat_list() {
    int count = 0;
    for (unsigned i = 0; i < message_count; i++) {
        char * message = message_get(current_player, i);
        if (message) {
            player_fwrite(current_player, "%s", message);
            count++;
        }
    }
    if (!count) {
        player_writeln(current_player, "No messages.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_chat_private(token_node parameters) {
    int target = player_findby_username(parameters->data);
    if (target == -1) {
        player_writeln(current_player, "That user does not exist.");
        return COMMAND_FAILURE;
    }
    char * sender;
    asprintf(&sender, "%s (to %s)", players[current_player].username, players[target].username);
    message_fwriteln((1 << current_player) | (1 << target), sender, "%s", current_command + parameters->next->pos);
    free(sender);
    return COMMAND_SUCCESS;
}

int cmdfn_chat_say(token_node parameters) {
    message_fwriteln(-1, players[current_player].username, "%s", current_command + parameters->pos);
    return COMMAND_SUCCESS;
}

command cmd_chat_list = {
    .trigger = "list", .next_count = 0, .end = cmdfn_chat_list,
    .usage = "chat list",
    .usage_params = &nullch,
    .help_brief = "Lists all messages",
    .param_count = 0
};

command cmd_chat_private = {
    .trigger = "private", .next_count = 0, .end = cmdfn_chat_private,
    .usage = "chat private",
    .usage_params = "[player] [message]",
    .help_brief = "Sends a [message] to only [player]",
    .param_count = -3
};

command cmd_chat_say = {
    .trigger = "say", .next_count = 0, .end = cmdfn_chat_say,
    .usage = "chat say",
    .usage_params = "[message]",
    .help_brief = "Sends [message] to all players",
    .param_count = -2
};

command cmd_chat = {
    .trigger = "chat", .next_count = 3, .nexts = { &cmd_chat_list, &cmd_chat_private, &cmd_chat_say },
    .help_title = "Chat Commands"
};

