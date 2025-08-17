#include "led.h"
#include "gpio.h"

#include <msp430.h>
#include <msp430g2553.h>

int main(void){

    // volatile unsigned long int i;

    // Stop the watchdog timer
    WDTCTL = WDTPW + WDTHOLD;

    // led_init();

    GPIO_InitAllPortPins();

    while(1){
        
        GPIO_ToggleOutputPin(GPIO_LED_PP);
        __delay_cycles(1000000);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        __delay_cycles(1000000);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        __delay_cycles(1000000);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        __delay_cycles(1000000);
    }
}
