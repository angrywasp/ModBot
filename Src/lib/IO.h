#ifndef __IO_H
#define __IO_H

#include "./refs.h"

typedef struct
{
    volatile int lastVal;
    volatile bool changed;
    volatile bool newValueReady;
    char* msg;
} io_binding_value_data_t;

typedef struct
{
    volatile int pin;
    volatile io_binding_value_data_t* value;
} io_switch_binding_t;

typedef struct
{
    volatile ad7812_context_t* adc;
    volatile int port;
    volatile io_binding_value_data_t* value;
} io_adc_binding_t;

static inline int io_compare(const void* a, const void* b)
{
    return *((int*)a)-*((int*)b);
}

io_switch_binding_t* io_add_switch_binding(int pin, char* msg);
io_adc_binding_t* io_add_adc_binding(volatile ad7812_context_t* adc_context, unsigned short port, char* msg);

int io_read(volatile io_adc_binding_t* b);

void io_switch(volatile io_switch_binding_t* b);
void io_adc(volatile io_adc_binding_t* b);
void io_joystick(volatile io_adc_binding_t* b, int center, bool invert, int min, int max);
int io_joystick_get_center(volatile io_adc_binding_t* b);

#endif