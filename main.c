/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks.
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Fast Interrupt Test" - A high frequency periodic interrupt is generated
 * using a free running timer to demonstrate the use of the
 * configKERNEL_INTERRUPT_PRIORITY configuration constant.  The interrupt
 * service routine measures the number of processor clocks that occur between
 * each interrupt - and in so doing measures the jitter in the interrupt
 * timing.  The maximum measured jitter time is latched in the usMaxJitter
 * variable, and displayed on the LCD by the 'Check' as described below.
 * The fast interrupt is configured and handled in the timer_test.c source
 * file.
 *
 * "LCD" task - the LCD task is a 'gatekeeper' task.  It is the only task that
 * is permitted to access the LCD directly.  Other tasks wishing to write a
 * message to the LCD send the message on a queue to the LCD task instead of
 * accessing the LCD themselves.  The LCD task just blocks on the queue waiting
 * for messages - waking and displaying the messages as they arrive.  The LCD
 * task is defined in lcd.c.
 *
 * "Check" task -  This only executes every three seconds but has the highest
 * priority so is guaranteed to get processor time.  Its main function is to
 * check that all the standard demo tasks are still operational.  Should any
 * unexpected behaviour within a demo task be discovered the 'check' task will
 * write "FAIL #n" to the LCD (via the LCD task).  If all the demo tasks are
 * executing with their expected behaviour then the check task writes the max
 * jitter time to the LCD (again via the LCD task), as described above.
 */




#include <xc.h>

#include "config_bits.h"     // MCU configuration - for better readabilty in separate file. Not a header really, only pragmas

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <p24FJ128GL306.h>

#include "uart1.h"
#include "pwm.h"
#include "adc.h"
#include "globals.h"

/* A NOTE ON TIMING AND FREQUENCIES: 
 * 
 * The main oscillator runs at 32 MHz as configured - that's the maximum. 
 *
 * If changing any timing characteristics, remember to change the following parameters: 
 * 
 * uart1.c -> UART1_Initialize() -> U1BRG    <-- This changes the UART transmission rate. Currently 115200
 * adc.c   -> AD1CON3bits.ADCS               <-- This changes the ADCs clock. The ADC clock runs at max. 3,33 MHz, this must be adjusted accordingly
 * pwm.c   -> CCP1PRL and CCP1RB             <-- The PWM pin goes high/low after that clock cycle. This effectively defines the PWM frequency.
 * 
 */


volatile int data_buf[VIDEO_SIGNAL_LENGTH]; 
volatile bool spectrum_done  = false; 
volatile int16_t loopCounter = 0;
volatile int conversions     = 0; // to count how many ADC conversions actually were triggered

static const bool use_kiss = true;

static const uint8_t frameStartEnd      = KISS_FEND;
static const uint8_t frameStartEsc      = KISS_FESC;
static const uint8_t transposedFrameEnd = KISS_TFEND;
static const uint8_t transposedFrameEsc = KISS_TFESC;

static void setup( void )
{
       
    UART1_Initialize();         // UART initializes on pin RF2/RF3 for UART->USB conversion
    
    printf("Starting ADC on RB6\n"); 
    setupADC_Oneshot();         // Analog in is at RB6
    printf("Starting PWM on RB5. START signal on pin RD1.\n"); 
    setupPWM();                    // PWM RUNS ON PIN RB5, START on RD1
    
    printf("Timer and PWM initialized, ACD on\n");
    enablePWM();
}

bool uart2sentiSendCommand()
{
    /* KISS protocol, used to send data to SentiBoard */
    int ret = 0;    
    ret = printf("%x", frameStartEnd); /* Send 0xC0*/
    
    if(ret < 0)
    {
        return ret;
    }
    int userBytesWritten = 0;
    int dataLen = VIDEO_SIGNAL_LENGTH; //sizeof(data_buf);
    
    for (int i = 0; i < dataLen; i++)
    {
        if(data_buf[i] == KISS_FEND)
        {
            printf("%x", frameStartEsc);
            printf("%x", transposedFrameEnd);
            userBytesWritten++;
        }
        else if (data_buf[i] == KISS_FESC)
        {
            printf("%x", frameStartEsc);
            printf("%x", transposedFrameEsc);
            userBytesWritten++;
        }
        else
        {
            printf("%x", data_buf[i]);
            userBytesWritten += sizeof(data_buf[i]);
        }
    }
    printf("%x", frameStartEnd); /* Send 0xC0*/
    return userBytesWritten;
}

static void loop_UART( void )
{
    while (1) {
        
        if (spectrum_done) {
            if (use_kiss)
            {
                uart2sentiSendCommand();
            }
            else
            {
                for (int i = 0; i < VIDEO_SIGNAL_LENGTH; i++) {
                    printf("%d,", data_buf[i]); 
                }
                printf("\n");             
            }
            spectrum_done = false; 
            //IEC1bits.T5IE = 1;
        }
    }
}

int main( void )
{
	/* Configure any hardware required for this demo. */
	setup();

	loop_UART(); 

	return 0;
}
