#define _GNU_SOURCE
#include "command_structure.h"
#include "player/player.h"
#include "util/token.h"
#include <string.h>

const int HELP_MODE_OFF = 0;
const int HELP_MODE_BRIEF = 1;
const int HELP_MODE_DETAILED = 2;

const char * how_to = " \r\n\
                       HOW TO USE THIS MONOPOLY DEAL SERVER - Probably Asked Questions (PAQ)\r\n\
                       1. What is the server?\r\n\
                       Essentially, it's a specialized chat room in which users\r\n\
                       can play Monopoly Deal together.\r\n\
                       2. How do I do things?\r\n\
                       Controls are provided as commands that users type in to execute.\r\n\
                       Commands include chatting, playing, and customizing the interface.\r\n\
                       The help command provides a description for following commands\r\n\
                       (type \"help\" in the prompt).\r\n\
                       3. Which commands should I use?\r\n\
                       Depends on what you want to do:\r\n\
                       The UI Commands customize the interface (colors, etc.)\r\n\
                       The Player Commands allow players to interact (listing / kicking players).\r\n\
                       The Chat Commands allow players to communicate.\r\n\
                       The Game Commands are for playing Monopoly Deal.\r\n\
                       Typing \"help\" followed by a command name gives a detailed\r\n\
                       description of a command, if availabe.\r\n\
                       4. What commands do I use to start a game?\r\n\
                       The host starts the game using \"game start\".\r\n\
                       After a game is started, other players join using \"game join\".\r\n\
                       The host can check who joined using \"player list\"\r\n\
                       (joined players are marked with \"in game\".\r\n\
                        When the host is ready to start the game, he uses \"game join\" to start playing.\r\n";

int eval_help(command * cmd, token_node tokens, int help_mode) {
    if (!token_size(tokens)) {
        if (cmd->next_count) {
            if (cmd->help_title) {
                player_fwriteln(current_player, "%s", cmd->help_title);
            }
            for (int i = 0; i < cmd->next_count; i++) {
                eval_help(cmd->nexts[i], 0, HELP_MODE_BRIEF);
            }
        } else {
            if (cmd->help_brief) {
                player_fwriteln(current_player, "%s %s%s%s - %s", cmd->help_detailed ? "(+)" : "   ", cmd->usage, (cmd->usage_params == &nullch) ? &nullch: " ", cmd->usage_params, cmd->help_brief);
            }
        }
        if ((help_mode == HELP_MODE_DETAILED) && cmd->help_detailed) {
            player_writeln(current_player, "Details:");
            player_fwrite(current_player, "%s", cmd->help_detailed);
        }
        return COMMAND_SUCCESS;
    }
    if (cmd->next_count) {
        for (int i = 0; i < cmd->next_count; i++) {
            if (!strcmp(tokens->data, cmd->nexts[i]->trigger)) {
                return eval_help(cmd->nexts[i], tokens->next, help_mode);
            }
        }
    }
    return cmdfn_invalid();
}

int cmdfn_main_help(token_node parameters) {
    int ret = eval_help(cmd_begin, parameters, HELP_MODE_DETAILED);
    return ret;
}

int cmdfn_main_howto() {
    player_fwrite(current_player, "%s", how_to);
    return COMMAND_SUCCESS;
}

int cmdfn_main_quit() {
    player_disconnect(current_player);
    return COMMAND_SUCCESS;
}

command cmd_main_help = {
    .trigger = "help", .next_count = 0, .end = cmdfn_main_help,
    .usage = "help",
    .usage_params = "[command]",
    .help_brief = "Explains [command]'s usage in detail if marked with \"(+)\", or, if no command is provided, briefly explains every command's usage",
    .param_count = -1
};

command cmd_main_howto = {
    .trigger = "howto", .next_count = 0, .end = cmdfn_main_help,
    .usage = "howto",
    .usage_params = "",
    .help_brief = "Provides an informative guide on how to use this server",
    .param_count = -1
};

command cmd_main_quit = {
    .trigger = "quit", .next_count = 0, .end = cmdfn_main_quit,
    .usage = "quit",
    .usage_params = &nullch,
    .help_brief = "Disconnects from server as a client, or closes server as a host",
    .param_count = 0
};

command cmd_main = {
    .trigger = 0, .next_count = 6, .nexts = { &cmd_main_help, &cmd_main_quit, &cmd_chat, &cmd_game, &cmd_player, &cmd_ui },
    .help_title = "Main Commands"
};
