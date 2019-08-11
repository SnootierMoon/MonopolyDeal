#include "player/player.h"
#include "command_structure.h"
#include <stdbool.h>

#define STATUS_DESCRIPTION "This option determines whether the status of a command will be printed after it is run.\r\nThe status message is either \"(failed)\" or \"(succeeded)\"\nUseful for debugging, especially when a command does not print anything by itself.\nAlso looks cool.\n"
#define ANSI_DESCRIPTION   "This option determines whether ANSI escape sequences will be used to color output text.\r\nOn modern terminals, this option looks nice and is recommended.\nOn old terminals, color may not be supported, and weird symbols will appear instead.\n"

void toggle(bool * b) { *b = !*b; }
const char * bool_to_string(bool * b) { return *b ? "on" : "off"; }
void on(bool * b) { *b = 1; }
void off(bool * b) { *b = 0; }

int cmdfn_ui_ansi_off() {
    off(&players[current_player].ansi);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_ansi_on() {
    on(&players[current_player].ansi);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_ansi_toggle() {
    toggle(&players[current_player].ansi);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_ansi_show() {
    player_fwriteln(current_player, "Ansi: %s", bool_to_string(&players[current_player].ansi));
    return COMMAND_SUCCESS;
}

int cmdfn_ui_status_off() {
    off(&players[current_player].status_print);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_status_on() {
    on(&players[current_player].status_print);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_status_toggle() {
    toggle(&players[current_player].status_print);
    return COMMAND_SUCCESS;
}

int cmdfn_ui_status_show() {
    player_fwriteln(current_player, "Status: %s", bool_to_string(&players[current_player].status_print));
    return COMMAND_SUCCESS;
}

command cmd_ui_ansi_off = {
    .trigger = "off", .next_count = 0, .end = cmdfn_ui_ansi_off,
    .usage = "ui ansi off",
    .usage_params = &nullch,
    .help_brief = "Disables ANSI setting",
    .help_detailed = ANSI_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_ansi_on = {
    .trigger = "on", .next_count = 0, .end = cmdfn_ui_ansi_on,
    .usage = "ui ansi on",
    .usage_params = &nullch,
    .help_brief = "Enables ANSI setting",
    .help_detailed = ANSI_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_ansi_show = {
    .trigger = "show", .next_count = 0, .end = cmdfn_ui_ansi_show,
    .usage = "ui ansi show",
    .usage_params = &nullch,
    .help_brief = "Shows current ANSI setting",
    .help_detailed = ANSI_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_ansi_toggle = {
    .trigger = "toggle", .next_count = 0, .end = cmdfn_ui_ansi_toggle,
    .usage = "ui ansi toggle",
    .usage_params = &nullch,
    .help_brief = "Toggles ANSI setting",
    .help_detailed = ANSI_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_ansi = {
    .trigger = "ansi", .next_count = 4, .nexts = { &cmd_ui_ansi_off, &cmd_ui_ansi_on, &cmd_ui_ansi_show, &cmd_ui_ansi_toggle },
    .help_title = "UI ANSI Commands",
    .help_detailed = ANSI_DESCRIPTION
};

command cmd_ui_status_off = {
    .trigger = "off", .next_count = 0, .end = cmdfn_ui_status_off,
    .usage = "ui status off",
    .usage_params = &nullch,
    .help_brief = "Disables status printing setting",
    .help_detailed = STATUS_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_status_on = {
    .trigger = "on", .next_count = 0, .end = cmdfn_ui_status_on,
    .usage = "ui status on",
    .usage_params = &nullch,
    .help_brief = "Enables status printing setting",
    .help_detailed = STATUS_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_status_show = {
    .trigger = "show", .next_count = 0, .end = cmdfn_ui_status_show,
    .usage = "ui status show",
    .usage_params = &nullch,
    .help_brief = "Shows current status printing setting",
    .help_detailed = STATUS_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_status_toggle = {
    .trigger = "toggle", .next_count = 0, .end = cmdfn_ui_status_toggle,
    .usage = "ui status toggle",
    .usage_params = &nullch,
    .help_brief = "Toggles status printing setting",
    .help_detailed = STATUS_DESCRIPTION,
    .param_count = 0
};

command cmd_ui_status = {
    .trigger = "status", .next_count = 4, .nexts = { &cmd_ui_status_off, &cmd_ui_status_on, &cmd_ui_status_show, &cmd_ui_status_toggle },
    .help_title = "UI Status Commands",
    .help_detailed = STATUS_DESCRIPTION
};

command cmd_ui = {
    .trigger = "ui", .next_count = 2, .nexts = { &cmd_ui_ansi, &cmd_ui_status }
};

