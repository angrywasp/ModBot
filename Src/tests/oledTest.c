#include "simpletools.h"

#include "./lib/drivers/SSD1306.h"
#include "./lib/refs.h"

#define OLED_SDA 17
#define OLED_SCL 16

void display_text(ssd1306_context_t* oled, ushort y, char* str)
{
    int len = ssd1306_string_width(str, strlen(str));
    ssd1306_txt(oled, (oled->width - len) / 2, y, str); 
}

int main()
{
    pause(1000);

    ssd1306_context_t* oled = ssd1306_init(SSD1306_GEOMETRY_128_32, OLED_SCL, OLED_SDA);
    printf(ssd1306_detect(oled) ? "OLED found\n" : "OLED not found\n");

    ssd1306_reset_display(oled);
    display_text(oled, 0, "This is a test");
    display_text(oled, 16, "Also a test");
    ssd1306_display(oled);
    pause(1000);
    ssd1306_reset_display(oled);

    int i = 0;
    char* str = (char*)malloc(3);
    char* baseText = "Testing: ";

    ssd1306_txt(oled, 0, 0, baseText);
    int len = ssd1306_string_width(baseText, strlen(baseText));

    while (true)
    {
        if (++i >= 10) i = 0;

        sprintf(str, "%d", i);
        ssd1306_txt(oled, len, 0, str);
        ssd1306_display(oled);
        pause(1000);
    }
}