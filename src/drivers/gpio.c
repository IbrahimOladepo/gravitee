#include "gpio.h"

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>


#define     GPIO_PORT_OFFSET        (3u)
#define     GPIO_PORT_MASK          (0x3u << GPIO_PORT_OFFSET)
#define     GPIO_PIN_MASK           (0x7u)

#define     GPIO_PORT_COUNT         (2u)
#define     GPIO_PORT_PIN_COUNT     (8u)


// PIN INITIALIZATIONS

/*
 * As per the datasheet, unused pins should not be left floating
 * Decided to set them as inputs and pull them low.
 * This seems to come with the risk of shortcircuiting though according to Artful Bytes.
 */
static const GPIO_PinConfig_t UNUSED_CONFIG = {
    .GPIO_Direction             = GPIO_DIR_INPUT,
    .GPIO_InterruptEdgeSelect   = 0,
    .GPIO_InterruptEnable       = GPIO_INTERRUPT_DISABLED,
    .GPIO_InterruptFlag         = 0,
    .GPIO_PortSelect            = 0,
    .GPIO_PortSelect2           = 0,
    .GPIO_ResistorEnable        = GPIO_RESISTOR_ENABLED,
    .GPIO_PullUpDown            = GPIO_OUTPUT_LOW
};

static const GPIO_PinConfig_t GPIO_TEST_LED = {
    .GPIO_Direction             = GPIO_DIR_OUTPUT,
    .GPIO_InterruptEdgeSelect   = 0,
    .GPIO_InterruptEnable       = GPIO_INTERRUPT_DISABLED,
    .GPIO_InterruptFlag         = 0,
    .GPIO_PortSelect            = 0,
    .GPIO_PortSelect2           = 0,
    .GPIO_ResistorEnable        = GPIO_RESISTOR_DISABLED,
    .GPIO_PullUpDown            = GPIO_OUTPUT_LOW
};


// This array holds the initial configuration for all IO pins.
static const GPIO_PinConfig_t GPIO_Configs[GPIO_PORT_COUNT * GPIO_PORT_PIN_COUNT] = {
    [GPIO_LED_PP] = GPIO_TEST_LED,
    [GPIO_UNUSED_1] = UNUSED_CONFIG,
    [GPIO_UNUSED_2] = UNUSED_CONFIG,
    [GPIO_UNUSED_3] = UNUSED_CONFIG,
    [GPIO_UNUSED_4] = UNUSED_CONFIG,
    [GPIO_UNUSED_5] = UNUSED_CONFIG,
    [GPIO_UNUSED_6] = UNUSED_CONFIG,
    [GPIO_UNUSED_7] = UNUSED_CONFIG,
    [GPIO_UNUSED_8] = UNUSED_CONFIG,
    [GPIO_UNUSED_9] = UNUSED_CONFIG,
    [GPIO_UNUSED_10] = UNUSED_CONFIG,
    [GPIO_UNUSED_11] = UNUSED_CONFIG,
    [GPIO_UNUSED_12] = UNUSED_CONFIG,
    [GPIO_UNUSED_13] = UNUSED_CONFIG,
    [GPIO_UNUSED_14] = UNUSED_CONFIG,
    [GPIO_UNUSED_15] = UNUSED_CONFIG
};


static volatile GPIO_RegDef_t* const GPIO_PortRegs[GPIO_PORT_COUNT] = GPIO_PORTS;


static inline uint8_t GPIO_PinIdx(GPIO_PinAssignments_e pAss){
    return pAss & GPIO_PIN_MASK;
}


static uint8_t GPIO_PinBit(GPIO_PinAssignments_e pAss){
    return 1 << GPIO_PinIdx(pAss);
}


static uint8_t GPIO_PortIdx(GPIO_PinAssignments_e pAss){
    return (pAss & GPIO_PORT_MASK) >> GPIO_PORT_OFFSET;
}


void GPIO_ConfigPinSelect(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    if (GPIO_Configs[pAss].GPIO_PortSelect == 0)
        pin_port->PxSEL &= ~pin_bit;
    else if (GPIO_Configs[pAss].GPIO_PortSelect == 1)
        pin_port->PxSEL |= pin_bit;
    
    if (GPIO_Configs[pAss].GPIO_PortSelect2 == 0)
        pin_port->PxSEL2 &= ~pin_bit;
    else if (GPIO_Configs[pAss].GPIO_PortSelect2 == 1)
        pin_port->PxSEL2 |= pin_bit;
}


void GPIO_ConfigPinResistor(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    switch(GPIO_Configs[pAss].GPIO_ResistorEnable) {
        case GPIO_RESISTOR_DISABLED:
            pin_port->PxREN &= ~pin_bit;
            break;
        case GPIO_RESISTOR_ENABLED:
            pin_port->PxREN |= pin_bit;
            break;
    }
}


void GPIO_ConfigPinDirection(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    switch(GPIO_Configs[pAss].GPIO_Direction) {
        case GPIO_DIR_INPUT:
            pin_port->PxDIR &= ~pin_bit;
            break;
        case GPIO_DIR_OUTPUT:
            pin_port->PxDIR |= pin_bit;
            break;
    }
}


void GPIO_ConfigOutputState(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    switch(GPIO_Configs[pAss].GPIO_PullUpDown) {
        case GPIO_OUTPUT_LOW:
            pin_port->PxOUT &= ~pin_bit;
            break;
        case GPIO_OUTPUT_HIGH:
            pin_port->PxOUT |= pin_bit;
            break;
    }
}


void GPIO_ConfigParameters(GPIO_PinAssignments_e pAss){
    GPIO_ConfigPinSelect(pAss);
    GPIO_ConfigPinDirection(pAss);
    GPIO_ConfigOutputState(pAss);
    GPIO_ConfigPinResistor(pAss);
}


void GPIO_InitAllPortPins(void){
    for (GPIO_PinAssignments_e pAss = (GPIO_PinAssignments_e) IO_10; pAss < sizeof(GPIO_Configs); pAss++){
        GPIO_ConfigParameters(pAss);
    }
}


uint8_t GPIO_CheckIODirection(GPIO_PinAssignments_e pAss){
    if (GPIO_Configs[pAss].GPIO_Direction == GPIO_DIR_INPUT)
        return GPIO_DIR_INPUT;
    else
        return GPIO_DIR_OUTPUT;
    
    return 0;
}

void GPIO_WriteToOutputPin(GPIO_PinAssignments_e pAss, uint8_t value){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    if (GPIO_CheckIODirection(pAss)){
        if (value == GPIO_PIN_RESET)
            pin_port->PxOUT &= ~pin_bit;
        else if (value == GPIO_PIN_SET)
            pin_port->PxOUT |= pin_bit;
    }
}

void GPIO_ToggleOutputPin(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    if (GPIO_CheckIODirection(pAss))
        pin_port->PxOUT ^= pin_bit;

}

uint8_t GPIO_ReadFromInputPin(GPIO_PinAssignments_e pAss){
    uint8_t pin_bit = GPIO_PinBit(pAss);
    volatile GPIO_RegDef_t *pin_port = GPIO_PortRegs[GPIO_PortIdx(pAss)];

    return (pin_port->PxIN & pin_bit) ? GPIO_INPUT_HIGH : GPIO_INPUT_LOW;
}
