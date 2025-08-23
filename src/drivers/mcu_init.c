#include "mcu_init.h"
#include "gpio.h"

#include <msp430.h>
#include <msp430g2553.h>


static inline void Init_Clocks(void){
    /*
     * Configure the internal main clock to run at 16 MHz.
     * This clock serves as a reference to the DCO.
     */
    BCSCTL1 |= CALBC1_16MHZ;

    /*
     * Set the clock rate for the internal digitally controlled oscillator (DCO)
     */
    DCOCTL |= CALDCO_16MHZ;

    /*
     * MCLK and SMCLK are sourced from DCOCLK by default.
     * So no need to configure the BCSCTL2 register.
     * CPU executes code from the MCLK.
     * SMCLK is software selectable for individual peripheral modules.
     */
    // BCSCTL2 -> default
}

static inline void Watchdog_Setup(void){
    // Stop the watchdog timer
    WDTCTL = WDTPW + WDTHOLD;
}

void MCU_Init(void){
    // Setup watchdog before anything else
    Watchdog_Setup();

    // Initialize clocks
    Init_Clocks();

    // Initialize all GPIO pins based on initial configurations
    GPIO_InitAllPortPins();

    // Enable interrupt globally
    _enable_interrupts();
}