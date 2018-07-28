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
        Device            :  PIC12F1572
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

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    uint16_t LED1 = 100; //Middle
    uint16_t LED2 = 200; //Left
    uint16_t LED3 = 0;   //Right
    uint16_t PERIOD = 2000;
    
    PWM1_Stop();
    PWM2_Stop();
    PWM3_Stop();
    
    PWM1_PhaseSet(0);
    PWM1_PeriodSet(PERIOD);
    PWM1_DutyCycleSet(LED1);
    
    PWM2_PhaseSet(0);
    PWM2_PeriodSet(PERIOD);
    PWM2_DutyCycleSet(LED2);
    
    PWM3_PhaseSet(0);
    PWM3_PeriodSet(PERIOD);
    PWM3_DutyCycleSet(LED3);
    
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
    
    PWM1_Start();
    PWM2_Start();
    PWM3_Start();
    
    while (1)
    {
        //Read ADC for Button Press
        uint16_t adc_result = 0;
        adc_result = ADC1_GetConversion(channel_AN0);
        
        //Serial Transmit
        RCSTAbits.SPEN = 1; //Enable UART
        __delay_ms(1); //Wait for High Level before sending
        //Send Serial Data
        EUSART_Write((uint8_t)(adc_result & 0xFF));
        EUSART_Write((uint8_t)((adc_result >> 8) & 0xFF));
        //Wait for Serial to finish and disable UART (for ADC to work)
        while(!EUSART_is_tx_done());
        
        uint8_t receive = EUSART_Read();
        if(receive == (adc_result & 0xFF))
        {
            EUSART_Write(0x55);
        }
        while(!EUSART_is_tx_done());
        RCSTAbits.SPEN = 0; //Disable UART
        
        //LED Control
        if(adc_result >= 0x99C0-10 && adc_result <= 0x99C0+10)LED1+=5;
        if(adc_result >= 0x8DC0-10 && adc_result <= 0x8DC0+10)LED2+=5;
        if(adc_result >= 0x7A40-10 && adc_result <= 0x7A40+10)LED3+=5;
        
        if(LED1 > 300) LED1 = 0;
        if(LED2 > 300) LED2 = 0;
        if(LED3 > 300) LED3 = 0;
        
        PWM1_DutyCycleSet(LED1);
        PWM1_LoadBufferSet();
        PWM2_DutyCycleSet(LED2);
        PWM2_LoadBufferSet();
        PWM3_DutyCycleSet(LED3);
        PWM3_LoadBufferSet();
        
        //Main Loop Delay
        __delay_ms(10);
    }
}
/**
 End of File
*/