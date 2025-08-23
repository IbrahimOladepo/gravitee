#ifndef DEFINES_H
#define DEFINES_H

#define SUPPRESS_UNUSED                 __attribute__((unused))

#define INTERRUPT_FUNCTION(vector)      void __attribute__((interrupt(vector)))

#define CYCLES_1MHZ                     (1000000u)
#define CYCLES_16MHz                    (16u * CYCLES_1MHZ)
#define CYCLES_PER_MS                   (CYCLES_16MHz / 1000u)
#define MS_TO_CYCLES(ms)                (CYCLES_PER_MS * ms)
#define BUSY_WAIT_MS(ms)                (__delay_cycles(MS_TO_CYCLES(ms)))

#define MCLK                            CYCLES_16MHZ
#define SMCLK                           MCLK

#endif