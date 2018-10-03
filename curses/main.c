
#include <ncurses.h>
#include <ctype.h>
#include <unistd.h>

#include "../gamecode/random.h"
#include "../gamecode/led.h"
#include "../gamecode/machine.h"
#include "../gamecode/director.h"
#include "../gamecode/flashValue.h"

int main (int argc, char **argv);

/* We define main() to call toplev::main(), which is defined in toplev.c.
   We do this in a separate file in order to allow the language front-end
   to define a different main(), if it so desires.  */

int main (int argc, char **argv)
{
	machine_initialise();

	led_initialise();
	
	initialise_random( 1 );


#ifdef TESTING
	// testing...
	led_set( green, true);
	delay();
	led_set( yellow, true);
	delay();
	led_set( red, true);
	delay();
	led_set( green, true);
	delay();
	led_set( yellow, false);
	delay();
	led_set( red, false);
	delay();

	flashValue(123);
	delay();
	// end testing
#endif

	noecho();

	director();

	echo();
	led_destroy();
	machine_destroy();
	return 0;
}
