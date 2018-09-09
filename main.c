/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC12LF1572
        Driver Version    :  2.00
*/

#include "mcc_generated_files/mcc.h"
#include "aj_hal.h"

#define ledadc


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    //Set initial LED Values
    uint16_t LED1 = 100;
    uint16_t LED2 = 200;
    uint16_t LED3 = 0;
    
    //Setup PWM Channels and set initial values
    setupLEDs(2000);
    setLEDs(LED1,LED2,LED3);
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    //Start PWM Channels
    startLEDs();
    
    uint16_t adc_result = 0;
    bool buttons[3] = {0,0,0};
    
    while (1)
    {
        int i;
        //For loop to reduce the frequency of serial transmit below
        //otherwise it slows down the LED cycle.
        for(i=0 ; i<10 ; i++)
        {
            //Read ADC for Button Press
            adc_result = ADC1_GetConversion(channel_AN0);
            
            //LED Control
            //#define ledadc to control LEDs with Buttons
#ifdef ledadc
            checkButtons(buttons);
            LED1 += buttons[0] * 5;
            LED2 += buttons[1] * 5;
            LED3 += buttons[2] * 5;
#else
            LED1+=5;
            LED2+=5;
            LED3+=5;
#endif
            
            //Limit LEDs to duty cycle of 300/PERIOD.
            //In the case PERIOD=2000, 300 gives approx max brightness.
            if(LED1 > 300) LED1 = 0;
            if(LED2 > 300) LED2 = 0;
            if(LED3 > 300) LED3 = 0;

            //Update PWM Modules
            setLEDs(LED1,LED2,LED3);

            //PWM Update Delay
            //Set speed of LED cycle
            __delay_ms(10);
        }
        
        //Serial Transmit
        enableSerial();
        //Send Serial Data
        EUSART_Write((uint8_t)((adc_result >> 8) & 0xFF));
        EUSART_Write(buttons[0] | buttons[1] << 1 | buttons[2] << 2);
        //EUSART_Write((uint8_t)(adc_result & 0xFF //Additional 8-bits
        //Wait for Serial TX to finish
        while(!EUSART_is_tx_done());
        
        //If the same value sent is received, then send a 0x55 ack.
        if(EUSART_RxCheck())
        {
            uint8_t receive = EUSART_Read();
            if(receive == ((adc_result >> 8) & 0xFF))
            {
                EUSART_Write(0x55);
            }
            while(!EUSART_is_tx_done());
        }
        //Disable UART (for ADC to work)
        disableSerial();
    }
}
/**
 End of File
*/