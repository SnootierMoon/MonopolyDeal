#include "command_structure.h"
#include "player/player.h"
#include "util/token.h"
#include <string.h>

const int COMMAND_FAILURE = -1;
const int COMMAND_NONE = 0;
const int COMMAND_SUCCESS = 1;
const char * current_command;
int current_player;
const char nullch = 0;
command * cmd_begin;

int cmdfn_invalid() {
    player_writeln(current_player, "Invalid Command");
    return COMMAND_FAILURE;
}

int eval(command * cmd, token_node tokens) {
    if (!token_size(tokens)) {
        if (cmd == cmd_begin) {
            return COMMAND_NONE;
        }
    }
    if (cmd->next_count) {
        for (int i = 0; i < cmd->next_count; i++) {
            if (!strcmp(tokens->data, cmd->nexts[i]->trigger)) {
                return eval(cmd->nexts[i], tokens->next);
            }
        }
    }
    if (!cmd->next_count) {
        if ((token_size(tokens) == cmd->param_count) || ((cmd->param_count < 0) && ((token_size(tokens)) >= -cmd->param_count - 1))) {
            return cmd->end(tokens);
        }
        player_fwriteln(current_player, "Usage: %s%s%s", cmd->usage, (cmd->usage_params == &nullch) ? &nullch: " ", cmd->usage_params);
        return COMMAND_FAILURE;
    }
    return cmdfn_invalid();
}

int command_run(int player, const char * command) {
    cmd_begin = &cmd_main;
    current_player = player;
    current_command = command;
    token_node tokens = tokenize(command);
    int result = eval(cmd_begin, tokens);
    token_free(tokens);
    return result;
}

