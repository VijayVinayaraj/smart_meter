#include "wifi.h"
static const char TAG[]="wifi";
EventGroupHandle_t wifi_event_group;
void event_handler(void* arg, esp_event_base_t event_base, int32_t eventId , void* event_data){
    static int retry_no =0;
    if (event_base == WIFI_EVENT && eventId==WIFI_EVENT_STA_START){
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED){
            if (retry_no < EXAMPLE_ESP_MAXIMUM_RETRY){
                esp_wifi_connect();
                retry_no++;
                ESP_LOGI(TAG,"retry to connect");
            }else{
                xEventGroupSetBits(wifi_event_group , WIFI_FAIL_BIT);
            }
            ESP_LOGI(TAG,"failed to connect to wifi");
    }
    else if (event_base==IP_EVENT && eventId==IP_EVENT_STA_GOT_IP){
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG,"got ip: "IPSTR,IP2STR(&event->ip_info.ip));
            retry_no=0;
            xEventGroupSetBits(wifi_event_group,WIFI_CONNECTED_BIT);   
    }
}




void wifi_init(){

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    };
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,
                                                        &event_handler,NULL,&instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,
                                                        &event_handler,NULL,&instance_got_ip));
    


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));


    wifi_config_t config={
        .sta={
        .ssid = SSID,
        .password= PASSWORD
    }};
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &config) );
    
    ESP_ERROR_CHECK(esp_wifi_start());
   ESP_LOGI(TAG,"starting connection");
   EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                            WIFI_FAIL_BIT|WIFI_CONNECTED_BIT,
                                            pdFALSE
                                            ,pdFALSE,
                                            portMAX_DELAY);
    if(bits & WIFI_CONNECTED_BIT){
        ESP_LOGI(TAG,"connected to ssis %s and password %s",SSID,PASSWORD);
    }else if (bits&WIFI_FAIL_BIT){
        ESP_LOGI(TAG,"failed to connect to ssid %s and password %s",SSID,PASSWORD);
    }else{
        ESP_LOGI(TAG,"unexpected error");
    }
}