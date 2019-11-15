;*******************************************************************************
;                                                                              *
;    Microchip licenses this software to you solely for use with Microchip     *
;    products. The software is owned by Microchip and/or its licensors, and is *
;    protected under applicable copyright laws.  All rights reserved.          *
;                                                                              *
;    This software and any accompanying information is for suggestion only.    *
;    It shall not be deemed to modify Microchip?s standard warranty for its    *
;    products.  It is your responsibility to ensure that this software meets   *
;    your requirements.                                                        *
;                                                                              *
;    SOFTWARE IS PROVIDED "AS IS".  MICROCHIP AND ITS LICENSORS EXPRESSLY      *
;    DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING  *
;    BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS    *
;    FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL          *
;    MICROCHIP OR ITS LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL,         *
;    INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO     *
;    YOUR EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR    *
;    SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY   *
;    DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER      *
;    SIMILAR COSTS.                                                            *
;                                                                              *
;    To the fullest extend allowed by law, Microchip and its licensors         *
;    liability shall not exceed the amount of fee, if any, that you have paid  *
;    directly to Microchip to use this software.                               *
;                                                                              *
;    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF    *
;    THESE TERMS.                                                              *
;                                                                              *
;*******************************************************************************
;                                                                              *
;    Filename:                                                                 *
;    Date:                                                                     *
;    File Version:                                                             *
;    Author:                                                                   *
;    Company:                                                                  *
;    Description:                                                              *
;                                                                              *
;*******************************************************************************
;                                                                              *
;    Notes: In the MPLAB X Help, refer to the MPASM Assembler documentation    *
;    for information on assembly instructions.                                 *
;                                                                              *
;*******************************************************************************
;                                                                              *
;    Known Issues: This template is designed for relocatable code.  As such,   *
;    build errors such as "Directive only allowed when generating an object    *
;    file" will result when the 'Build in Absolute Mode' checkbox is selected  *
;    in the project properties.  Designing code in absolute mode is            *
;    antiquated - use relocatable mode.                                        *
;                                                                              *
;*******************************************************************************
;                                                                              *
;    Revision History:                                                         *
;                                                                              *
;*******************************************************************************



