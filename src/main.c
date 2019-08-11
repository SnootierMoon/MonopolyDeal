#include "game/card.h"
#include "game/game.h"
#include "message.h"
#include "player/player.h"
#include "util/net.h"
#include <stdio.h>

bool server_closed;

void main_libinit() {
    game_libinit();
    monocards_libinit();
    message_libinit();
    net_libinit();
    player_libinit();
}

void main_iteration() {
    net_select();
    for (unsigned player = 0; player < player_count; player++) {
        if (net_has_input(player)) {
            player_read(player);
        }
    }
    if (net_has_connection()) {
        player_accept();
    }
}

void main_libfree() {
    player_libfree();
    message_libfree();
    net_libfree();
}

int main() {
    main_libinit();
    while (!server_closed) {
        main_iteration();
    }
    main_libfree();
}

