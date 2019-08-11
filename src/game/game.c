#include "card.h"
#include "util/token.h"
#include "command/command_structure.h"
#include "game.h"
#include "message.h"
#include "player/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *
 *  THIS FILE IS IN AN EARLY DEVELOPMENT STAGE
 *  THE PURPOSE OF THIS FILE IS TO GET A WORKING GAME FUNCTIONALLY RUNNING
 *  THIS FILE WILL BE COMPLETELY REFACTORED LATER
 *
 */

const unsigned INVENTORY_DECK = 0;
const unsigned INVENTORY_SHOP = 1;
const unsigned INVENTORY_HAND = 0;
const unsigned INVENTORY_PROPERTIES = 1;
const unsigned INVENTORY_BANK = 2;


#define card_count (108)
struct {
    unsigned id;
    union {
        unsigned full_location;
        struct {
            unsigned with_player : 1;
            unsigned which_inventory : 2;
            unsigned which_player : 5;
        };
    };
    struct {
        int money : 1;
        int action : 1;
        int dual : 1;
    } properties;
} cards[card_count];

const int GAME_STATUS_NONE = 0;
const int GAME_STATUS_STARTED = 1;
const int GAME_STATUS_RUNNING = 2;

int game_stat = 0;
int game_turn = 0;
int placed_cards_remaining = 0;

int game_random_draw(int player, int count);
void game_next_turn();
int game_reset();
void game_reset_card(int i);

int game_libinit() {
    game_reset();
    return COMMAND_SUCCESS;
}

void game_playerdc(int player) {
    if (!game_stat) {
        return;
    }
    message_fwriteln(-1, ".Game", "%s left the game.", players[player].username);
    players[player].playing = false;
    players[player].owe = 0;
    if (game_turn == player) {
        game_next_turn();
    }
    for (int i = 0; i < card_count; i++) {
        if (cards[i].with_player && cards[i].which_player == player) {
            game_reset_card(i);
        }
    }
}

int game_reset() {
    for (int i = 0; i < card_count; i++) {
        game_reset_card(i);
    }
    for (int i = 0; i < player_count; i++) {
        players[i].playing = false;
    }
    game_stat = GAME_STATUS_NONE;
    game_turn = 0;
    return COMMAND_SUCCESS;
}

void game_reset_card(int i) {
    cards[i].id = card_index_to_id(i);
    if (i < 28) {
        cards[i].full_location = 0;
        cards[i].which_inventory = INVENTORY_SHOP;
    } else {
        cards[i].full_location = 0;
    }
    cards[i].properties.money = (27 < cards[i].id) && (cards[i].id != 35);
    cards[i].properties.dual = ((27 < cards[i].id) && (cards[i].id < 36)) || ((41 < cards[i].id) && (cards[i].id < 48));
    cards[i].properties.action = 47 < cards[i].id;
}

void game_next_turn() {
    for (int i = 0; i < player_count; i++) {
        players[i].owe = false;
    }
    int next_turn = game_turn;
    while (true) {
        next_turn = (next_turn + 1) % player_count;
        if (players[next_turn].playing) {
            game_turn = next_turn;
            placed_cards_remaining = 3;
            player_write_alert(game_turn, "It's your turn.");
            message_fwriteln(-1, ".Game", "It's %s's turn.", players[game_turn].username);
            game_random_draw(game_turn, 3);
            break;
        }
        if (next_turn == game_turn) {
            game_reset();
            break;
        }
    }
}

int game_random_draw1card(int player) {
    int count = 0;
    for (int i = 0; i < card_count; i++) {
        if (cards[i].with_player == false && cards[i].which_inventory == INVENTORY_DECK) {
            count++;
        }
    }
    if (count == 0) {
        player_writeln(player, "No cards remain in the deck.");
        return COMMAND_FAILURE;
    }
    int sel = rand() % count;
    for (int i = 0; i < card_count; i++) {
        if (cards[i].with_player == false && cards[i].which_inventory == INVENTORY_DECK) {
            count--;
            if (count == sel) {
                cards[i].with_player = true;
                cards[i].which_player = player;
                cards[i].which_inventory = INVENTORY_HAND;
                player_fwriteln(player, "You drew a %s.", card_string(player, cards[i].id));
            }
        }
    }
    return COMMAND_SUCCESS;
}