;*******************************************************************************
; Processor Inclusion
;
; TODO Step #1 Open the task list under Window > Tasks.  Include your
; device .inc file - e.g. #include <device_name>.inc.  Available
; include files are in C:\Program Files\Microchip\MPLABX\mpasmx
; assuming the default installation path for MPLAB X.  You may manually find
; the appropriate include file for your device here and include it, or
; simply copy the include generated by the configuration bits
; generator (see Step #2).
;
;*******************************************************************************

#include "p16f18346.inc"

;*******************************************************************************
;
; TODO Step #2 - Configuration Word Setup
;
; The 'CONFIG' directive is used to embed the configuration word within the
; .asm file. MPLAB X requires users to embed their configuration words
; into source code.  See the device datasheet for additional information
; on configuration word settings.  Device configuration bits descriptions
; are in C:\Program Files\Microchip\MPLABX\mpasmx\P<device_name>.inc
; (may change depending on your MPLAB X installation directory).
;
; MPLAB X has a feature which generates configuration bits source code.  Go to
; Window > PIC Memory Views > Configuration Bits.  Configure each field as
; needed and select 'Generate Source Code to Output'.  The resulting code which
; appears in the 'Output Window' > 'Config Bits Source' tab may be copied
; below.
;
;*******************************************************************************

; CONFIG1
; __config 0xFFEA
 __CONFIG _CONFIG1, _FEXTOSC_HS & _RSTOSC_HFINT1 & _CLKOUTEN_OFF & _CSWEN_ON & _FCMEN_ON
; CONFIG2
; __config 0xFFF3
 __CONFIG _CONFIG2, _MCLRE_ON & _PWRTE_OFF & _WDTE_OFF & _LPBOREN_OFF & _BOREN_ON & _BORV_LOW & _PPS1WAY_ON & _STVREN_ON & _DEBUG_OFF
; CONFIG3
; __config 0x2003
 __CONFIG _CONFIG3, _WRT_OFF & _LVP_ON
; CONFIG4
; __config 0x3
 __CONFIG _CONFIG4, _CP_OFF & _CPD_OFF

;*******************************************************************************
;
; TODO Step #3 - Variable Definitions
;
; Refer to datasheet for available data memory (RAM) organization assuming
; relocatible code organization (which is an option in project
; properties > mpasm (Global Options)).  Absolute mode generally should
; be used sparingly.
;
; Example of using GPR Uninitialized Data
;
;   GPR_VAR        UDATA
;   MYVAR1         RES        1      ; User variable linker places
;   MYVAR2         RES        1      ; User variable linker places
;   MYVAR3         RES        1      ; User variable linker places
;
;   ; Example of using Access Uninitialized Data Section (when available)
;   ; The variables for the context saving in the device datasheet may need
;   ; memory reserved here.
;   INT_VAR        UDATA_ACS
;   W_TEMP         RES        1      ; w register for context saving (ACCESS)
;   STATUS_TEMP    RES        1      ; status used for context saving
;   BSR_TEMP       RES        1      ; bank select used for ISR context saving
;
;*******************************************************************************

GPR_VAR UDATA
    STATE RES 1

;*******************************************************************************
; Reset Vector
;*******************************************************************************

RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program

;*******************************************************************************
; TODO Step #4 - Interrupt Service Routines
;
; There are a few different ways to structure interrupt routines in the 8
; bit device families.  On PIC18's the high priority and low priority
; interrupts are located at 0x0008 and 0x0018, respectively.  On PIC16's and
; lower the interrupt is at 0x0004.  Between device families there is subtle
; variation in the both the hardware supporting the ISR (for restoring
; interrupt context) as well as the software used to restore the context
; (without corrupting the STATUS bits).
;
; General formats are shown below in relocatible format.
;
;------------------------------PIC16's and below--------------------------------
;
; ISR       CODE    0x0004           ; interrupt vector location
;
;     <Search the device datasheet for 'context' and copy interrupt
;     context saving code here.  Older devices need context saving code,
;     but newer devices like the 16F#### don't need context saving code.>
;
;     RETFIE
;
;----------------------------------PIC18's--------------------------------------
;
; ISRHV     CODE    0x0008
;     GOTO    HIGH_ISR
; ISRLV     CODE    0x0018
;     GOTO    LOW_ISR
;
; ISRH      CODE                     ; let linker place high ISR routine
; HIGH_ISR
;     <Insert High Priority ISR Here - no SW context saving>
;     RETFIE  FAST
;
; ISRL      CODE                     ; let linker place low ISR routine
; LOW_ISR
;       <Search the device datasheet for 'context' and copy interrupt
;       context saving code here>
;     RETFIE
;
;*******************************************************************************

ISR	    CODE    0X0004
    BANKSEL STATE
    BTFSC STATE, 0
    GOTO ALARM_STATE
    
    BANKSEL PIR0
    BTFSC PIR0, 5 ; TMR0IF
    GOTO TRIGGER_ALARM_STATE
    
    BANKSEL PIR1
    BTFSC PIR1, 1 ; TMR2IF
    GOTO TOGGLE_LED
    
    GOTO END_IFELSE
    
    TOGGLE_LED
	BANKSEL PIR1
	BCF PIR1, 1
	
	BANKSEL PORTB
	MOVFW PORTB
	XORLW B'00110000' ; INVERT BITS 4 AND 5
	MOVWF PORTB
	GOTO END_IFELSE
    
    
    TRIGGER_ALARM_STATE
	BANKSEL PIR0
	BTFSS PIR0, 5 ; TMR0IF
	GOTO END_IFELSE
	
	BCF PIR0, 5 ; TMR01F
	
	BANKSEL PORTB
	BSF PORTB, 4
	BSF PORTB, 5
	
	BANKSEL STATE
	BSF STATE, 0 ; SET STATE TO ALARM MODE
	
	BANKSEL PWM5CON
	BSF PWM5CON, 7
	
	GOTO END_IFELSE
    
    ALARM_STATE
	BANKSEL PIR1
	BTFSS PIR1, 1
	GOTO END_IFELSE
	
	BANKSEL PWM5DCH
	
	BTFSS PWM5DCH, 5 ; SKIP IF 500HZ DUTY CYCLE
	MOVLW B'0010000'
	
	BTFSS PWM5DCH, 3; SKIP IF 250HZ DUTY CYCLE
	MOVLW B'0000100'
	
	MOVWF PWM5DCH
	
	BANKSEL PWMTMRS
	BTFSS PWMTMRS, 0 ; SKIP IF CURRENTLY 500HZ
	MOVLW B'00000011'
	
	BTFSC PWMTMRS, 0 ; SKIP IF CURRENTLY 500KHZ
	MOVLW B'00000010'
	
	MOVWF PWMTMRS
	
	GOTO END_IFELSE
    
    END_IFELSE
    
    BANKSEL PIR0
    CLRF PIR0
    
    BANKSEL PIR1
    CLRF PIR1
    
    RETFIE

;*******************************************************************************
; MAIN PROGRAM
;*******************************************************************************

MAIN_PROG CODE                      ; let linker place main program
 
INIT
    BANKSEL STATE
    CLRF STATE
    
    BANKSEL OSCCON1
    MOVLW B'01100101' ; 32KHZ CLOCK
    MOVWF OSCCON1
    
    BANKSEL PORTC
    CLRF PORTC
    BANKSEL ANSELC
    CLRF ANSELC
    BANKSEL LATC
    CLRF LATC
    BANKSEL TRISC
    CLRF TRISC
    BSF TRISC, 4
    
    BANKSEL PORTB
    CLRF PORTB
    BANKSEL ANSELB
    CLRF ANSELB
    BANKSEL LATB
    CLRF LATB
    BANKSEL TRISB
    CLRF TRISB
    
    BANKSEL T0CON0
    CLRF T0CON0
    
    BANKSEL T0CON1 ; SET PRESCALER VALUE
    MOVLW B'10011000'
    MOVWF T0CON1
    
    BANKSEL TMR0H ; SETS THE TIMER TARGET
    MOVLW 0xFA ; 250
    MOVWF TMR0H
    
    BANKSEL TMR0L
    CLRF TMR0L
    
    BANKSEL T2CON
    MOVLW B'01111011'
    MOVWF T2CON
    
    BANKSEL PR2
    MOVLW 150
    MOVWF PR2
    
    BANKSEL T4CON
    MOVLW B'00000100'
    MOVWF T4CON
    
    BANKSEL PR4
    MOVLW 63
    MOVWF PR4
    
    BANKSEL T6CON
    MOVLW B'00000100'
    MOVWF T6CON
    
    BANKSEL PR6
    MOVLW 127
    MOVWF PR6
    
    BANKSEL INTCON
    MOVLW B'11000001'
    MOVWF INTCON
    
    BANKSEL PIE0
    MOVLW B'00100001'
    MOVWF PIE0
    
    BANKSEL PIE1
    CLRF PIE1
    BSF PIE1, 1 ; TIMER 2
    
    ; BANKSEL PIE2
    ; CLRF PIE2
    ; BSF PIE2, 1 ; TIMER 4
    ; BSF PIE2, 7 ; TIMER 6
    
    BANKSEL PWM5CON ; BIT 7 = ENABLE
    CLRF PWM5CON
    BANKSEL PWM5DCH
    CLRF PWM5DCH
    BSF PWM5DCH, 5
    BANKSEL PWM5DCL
    CLRF PWM5DCL
    
    BANKSEL PWMTMRS
    MOVLW B'00000010' ;10 = TMR4 11 = TMR6
    MOVWF PWMTMRS
    
    BANKSEL RB6PPS
    MOVLW B'00000010' ; PWM 5 SOURCE
    MOVWF RB6PPS
    
    RETURN

DISABLE_TIMER0
    BANKSEL T0CON0
    BCF T0CON0, 7
    
    BANKSEL TMR0L
    CLRF TMR0L
    RETURN
    
ENABLE_TIMER0
    BANKSEL T0CON0
    BSF T0CON0, 7
    RETURN

; TIMER0 -> PUSHBUTTON
; TIMER2 -> 1SEC LED
; TIMER4 -> 250Hz TONE
; TIMER6 -> 500Hz TONE
START
    
    CALL INIT
    
    BANKSEL T2CON
    BSF T2CON, 2
    
    BANKSEL PORTB
    BSF PORTB, 4
    BCF PORTB, 5
    
    LOOP

    BANKSEL PORTC
    BTFSC PORTC, 4
    CALL DISABLE_TIMER0
    
    BANKSEL PORTC
    BTFSS PORTC, 4
    CALL ENABLE_TIMER0
    
    GOTO LOOP                          ; loop forever

    END