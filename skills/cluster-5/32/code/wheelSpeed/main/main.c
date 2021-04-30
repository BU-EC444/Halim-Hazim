
/* ADC1 Example
  This example code is in the Public Domain (or CC0 licensed, at your option.)
  Unless required by applicable law or agreed to in writing, this
  software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
  CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "driver/i2c.h"
#include "driver/i2c.h"
#include <math.h>

#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling


int transitions = 0;

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

static void check_efuse(void)
{
   //Check TP is burned into eFuse
   if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
       printf("eFuse Two Point: Supported\n");
   } else {
       printf("eFuse Two Point: NOT supported\n");
   }

   //Check Vref is burned into eFuse
   if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
       printf("eFuse Vref: Supported\n");
   } else {
       printf("eFuse Vref: NOT supported\n");
   }
}

static void print_char_val_type(esp_adc_cal_value_t val_type){
   if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
       printf("Characterized using Two Point Value\n");
   } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
       printf("Characterized using eFuse Vref\n");
   } else {
       printf("Characterized using Default Vref\n");
   }
}

void get_wheel_speed(uint32_t time){
    time  = time / 1000000.0;
    double speed = 0;
    int circumference = 23; // radius is 3.5cm
    speed = circumference / time; // cm/ms
    printf("Current speed: %.2f\n", speed);

}


void encoder_adc(){
    
    while (1) {
        uint32_t black = 0; //black = 0 means that encoder is on white
        uint32_t new_adc_reading = 0;
        
        uint32_t time = esp_timer_get_time();
        
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                new_adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                new_adc_reading += raw;
            }
        }
        new_adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        double voltage = esp_adc_cal_raw_to_voltage(new_adc_reading, adc_chars);
       
        printf("Raw: %d\tVoltage: %.2fmV\n", new_adc_reading, voltage);
        
        
        if((new_adc_reading > 2000  && black == 0) || (new_adc_reading < 2000 && black == 1)){
            transitions++;
            black = ~black;
            
        }
        
        vTaskDelay(pdMS_TO_TICKS(1500));
        
        if(transitions == 7){
            printf("Transitions: %d\n", transitions);
            uint32_t time_2 = esp_timer_get_time();
            uint32_t time_lapse = time_2 - time;
            get_wheel_speed(time_lapse);
            transitions = 0;
        }
    }
    
}



void app_main(void)
{
   
    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);


    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,
                                         256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    xTaskCreate(encoder_adc,"encoder_adc",4096, NULL, 5, NULL);
    
    
}

