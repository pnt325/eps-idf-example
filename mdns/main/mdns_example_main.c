/* MDNS-SD Query and advertise Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif_ip_addr.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "mdns.h"
#include "driver/gpio.h"
#include "netdb.h"

#define TAG "mdns"

static void ex_mdns_init(const char* hostname, uint16_t port, mdns_txt_item_t* items, size_t item_size, const char* service_name, const char* ins_name)
{
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set(hostname));
    ESP_ERROR_CHECK(mdns_instance_name_set(ins_name));
    ESP_ERROR_CHECK(mdns_service_add(service_name, "_http", "_tcp", port, items, item_size));
}

static void initialise_mdns(void)
{
    char * hostname = "ac-socket";
    ESP_LOGI(TAG,"Hostname: %s", hostname);

    //initialize mDNS
    ESP_ERROR_CHECK( mdns_init() );
    //set mDNS hostname (required if you want to advertise services)
    ESP_ERROR_CHECK( mdns_hostname_set(hostname) );
    ESP_LOGI(TAG, "mdns hostname set to: [%s]", hostname);
    //set default mDNS instance name
    ESP_ERROR_CHECK( mdns_instance_name_set("Example mdns") );

    //structure with TXT records
    mdns_txt_item_t serviceTxtData[3] = {
        {"board", "esp32"},
        {"u", "user"},
        {"p", "password"}
    };

    //initialize service
    ESP_ERROR_CHECK( mdns_service_add("AC-Socket", "_http", "_tcp", 80, serviceTxtData, 3) );
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // initialise_mdns();
    mdns_txt_item_t items[] = 
    {
        {"sn", "ABC-DEF"},
        {"connection", "tcp-local"},
        {"is_init", "false"}
    };
    ex_mdns_init("ac-socket", 80, items, 3, "Smart AC-Socket", "AC Socket Smart Home");

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
}