int game_random_draw(int player, int count) {
    for (int i = 0; i < count; i++) {
        if (game_random_draw1card(player) == -1) {
            break;
        }
    }
    return COMMAND_SUCCESS;
}
int cmdfn_game_end() {
    if (!game_stat) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (current_player) {
        player_writeln(current_player, "Ask the host to end the game!");
        return COMMAND_FAILURE;
    }
    game_reset();
    return COMMAND_SUCCESS;
}

int cmdfn_game_join() {
    if (!game_stat) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (game_stat == GAME_STATUS_RUNNING) {
        player_writeln(current_player, "A game already started.");
        return COMMAND_FAILURE;
    }
    message_fwriteln(-1, ".Game", "%s joined the game.", players[current_player].username);
    if (!current_player) {
        int playing_count = 0;
        for (int i = 1; i < player_count; i++) {
            playing_count += players[i].playing;
        }
        if (!playing_count) {
            player_writeln(current_player, "Wait until another player joins first!");
            return COMMAND_FAILURE;
        }
        player_fwriteln(current_player, "Starting a game with %i people.", playing_count + 1);
        game_stat = GAME_STATUS_RUNNING;
        player_writeln(current_player, "The game has begun. It is your turn.");
        for (int i = 1; i < player_count; i++) {
            if (players[i].playing) {
                player_write_alert(i, "The game has begun. It is the host's turn.");
            }
        }
        game_turn = -1;
        game_next_turn();
    } else {
        players[current_player].playing = true;
        player_writeln(current_player, "You're in the game. The game will begin when the host joins.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_pay(token_node remaining) {
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (game_turn == current_player) {
        player_writeln(current_player, "You can't pay yourself!");
        return COMMAND_FAILURE;
    }
    char * e = 0;
    int index = strtol(remaining->data, &e, 10);
    if (e - remaining->data != strlen(remaining->data)) {
        player_writeln(current_player, "That is not a valid integer.");
        return COMMAND_FAILURE;
    }
    int count = 0;
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 1) && (cards[i].which_player == current_player) && (cards[i].which_inventory == INVENTORY_HAND)) {
            if (count == index) {
                cards[i].which_player = game_turn;
                players[current_player].owe -= cards[i].which_player;
                if (players[current_player].owe < 0) {
                    players[current_player].owe = 0;
                }
            }
            count++;
        }
    }
    player_writeln(current_player, "Index out of hand range.");
    return COMMAND_FAILURE;
}

int cmdfn_game_quit() {
    if (!game_stat) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    game_playerdc(current_player);
    return COMMAND_SUCCESS;
}

