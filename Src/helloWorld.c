#include "simpletools.h"

#include "./Src/lib/refs.h"
#include "./Src/lib/modules/MCU.h"

int main()
{
    mcu_context_t *cxt = mcu_init();

    pause(1000);
    printf("Starting...\n");
    pause(1000);

    int i = 0;

    while (true)
    {
        printf("%d\n", ++i);
        pause(1000);
    }
}