#ifndef __httpapi_h__
#define __httpapi_h__



#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "esp_crt_bundle.h"


#define SSID "wifi"
#define PASSWORD "1234567890"
#define EXAMPLE_ESP_MAXIMUM_RETRY  10


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1





void event_handler(void* arg, esp_event_base_t event_base, int32_t eventId , void* event_data);
void wifi_init();
void post_power(double power);
void post_current(double current);
void postCurrentAndPower(double current, double power);
esp_err_t _http_event_handler(esp_http_client_event_handle_t event);
void httpInit();





#endif