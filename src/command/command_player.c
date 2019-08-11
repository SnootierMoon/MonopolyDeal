#include "command_structure.h"
#include "message.h"
#include "player/player.h"
#include "util/token.h"

int cmdfn_player_id_kick(int target) {
    if (target == -1) {
        player_writeln(current_player, "That user does not exist.");
        return COMMAND_FAILURE;
    }
    if (current_player) {
        player_writeln(current_player, "Ask the host to kick that user!");
        return COMMAND_FAILURE;
    }
    message_fwriteln(-1, ".System", "%s was kicked.", players[current_player].username);
    player_disconnect(target);
    return COMMAND_SUCCESS;
}

int cmdfn_player_ip_kick(token_node remaining) {
    return cmdfn_player_id_kick(player_findby_ip(remaining->data));
}

int cmdfn_player_ip_list() {
    int count = 0;
    for (unsigned i = 0; i < player_count; i++) {
        if (players[i].out_fd != -1) {
            player_fwriteln(current_player, "%i. %s, %s:%i", ++count, players[i].username, player_ip(i), player_port(i));
        }
    }
    player_fwriteln(current_player, "Total of %i connections (out of %i)", count, player_count);
    return COMMAND_SUCCESS;
}

int cmdfn_player_kick(token_node remaining) {
    return cmdfn_player_id_kick(player_findby_username(remaining->data));
}

int cmdfn_player_list() {
    int count = 0;
    for (unsigned i = 0; i < player_count; i++) {
        if (players[i].out_fd != -1) {
            player_fwriteln(current_player, "%i. %s%s", ++count, players[i].username, players[i].playing ? " (in game)" : "");
        }
    }
    player_fwriteln(current_player, "Total of %i connections (out of %i)", count, player_count);
    return COMMAND_SUCCESS;
}


command cmd_player_ip_kick = {
    .trigger = "kick", .next_count = 0, .end = cmdfn_player_ip_kick,
    .usage = "player ip kick",
    .usage_params = "[ip]",
    .help_brief = "Kicks the player with the provided IPv4 address",
    .param_count = 1
};

command cmd_player_ip_list = {
    .trigger = "list", .next_count = 0, .end = cmdfn_player_ip_list,
    .usage = "player ip list",
    .usage_params = &nullch,
    .help_brief = "Lists all usernames and IPv4 addresses of all connected players",
    .param_count = 0
};

command cmd_player_ip = {
    .trigger = "ip", .next_count = 2, .nexts = { &cmd_player_ip_kick, &cmd_player_ip_list }
};

command cmd_player_kick = {
    .trigger = "kick", .next_count = 0, .end = cmdfn_player_kick,
    .usage = "player kick",
    .usage_params = "[username]",
    .help_brief = "Kicks the player with the provided username",
    .param_count = 1
};

command cmd_player_list = {
    .trigger = "list", .next_count = 0, .end = cmdfn_player_list,
    .usage = "player list",
    .usage_params = &nullch,
    .help_brief = "Lists usernames of all connected players",
    .param_count = 0
};

command cmd_player = {
    .trigger = "player", .next_count = 3, .nexts = { &cmd_player_ip, &cmd_player_kick, &cmd_player_list },
    .help_title = "Player Commands"
};

