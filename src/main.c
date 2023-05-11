
#include "main.h"
//static const char TAG[]="main";

void app_main(){

    spi_init();
    calibrateIC();
double accumulatedEnergy =0;
 //double accumulatedRevEnergy=0;
//     for (;;){
//     ESP_LOGI(TAG,"meter status: %x",getMeterStatus());
//     ESP_LOGI(TAG,"AP: %f",getActivePower());
//     ESP_LOGI(TAG,"LC: %f",getLineCurrent());
//     ESP_LOGI(TAG,"LV: %f",getLineVoltage());
//     ESP_LOGI(TAG,"PF: %f",getPowerFactor());
//     ESP_LOGI(TAG,"SS: %x",getSystemStatus());
    double energy = getEnergy();
    double current = getLineCurrent();
    double voltage = getLineVoltage();
//     double energyRev=getEnergyRev();
    accumulatedEnergy =accumulatedEnergy+ energy;
//     accumulatedRevEnergy =accumulatedRevEnergy+ energyRev; 
   // ESP_LOGI(TAG,"energy fwd : %f , rev : %f",energy,energyRev);
   //  ESP_LOGI(TAG,"accumulate energy fwd : %f  ,  rev: %f",accumulatedEnergy,accumulatedRevEnergy);
    

//     vTaskDelay(200);
//     }
wifi_init();
mqttInit();

char json[200];
    for(;;){
       char time[64];
      strcpy(time,getTime());
      sprintf(json,"{\"Time\":\"%s\",\"Energy\":%f,\"AccEnergy\":%f,\"Current\":%f,\"Voltage\":%f}",time,energy,accumulatedEnergy,current,voltage);
     // ESP_LOGI(TAG,"from main : %s", json);
     mqttSendDData(json);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
    
   
}

