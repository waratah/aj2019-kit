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

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "aj_hal.h"

//#define ledadc

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    uint16_t LED1 = 100;
    uint16_t LED2 = 200;
    uint16_t LED3 = 0;
    
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
    
    startLEDs();
    
    uint16_t adc_result = 0;
    
    while (1)
    {
        int i;
        //Added for loop to reduce the frequency of serial transmit below
        //otherwise it slows down the LED cycle.
        for(i=0 ; i<10 ; i++)
        {
            //Read ADC for Button Press
            adc_result = ADC1_GetConversion(channel_AN0);
            
            //LED Control
            //uncomment #define ledadc at top to control LEDs with Buttons
            //commant the #define to unconditionally increment LEDs
#ifdef ledadc
            if(adc_result >= 0x99C0-10 && adc_result <= 0x99C0+10)LED1+=5;
            if(adc_result >= 0x8DC0-10 && adc_result <= 0x8DC0+10)LED2+=5;
            if(adc_result >= 0x7A40-10 && adc_result <= 0x7A40+10)LED3+=5;
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
        EUSART_Write((uint8_t)(adc_result & 0xFF));
        //Wait for Serial to finish and disable UART (for ADC to work)
        while(!EUSART_is_tx_done());
        
        //commented out the receive for testing.
        //If the same (first) value sent is received, then send a 0x55 ack.
        //The EUSART_Read and while(!EUSART_is_tx_done) functions are blocking
//        uint8_t receive = EUSART_Read();
//        if(receive == (adc_result & 0xFF))
//        {
//            EUSART_Write(0x55);
//        }
//        while(!EUSART_is_tx_done());
        disableSerial();
    }
}
/**
 End of File
*/