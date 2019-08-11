#include "card.h"
#include "player/player.h"
#include <stdio.h>

const char * card_names[58];
const char * ansi_card_names[58];
int card_values[58];

const char * value_color(int card_id) {
    switch (card_values[card_id]) {
        //    case 0: return ANSI_FG_WHITE;
        //    case 1: return ANSI_FG_CYELLOW;
        //    case 2: return ANSI_FG_CRED;
        //    case 3: return ANSI_FG_CGREEN;
        //    case 4: return ANSI_FG_CBLUE;
        //    case 5: return ANSI_FG_CPURPLE;
        //    case 10: return ANSI_FG_CORANGE;
    default:
        return 0;
    }
}

char buffer[90];

const char * card_string(int player, int card_id) {
    //    if (players[player].ansi) {
    //        sprintf(buffer, "%s%s($%iM) %s" ANSI_BG_RESET " %s($%iM)" ANSI_BFG_RESET, (card_values[card_id] < 10 ? " " : ""), value_color(card_id), card_values[card_id], ansi_card_names[card_id], value_color(card_id), card_values[card_id]);
    //    } else {
    sprintf(buffer, "%s($%iM) %s ($%iM)", (card_values[card_id] < 10 ? " " : ""), card_values[card_id], card_names[card_id], card_values[card_id]);
    //    }
    return buffer;
}

bool card_property(unsigned card_id) {
    return card_id <= water_works;
}

bool card_wild(unsigned card_id) {
    return (card_id >= purple_orange_wildcard) && (card_id <= multi_color_wildcard);
}

bool card_money(unsigned card_id) {
    return (card_id >= money10) && (card_id <= money1);
}

bool card_action(unsigned card_id) {
    return (card_id >= money10) && (card_id <= money1);
}

unsigned card_index_to_id(unsigned index) {
    if (index < 29) { return index; }
    if (index < 35) { return index - 1; }
    if (index < 38) { return index - 2; }
    if (index < 41) { return index - 3; }
    if (index == 41) { return money5; }
    if (index < 45) { return money4; }
    if (index < 48) { return money3; }
    if (index < 53) { return money2; }
    if (index < 59) { return money1; }
    if (index < 71) { return ((index + 25) / 2); }
    if (index == 71) { return multi_color_rent; }
    if (index < 83) { return ((index + 73) / 3); }
    if (index < 90) { return ((index + 72) / 3); }
    if (index < 100) { return pass_go; }
    if (index < 103) { return house; }
    if (index < 106) { return hotel; }
    if (index < 108) { return double_the_rent; }
    return -1;
}

const unsigned mediterranean_avenue = 0;
const unsigned baltic_avenue = 1;
const unsigned oriental_avenue = 2;
const unsigned vermont_avenue = 3;
const unsigned connecticut_avenue = 4;
const unsigned st_charles_place = 5;
const unsigned states_avenue = 6;
const unsigned virginia_avenue = 7;
const unsigned st_james_place = 8;
const unsigned tennessee_avenue = 9;
const unsigned new_york_avenue = 10;
const unsigned kentucky_avenue = 11;
const unsigned indiana_avenue = 12;
const unsigned illinois_avenue = 13;
const unsigned atlantic_avenue = 14;
const unsigned ventnor_avenue = 15;
const unsigned marvin_gardens = 16;
const unsigned pacific_avenue = 17;
const unsigned north_carolina_avenue = 18;
const unsigned pennsylvania_avenue = 19;
const unsigned park_place = 20;
const unsigned boardwalk = 21;
const unsigned reading_railroad = 22;
const unsigned pennsylvania_railroad = 23;
const unsigned bno_railroad = 24;
const unsigned short_line = 25;
const unsigned electric_company = 26;
const unsigned water_works = 27;
const unsigned purple_orange_wildcard = 28;
const unsigned brown_light_blue_wildcard = 29;
const unsigned light_blue_railroad_wildcard = 30;
const unsigned green_blue_wildcard = 31;
const unsigned green_railroad_wildcard = 32;
const unsigned red_yellow_wilcard = 33;
const unsigned railroad_utility_wildcard = 34;
const unsigned multi_color_wildcard = 35;
const unsigned money10 = 36;
const unsigned money5 = 37;
const unsigned money4 = 38;
const unsigned money3 = 39;
const unsigned money2 = 40;
const unsigned money1 = 41;
const unsigned purple_orange_rent = 42;
const unsigned brown_light_blue_rent = 43;
const unsigned green_blue_rent = 44;
const unsigned red_yellow_rent = 45;
const unsigned railroad_utility_rent = 46;
const unsigned multi_color_rent = 47;
const unsigned deal_breaker = 48;
const unsigned just_say_no = 49;
const unsigned sly_deal = 50;
const unsigned forced_deal = 51;
const unsigned debt_collector = 52;
const unsigned its_my_birthday = 53;
const unsigned pass_go = 54;
const unsigned house = 55;
const unsigned hotel = 56;
const unsigned double_the_rent = 57;

