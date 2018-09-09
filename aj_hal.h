/* 
 * File:   aj_hal.h
 * Author: Lachlan
 *
 * Created on September 9, 2018, 5:10 PM
 */

#ifndef AJ_HAL_H
#define	AJ_HAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

//################### BUTTONS ####################
//checkButtons - Disable Serial, Read the ADC (blocking), update buttons states.
//Pass in buttons[3] which contains boolean states (1=pressed) of each button.
void checkButtons (bool buttons[]); //buttons[3]

//################### LED PWM ####################
// setupLEDs - Turn off PWM Channels, set period, phase=0, duty=0
void setupLEDs (uint16_t period);
//stopLEDs - Turn off PWM Channels
void stopLEDs ();
//startLEDs - Turn on PWM Channels
void startLEDs ();
//setLEDs - Set duty cycle for all LEDs
void setLEDs (uint16_t led1, uint16_t led2, uint16_t led3);
//setLED - Set duty cycle for one LED. Pass led number (1-3)
void setLED (uint16_t duty, uint8_t LEDno);

//################### SERIAL #####################
//enableSerial - Enable TX channel
void enableSerial ();
//disableSerial - Disable RX channel
void disableSerial ();
//EUSART_RxCheck - Check for received bytes
bool EUSART_RxCheck ();

#ifdef	__cplusplus
}
#endif

#endif	/* AJ_HAL_H */

