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

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()         do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode()        do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set US_Trigger aliases
#define US_Trigger_TRIS                 TRISBbits.TRISB6
#define US_Trigger_LAT                  LATBbits.LATB6
#define US_Trigger_PORT                 PORTBbits.RB6
#define US_Trigger_WPU                  WPUBbits.WPUB6
#define US_Trigger_OD                   ODCONBbits.ODCB6
#define US_Trigger_ANS                  ANSELBbits.ANSB6
#define US_Trigger_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define US_Trigger_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define US_Trigger_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define US_Trigger_GetValue()           PORTBbits.RB6
#define US_Trigger_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define US_Trigger_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define US_Trigger_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define US_Trigger_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define US_Trigger_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define US_Trigger_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define US_Trigger_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define US_Trigger_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set Box_Switch aliases
#define Box_Switch_TRIS                 TRISBbits.TRISB7
#define Box_Switch_LAT                  LATBbits.LATB7
#define Box_Switch_PORT                 PORTBbits.RB7
#define Box_Switch_WPU                  WPUBbits.WPUB7
#define Box_Switch_OD                   ODCONBbits.ODCB7
#define Box_Switch_ANS                  ANSELBbits.ANSB7
#define Box_Switch_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define Box_Switch_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define Box_Switch_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define Box_Switch_GetValue()           PORTBbits.RB7
#define Box_Switch_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define Box_Switch_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define Box_Switch_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define Box_Switch_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define Box_Switch_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define Box_Switch_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define Box_Switch_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define Box_Switch_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)

// get/set US_Echo aliases
#define US_Echo_TRIS                 TRISCbits.TRISC6
#define US_Echo_LAT                  LATCbits.LATC6
#define US_Echo_PORT                 PORTCbits.RC6
#define US_Echo_WPU                  WPUCbits.WPUC6
#define US_Echo_OD                   ODCONCbits.ODCC6
#define US_Echo_ANS                  ANSELCbits.ANSC6
#define US_Echo_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define US_Echo_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define US_Echo_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define US_Echo_GetValue()           PORTCbits.RC6
#define US_Echo_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define US_Echo_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define US_Echo_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define US_Echo_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define US_Echo_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define US_Echo_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define US_Echo_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define US_Echo_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set PIR_Sensor aliases
#define PIR_Sensor_TRIS                 TRISCbits.TRISC7
#define PIR_Sensor_LAT                  LATCbits.LATC7
#define PIR_Sensor_PORT                 PORTCbits.RC7
#define PIR_Sensor_WPU                  WPUCbits.WPUC7
#define PIR_Sensor_OD                   ODCONCbits.ODCC7
#define PIR_Sensor_ANS                  ANSELCbits.ANSC7
#define PIR_Sensor_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define PIR_Sensor_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define PIR_Sensor_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define PIR_Sensor_GetValue()           PORTCbits.RC7
#define PIR_Sensor_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define PIR_Sensor_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define PIR_Sensor_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define PIR_Sensor_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define PIR_Sensor_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define PIR_Sensor_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define PIR_Sensor_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define PIR_Sensor_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)

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