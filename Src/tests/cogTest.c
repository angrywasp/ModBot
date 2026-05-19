#include "simpletools.h"

#include "./lib/modules/P8X32A_AD.h"
#include "./lib/modules/PSU.h"

#include "./lib/serial_buffer.h"
#include "./lib/refs.h"

mcu_context_t *mcu;
psu_context_t * psu;

unsigned int cog_io_stack[40 + 25];

volatile int v;

void cog_io(void *par)
{
    mcu = mcu_init();

    while (true)
    {
        pause(1000);
        v = ad7812_read(mcu->adc, ad7812_A8) >> 2;
    }
}

int main()
{
    
    psu = psu_init(22000, 47000, 3.3f, 2.75f, 4.2f);

    printf("Online\n");

    cog_run(cog_io, 128);

    while (true)
    {
        pause(1000);

        //int v = ad7812_read(mcu->adc, ad7812_A8) >> 2;
        printf("%d\n", v);
    }
}