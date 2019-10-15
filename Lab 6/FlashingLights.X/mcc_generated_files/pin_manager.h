/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16F18346
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RedLight aliases
#define RedLight_TRIS                 TRISBbits.TRISB4
#define RedLight_LAT                  LATBbits.LATB4
#define RedLight_PORT                 PORTBbits.RB4
#define RedLight_WPU                  WPUBbits.WPUB4
#define RedLight_OD                   ODCONBbits.ODCB4
#define RedLight_ANS                  ANSELBbits.ANSB4
#define RedLight_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RedLight_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RedLight_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RedLight_GetValue()           PORTBbits.RB4
#define RedLight_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RedLight_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RedLight_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define RedLight_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define RedLight_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define RedLight_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define RedLight_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define RedLight_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set GreenLight aliases
#define GreenLight_TRIS                 TRISBbits.TRISB5
#define GreenLight_LAT                  LATBbits.LATB5
#define GreenLight_PORT                 PORTBbits.RB5
#define GreenLight_WPU                  WPUBbits.WPUB5
#define GreenLight_OD                   ODCONBbits.ODCB5
#define GreenLight_ANS                  ANSELBbits.ANSB5
#define GreenLight_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define GreenLight_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define GreenLight_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define GreenLight_GetValue()           PORTBbits.RB5
#define GreenLight_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define GreenLight_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define GreenLight_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define GreenLight_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define GreenLight_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define GreenLight_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define GreenLight_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define GreenLight_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/