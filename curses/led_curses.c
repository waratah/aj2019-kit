#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "../gamecode/led.h"
#include "../gamecode/machine.h"

#define LED_OFF 1
#define LED_GREEN 2
#define LED_YELLOW 3
#define LED_RED 4
#define HEADING 6
#define LED_LENGTH 4

#define MAXLENGTH 10

struct led {
    bool led_state;
    short led_color;
    short led_off;
    short x;
};

static struct led led_array[3];

void led_initialise() {
    initscr();			/* Start curses mode 		  */
    curs_set(0);        /* hide the cursor */

    start_color();     /* colour mode */
    // init_pair(LED_OFF, COLOR_BLACK, COLOR_MAGENTA );
    init_pair(LED_OFF, COLOR_WHITE, COLOR_BLACK );
    init_pair(LED_RED, COLOR_WHITE, COLOR_RED );
    init_pair(LED_GREEN, COLOR_BLACK, COLOR_GREEN );
    init_pair(LED_YELLOW, COLOR_BLACK, COLOR_YELLOW );
    init_pair(HEADING, COLOR_YELLOW, COLOR_BLACK );

    struct led *led = &led_array[0];
    (*led).led_color = LED_GREEN;
    (*led).led_off = LED_OFF;
    (*led).led_state = false;
    (*led).x = 24;

    led = &led_array[1];
    (*led).led_color = LED_YELLOW;
    (*led).led_off = LED_OFF;
    (*led).led_state = false;
    (*led).x = 30;

    led = &led_array[2];
    (*led).led_color = LED_RED;
    (*led).led_off = LED_OFF;
    (*led).led_state = false;
    (*led).x = 36;

    attron(COLOR_PAIR( HEADING ));
    mvwprintw(stdscr, 2, 20, "Welcome to the scouts game");
    attroff(COLOR_PAIR( HEADING ));

    attron(COLOR_PAIR( HEADING ));
    mvwprintw(stdscr, 12, 20, "(R) - red button, (Y) - yellow button,  (G) - green button");
    mvwprintw(stdscr, 14, 20, "(S) - Set machine number, (M) - match machine number,  (F) - Send high values plugin");
    mvwprintw(stdscr, 15, 20, "(N) - New game, (C) - show count, (T) - top score,  (Q) - Quit");
    attroff(COLOR_PAIR( HEADING ));


	refresh();			/* Print heading on to the real screen */

}

void led_set( enum Led colour, bool status ) {
    struct led led = led_array[colour];
    led.led_state = status;
    short ledcol = status? led.led_color: led.led_off; 
    mvchgat(5, led.x, LED_LENGTH, A_NORMAL, ledcol, NULL);	
    mvchgat(6, led.x, LED_LENGTH, A_NORMAL, ledcol, NULL);	
    mvchgat(7, led.x, LED_LENGTH, A_NORMAL, ledcol, NULL);	
    refresh();			/* Print it on to the real screen */
}

void led_line(bool status ) {
    led_set(green, status);
    led_set(yellow, status);
    led_set(red, status);
}

void led_destroy() {
	endwin();			/* End curses mode		  */
}

void message(char * string ) {
    mvwprintw(stdscr, 17, 20, "                                               ");
    mvwprintw(stdscr, 17, 20, string);
    refresh();
}

void led_set_machine() {
    char * str = malloc(MAXLENGTH + 1);
    message("Setting machine Id: ");
    echo();

    mvgetnstr(17, 41, str, MAXLENGTH);
    noecho();
    setDevice( atoi(str));
    free(str);
}

void led_show_count(char * text, unsigned value) {

    char * buf = malloc(41);
    snprintf(buf, 40, "%s %d", text, value );
    message(buf);
    free( buf );
    usleep(100000);
}
