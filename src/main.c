
#include "m90e26.h"

static const char TAG[]="main";
void app_main(){

    spi_init();
    calibrateIC();
    ESP_LOGI(TAG,"lagin %02x",m90e26ReadU16(L_GAIN));
    ESP_LOGI(TAG,"ugain %02x",m90e26ReadU16(U_GAIN));

    for (;;){
    ESP_LOGI(TAG,"meter status: %x",getMeterStatus());
    ESP_LOGI(TAG,"AP: %f",getActivePower());
    ESP_LOGI(TAG,"LC: %f",getLineCurrent());
    ESP_LOGI(TAG,"LV: %f",getLineVoltage());
    ESP_LOGI(TAG,"PF: %f",getPowerFactor());
    ESP_LOGI(TAG,"SS: %x",getSystemStatus());

    vTaskDelay(200);
    }
    
    
    
   
}