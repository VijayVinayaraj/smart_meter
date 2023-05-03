
#include "m90e26.h"

static const char TAG[]="main";
void app_main(){

    spi_init();
    calibrateIC();
double accumulatedEnergy =0;
 double accumulatedRevEnergy=0;
    for (;;){
    ESP_LOGI(TAG,"meter status: %x",getMeterStatus());
    ESP_LOGI(TAG,"AP: %f",getActivePower());
    ESP_LOGI(TAG,"LC: %f",getLineCurrent());
    ESP_LOGI(TAG,"LV: %f",getLineVoltage());
    ESP_LOGI(TAG,"PF: %f",getPowerFactor());
    ESP_LOGI(TAG,"SS: %x",getSystemStatus());
    double energy = getEnergy();
    double energyRev=getEnergyRev();
     accumulatedEnergy =accumulatedEnergy+ energy;
    accumulatedRevEnergy =accumulatedRevEnergy+ energyRev; 
    ESP_LOGI(TAG,"energy fwd : %f , rev : %f",energy,energyRev);
    ESP_LOGI(TAG,"accumulate energy fwd : %f  ,  rev: %f",accumulatedEnergy,accumulatedRevEnergy);
    

    vTaskDelay(200);
    }
    
    
    
   
}