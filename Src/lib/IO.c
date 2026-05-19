#include "./drivers/AD7812.h"
#include "./IO.h"
#include "./refs.h"

#define JOYSTICK_DEADZONE 4
#define JOYSTICK_MIDPOINT 128

io_switch_binding_t* io_add_switch_binding(int pin, char* msg)
{
    dir_in(pin);
    io_switch_binding_t* b = (io_switch_binding_t*)malloc(sizeof(io_switch_binding_t));
    b->value = (io_binding_value_data_t*)malloc(sizeof(io_binding_value_data_t) + strlen(msg));

    b->pin = pin;

    b->value->msg = msg;
    b->value->lastVal = -1;
    b->value->changed = false;
    b->value->newValueReady = false;

    return b;
}

io_adc_binding_t* io_add_adc_binding(volatile ad7812_context_t* adc_context, unsigned short port, char* msg)
{
    io_adc_binding_t* b = (io_adc_binding_t*)malloc(sizeof(io_adc_binding_t));
    b->value = (io_binding_value_data_t*)malloc(sizeof(io_binding_value_data_t) + strlen(msg));

    b->port = port;
    b->adc = adc_context;

    b->value->msg = msg;
    b->value->lastVal = -1;
    b->value->changed = false;
    b->value->newValueReady = false;

    return b;
}

int io_read(volatile io_adc_binding_t* b)
{
    int values[5];
    for (int i = 0; i < 5; i++)
    {
        int v = ad7812_read(b->adc, b->port);
        values[i] = v >> 2;
    }

    qsort(values, 5, sizeof(int), io_compare);

    return values[2];
}

void io_switch(volatile io_switch_binding_t* b)
{
    if (b->value->newValueReady)
        return;

    int val = in(b->pin);

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}

//The AD7812 returns a 10-bit value. We shift this 2 bits to place it in the 0-255 range
//makes the ADC readings less accurate, but also less jittery

void io_adc(volatile io_adc_binding_t* b)
{
    //previously obtained value has not been consumed
    if (b->value->newValueReady)
        return;

    int val = io_read(b);

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}

int io_joystick_get_center(volatile io_adc_binding_t* b)
{
    return io_read(b) - JOYSTICK_MIDPOINT;
}

//center: value to offset the joystick value to trim the output
//invert: flip value +-
//min, max: clamp value to range
void io_joystick(volatile io_adc_binding_t* b, int center, bool invert, int min, int max)
{
    if (b->value->newValueReady)
        return;

    int val = io_read(b);
    val -= JOYSTICK_MIDPOINT;
    val += center;

    //output conditioning
    if (invert) val *= -1;
    if (val < min) val = min;
    if (val > max) val = max;

    if (val > -JOYSTICK_DEADZONE && val < JOYSTICK_DEADZONE)
        val = 0;

    if (val == b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}
