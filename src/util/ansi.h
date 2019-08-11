#pragma once

// ansi global colors

#define ANSI_BG_WHITE "\033[48;5;231m"
#define ANSI_FG_BLACK "\033[38;5;232m"
#define ANSI_FG_GREEN "\033[38;5;46m"
#define ANSI_FG_RED "\033[38;5;196m"
#define ANSI_FG_WHITE "\033[38;5;231m"

// ansi special text effects

#define ANSI_BLINK "\033[5m"
#define ANSI_BOLD "\033[1m"
#define ANSI_CROSSED "\033[9m"
#define ANSI_FAINT "\033[2m"
#define ANSI_INVERSE "\033[7m"
#define ANSI_INVISABLE "\033[8m"
#define ANSI_ITALICS "\033[3m"
#define ANSI_UNDERSCORE "\033[4m"

#define ANSI_NO_BLINK "\033[25m"
#define ANSI_NO_BOLD "\033[21m"
#define ANSI_NO_CROSSED "\033[29m"
#define ANSI_NO_FAINT "\033[22m"
#define ANSI_NO_INVERSE "\033[27m"
#define ANSI_NO_ITALICS "\033[23m"
#define ANSI_NO_UNDERSCORE "\033[24m"
#define ANSI_VISABLE "\033[28m"

#define ANSI_BG_RESET "\033[49m"
#define ANSI_FG_RESET "\033[39m"
#define ANSI_BFG_RESET "\033[39;49m"
#define ANSI_RESET "\033[0m" ANSI_NO_BOLD ANSI_NO_FAINT ANSI_NO_ITALICS ANSI_NO_UNDERSCORE ANSI_NO_BLINK ANSI_NO_INVERSE ANSI_VISABLE ANSI_NO_CROSSED

// ansi card-specific colors

#define ANSI_FG_CBLUE "\033[38;5;153m"
#define ANSI_FG_CGREEN "\033[38;5;150m"
#define ANSI_FG_CORANGE "\033[38;5;214m"
#define ANSI_FG_CPURPLE "\033[38;5;141m"
#define ANSI_FG_CRED "\033[38;5;209m"
#define ANSI_FG_CYELLOW "\033[38;5;222m"

// ansi property-specific colors

#define ANSI_BG_PBLACK  "\033[48;5;232m"
#define ANSI_BG_PBLUE   "\033[48;5;27m"
#define ANSI_BG_PBROWN  "\033[48;5;94m"
#define ANSI_BG_PGREEN  "\033[48;5;28m"
#define ANSI_BG_PLBLUE  "\033[48;5;110m"
#define ANSI_BG_PLGREEN "\033[48;5;121m"
#define ANSI_BG_PORANGE "\033[48;5;202m"
#define ANSI_BG_PPURPLE "\033[48;5;200m"
#define ANSI_BG_PRED    "\033[48;5;196m"
#define ANSI_BG_PYELLOW "\033[48;5;226m"
#define ANSI_FG_PBLACK  "\033[38;5;232m"
#define ANSI_FG_PBLUE   "\033[38;5;27m"
#define ANSI_FG_PBROWN  "\033[38;5;94m"
#define ANSI_FG_PGREEN  "\033[38;5;28m"
#define ANSI_FG_PLBLUE  "\033[38;5;110m"
#define ANSI_FG_PLGREEN "\033[38;5;121m"
#define ANSI_FG_PORANGE "\033[38;5;208m"
#define ANSI_FG_PPURPLE "\033[38;5;200m"
#define ANSI_FG_PRED    "\033[38;5;196m"
#define ANSI_FG_PYELLOW "\033[38;5;226m"

#define _PLAYER_IF_ANSI(PLAYER, ANSI_BEGIN, ANSI_END, STRING) (players[PLAYER].ansi ? ANSI_BEGIN STRING ANSI_END: STRING)

// ansi customizable colors (change as see fit)

// for output with success/failure messages
#define ANSI_FAILURE(PLAYER, STRING) _PLAYER_IF_ANSI(PLAYER, ANSI_FG_RED, ANSI_FG_RESET, STRING)
#define ANSI_SUCCESS(PLAYER, STRING) _PLAYER_IF_ANSI(PLAYER, ANSI_FG_GREEN, ANSI_FG_RESET, STRING)

// for output with name/value relationships
#define ANSI_NAME(PLAYER, STRING)
#define ANSI_VALUE(PLAYER, STRING)

// for the extra help jazz
#define ANSI_HELP_TITLE(PLAYER, STRING) _PLAYER_IF_ANSI(PLAYER, ANSI_UNDERSCORE, ANSI_NO_UNDERSCORE, STRING)
#define ANSI_HELP_DETAILS(PLAYER, STRING) _PLAYER_IF_ANSI(PLAYER, ANSI_ITALICS, ANSI_NO_ITALICS, STRING)

