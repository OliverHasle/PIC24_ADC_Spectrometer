/**
  MCCP1 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    mccp1.c

  @Summary
    This is the generated driver implementation file for the MCCP1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for MCCP1. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ128GL306
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB             :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "pwm.h"
#include "globals.h"
#include "stdio.h"

extern int data_buf[VIDEO_SIGNAL_LENGTH]; 
extern volatile bool spectrum_done; 
extern volatile int16_t loopCounter;
extern volatile int conversions; 

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/


/**
  Section: Driver Interface
*/


void setupPWM (void)
{
    TRISDbits.TRISD1 = 0;  // Set RD1 to output for START pulse
    TRISDbits.TRISD8 = 0; 
    
    CCP1CON1Lbits.CCPON   = 0;       // Disable CCP1
    CCP1CON1Lbits.CCPSIDL = 1;       // continue in idle
    CCP1CON1Lbits.CCPSLP  = 1;       // continue in Sleep
    CCP1CON1Lbits.TMRSYNC = 1;       // Synchronize to base clock
    CCP1CON1Lbits.CLKSEL  = 0b000;   // Use system clock TODO: should thi sbe clock 2?
    CCP1CON1Lbits.TMRPS   = 0b00;    // 1:1 prescaler
    CCP1CON1Lbits.T32     = 0;       // 16 bit Timer
    CCP1CON1Lbits.CCSEL   = 0;       // Ouptut mode
    CCP1CON1Lbits.MOD     = 0b0101;   // Dual Edge Buffered Compare (PWM) mode
   
    CCP1CON1Hbits.OPSSRC  = 0;       // Postcaler scales timer interrupt events
    CCP1CON1Hbits.RTRGEN  = 1;       // Retrigger possible
    CCP1CON1Hbits.OPS     = 0;       // interrupt every event
    CCP1CON1Hbits.TRIGEN  = 0;       // Disable triggered operation
    CCP1CON1Hbits.ONESHOT = 0;       // ONESHOT mode disabled
    CCP1CON1Hbits.ALTSYNC = 0;       // Module sync output signal is time base reset event
    CCP1CON1Hbits.SYNC    = 0;       // no external synchronization (TODO: what does that mean?)
    
    CCP1CON2Lbits.PWMRSEN = 1;       // PWM reenables itself
    CCP1CON2Lbits.ASDGM   = 1;       // Shutdown only on Timer Base Reset or rollover
    CCP1CON2Lbits.SSDG    = 0;       // Normal software shutdown mode 
    CCP1CON2Lbits.ASDG    = 0;       // Disable all auto shutdown events for ports 0..7
    
    CCP1CON2Hbits.OENSYNC = 1;       // Update only on rollover or event
    CCP1CON2Hbits.OCFEN   = 0;       // OCF pin is not under CCP control
    CCP1CON2Hbits.OCEEN   = 0;       // OCE pin is not under CCP control
    CCP1CON2Hbits.OCDEN   = 0;       // OCD pin is not under CCP control
    CCP1CON2Hbits.OCCEN   = 0;       // OCC pin is not under CCP control
    CCP1CON2Hbits.OCBEN   = 0;       // OCB pin is not under CCP control
    CCP1CON2Hbits.OCAEN   = 1;       // OCA pin is under CCP control
    CCP1CON2Hbits.ICGSM   = 0b10;    // Falling edge disables all future capture events - TODO: What does that mean?
    CCP1CON2Hbits.AUXOUT  = 0;       // Disable auxilliary output signal
    CCP1CON2Hbits.ICS     = 0;       // Capture source 1 - Should not matter? 

    CCP1CON3Lbits.DT      = 0;       // No dead time
 
    CCP1CON3Hbits.OETRIG  = 0;       // PWM Dead-Time select bin mode Normal
    CCP1CON3Hbits.OSCNT   = 0;       // No extended one-shot trigger event
    CCP1CON3Hbits.OUTM    = 0;       // PWM output mode is Steerable Single Output
    CCP1CON3Hbits.POLACE  = 0;       // Output pins OC1A, OC1C, OC1E is active-high
    CCP1CON3Hbits.POLBDF  = 0;       // Output pins OC1B, OC1D, OC1F is active-high
    CCP1CON3Hbits.PSSACE  = 0;       // Pins A, C, E in high impedance mode on Shutdown event
    CCP1CON3Hbits.PSSBDF  = 0;       // Pins B, D, F in high impedance mode on Shutdown event
    
    CCP1STATL             = 0;       // Clear all status bits
    
    CCP1TMRL              = 0;       // Initialize timer
    CCP1PRL               = 200;       // Timer period 66
            
    CCP1RA                = 0;       // Edge aligned
    CCP1RB                = 100;       // falling edge after 200/2 cycles 33
    
    //IEC3bits.CCP1IE       = 1;       // CCP Interrupt fired on FALLING edge
    IEC0bits.CCT1IE       = 1;       // CCT Interrupt fired on RISING edge
    //IFS3bits.CCP1IF       = 0;       // Reset interrupt state
    IFS0bits.CCT1IF       = 0; 
    IPC0bits.CCT1IP       = 5; 
    //IPC15bits.CCP1IP      = 5; 
 }
