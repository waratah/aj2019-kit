#include <stdbool.h>
#include "director.h"

#ifdef LINUX
    #include <ctype.h>
    #include <ncurses.h>
#endif
#include "led.h"
#include "led_curses.h"
#include "game.h"
#include "flashValue.h"

char director() {

	while(1) {
        int buttons = get_buttons();
        switch(buttons ) {
            case 'g':
                game_check_it( green);
                continue;
            case 'y':
                game_check_it( yellow);
                continue;
            case 'r':
                game_check_it( red);
                continue;
            case 'n':
                game_initialise();
                continue;

            case 'm':
                // match_machine();
                continue;
                
            case 'f':
                // match_dump_status();
                continue;

            case 's':
                led_set_machine();
                continue;

            case 'c':
                flashValue(game_topscore());
                continue;

            default:
                continue;
        }
	}

}