void monocards_libinit() {
    card_names[mediterranean_avenue] =         "[      Mediterranean Avenue      ]";
    card_names[baltic_avenue] =                "[         Baltic Avenue          ]";
    card_names[oriental_avenue] =              "[        Oriental Avenue         ]";
    card_names[vermont_avenue] =               "[         Vermont Avenue         ]";
    card_names[connecticut_avenue] =           "[       Connecticut Avenue       ]";
    card_names[st_charles_place] =             "[       St. Charles Place        ]";
    card_names[states_avenue] =                "[         States Avenue          ]";
    card_names[virginia_avenue] =              "[        Virginia Avenue         ]";
    card_names[st_james_place] =               "[        St. James Place         ]";
    card_names[tennessee_avenue] =             "[        Tennessee Avenue        ]";
    card_names[new_york_avenue] =              "[        New York Avenue         ]";
    card_names[kentucky_avenue] =              "[        Kentucky Avenue         ]";
    card_names[indiana_avenue] =               "[         Indiana Avenue         ]";
    card_names[illinois_avenue] =              "[        Illinois Avenue         ]";
    card_names[atlantic_avenue] =              "[        Atlantic Avenue         ]";
    card_names[ventnor_avenue] =               "[         Ventnor Avenue         ]";
    card_names[marvin_gardens] =               "[         Marvin Gardens         ]";
    card_names[pacific_avenue] =               "[         Pacific Avenue         ]";
    card_names[north_carolina_avenue] =        "[     North Carolina Avenue      ]";
    card_names[pennsylvania_avenue] =          "[      Pennsylvania Avenue       ]";
    card_names[park_place] =                   "[           Park Place           ]";
    card_names[boardwalk] =                    "[           Boardwalk            ]";
    card_names[reading_railroad] =             "[        Reading Railroad        ]";
    card_names[pennsylvania_railroad] =        "[     Pennsylvania Railroad      ]";
    card_names[bno_railroad] =                 "[          B&O Railroad          ]";
    card_names[short_line] =                   "[           Short Line           ]";
    card_names[electric_company] =             "[        Electric Company        ]";
    card_names[water_works] =                  "[          Water Works           ]";
    card_names[purple_orange_wildcard] =       "[    Purple / Orange Wildcard    ]";
    card_names[brown_light_blue_wildcard] =    "[  Brown / Light Blue Wildcard   ]";
    card_names[light_blue_railroad_wildcard] = "[ Light Blue / Railroad Wildcard ]";
    card_names[green_blue_wildcard] =          "[     Green / Blue Wildcard      ]";
    card_names[green_railroad_wildcard] =      "[   Green / Railroad Wildcard    ]";
    card_names[red_yellow_wilcard] =           "[     Red / Yellow Wildcard      ]";
    card_names[railroad_utility_wildcard] =    "[  Railroad / Utility Wildcard   ]";
    card_names[multi_color_wildcard] =         "[      Multi-Color Wildcard      ]";
    card_names[money10] =                      "[             Money              ]";
    card_names[money5] =                       card_names[money10];
    card_names[money4] =                       card_names[money10];
    card_names[money3] =                       card_names[money10];
    card_names[money2] =                       card_names[money10];
    card_names[money1] =                       card_names[money10];
    card_names[purple_orange_rent] =           "[      Purple / Orange Rent      ]";
    card_names[brown_light_blue_rent] =        "[    Brown / Light Blue Rent     ]";
    card_names[green_blue_rent] =              "[       Green / Blue Rent        ]";
    card_names[red_yellow_rent] =              "[       Red / Yellow Rent        ]";
    card_names[railroad_utility_rent] =        "[    Railroad / Utility Rent     ]";
    card_names[multi_color_rent] =             "[        Multi-Color Rent        ]";
    card_names[deal_breaker] =                 "[          Deal Breaker          ]";
    card_names[just_say_no] =                  "[          Just Say No           ]";
    card_names[sly_deal] =                     "[            Sly Deal            ]";
    card_names[forced_deal] =                  "[          Forced Deal           ]";
    card_names[debt_collector] =               "[         Debt Collector         ]";
    card_names[its_my_birthday] =              "[        It's My Birthday        ]";
    card_names[pass_go] =                      "[            Pass Go             ]";
    card_names[house] =                        "[             House              ]";
    card_names[hotel] =                        "[             Hotel              ]";
    card_names[double_the_rent] =              "[        Double The Rent         ]";

    //    ansi_card_names[mediterranean_avenue] =         ANSI_BG_PBROWN  ANSI_FG_BLACK "[      Mediterranean Avenue      ]";
    //    ansi_card_names[baltic_avenue] =                ANSI_BG_PBROWN  ANSI_FG_BLACK "[         Baltic Avenue          ]";
    //    ansi_card_names[oriental_avenue] =              ANSI_BG_PLBLUE  ANSI_FG_BLACK "[        Oriental Avenue         ]";
    //    ansi_card_names[vermont_avenue] =               ANSI_BG_PLBLUE  ANSI_FG_BLACK "[         Vermont Avenue         ]";
    //    ansi_card_names[connecticut_avenue] =           ANSI_BG_PLBLUE  ANSI_FG_BLACK "[       Connecticut Avenue       ]";
    //    ansi_card_names[st_charles_place] =             ANSI_BG_PPURPLE ANSI_FG_BLACK "[       St. Charles Place        ]";
    //    ansi_card_names[states_avenue] =                ANSI_BG_PPURPLE ANSI_FG_BLACK "[         States Avenue          ]";
    //    ansi_card_names[virginia_avenue] =              ANSI_BG_PPURPLE ANSI_FG_BLACK "[        Virginia Avenue         ]";
    //    ansi_card_names[st_james_place] =               ANSI_BG_PORANGE ANSI_FG_BLACK "[        St. James Place         ]";
    //    ansi_card_names[tennessee_avenue] =             ANSI_BG_PORANGE ANSI_FG_BLACK "[        Tennessee Avenue        ]";
    //    ansi_card_names[new_york_avenue] =              ANSI_BG_PORANGE ANSI_FG_BLACK "[        New York Avenue         ]";
    //    ansi_card_names[kentucky_avenue] =              ANSI_BG_PRED    ANSI_FG_BLACK "[        Kentucky Avenue         ]";
    //    ansi_card_names[indiana_avenue] =               ANSI_BG_PRED    ANSI_FG_BLACK "[         Indiana Avenue         ]";
    //    ansi_card_names[illinois_avenue] =              ANSI_BG_PRED    ANSI_FG_BLACK "[        Illinois Avenue         ]";
    //    ansi_card_names[atlantic_avenue] =              ANSI_BG_PYELLOW ANSI_FG_BLACK "[        Atlantic Avenue         ]";
    //    ansi_card_names[ventnor_avenue] =               ANSI_BG_PYELLOW ANSI_FG_BLACK "[         Ventnor Avenue         ]";
    //    ansi_card_names[marvin_gardens] =               ANSI_BG_PYELLOW ANSI_FG_BLACK "[         Marvin Gardens         ]";
    //    ansi_card_names[pacific_avenue] =               ANSI_BG_PGREEN  ANSI_FG_BLACK "[         Pacific Avenue         ]";
    //    ansi_card_names[north_carolina_avenue] =        ANSI_BG_PGREEN  ANSI_FG_BLACK "[     North Carolina Avenue      ]";
    //    ansi_card_names[pennsylvania_avenue] =          ANSI_BG_PGREEN  ANSI_FG_BLACK "[      Pennsylvania Avenue       ]";
    //    ansi_card_names[park_place] =                   ANSI_BG_PBLUE   ANSI_FG_BLACK "[           Park Place           ]";
    //    ansi_card_names[boardwalk] =                    ANSI_BG_PBLUE   ANSI_FG_BLACK "[           Boardwalk            ]";
    //    ansi_card_names[reading_railroad] =             ANSI_BG_PBLACK  ANSI_FG_WHITE "[        Reading Railroad        ]";
    //    ansi_card_names[pennsylvania_railroad] =        ANSI_BG_PBLACK  ANSI_FG_WHITE "[     Pennsylvania Railroad      ]";
    //    ansi_card_names[bno_railroad] =                 ANSI_BG_PBLACK  ANSI_FG_WHITE "[          B&O Railroad          ]";
    //    ansi_card_names[short_line] =                   ANSI_BG_PBLACK  ANSI_FG_WHITE "[           Short Line           ]";
    //    ansi_card_names[electric_company] =             ANSI_BG_PLGREEN ANSI_FG_BLACK "[        Electric Company        ]";
    //    ansi_card_names[water_works] =                  ANSI_BG_PLGREEN ANSI_FG_BLACK "[          Water Works           ]";
    //    ansi_card_names[purple_orange_wildcard] =       ANSI_BG_WHITE  ANSI_FG_BLACK "[    " ANSI_BG_PPURPLE "Purple" ANSI_BG_WHITE " / " ANSI_BG_PORANGE "Orange" ANSI_BG_WHITE " Wildcard    ]";
    //    ansi_card_names[brown_light_blue_wildcard] =    ANSI_BG_WHITE  ANSI_FG_BLACK "[  " ANSI_BG_PBROWN "Brown" ANSI_BG_WHITE " / " ANSI_BG_PLBLUE "Light Blue" ANSI_BG_WHITE " Wildcard   ]";
    //    ansi_card_names[light_blue_railroad_wildcard] = ANSI_BG_WHITE  ANSI_FG_BLACK "[ " ANSI_BG_PLBLUE "Light Blue" ANSI_BG_WHITE " / " ANSI_BG_PBLACK ANSI_FG_WHITE "Railroad" ANSI_BG_WHITE ANSI_FG_BLACK " Wildcard ]";
    //    ansi_card_names[green_blue_wildcard] =          ANSI_BG_WHITE  ANSI_FG_BLACK "[     " ANSI_BG_PGREEN "Green" ANSI_BG_WHITE " / " ANSI_BG_PBLUE "Blue" ANSI_BG_WHITE " Wildcard      ]";
    //    ansi_card_names[green_railroad_wildcard] =      ANSI_BG_WHITE  ANSI_FG_BLACK "[   " ANSI_BG_PGREEN "Green" ANSI_BG_WHITE " / " ANSI_BG_PBLACK ANSI_FG_WHITE "Railroad" ANSI_BG_WHITE ANSI_FG_BLACK " Wildcard    ]";
    //    ansi_card_names[red_yellow_wilcard] =           ANSI_BG_WHITE  ANSI_FG_BLACK "[     " ANSI_BG_PRED "Red" ANSI_BG_WHITE " / " ANSI_BG_PYELLOW "Yellow" ANSI_BG_WHITE " Wildcard      ]";
    //    ansi_card_names[railroad_utility_wildcard] =    ANSI_BG_WHITE  ANSI_FG_BLACK "[  " ANSI_BG_PBLACK ANSI_FG_WHITE "Railroad" ANSI_BG_WHITE ANSI_FG_BLACK " / " ANSI_BG_PLGREEN "Utility" ANSI_BG_WHITE " Wildcard   ]";
    //    ansi_card_names[multi_color_wildcard] =         ANSI_BG_WHITE  ANSI_FG_BLACK "[      Multi-Color Wildcard      ]";
    //    ansi_card_names[money10] =                      ANSI_BG_WHITE  ANSI_FG_BLACK "[             Money              ]";
    //    ansi_card_names[money5] =                       ansi_card_names[money10];
    //    ansi_card_names[money4] =                       ansi_card_names[money10];
    //    ansi_card_names[money3] =                       ansi_card_names[money10];
    //    ansi_card_names[money2] =                       ansi_card_names[money10];
    //    ansi_card_names[money1] =                       ansi_card_names[money10];
    //
    //    ansi_card_names[purple_orange_rent] =           ANSI_BG_WHITE  ANSI_FG_BLACK "[      " ANSI_BG_PPURPLE "Purple" ANSI_BG_WHITE " / " ANSI_BG_PORANGE "Orange" ANSI_BG_WHITE " Rent      ]";
    //    ansi_card_names[brown_light_blue_rent] =        ANSI_BG_WHITE  ANSI_FG_BLACK "[    " ANSI_BG_PBROWN "Brown" ANSI_BG_WHITE " / " ANSI_BG_PLBLUE "Light Blue" ANSI_BG_WHITE " Rent     ]";
    //    ansi_card_names[green_blue_rent] =              ANSI_BG_WHITE  ANSI_FG_BLACK "[       " ANSI_BG_PGREEN "Green" ANSI_BG_WHITE " / " ANSI_BG_PBLUE "Blue" ANSI_BG_WHITE " Rent        ]";
    //    ansi_card_names[red_yellow_rent] =              ANSI_BG_WHITE  ANSI_FG_BLACK "[       " ANSI_BG_PRED "Red" ANSI_BG_WHITE " / " ANSI_BG_PYELLOW "Yellow" ANSI_BG_WHITE " Rent        ]";
    //    ansi_card_names[railroad_utility_rent] =        ANSI_BG_WHITE  ANSI_FG_BLACK "[     " ANSI_BG_PBLACK ANSI_FG_WHITE "Railroad" ANSI_BG_WHITE ANSI_FG_BLACK " / " ANSI_BG_PLGREEN "Utility" ANSI_BG_WHITE " Rent    ]";
    //    ansi_card_names[multi_color_rent] =             ANSI_BG_WHITE  ANSI_FG_BLACK     "[        Multi-Color Rent        ]";
    //    ansi_card_names[deal_breaker] =                 ANSI_BG_WHITE  ANSI_FG_BLACK "[          Deal Breaker          ]";
    //    ansi_card_names[just_say_no] =                  ANSI_BG_WHITE  ANSI_FG_BLACK "[          Just Say No           ]";
    //    ansi_card_names[sly_deal] =                     ANSI_BG_WHITE  ANSI_FG_BLACK "[            Sly Deal            ]";
    //    ansi_card_names[forced_deal] =                  ANSI_BG_WHITE  ANSI_FG_BLACK "[          Forced Deal           ]";
    //    ansi_card_names[debt_collector] =               ANSI_BG_WHITE  ANSI_FG_BLACK "[         Debt Collector         ]";
    //    ansi_card_names[its_my_birthday] =              ANSI_BG_WHITE  ANSI_FG_BLACK "[        It's My Birthday        ]";
    //    ansi_card_names[pass_go] =                      ANSI_BG_WHITE  ANSI_FG_BLACK "[            Pass Go             ]";
    //    ansi_card_names[house] =                        ANSI_BG_WHITE  ANSI_FG_BLACK "[             House              ]";
    //    ansi_card_names[hotel] =                        ANSI_BG_WHITE  ANSI_FG_BLACK "[             Hotel              ]";
    //    ansi_card_names[double_the_rent] =              ANSI_BG_WHITE  ANSI_FG_BLACK "[        Double The Rent         ]";

    card_values[mediterranean_avenue] = 1;
    card_values[baltic_avenue] = 1;
    card_values[oriental_avenue] = 1;
    card_values[vermont_avenue] = 1;
    card_values[connecticut_avenue] = 1;
    card_values[st_charles_place] = 2;
    card_values[states_avenue] = 2;
    card_values[virginia_avenue] = 2;
    card_values[st_james_place] = 2;
    card_values[tennessee_avenue] = 2;
    card_values[new_york_avenue] = 2;
    card_values[kentucky_avenue] = 3;
    card_values[indiana_avenue] = 3;
    card_values[illinois_avenue] = 3;
    card_values[atlantic_avenue] = 3;
    card_values[ventnor_avenue] = 3;
    card_values[marvin_gardens] = 3;
    card_values[pacific_avenue] = 4;
    card_values[north_carolina_avenue] = 4;
    card_values[pennsylvania_avenue] = 4;
    card_values[park_place] = 4;
    card_values[boardwalk] = 4;
    card_values[reading_railroad] = 2;
    card_values[pennsylvania_railroad] = 2;
    card_values[bno_railroad] = 2;
    card_values[short_line] = 2;
    card_values[electric_company] = 2;
    card_values[water_works] = 2;
    card_values[purple_orange_wildcard] = 2;
    card_values[brown_light_blue_wildcard] = 1;
    card_values[light_blue_railroad_wildcard] = 4;
    card_values[green_blue_wildcard] = 4;
    card_values[green_railroad_wildcard] = 4;
    card_values[red_yellow_wilcard] = 3;
    card_values[railroad_utility_wildcard] = 2;
    card_values[multi_color_wildcard] = 0;
    card_values[money10] = 10;
    card_values[money5] = 5;
    card_values[money4] = 4;
    card_values[money3] = 3;
    card_values[money2] = 2;
    card_values[money1] = 1;
    card_values[purple_orange_rent] = 1;
    card_values[brown_light_blue_rent] = 1;
    card_values[green_blue_rent] = 1;
    card_values[red_yellow_rent] = 1;
    card_values[railroad_utility_rent] = 1;
    card_values[multi_color_rent] = 3;
    card_values[deal_breaker] = 5;
    card_values[just_say_no] = 4;
    card_values[sly_deal] = 3;
    card_values[forced_deal] = 3;
    card_values[debt_collector] = 3;
    card_values[its_my_birthday] = 2;
    card_values[pass_go] = 1;
    card_values[house] = 3;
    card_values[hotel] = 4;
    card_values[double_the_rent] = 1;
}
