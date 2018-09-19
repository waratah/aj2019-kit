#include <stdbool.h>
#include <stdint.h>
#include "mcc_generated_files/mcc.h"

//Contains HAL-ish functions to further abstract buttons, LEDs and Serial

//################### SERIAL #####################
//enableSerial - Enable TX channel
void enableSerial ()
{
    RCSTAbits.SPEN = 1; //Enable UART (sets RX/TX Pins as digital serial)
    __delay_ms(1); //Wait for idle line
}

//disableSerial - Disable RX channel
void disableSerial ()
{
    __delay_ms(1); //Wait for any stop bits
    RCSTAbits.SPEN = 0; //Disable UART (releases RX/TX Pins from digital)
}

//EUSART_RxCheck - Check for received bytes
bool EUSART_RxCheck ()
{
    return PIR1bits.RCIF;
}

//################### BUTTONS ####################
#define B_7 162.1 // 111
#define B_6 151.5 // 110
#define B_5 142.6 // 101
#define B_4 128.4 // 100
#define B_3 106.3 // 011
#define B_2 79.7 // 010
#define B_1 51.9 // 001
#define B_0 0.0  // 000

#define B_7up (int)(B_7 + (B_7-B_6))
#define B_67 (int)(B_6 + (B_7-B_6)/2)
#define B_56 (int)(B_5 + (B_6-B_5)/2)
#define B_45 (int)(B_4 + (B_5-B_4)/2)
#define B_34 (int)(B_3 + (B_4-B_3)/2)
#define B_23 (int)(B_2 + (B_3-B_2)/2)
#define B_12 (int)(B_1 + (B_2-B_1)/2)
#define B_01 (int)(B_0 + (B_1-B_0)/2)


void checkButtons (bool buttons[]) //buttons[3]
{
    disableSerial();
    uint16_t adc = (ADC1_GetConversion(channel_AN0) >> 8) & 0xFF;
    uint8_t i = 0;
    for(i = 0; i < 3; i++)
    {
        buttons[i] = 0;
    }
    
    if(adc <= B_01)
    {
        //No Button
    }
    else if(adc > B_01 && adc <= B_12)
    {
        //Right Button
        buttons[0] = 1;
    }
    else if(adc > B_12 && adc <= B_23)
    {
        //Middle Button
        buttons[1] = 1;
    }
    else if(adc > B_23 && adc <= B_34)
    {
        //Right,Middle Button
        buttons[0] = 1;
        buttons[1] = 1;
    }
    else if(adc > B_34 && adc <= B_45)
    {
        //Left Button
        buttons[2] = 1;
    }
    else if(adc > B_45 && adc <= B_56)
    {
        //Right,Left Button
        buttons[0] = 1;
        buttons[2] = 1;
    }
    else if(adc > B_56 && adc <= B_67)
    {
        //Left,Middle Button
        buttons[1] = 1;
        buttons[2] = 1;
    }
    else if(adc > B_67 && adc <= B_7up)
    {
        //Right,Middle,Left Button
        buttons[0] = 1;
        buttons[1] = 1;
        buttons[2] = 1;
    }
}

//################### LED PWM ####################
void stopLEDs ()
{
    PWM1_Stop();
    PWM2_Stop();
    PWM3_Stop();
}

void startLEDs ()
{
    PWM1_Start();
    PWM2_Start();
    PWM3_Start();
}

void setLEDs (uint16_t led1, uint16_t led2, uint16_t led3)
{
    PWM1_DutyCycleSet(led1);
    PWM1_LoadBufferSet();
    PWM2_DutyCycleSet(led2);
    PWM2_LoadBufferSet();
    PWM3_DutyCycleSet(led3);
    PWM3_LoadBufferSet();
}

void setupLEDs (uint16_t period)
{
    const uint16_t initial_duty = 0;
    const uint16_t initial_phase = 0;
    stopLEDs();
    
    PWM1_PhaseSet(initial_phase);
    PWM1_PeriodSet(period);
    
    PWM2_PhaseSet(initial_phase);
    PWM2_PeriodSet(period);
    
    PWM3_PhaseSet(initial_phase);
    PWM3_PeriodSet(period);
    
    setLEDs(initial_duty, initial_duty, initial_duty);
}

void setLED (uint16_t duty, uint8_t LEDno)
{
    if(LEDno == 1)
    {
        PWM1_DutyCycleSet(duty);
        PWM1_LoadBufferSet();
    }
    else if(LEDno == 2)
    {
        PWM2_DutyCycleSet(duty);
        PWM2_LoadBufferSet();
    }
    else if(LEDno == 3)
    {
        PWM3_DutyCycleSet(duty);
        PWM3_LoadBufferSet();
    }
}

//#################### SLEEP #####################
//wakeHandler - interrupt handler for Button, checks for reset/wake
void wakeHandler ()
{
    
}
//setupButtonInterrupt - sets wakeHandler as interrupt handler, set GIE, PIE
void setupButtonInterrupt ()
{
    IOCAF0_SetInterruptHandler(wakeHandler);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}
//goToSleep - Sets RA0 digital in, Turn off LEDs, Serial off, sleep, wake-up nop
void goToSleep ()
{
    //Set RA0 for IOC
    disableSerial(); //Not at all necessary, can be enabled.
    ADCON0bits.ADON = 0; //Turn off ADC0 - Not necessary
    ANSELAbits.ANSA0 = 0; //Set RA0 as digital - IS necessary
    TRISAbits.TRISA0 = 1; //Set RA0 as input (already set)
    
    //Disable Peripherals for minimum Power
    setLEDs(0,0,0);
    
    //Sleep
    SLEEP();
    
    //Sleeping...
    
    //Wake Up NOP
    NOP();
    
    //Execute wakeHandler (IOC Interrupt Handler)
    
    TRISAbits.TRISA0 = 1; //Set RA0 as input (already set)
    ANSELAbits.ANSA0 = 0; //Set RA0 as digital - IS necessary
    ADCON0bits.ADON = 0; //Turn off ADC0 - Not necessary
}