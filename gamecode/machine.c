#include <stdbool.h>
#include <stdlib.h>
#ifdef LINUX
    #include <unistd.h>
#endif

#include "led.h"
#include "machine.h"
#include "random.h"

#define COUNT_BUFFER_SIZE 2048 >> 3

static unsigned device_id = 1;

static unsigned char * device_map = NULL;

static unsigned seen = 0;


void setDevice( unsigned device) {
    device_id = device;
    initialise_random( 1 );
}

unsigned device() {
    return device_id;
}

void machine_warn_user() {
    led_line(true);
    delay();
    led_line(false);
    delay();
    led_line(true);
    delay();
    led_line(false);
    delay();
}

void machine_led_flash(enum Led led) {
    led_set(led, true);
    delay();
    led_set(led, false);
    delay();
}

void machine_initialise() {
    machine_warn_user();
    device_map = calloc( COUNT_BUFFER_SIZE, sizeof(char));
    if( ! device_map) {
        // flashing red light to indicate error,  pull battery out. (control-c)
        delay();
        machine_warn_user();
        bool flag = false;
        while(1) {
            flag = ! flag;
            led_set(red, flag);
            delay();
        }
    }
}

void machine_flag_seen(unsigned machineId ) {
    int element = machineId >> 3;
    int bit_set = machineId & 7;
    unsigned char temp = device_map[element];
    
    device_map[element] |= 1 << bit_set;

    if( temp != device_map[element] ) {
        seen++;
    }

}

char* machine_seen()
{
    return device_map;
}


unsigned int countSetBits(unsigned char n) 
{ 
  unsigned int count = 0; 
  while (n) 
  { 
    count += n & 1; 
    n >>= 1;
  }
  return count; 
} 

unsigned machine_connections()
{
    unsigned count = 0;
    for( int i = 0; i < COUNT_BUFFER_SIZE; i++)
    {
        count += countSetBits(device_map[i]);
    }
    return count;
}


void machine_destroy() {
    free(device_map);
}

void machine_sleep() {
    //  Turn off
    led_line(false); // led
    //  ADC,
    //  EUSART,
    //  Timers,
    //    etc.
}

void delay() {
    usleep(500000);
}