#include <stdlib.h>
#include <stdbool.h>

#include "led.h"
#include "game.h"
#include "random.h"
#include "machine.h"

static unsigned game_level = 2;
static unsigned top_score = 1;
static enum Led *moves = NULL;
static unsigned move_current = 0;

void free_moves() {
    if( moves != NULL ) {
        free( moves);
    }
}

void new_level()
{
    machine_warn_user();
    free_moves();
    if( game_level > top_score) {
        top_score = game_level;
    }
    game_level++;
    move_current = 0;
    moves = malloc( sizeof(enum Led) * game_level );
    for( unsigned i = 0; i < game_level; i++ ) {
        moves[i] = random_value(3);
        machine_led_flash(moves[i]);
    }
}

void game_initialise()
{
   game_level = 1;
   new_level();
}

void game_destroy() {
    free_moves();
    game_level = 0;
}

void game_display()
{
    if( moves != 0 ) {
        free( moves);
    }
}

bool game_check_it( enum Led led ) {
    bool result = 0;
    if( moves[move_current] == led) {
        move_current++;
        result = 1;
    }
    else {
        machine_warn_user();
        result = 0;
    }
    if( ! result ) {
        game_level--;
        new_level();
    }
    if( move_current >= game_level) {
        new_level();
    }
}

void game_key(char key) {
    switch( key) {
        case 'g':
            game_check_it( green );
            break;
        case 'r':
            game_check_it( red );
          break;
        case 'y':
            game_check_it( yellow );
          break;
    }
}

bool game_status()
{

}

unsigned game_topscore()
{
    return top_score;
}