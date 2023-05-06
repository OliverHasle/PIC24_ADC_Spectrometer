


#include <p24FJ128GL306.h>

#include "globals.h"
#include "adc.h"

extern volatile int data_buf[VIDEO_SIGNAL_LENGTH]; //Why is that here?
extern volatile int16_t loopCounter;
extern volatile int conversions; 

void setupADC_Oneshot(void) {
    // Configure AD1CON1 - AD1CON5
    //TRISAbits.TRISA0 = 1; // select mode INPUT for RB6
    //ANSELAbits.ANSA0 = 1; 
    TRISBbits.TRISB6 = 1; 
    ANSBbits.ANSB6 = 1; // input pin 
    AD1CON1bits.ADON        = 0;        // Turn AD1 OFF
    AD1CON1bits.ADSIDL      = 1;        // Continue in Idle
    AD1CON1bits.DMABM       = 0;        // Extended buffer mode: Conversion values are stored in the order which they were converted
    AD1CON1bits.DMAEN       = 0;        // DMA disabled
    AD1CON1bits.MODE12      = 1;        // 12 bit mode enabled (else 10 bit)
    AD1CON1bits.FORM        = 0b00;     // absolute decimal result, unsigned, right-justified
    AD1CON1bits.SSRC        = 0b0000;   // Software-triggered
    AD1CON1bits.ASAM        = 1;        // Samle starts automatically after last conversion
    AD1CON1bits.SAMP        = 1;        // Sample-and-hold is holding
    AD1CON1bits.DONE        = 0;        // Not done
    
    AD1CON2bits.PVCFG       = 0b00;     // Reference Voltage = AVdd
    AD1CON2bits.NVCFG0      = 0;        // Reverence negative = AVss
//  AD1CON2bits.OFFCAL      = 0;        // Inputs of S&H are connected to normal inputs - TODO. what does that mean?
    AD1CON2bits.BUFREGEN    = 0;        // Enable AD Buffer
    AD1CON2bits.CSCNA       = 0;        // Scan inputs according to AD1CSSH / AD1CSSL 
    AD1CON2bits.BUFS        = 0;        // Clear status bit
    AD1CON2bits.SMPI        = 0;        // Increment DMA address every result
    AD1CON2bits.BUFM        = 0;        // Buffer fill mode in Split Buffer moden (??)
    AD1CON2bits.ALTS        = 0;        // Always uses channel input selects for sample A, channel B is inactive
    
    AD1CON3bits.ADRC        = 0;        // Clock derived from System Clock
    AD1CON3bits.EXTSAM      = 0;        // Readonly bit
    AD1CON3bits.PUMPEN      = 0;        // Disable Charge PUmp (??)
    AD1CON3bits.SAMC        = 0;        // Auto sample Time Select bits (0 Cycles)
    AD1CON3bits.ADCS        = 5;        // TAD
    
    AD1CON4                 = 0; 
    AD1CON5                 = 0; 
    
    AD1CHSbits.CH0NA        = 0b000;    // Negative refreence Sample A channel 0: Vss
    AD1CHSbits.CH0SA        = 6;        // select channel 6 as input
    
    AD1CSSH                 = 0;        // Disable scanning
    AD1CSSL                 = 0;        // Disable scanning
    AD1CHITH                = 0;        // Clear Scan Compare register High Word
    AD1CHITL                = 0;        // Clear Scan Compare register Low Word
    
  
       
//  AD1CTMENH               = 0;        // Disable CTMU for channels 16..32
//  AD1CTMENL               = 0;        // Disable CTMU for channels 0..15
    

    IEC0bits.AD1IE = 0; // do not enable interrupt yet. Will be done by Timer 5 on demand. 
    IFS0bits.AD1IF = 0; 
    
    // We initialized everything, start AD1: 
        
    AD1CON1bits.ADON        = 1;        // Turn AD1 ON      
}
