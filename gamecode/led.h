void led_initialise();
void led_destroy();

enum Led {
    green = 0,
    yellow = 1,
    red = 2,
};

void led_set( enum Led colour, _Bool status );
void led_line(_Bool status);
