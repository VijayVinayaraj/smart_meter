#include "httpapi.h"

EventGroupHandle_t wifi_event_group;

static const char *TAG = "wifi"; 
static const char *TAG_HTTP ="http";



void httpInit()
{	
    
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    };

    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG,"wifi sta mode start");
    wifi_init();
   
    

   
    
   
    
}


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











esp_err_t _http_event_handler(esp_http_client_event_handle_t event){
    static char *output_buffer;
    static int output_len;
    switch(event->event_id){
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG_HTTP, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG_HTTP, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
                ESP_LOGD(TAG_HTTP,"HTTP_EVENT_ON-DATA len = %d",event->data_len);

                if(!esp_http_client_is_chunked_response(event->client)){
                    if(event->user_data){
                        memcpy(event->user_data+ output_len,event->data,event->data_len);

                    }else{
                        if(output_buffer ==NULL){
                            output_buffer = (char *) malloc(esp_http_client_get_content_length(event->client));
                            output_len=0;
                            if(output_buffer==NULL){
                                ESP_LOGE(TAG_HTTP,"failed to allocate memory for output buffer");
                                return ESP_FAIL;
                            }
                        }
                        memcpy(output_buffer + output_len,event->data,event->data_len);

                    }
                    output_len +=event->data_len;
                }

            break;
         case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG_HTTP, "HTTP_EVENT_ON_FINISH");
            if(output_buffer !=NULL){
                free(output_buffer);
                output_buffer=NULL;
                output_len=0;
            }
            break;
         case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_HTTP, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err=0;
            esp_err_t err = esp_tls_get_and_clear_last_error(event->data,&mbedtls_err,NULL);
            if (err!=0){
                if(output_buffer !=NULL){
                    free(output_buffer);
                    output_buffer=NULL;
                    output_len=0;

                }
                ESP_LOGI(TAG_HTTP,"last esp error code : 0x%x",err);
                ESP_LOGI(TAG_HTTP,"last mbedtls failure: 0x%x",mbedtls_err);
            }
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG_HTTP, "HTTP_EVENT_REDIRECT");
            break;
        default:
         ESP_LOGI(TAG_HTTP,"error");
         break;
    }
    return ESP_OK;
}



void postCurrentAndPower(double current, double power){
     esp_http_client_config_t config={
        .url="https://fair-dingo-63.hasura.app/api/rest/post-powerandcurrent",
        .event_handler=_http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .method=HTTP_METHOD_POST,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
     
    if(current > 0){
     char str[50];
    sprintf(str,"{\"current\":%f,\"power\":%f}",current,power);

    ESP_LOGI(TAG_HTTP,"%s",str);
    
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client,str, strlen(str));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        // ESP_LOGI(TAG_HTTP, "HTTP POST Status = %d, content_length = %lld",
        //         esp_http_client_get_status_code(client),
        //         esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG_HTTP, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
     }
}

void post_current(double current){
    esp_http_client_config_t config={
        .url="https://fair-dingo-63.hasura.app/api/rest/post-current",
        .event_handler=_http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .method=HTTP_METHOD_POST,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
     
    if(current > 0){
     char str[50];
    sprintf(str,"{ \"current\": %f}",current);

   // ESP_LOGI(TAG_HTTP,"%s",str);
    
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client,str, strlen(str));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        // ESP_LOGI(TAG_HTTP, "HTTP POST Status = %d, content_length = %lld",
        //         esp_http_client_get_status_code(client),
        //         esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG_HTTP, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
     }
 }


void post_power(double power){
    esp_http_client_config_t config={
        .url="https://fair-dingo-63.hasura.app/api/rest/post-power",
        .event_handler=_http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .method=HTTP_METHOD_POST,
        
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
     
   
     char str[50];
    sprintf(str,"{ \"power\": %f}",power);

   // ESP_LOGI(TAG_HTTP,"%s",str);
    
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client,str, strlen(str));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        // ESP_LOGI(TAG_HTTP, "HTTP POST Status = %d, content_length = %lld",
        //         esp_http_client_get_status_code(client),
        //         esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG_HTTP, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
     }
 
