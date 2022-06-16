#include "ble.hpp"

esp_ble_adv_params_t BLE::adv_params = {
    .adv_int_min        = 0x0020,
    .adv_int_max        = 0x0020,
    .adv_type           = ADV_TYPE_NONCONN_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY
};

std::string BLE::application_name = ""; 

void BLE::begin() {
    nvs_flash_init();

    const uint8_t mac[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    esp_base_mac_addr_set(mac);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);

    esp_bluedroid_init();
    esp_bluedroid_enable();
    
    esp_ble_gap_register_callback(gap_event_handler);
    esp_ble_gatts_register_callback(gatts_event_handler);

    esp_ble_gap_set_device_name(application_name.c_str());

    esp_ble_adv_data_t adv_data = {
        .set_scan_rsp = false,
        .include_name = false,
        .include_txpower = false,

        .min_interval = 0xFFFF,
        .max_interval = 0xFFFF,

        .appearance = 0x00,

        .manufacturer_len = 0,
        .p_manufacturer_data = nullptr,

        .service_data_len = 0,
        .p_service_data = nullptr,

        .service_uuid_len = 0,
        .p_service_uuid = nullptr,

        .flag = 0
    };
    esp_ble_gap_config_adv_data(&adv_data);

    esp_ble_gatts_app_register(0);
}

void BLE::gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "Advertising data set complete");
            esp_ble_gap_start_advertising(&adv_params);
            break;

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            ESP_LOGI(BLE_TAG, "Advertising started");
            break;

        default:
            ESP_LOGI(BLE_TAG, "Unknown gap event");
            break;

    }
}

void BLE::gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(BLE_TAG, "Client connected");
            break;

        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(BLE_TAG, "Client disconnected");
            esp_ble_gap_start_advertising(&adv_params);
            break;

        default:
            ESP_LOGI(BLE_TAG, "Unknown gatts event");
            break;
    }
}