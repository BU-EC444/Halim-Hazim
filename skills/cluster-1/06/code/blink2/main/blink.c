#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"

#define TOGGLE_GPIO 13

void app_main()
{

    gpio_pad_select_gpio(TOGGLE_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(TOGGLE_GPIO, GPIO_MODE_OUTPUT);
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,
      256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);


    char mode = 't';
    int num = 0;
    int check = 0;
    while(1) {


    if(mode == 't'){

      if(check == 0){
        printf("%s\n", "toggle mode");
      }
    //  printf("%s\n", "toggle mode");
        char buf[5];
    //  printf("%s\n", "toggle mode");
        gets(buf);
        printf("Read: %s\n",buf);

        if(buf[0] == 't'){
          //turn on LED
          num = !num;
          gpio_set_level(TOGGLE_GPIO,num);

        }else if(buf[0] == 's'){
          mode = 'i';
          printf("%s\n","echo input" );
          check = 0;
          continue;
        }

        check = 1;

    }

      if(mode == 'i'){ //i for echo input
        //printf("%s\n","echo input" );
        char buf1[5];
        gets(buf1);
        printf("echo: %s\n",buf1);

        if(buf1[0] == 's'){
          mode = 'h';
          printf("%s\n","echo decimal to hex" );
          continue;
        }



    }
      if(mode == 'h'){//dec to hex
      char dec[5];
      char hex[5];
      int is;
      //char si[] = "s";

      //printf("%s\n","echo decimal to hex" );
      printf("%s\n","Enter an integer: " );
      gets(dec);
      if(dec[0] == 's'){
        mode = 't';
        //printf("%s\n", "toggle mode");
        continue;
      }

      is = atoi(dec);
      itoa(is,hex,16);
      printf("Hex: %s\n",hex);


        if(dec[0] == 's'){
          mode = 't';
          //printf("%s\n", "toggle mode");
          continue;
        }


    }


            // or

      // scanf()

      vTaskDelay(50 / portTICK_RATE_MS);
    }
}
