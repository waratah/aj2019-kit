#include <stdbool.h>

void game_initialise();
void game_destroy();
void game_display();
void game_wait();
bool game_status();
bool game_check_it( enum Led led );
void game_key(char key);
unsigned game_topscore();