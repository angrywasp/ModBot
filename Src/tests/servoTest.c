#include "simpletools.h"
#include "servo.h"

#include "./lib/modules/P8X32A_AD.h"
#include "./lib/IO.h"
#include "./lib/refs.h"

volatile io_adc_binding_t *vr_x;
volatile io_adc_binding_t *vr_y;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    mcu_context_t *cxt = mcu_init();

    pause(1000);

    vr_x = io_add_adc_binding(cxt->adc, ad7812_A7, "X %d\n");
    vr_y = io_add_adc_binding(cxt->adc, ad7812_A8, "Y %d\n");

    while (true)
    {
        io_joystick(vr_x);
        io_joystick(vr_y);

        if (vr_x->value->newValueReady && vr_x->value->changed)
        {
            int x = (int)map(vr_x->value->lastVal, -255, 255, 0, 1800);
            servo_angle(26, x);
            //printf(vr_x->value->msg, x);
            vr_x->value->newValueReady = false;
        }

        if (vr_y->value->newValueReady && vr_y->value->changed)
        {
            int y = (int)map(vr_y->value->lastVal, -255, 255, 0, 1800);
            servo_angle(27, y);
            //printf(vr_y->value->msg, y);
            vr_y->value->newValueReady = false;
        }
    }
}