#ifdef LINUX
    #include <stdio.h>
#endif
#include "led.h"
#include "flashValue.h"
#include "machine.h"

void flashit(enum Led colour, int flash)
{
    while( flash > 0 ) {
        led_set(colour, 1);
        delay();
        led_set(colour, 0);
        delay();
        flash--;
    }
}

void flashValue(int value) {
    // printf(" flashing %d\n", value);

    int flash;
    led_line(0);
    if( value >= 100) {
        flashit( red, value / 100 );
        value = value % 100;
    }
    if( value >= 10) {
        flashit( yellow, value / 10 );
        value = value % 10;
    }
    if( value >= 1) {
        flashit( green, value );
    }
}

