#include "gpio.h"
#include "defines.h"
#include "mcu_init.h"

#include <msp430.h>
#include <msp430g2553.h>


SUPPRESS_UNUSED
static void test_setup(void){
    MCU_Init();
}

SUPPRESS_UNUSED
static void test_blink_LED(void){
    test_setup();

    uint8_t i = 0;
    
    for (i = 0; i < 3; i++){
        GPIO_ToggleOutputPin(GPIO_LED_PP);
        BUSY_WAIT_MS(100);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        BUSY_WAIT_MS(100);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        BUSY_WAIT_MS(100);

        GPIO_ToggleOutputPin(GPIO_LED_PP);
        BUSY_WAIT_MS(1000);
    }
}

SUPPRESS_UNUSED
static void GPIO_11_ISR(void)
{
    GPIO_WriteToOutputPin(GPIO_LED_PP, GPIO_OUTPUT_HIGH);
}

SUPPRESS_UNUSED
static void GPIO_20_ISR(void)
{
    GPIO_WriteToOutputPin(GPIO_LED_PP, GPIO_OUTPUT_LOW);
}

SUPPRESS_UNUSED
static void test_GPIO_interrupt(void){
    test_setup();

    // GPIO_ConfigParameters(IO_11);
    // GPIO_ConfigParameters(IO_20);
    
    GPIO_ConfigureInterrupt(IO_11, GPIO_TRIGGER_FALLING, GPIO_11_ISR);
    GPIO_ConfigureInterrupt(IO_20, GPIO_TRIGGER_FALLING, GPIO_20_ISR);

    GPIO_EnableInterrupt(IO_11);
    GPIO_EnableInterrupt(IO_20);

    while(1);
}

int main()
{
    TEST();

    return 0;
}
