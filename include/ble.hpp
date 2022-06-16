#pragma once

#include <string>
#include <vector>
#include <array>

#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"

#define BLE_TAG "BLE"

class BLE {

    static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
    static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

    static esp_ble_adv_params_t     adv_params;

    BLE();
    BLE(const BLE&);

public:

    static std::string              application_name;
    static std::array<uint8_t, 16>  uuid;
    static std::vector<uint8_t>     service_data;
    static std::vector<uint8_t>     manufacturer_data;

    static void begin();

};
