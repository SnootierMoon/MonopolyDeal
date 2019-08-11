#pragma once

typedef struct _token_node_struct * token_node;
typedef struct _command_object_struct command;

struct _command_object_struct {
    const char * trigger;
    int next_count;
    union {
        struct {
            command * nexts[10];
            const char * help_title;
        };
        struct {
            int (*end)(token_node);
            const char * usage;
            const char * usage_params;
            int param_count;
            const char * help_brief;
        };
    };
    const char * help_detailed;
};

int command_run(int player, const char * command);
int cmdfn_invalid();

extern const char nullch;

extern const int COMMAND_FAILURE;
extern const int COMMAND_NONE;
extern const int COMMAND_SUCCESS;

extern int current_player;
extern const char * current_command;

extern command * cmd_begin;
extern command cmd_main;
extern command cmd_chat;
extern command cmd_game;
extern command cmd_player;
extern command cmd_ui;

