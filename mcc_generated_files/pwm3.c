/**
  PWM3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm3.c

  @Summary
    This is the generated driver implementation file for the PWM3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for PWM3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC12LF1572
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45 or later
        MPLAB             :  MPLAB X 4.15
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "pwm3.h"

/**
  Section: PWM3 APIs
*/

void PWM3_Initialize(void)
{
    // set the PWM3 to the options selected in the User Interface

     //PHIE disabled; DCIE disabled; OFIE disabled; PRIE disabled; 
    PWM3INTE = 0x00;

     //PHIF cleared; OFIF cleared; DCIF cleared; PRIF cleared; 
    PWM3INTF = 0x00;

     //PS No_Prescalar; CS FOSC; 
    PWM3CLKCON = 0x00;

     //LDS reserved; LDT disabled; LDA do_not_load; 
    PWM3LDCON = 0x00;

     //OFM independent_run; OFS reserved; OFO match_incrementing; 
    PWM3OFCON = 0x00;

     //PWM3PHH 0; 
    PWM3PHH = 0x00;

     //PWM3PHL 0; 
    PWM3PHL = 0x00;

     //PWM3DCH 0; 
    PWM3DCH = 0x00;

     //PWM3DCL 0; 
    PWM3DCL = 0x00;

     //PWM3PRH 0; 
    PWM3PRH = 0x00;

     //PWM3PRL 1; 
    PWM3PRL = 0x01;

     //PWM3OFH 0; 
    PWM3OFH = 0x00;

     //PWM3OFL 1; 
    PWM3OFL = 0x01;

     //PWM3TMRH 0; 
    PWM3TMRH = 0x00;

     //PWM3TMRL 0; 
    PWM3TMRL = 0x00;

     //MODE standard_PWM; POL active_hi; OE enabled; EN enabled; 
    PWM3CON = 0xC0;

}    


void PWM3_Start(void)
{
    PWM3CONbits.EN = 1;		
}

void PWM3_Stop(void)
{
    PWM3CONbits.EN = 0;		
}

bool PWM3_CheckOutputStatus(void)
{
    return (PWM3CONbits.OUT);		
}

void PWM3_LoadBufferSet(void)
{
    PWM3LDCONbits.LDA = 1;		
}

void PWM3_PhaseSet(uint16_t phaseCount)
{
    PWM3PHH = (phaseCount>>8);        //writing 8 MSBs to PWMPHH register
    PWM3PHL = (phaseCount);           //writing 8 LSBs to PWMPHL register
}

void PWM3_DutyCycleSet(uint16_t dutyCycleCount)
{
    PWM3DCH = (dutyCycleCount>>8);	//writing 8 MSBs to PWMDCH register
    PWM3DCL = (dutyCycleCount);	//writing 8 LSBs to PWMDCL register		
}

void PWM3_PeriodSet(uint16_t periodCount)
{
    PWM3PRH = (periodCount>>8);	//writing 8 MSBs to PWMPRH register
    PWM3PRL = (periodCount);	//writing 8 LSBs to PWMPRL register		
}

void PWM3_OffsetSet(uint16_t offsetCount)
{
    PWM3OFH = (offsetCount>>8);	//writing 8 MSBs to PWMOFH register
    PWM3OFL = (offsetCount);	//writing 8 LSBs to PWMOFL register		
}

uint16_t PWM3_TimerCountGet(void)
{
    return ((uint16_t)((PWM3TMRH<<8) | PWM3TMRL));       		
}

bool PWM3_IsOffsetMatchOccured(void)
{
    return (PWM3INTFbits.OFIF);		
}

bool PWM3_IsPhaseMatchOccured(void)
{
    return (PWM3INTFbits.PHIF);	
}

bool PWM3_IsDutyCycleMatchOccured(void)
{
    return (PWM3INTFbits.DCIF);		
}

bool PWM3_IsPeriodMatchOccured(void)
{
    return (PWM3INTFbits.PRIF);		
}

/**
 End of File
*/


