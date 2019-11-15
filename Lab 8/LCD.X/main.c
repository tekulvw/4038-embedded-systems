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
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16F18346
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
#include "lcd.h"
#include <string.h>

const char* NAME_WT = "Will Tekulve";
const char* NAME_SC = "Steven Campbell";

/*
 * Touchpad PIN8 = NA  // ROW 0
 *          PIN7 = RB4 // ROW 1
 *          PIN6 = RA2 // ROW 2
 *          PIN5 = RA1 // ROW 3
 *          PIN4 = RA0 // COL 0
 *          PIN3 = RA5 // COL 1
 *          PIN2 = RA4 // COL 2
 *          PIN1 = RB7 // COL 3
 */

const char* TOUCHPAD[4][4] = {
    {"0", "0", "0", "0"},
    {"4", "5", "6", "B"},
    {"7", "8", "9", "C"},
    {"*", "0", "#", "D"}
};

/*
                         Main application
 */

void main(void)
{    
    // initialize the device
    SYSTEM_Initialize();

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
    
    LCD_Initialize();
    
    LCDPutStr(NAME_WT);
    LCDLine2();
    LCDPutStr(NAME_SC);
    
    __delay_ms(2000);
    DisplayClr();
    
    char activeCols = 0;
    int currentRow = 0;

    while (1)
    {
        for (int i = 1; i < 4; i++) {
            switch (i) {
                case 1:
                    ROW1_SetHigh();
                case 2:
                    ROW2_SetHigh();
                case 3:
                    ROW3_SetHigh();
            }
            __delay_ms(1000);

            activeCols |= (1 & COL0_GetValue()) << 0;
            activeCols |= (1 & COL1_GetValue()) << 1;
            activeCols |= (1 & COL2_GetValue()) << 2;
            activeCols |= (1 & COL3_GetValue()) << 3;
            
            if (COL0_GetValue() == HIGH) {
                LCDPutStr("4");
            }

            switch (i) {
                case 1:
                    ROW1_SetLow();
                case 2:
                    ROW2_SetLow();
                case 3:
                    ROW3_SetLow();
            }
            for (int j = 0; j < 4; j++) {
                if (activeCols & (1 << j)) {
                    if (i == 3 && j == 3) {
                        DisplayClr();
                    } else {
                        char* val = TOUCHPAD[i][j];
                        LCDPutStr(val);
                    }
                }
            }
            activeCols = 0;
            __delay_ms(1000);
        }
    }
}

/**
 End of File
*/