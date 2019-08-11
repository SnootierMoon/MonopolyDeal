#pragma once

#include <stdbool.h>

const char * card_string(int player, int card_id);

// exclusive type checks
bool card_property(unsigned card_id);  // all & only properties
bool card_wild(unsigned card_id);      // all & only wild cards
bool card_money(unsigned card_id);     // all & only money cards
bool card_action(unsigned card_id);    // all & only action cards

unsigned card_index_to_id(unsigned index);

extern const unsigned mediterranean_avenue;
extern const unsigned baltic_avenue;
extern const unsigned oriental_avenue;
extern const unsigned vermont_avenue;
extern const unsigned connecticut_avenue;
extern const unsigned st_charles_place;
extern const unsigned states_avenue;
extern const unsigned virginia_avenue;
extern const unsigned st_james_place;
extern const unsigned tennessee_avenue;
extern const unsigned new_york_avenue;
extern const unsigned kentucky_avenue;
extern const unsigned indiana_avenue;
extern const unsigned illinois_avenue;
extern const unsigned atlantic_avenue;
extern const unsigned ventnor_avenue;
extern const unsigned marvin_gardens;
extern const unsigned pacific_avenue;
extern const unsigned north_carolina_avenue;
extern const unsigned pennsylvania_avenue;
extern const unsigned park_place;
extern const unsigned boardwalk;
extern const unsigned reading_railroad;
extern const unsigned pennsylvania_railroad;
extern const unsigned bno_railroad;
extern const unsigned short_line;
extern const unsigned electric_company;
extern const unsigned water_works;
extern const unsigned purple_orange_wildcard;
extern const unsigned brown_light_blue_wildcard;
extern const unsigned light_blue_railroad_wildcard;
extern const unsigned green_blue_wildcard;
extern const unsigned green_railroad_wildcard;
extern const unsigned red_yellow_wilcard;
extern const unsigned railroad_utility_wildcard;
extern const unsigned multi_color_wildcard;
extern const unsigned money10;
extern const unsigned money5;
extern const unsigned money4;
extern const unsigned money3;
extern const unsigned money2;
extern const unsigned money1;
extern const unsigned purple_orange_rent;
extern const unsigned brown_light_blue_rent;
extern const unsigned green_blue_rent;
extern const unsigned red_yellow_rent;
extern const unsigned railroad_utility_rent;
extern const unsigned multi_color_rent;
extern const unsigned deal_breaker;
extern const unsigned just_say_no;
extern const unsigned sly_deal;
extern const unsigned forced_deal;
extern const unsigned debt_collector;
extern const unsigned its_my_birthday;
extern const unsigned pass_go;
extern const unsigned house;
extern const unsigned hotel;
extern const unsigned double_the_rent;

void monocards_libinit();

