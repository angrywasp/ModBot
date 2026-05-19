#include "./MCU.h"

mcu_context_t* mcu_init()
{
    mcu_context_t* b = (mcu_context_t*)malloc(sizeof(mcu_context_t));

    //default all IO pins to low output
    for (int i = 0; i < 27; i++)
    {
        lo(i);
        dir_out(i);
    }

    return b;
}

#ifdef ADC_EN

ad7812_context_t* adc_init()
{
    return ad7812_init(ADC_DIN, ADC_DOUT, ADC_SCLK, ADC_CONVST);
}

#endif