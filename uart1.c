/**
  UART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated driver implementation file for the UART1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for UART1.
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
#include <xc.h>
#include "uart1.h"

/**
  Section: UART1 APIs
*/

void UART1_Initialize()
{
/**    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    
    RPINR18bits.U1RXR = 0x001E;    //RF2->UART1:U1RX
    RPOR8bits.RP16R = 0x0003;      //RF3->UART1:U1TX
    // NOTE: the UART->USB converter sits at pins RF2/RF3. Other pins are available for UART communication through the headers. 
    
    U1MODEbits.UARTEN     = 1;     // Disable UART
 // U1MODEbits.UFRZ       = 0;     // don't freeze in Debug mode
    U1MODEbits.USIDL      = 0;     // Continue in Idle mode 
    U1MODEbits.IREN       = 0;     // Disable IrDA
    U1MODEbits.RTSMD      = 0;     // U1RTS in Flow Control mode
 // U1MODEbits.ALTIO      = 0;     // U1 uses U1TX and U1RX pins, not alternative
    U1MODEbits.UEN        = 0b00;  // ?? 
    U1MODEbits.WAKE       = 0;     // Disable wake up on sleep
    U1MODEbits.LPBACK     = 0;     // Loopback mode disabled
    U1MODEbits.ABAUD      = 0;     // Disable automatic baud rate measurement
    U1MODEbits.RXINV      = 0;     // RX idle state is "1"
    U1MODEbits.BRGH       = 1;     // High baud rate
    U1MODEbits.PDSEL      = 0b00;  // 8 bit data, no parity
    U1MODEbits.STSEL      = 0;     // 1 stop bit
            
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x00;
    
    // Acc tbl. 21-2 UART manual, for FCY = 16 MHz and 115200 use 
    // NOTE: use table for BRGH = 1!! ( High baud rate)
    U1BRG = 34; 
    
    
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1STAbits.UTXEN = 1;
}

//void UART2_Initialize()
//{
/**    
     Set the UART2 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
//    RPINR19bits.U2RXR = 0x001E;    //RF2->UART2:U2RX             TBD: make sure 0x001E is the value to be set here
//    RPOR8bits.RP16R   = 0x0003;      //RF3->UART2:U2TX           TBD: make sure this setting is correct
//    // NOTE: the UART->USB converter sits at pins RF2/RF3. Other pins are available for UART communication through the headers. 
    
//    U2MODEbits.UARTEN     = 1;     // Disable UART                  TBD: make sure 0x001E is the value to be set here
// // U2MODEbits.UFRZ       = 0;     // don't freeze in Debug mode
//    U2MODEbits.USIDL      = 0;     // Continue in Idle mode 
//    U2MODEbits.IREN       = 0;     // Disable IrDA
//    U2MODEbits.RTSMD      = 0;     // U1RTS in Flow Control mode
// // U2MODEbits.ALTIO      = 0;     // U1 uses U1TX and U1RX pins, not alternative
//    U2MODEbits.UEN        = 0b00;  // ?? 
//    U2MODEbits.WAKE       = 0;     // Disable wake up on sleep
//    U2MODEbits.LPBACK     = 0;     // Loopback mode disabled
//    U2MODEbits.ABAUD      = 0;     // Disable automatic baud rate measurement
//    U2MODEbits.RXINV      = 0;     // RX idle state is "1"
//    U2MODEbits.BRGH       = 1;     // High baud rate
//    U2MODEbits.PDSEL      = 0b00;  // 8 bit data, no parity
//    U2MODEbits.STSEL      = 0;     // 1 stop bit
            
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
//    U2STA = 0x00;
    
    // Acc tbl. 21-2 UART manual, for FCY = 16 MHz and 115200 use 
    // NOTE: use table for BRGH = 1!! ( High baud rate)
//    U2BRG = 34; 
    
    
//    U2MODEbits.UARTEN = 1;   // enabling UART ON bit
//    U2STAbits.UTXEN = 1;
//}

uint8_t UART1_Read(void)
{
    while(!(U1STAbits.URXDA == 1))
    {
        
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }
    
    return U1RXREG;
}

void UART1_Write(uint8_t txData)
{
    while(U1STAbits.UTXBF == 1)
    {
        
    }

    U1TXREG = txData;    // Write the data byte to the USART.
}

bool UART1_IsRxReady(void)
{
    return U1STAbits.URXDA;
}

bool UART1_IsTxReady(void)
{
    return ((!U1STAbits.UTXBF) && U1STAbits.UTXEN );
}

bool UART1_IsTxDone(void)
{
    return U1STAbits.TRMT;
}

int __attribute__((__section__(".libc.write"))) write(int handle, void *buffer, unsigned int len) 
{
    unsigned int i;

    for (i = len; i; --i)
    {
        UART1_Write(*(char*)buffer++);
    }
    return(len);
}