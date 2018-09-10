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
void checkButtons (bool buttons[]) //buttons[3]
{
    disableSerial();
    uint16_t adc = (ADC1_GetConversion(channel_AN0) >> 8) & 0xFF;
    uint8_t i = 0;
    for(i = 0; i < 3; i++)
    {
        buttons[i] = 0;
    }
    
    if(adc <= 23)
    {
        //No Button
    }
    else if(adc > 23 && adc <= 63)
    {
        //Left Button
        buttons[2] = 1;
    }
    else if(adc > 63 && adc <= 91)
    {
        //Middle Button
        buttons[1] = 1;
    }
    else if(adc > 91 && adc <= 112)
    {
        //Left,Middle Button
        buttons[2] = 1;
        buttons[1] = 1;
    }
    else if(adc > 112 && adc <= 128)
    {
        //Right Button
        buttons[0] = 1;
    }
    else if(adc > 128 && adc <= 140)
    {
        //Left,Right Button
        buttons[2] = 1;
        buttons[0] = 1;
    }
    else if(adc > 140 && adc <= 151)
    {
        //Right,Middle Button
        buttons[1] = 1;
        buttons[0] = 1;
    }
    else if(adc > 151 && adc <= 200)
    {
        //Left,Middle,Right Button
        buttons[2] = 1;
        buttons[1] = 1;
        buttons[0] = 1;
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