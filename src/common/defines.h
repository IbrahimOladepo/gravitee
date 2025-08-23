#ifndef DEFINES_H
#define DEFINES_H

#define SUPPRESS_UNUSED                 __attribute__((unused))

#define INTERRUPT_FUNCTION(vector)      void __attribute__((interrupt(vector)))

#endif