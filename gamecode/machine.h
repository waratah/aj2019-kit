void machine_initialise();
void machine_destroy();
void delay();
void machine_warn_user();
void machine_led_flash(enum Led led);
void machine_flag_seen(unsigned machineId);
void setDevice(unsigned device);
unsigned machine_connections();