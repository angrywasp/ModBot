#ifndef __PSU_H
#define __PSU_H

#include "../IO.h"
#include "../refs.h"

typedef struct
{
    volatile float battery_min_voltage;
    volatile float battery_max_voltage;
    volatile ushort adc_min;
    volatile ushort adc_max;
    volatile float vref;
} psu_context_t;

psu_context_t* psu_init(int vd_r1, int vd_r2, float vref, float battery_min_voltage, float battery_max_voltage);

ushort psu_calculate_percent(psu_context_t* cxt, int adc_reading);
void psu_read_battery(psu_context_t* cxt, volatile io_adc_binding_t* b);

#endif