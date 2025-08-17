#include "led.h"

#include <msp430.h>
#include <msp430g2553.h>

void led_init(){
    // Set LED pin to output direction
    // P1 -> PORT 1
    P1DIR |= 0x01;
}

void led_toggle(){
    // Toggle the P1 pin
    P1OUT ^= 0x01;
}
