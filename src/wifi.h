#ifndef __wifi_h__
#define __wifi_h__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/queue.h"
#define SSID "wifi"
 #define PASSWORD "1234567890"
//#define SSID "GNXS"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_init();
void event_handler(void* arg, esp_event_base_t event_base, int32_t eventId , void* event_data);
 
#endif