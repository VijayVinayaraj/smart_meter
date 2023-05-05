#ifndef __realtime_h__
#define __realtime_h__
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "esp_sntp.h"


static void initialize_sntp(void);
void time_sync_notification_cb(struct timeval *tv);
char* getTime();
static void obtain_time(void);

#endif