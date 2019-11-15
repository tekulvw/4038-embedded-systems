#include "ultrasonic.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/device_config.h"

float US_GetDistance(void) {
    long int ret = 0;
    US_Trigger_SetHigh();
    __delay_us(10);
    US_Trigger_SetLow();
    
    while (US_Echo_GetValue() == LOW) ;
    while (US_Echo_GetValue() == HIGH) {
        __delay_us(1);
        ret++;
    }
    
    return (float)(ret / 2) * 0.0343;
}