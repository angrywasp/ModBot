#include "../drivers/AD7812.h"
#include "./PSU.h"
#include "../IO.h"
#include "../refs.h"

psu_context_t* psu_init(int vd_r1, int vd_r2, float vref, float battery_min_voltage, float battery_max_voltage)
{
    psu_context_t* b = (psu_context_t*)malloc(sizeof(psu_context_t));

    b->battery_min_voltage = battery_min_voltage;
    b->battery_max_voltage = battery_max_voltage;
    b->vref = vref;

    //Calculate the min and max voltages expected to be seen on the adc line
    //vMin = (min * r2) / (r1 + r2)
    //vMax = (max * r2) / (r1 + r2)

    //convert these voltages to expected adc value range
    //aMin = (vMin / vRef) * 256
    //aMax = (vMax / vRef) * 256

    //vRange = 1.873 - 2.860 volts
    //aRange = 145 - 221
    
    //2.75 * 47000 / 69000 = 1.873
    float vd_min = (battery_min_voltage * (float)vd_r2) / (float)(vd_r1 + vd_r2);
    //4.2 * 47000 / 69000 = 2.860
    float vd_max = (battery_max_voltage * (float)vd_r2) / (float)(vd_r1 + vd_r2);

    //1.873 / 3.3 * 256 = 145
    b->adc_min = (ushort)((vd_min / vref) * 256.0f);
    //2.860 / 3.3 * 256 = 221
    b->adc_max = (ushort)((vd_max / vref) * 256.0f);

    return b;
}

ushort psu_calculate_percent(psu_context_t* cxt, int adc_reading)
{
    float max = (float)cxt->adc_max - (float)cxt->adc_min;
    float val = (float)adc_reading - (float)cxt->adc_min;
    ushort ret = (ushort)((val / max) * 100.0f);

    if (ret > 100) ret = 100;

    return ret;
}

void psu_read_battery(psu_context_t* cxt, volatile io_adc_binding_t* b)
{
    //previously obtained value has not been consumed
    if (b->value->newValueReady)
        return;

    int adc = io_read(b);
    int val = psu_calculate_percent(cxt, adc);

    if (val >= b->value->lastVal)
    {
        b->value->changed = false;
        return;
    }

    b->value->lastVal = val;
    b->value->changed = true;
    b->value->newValueReady = true;
}
