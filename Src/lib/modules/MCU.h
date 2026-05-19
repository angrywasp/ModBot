#ifndef __MCU_H
#define __MCU_H

//todo: MCU module comes in two flavours, with and without ADC.
//need to move this to the build script, but this is fine for now
#ifndef ADC_EN
#define ADC_EN
#endif

#include "simplei2c.h"

#include "../refs.h"

#ifdef ADC_EN
#include "../drivers/AD7812.h"
#define ADC_DIN 6
#define ADC_DOUT 7
#define ADC_SCLK 5
#define ADC_CONVST 4
#endif

typedef struct
{
#ifdef ADC_EN
    volatile ad7812_context_t* adc;
#endif

    volatile i2c *i2c;
} mcu_context_t;

mcu_context_t* mcu_init();

#ifdef ADC_EN

ad7812_context_t* adc_init();

#endif

#endif