int cmdfn_game_show_bank(token_node remaining) {
    int target = player_findby_username(remaining->data);
    if (strcmp(remaining->data, "all") && (target == -1)) {
        player_writeln(current_player, "That user does not exist.");
        return COMMAND_FAILURE;
    }
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (target == -1) {
        for (int i = 0; i < player_count; i++) {
            if (players[i].playing) {
                cmdfn_game_show_bank(remaining);
            }
        }
        return COMMAND_SUCCESS;
    }
    int count = 0;
    player_fwriteln(current_player, "%s's bank:", players[target].username);
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 1) && (cards[i].which_player == target) && (cards[i].which_inventory == INVENTORY_BANK)) {
            player_fwriteln(current_player, "%s", card_string(current_player, cards[i].id));
            count++;
        }
    }
    if (!count) {
        player_writeln(current_player, "No cards.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_show_hand() {
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    int count = 0;
    player_fwriteln(current_player, "Your hand:");
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 1) && (cards[i].which_player == current_player) && (cards[i].which_inventory == INVENTORY_HAND)) {
            player_fwriteln(current_player, "%s", card_string(current_player, cards[i].id));
            count++;
        }
    }
    if (!count) {
        player_writeln(current_player, "No cards.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_show_properties(token_node remaining) {
    int target = player_findby_username(remaining->data);
    if (strcmp(remaining->data, "all") && (target == -1)) {
        player_writeln(current_player, "That user does not exist.");
        return COMMAND_FAILURE;
    }
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (target == -1) {
        for (int i = 0; i < player_count; i++) {
            if (players[i].playing) {
                cmdfn_game_show_properties(remaining);
            }
        }
        return COMMAND_SUCCESS;
    }
    int count = 0;
    player_fwriteln(current_player, "%s's properties:", players[target].username);
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 1) && (cards[i].which_player == target) && (cards[i].which_inventory == INVENTORY_PROPERTIES)) {
            player_fwriteln(current_player, "%s", card_string(current_player, cards[i].id));
            count++;
        }
    }
    if (!count) {
        player_writeln(current_player, "No cards.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_show_shop() {
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    int count = 0;
    player_fwriteln(current_player, "Shop:");
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 0) && (cards[i].which_inventory == INVENTORY_SHOP)) {
            player_fwriteln(current_player, "%s", card_string(current_player, cards[i].id));
            count++;
        }
    }
    if (!count) {
        player_writeln(current_player, "No cards.");
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_start() {
    if (game_stat) {
        player_writeln(current_player, "A game already started.");
        return COMMAND_FAILURE;
    }
    if (current_player != 0) {
        player_writeln(current_player, "Ask the host to start the game!");
        return COMMAND_FAILURE;
    }
    game_stat = GAME_STATUS_STARTED;
    player_writeln(current_player, "Starting the game...");
    message_writeln(-1, ".Game", "A game is starting...");
    for (int i = 1; i < player_count; i++) {
        player_write_alert(i, "A game is starting! Type \"game join\" to join!");
    }
    players[current_player].playing = true;
    return COMMAND_SUCCESS;
}

int cmdfn_game_status() {
    if (!game_stat) {
        player_writeln(current_player, "No game is in progress.");
        return COMMAND_SUCCESS;
    }
    if (game_stat == GAME_STATUS_STARTED) {
        player_writeln(current_player, "A game is starting! Type \"game join\" to join!");
        return COMMAND_SUCCESS;
    }
    if (game_stat == GAME_STATUS_RUNNING) {
        player_writeln(current_player, "A game is currently running.");
    }
    player_fwriteln(current_player, "It's %s's turn.", players[game_turn].username);
    for (int i = 0; i < player_count; i++) {
        if (players[i].playing && players[i].owe) {
            if (game_turn == current_player) {
                player_fwriteln(current_player, "%s owes %s %i.", players[i].username, players[game_turn].username, players[i].owe);
            } else {
                player_fwriteln(current_player, "%s owes you %i.", players[i].username, players[i].owe);
            }
        }
    }
    if (game_turn == current_player) {
        player_fwriteln(current_player, "You have %i cards to place down remaining", placed_cards_remaining);
    }
    return COMMAND_SUCCESS;
}

int cmdfn_game_turn_end() {
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (game_turn != current_player) {
        player_writeln(current_player, "It's not your turn!");
        return COMMAND_FAILURE;
    }
    for (int i = 0; i < player_count; i++) {
        if (players[i].owe) {
            player_writeln(current_player, "You cannot end your turn when a player still owes you.");
            return COMMAND_FAILURE;
        }
    }
    game_next_turn();
    return COMMAND_SUCCESS;
}

int cmdfn_game_use_action() {
    player_writeln(current_player, "You can't use cards yet lol");
    return COMMAND_SUCCESS;
}

int cmdfn_game_use_dual(token_node remaining) {
    player_writeln(current_player, "You can't use cards yet lol");
    return COMMAND_SUCCESS;
}

int cmdfn_game_use_money(token_node remaining) {
    if (game_stat != GAME_STATUS_RUNNING) {
        player_writeln(current_player, "No game in progress.");
        return COMMAND_FAILURE;
    }
    if (game_turn != current_player) {
        player_writeln(current_player, "It's not your turn!");
        return COMMAND_FAILURE;
    }
    if (placed_cards_remaining <= 0) {
        player_writeln(current_player, "You can't use any more cards.");
        return COMMAND_FAILURE;
    }
    char * e = 0;
    int index = strtol(remaining->data, &e, 10);
    if (e - remaining->data != strlen(remaining->data)) {
        player_writeln(current_player, "That is not a valid integer.");
        return COMMAND_FAILURE;
    }
    int count = 0;
    for (int i = 0; i < card_count; i++) {
        if ((cards[i].with_player == 1) && (cards[i].which_player == current_player) && (cards[i].which_inventory == INVENTORY_HAND)) {
            if (count == index) {
                if (!cards[i].properties.money) {
                    player_writeln(current_player, "That card cannot be used as money.");
                    return COMMAND_FAILURE;
                }
                placed_cards_remaining--;
                cards[i].which_inventory = INVENTORY_BANK;
                return COMMAND_SUCCESS;
            }
            count++;
        }
    }
    player_writeln(current_player, "Index out of hand range.");
    return COMMAND_FAILURE;
}

command cmd_game_end = {
    .trigger = "end", .next_count = 0, .end = cmdfn_game_end,
    .usage = "game end",
    .usage_params = &nullch,
    .help_brief = "Ends game",
    .param_count = 0
};

command cmd_game_join = {
    .trigger = "join", .next_count = 0, .end = cmdfn_game_join,
    .usage = "game join",
    .usage_params = &nullch,
    .help_brief = "After the host starts a game, players use this to join. When the host joins, the game starts and players can no longer join",
    .param_count = 0
};

command cmd_game_pay = {
    .trigger = "pay", .next_count = 0, .end = cmdfn_game_pay,
    .usage = "game pay",
    .usage_params = "[card index]",
    .help_brief = "Pays current player",
    .param_count = 1
};

command cmd_game_quit = {
    .trigger = "quit", .next_count = 0, .end = cmdfn_game_quit,
    .usage = "game quit",
    .usage_params = &nullch,
    .help_brief = "quits the game",
    .param_count = 0
};

command cmd_game_start = {
    .trigger = "start", .next_count = 0, .end = cmdfn_game_start,
    .usage = "game start",
    .usage_params = &nullch,
    .help_brief = "Starts game, everyone is asked to join",
    .param_count = 0
};

command cmd_game_show_bank = {
    .trigger = "bank", .next_count = 0, .end = cmdfn_game_show_bank,
    .usage = "game show bank",
    .usage_params = "[player]",
    .help_brief = "Shows the bank of [player], or everyone if [player] is \"all\"",
    .param_count = 1
};

command cmd_game_show_hand = {
    .trigger = "hand", .next_count = 0, .end = cmdfn_game_show_hand,
    .usage = "game show hand",
    .usage_params = &nullch,
    .help_brief = "Shows your hand",
    .param_count = 0
};

command cmd_game_show_properties = {
    .trigger = "properties", .next_count = 0, .end = cmdfn_game_show_properties,
    .usage = "game show properties",
    .usage_params = "[player]",
    .help_brief = "Shows which properties belong to [player], or everyone if [player] is \"all\"",
    .param_count = 1
};

command cmd_game_show_shop = {
    .trigger = "shop", .next_count = 0, .end = cmdfn_game_show_shop,
    .usage = "game show shop",
    .usage_params = &nullch,
    .help_brief = "Shows all properties for sale",
    .param_count = 0
};

command cmd_game_show = {
    .trigger = "show", .next_count = 4, .nexts = { &cmd_game_show_bank, &cmd_game_show_hand, &cmd_game_show_properties, &cmd_game_show_shop },
};

command cmd_game_status = {
    .trigger = "status", .next_count = 0, .end = cmdfn_game_status,
    .usage = "game status",
    .usage_params = &nullch,
    .help_brief = "Shows status of the game",
    .param_count = 0
};

command cmd_game_turn_end = {
    .trigger = "end", .next_count = 0, .end = cmdfn_game_turn_end,
    .usage = "game turn end",
    .usage_params = &nullch,
    .help_brief = "Ends turn",
    .param_count = 0
};

command cmd_game_turn = {
    .trigger = "turn", .next_count = 1, .nexts = { &cmd_game_turn_end }
};

command cmd_game_use_action = {
    .trigger = "action", .next_count = 0, .end = cmdfn_game_use_action,
    .usage = "game use action",
    .usage_params = "[card index]",
    .help_brief = "Uses an action card, given it's position in your hand",
    .param_count = 1
};

command cmd_game_use_dual = {
    .trigger = "dual", .next_count = 0, .end = cmdfn_game_use_dual,
    .usage = "game use dual",
    .usage_params = "[card index] [color]",
    .help_brief = "Uses rent or wildcard, given it's position in your hand, and the color you want to use it as",
    .param_count = 2
};

command cmd_game_use_money = {
    .trigger = "money", .next_count = 0, .end = cmdfn_game_use_money,
    .usage = "game use money",
    .usage_params = "[card index]",
    .help_brief = "Puts a card in your bank, given it's position in your hand",
    .param_count = 1
};

command cmd_game_use = {
    .trigger = "use", .next_count = 3, .nexts = { &cmd_game_use_action, &cmd_game_use_dual, &cmd_game_use_money },
};

command cmd_game = {
    .trigger = "game", .next_count = 9, .nexts = { &cmd_game_end, &cmd_game_join, & cmd_game_pay, &cmd_game_quit, &cmd_game_show, &cmd_game_start, &cmd_game_status, &cmd_game_turn, &cmd_game_use },
    .help_title = "Game Commands",
};

