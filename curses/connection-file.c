#include <stdio.h>
#include <stdlib.h>

#include "../gamecode/connection.h"
#include "led_curses.h"
#include "../gamecode/random.h"

FILE *fp;

#define LINE_LENGTH 60


void connection_initialise(){
   
}
void connection_open() {
    connection_close();
    fp = fopen("numbers.txt", "r");
}
void connection_close(){
    if( fp ) {
        fclose(fp);
        fp = NULL;
    }
}
void connection_sleep(){

}

unsigned connection_read(){
    unsigned result = 0;
    if( fp ) {
        char * buf = malloc( LINE_LENGTH + 1);
        if( fgets( buf, LINE_LENGTH, fp) ) {
            result = atoi(buf);
        }
        else {
            fclose(fp);
            fp = NULL;
        }
        free( buf);
    }
    return result;
}


void createFile() {
    if( fp ) {
        fclose(fp);
    }
    if( fp = fopen("numbers.txt", "w" ) ) {
        for( int i = 0; i < 100; i++ ) {
            fprintf( fp,  "%d\n", random_value(2000) + 1);
        }
        fclose(fp);
        fp = fopen("numbers.txt", "r" );
    }
    else {
        message("Unable to write numbers.txt");
    }
}