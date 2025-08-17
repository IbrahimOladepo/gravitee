
#include <stdint.h>


// STRUCTURES

/*
 * Structure to configure a GPIO pin
 */
typedef struct
{
    uint8_t GPIO_Direction;
    uint8_t GPIO_InterruptFlag;
    uint8_t GPIO_InterruptEdgeSelect;
    uint8_t GPIO_InterruptEnable;
    uint8_t GPIO_PortSelect;
    uint8_t GPIO_PortSelect2;
    uint8_t GPIO_ResistorEnable;
    uint8_t GPIO_PullUpDown;
}GPIO_PinConfig_t;

/*
 * GPIO register definition structures
 */
typedef struct 
{
    uint8_t PxIN;
    uint8_t PxOUT;
    uint8_t PxDIR;
    uint8_t PxIFG;
    uint8_t PxIES;
    uint8_t PxIE;
    uint8_t PxSEL;
    uint8_t PxSEL2;
    uint8_t PxREN;
}GPIO_RegDef_t;

/*
 * GPIO pin number enumeration
 * The port number can be extracted by masking bits 3 and 4 and offsetting by 3
 * The pin numner can be obtained by masking, leaving only the first 3 bits
 */
typedef enum {
    IO_10, IO_11, IO_12, IO_13, IO_14, IO_15, IO_16, IO_17,
    IO_20, IO_21, IO_22, IO_23, IO_24, IO_25, IO_26, IO_27,
    IO_30, IO_31, IO_32, IO_33, IO_34, IO_35, IO_36, IO_37
}GPIO_PortPinNumber_e;

/*
 * Pin assignment to functions
 */
typedef enum {
    GPIO_LED_PP = IO_10,
    GPIO_UNUSED_1 = IO_11,
    GPIO_UNUSED_2 = IO_12,
    GPIO_UNUSED_3 = IO_13,
    GPIO_UNUSED_4 = IO_14,
    GPIO_UNUSED_5 = IO_15,
    GPIO_UNUSED_6 = IO_16,
    GPIO_UNUSED_7 = IO_17,
    GPIO_UNUSED_8 = IO_20,
    GPIO_UNUSED_9 = IO_21,
    GPIO_UNUSED_10 = IO_22,
    GPIO_UNUSED_11 = IO_23,
    GPIO_UNUSED_12 = IO_24,
    GPIO_UNUSED_13 = IO_25,
    GPIO_UNUSED_14 = IO_26,
    GPIO_UNUSED_15 = IO_27
}GPIO_PinAssignments_e;


typedef enum {
    PORT1,
    PORT2
}GPIO_AvailablePorts;


typedef enum {
    GPIO_INPUT_LOW,
    GPIO_INPUT_HIGH
}GPIO_InputState;


// DEFINES

/*
 * Input mode pull resistors set through the PxOUT regs
 * Configure pull mode after enabling pull resistor regs
 */
#define     GPIO_INPUT_PULLDOWN     (0u)
#define     GPIO_INPUT_PULLUP       (1u)

/*
 * GPIO mode is set by the PxSEL and PxSEL2 registers
 * NB: Setting the GPIO mode to IO does not automatically set pin direction
 */
#define     GPIO_MODE_IO            (0u)
#define     GPIO_MODE_PERIPHERAL1   (1u)
#define     GPIO_MODE_RESERVED      (2u)
#define     GPIO_MODE_PERIPHERAL2   (3u)

/*
 * Values for the PxDIR port direction register
 */
#define     GPIO_DIR_INPUT          (0u)
#define     GPIO_DIR_OUTPUT         (1u)

/*
 * Values for PxREN resistor pull enable/disable register
 */
#define     GPIO_RESISTOR_DISABLED  (0u)
#define     GPIO_RESISTOR_ENABLED   (1u)

/*
 * @GPIO_PORT_BASE_ADDRESSES
 * Base addresses for the GPIO ports
 */
#define     PORT1_BADDR             0x0020
#define     PORT2_BADDR             0x0028
// #define     PORT3_BADDR             &P3IN   // 0x0018

#define     GPIO_OUTPUT_LOW         (0u)
#define     GPIO_OUTPUT_HIGH        (1u)

#define     GPIO_INTERRUPT_DISABLED (0u)
#define     GPIO_INTERRUPT_ENABLED  (1u)

#define     GPIO_PIN_RESET          (0u)
#define     GPIO_PIN_SET            (1u)

#define     GPIO_PORTS              \
{                                   \
    (GPIO_RegDef_t*) PORT1_BADDR,   \
    (GPIO_RegDef_t*) PORT2_BADDR    \
}


// FUNCTION PROTOTYPES
void GPIO_ConfigPinSelect(GPIO_PinAssignments_e pAss);
void GPIO_ConfigPinResistor(GPIO_PinAssignments_e pAss);
void GPIO_ConfigPinDirection(GPIO_PinAssignments_e pAss);
void GPIo_ConfigOutputState(GPIO_PinAssignments_e pAss);
void GPIO_ConfigParameters(GPIO_PinAssignments_e pAss);
void GPIO_InitAllPortPins(void);

uint8_t GPIO_CheckIODirection(GPIO_PinAssignments_e pAss);
void GPIO_WriteToOutputPin(GPIO_PinAssignments_e pAss, uint8_t value);
void GPIO_ToggleOutputPin(GPIO_PinAssignments_e pAss);
uint8_t GPIO_ReadFromInputPin(GPIO_PinAssignments_e pAss);