/*
void __attribute__((interrupt, shadow,no_auto_psv)) _CCP1Interrupt(void) {  // FALLING edge
    // During the falling edge, we do all setup 
    IFS3bits.CCP1IF = false; // Reset interrupt state

    // end sampling process before clock toggle, ie. immediately. This gives us the analog value for the PREVIOUS iteration: 
    if (spectrum_done) {
        //LATDbits.LATD8 = 1; 
        return; // this means that UART isn't done writing the data and we don't need to do anything here
    }
    if (loopCounter == 0) {    
        LATDbits.LATD1 = 1; // START PULSE HIGH 
        LATDbits.LATD8 = 1; 
    } else if (loopCounter == START_PULSE_LEN_CYC) {
        LATDbits.LATD1 = 0; 
    } else if (loopCounter >= START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET && 
               loopCounter <  START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET + VIDEO_SIGNAL_LENGTH - 1) {
        
        //while (!AD1CON1bits.DONE) {} ; // wait for data to become ready
        data_buf[loopCounter - (START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET) ] = ADC1BUF0; 

        //LATDbits.LATD8 = 1; 
    } else if (loopCounter >= START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET + VIDEO_SIGNAL_LENGTH - 1){
        LATDbits.LATD8 = 0; 
        spectrum_done = true; 
        loopCounter = -1;
    }
    loopCounter++;
}
 * */

void __attribute__((interrupt,shadow,no_auto_psv)) _CCT1Interrupt(void) {  // RISING edge
    // during the rising edge we only trigger the ADC
    //prinft("Interrupt CCT1 fired"); 
    //LATDbits.LATD8 = 0; 
    AD1CON1bits.SAMP = 0;
    IFS0bits.CCT1IF  = false; 
    int oldData = ADC1BUF0; 

    if (spectrum_done) {
        //LATDbits.LATD8 = 1; 
        return; // this means that UART isn't done writing the data and we don't need to do anything here
    }
    if (loopCounter == 0) {    
        LATDbits.LATD1 = 1; // START PULSE HIGH 
    } else if (loopCounter == START_PULSE_LEN_CYC) {
        LATDbits.LATD1 = 0; 
    } else if (loopCounter >= START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET && 
               loopCounter <  START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET + VIDEO_SIGNAL_LENGTH - 1) {
        LATDbits.LATD8 = 1; 
        //while (!AD1CON1bits.DONE) {} ; // wait for data to become ready
        data_buf[loopCounter - (START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET) ] = oldData; 
        //data_buf[loopCounter - (START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET) ] = loopCounter;  

        //LATDbits.LATD8 = 1; 
    } else if (loopCounter >= START_PULSE_LEN_CYC + VIDEO_ENABLE_OFFSET + VIDEO_SIGNAL_LENGTH - 1){
        LATDbits.LATD8 = 0; 
        spectrum_done = true; 
        loopCounter = -1;
    }
    loopCounter++;
}

void enablePWM(void) {
    CCP1CON1Lbits.CCPON   = 0x1;     //Enabling CCP
}


/**
 End of File
*/
