#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcd.h"
#include "keyboard.h"

void app_main(void)
{
    unsigned char tecla = 0;
    char str[] = "Tecla:    ";
    lcd_init();
    keyboard_init();

    lcd_print(0,0," *** ESP32 *** ");

    while( 1 )
    {
        tecla = keyboard();
        str[7] = tecla ? tecla : ' ';
        lcd_print(1,0, str );
        // vTaskDelay( pdMS_TO_TICKS(100));
    }
}
