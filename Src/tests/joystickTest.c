#include "simpletools.h"

#include "../lib/modules/P8X32A_AD.h"
#include "../lib/IO.h"
#include "../lib/refs.h"

volatile io_adc_binding_t *vr_x;
volatile io_adc_binding_t *vr_y;

int main()
{
    mcu_context_t *cxt = mcu_init();

    pause(1000);

    vr_x = io_add_adc_binding(cxt->adc, ad7812_A7, "X %d\n");
    vr_y = io_add_adc_binding(cxt->adc, ad7812_A8, "Y %d\n");

    while(true)
    {
        io_joystick(vr_x);
        io_joystick(vr_y);

        if (vr_x->value->newValueReady && vr_x->value->changed)
        {
            printf(vr_x->value->msg, vr_x->value->lastVal);
            vr_x->value->newValueReady = false;
        }

        if (vr_y->value->newValueReady && vr_y->value->changed)
        {
            printf(vr_y->value->msg, vr_y->value->lastVal);
            vr_y->value->newValueReady = false;
        }
    }
}
