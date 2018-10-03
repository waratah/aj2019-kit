#include <stdbool.h>
#include "../gamecode/director.h"

#ifdef LINUX
#include <ctype.h>
#include <ncurses.h>
#endif
#include "../gamecode/led.h"
#include "led_curses.h"
#include "../gamecode/game.h"
#include "../gamecode/flashValue.h"
#include "../gamecode/connection.h"
#include "connection-file.h"
#include "../gamecode/machine.h"

char director()
{
    unsigned m;
    char c = ' ';
    while (c != 'q' && c != 0x03)
    {
        c = tolower(getch()); /* Wait for user input */
        switch (c)
        {
        case 'g':
            game_check_it(green);
            continue;

        case 'y':
            game_check_it(yellow);
            continue;

        case 'r':
            game_check_it(red);
            continue;

        case 'n':
            game_initialise();
            continue;

        case 'm':
            connection_open();
            m = connection_read();
            while (m)
            {
                machine_flag_seen(m);
                m = connection_read();
            }
            connection_close();
            continue;

        case 'f':
            createFile();
            continue;

        case 's':
            led_set_machine();
            continue;

        case 'c':
            m = machine_connections();
            led_show_count("Count: ", m);
            flashValue(m);
            continue;

        case 't':
            flashValue(game_topscore());
            continue;

        default:
            continue;
        }
    }
    echo();

}