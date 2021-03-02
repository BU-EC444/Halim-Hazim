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

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling


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

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
   if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
       printf("Characterized using Two Point Value\n");
   } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
       printf("Characterized using eFuse Vref\n");
   } else {
       printf("Characterized using Default Vref\n");
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

    
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        double voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
       
        //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        double volt_per_inch = 3.3 / 512; //this equals to 0.006445V per inch
        double mV_per_inch = volt_per_inch*1000;
        //       //range formula (Vm/Vi) = Ri
        double range_inches = voltage / mV_per_inch;
        double range_cm = (range_inches * 2.54) - 55.96;// unit is centimete
        double range_tare = range_cm - 40;
        if(range_tare > 0){
            
            printf("Range is: %.2f cm\n", range_tare);
        }else{
            printf("%s\n", "Range is less than 40.0cm");
        }

}
}

