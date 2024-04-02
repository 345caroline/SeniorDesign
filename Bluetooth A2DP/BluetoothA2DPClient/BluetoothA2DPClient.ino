//Documentation for when things go wrong
//https://pschatzmann.github.io/ESP32-A2DP/html/class_bluetooth_a2_d_p_sink.html

//Taken from this example
//https://github.com/pschatzmann/ESP32-A2DP/blob/main/examples/bt_music_receiver_and_BLE/bt_music_receiver_and_BLE.ino

#include "BluetoothA2DPSink.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BluetoothA2DPSink a2dp_sink;
BLECharacteristic *pCharacteristic;
char title[160] = {"Undefined"};

void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
  Serial.printf("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
  if (id == ESP_AVRC_MD_ATTR_TITLE) {
    strncpy(title, (const char *)text, 160);
    pCharacteristic->setValue(title);
  }
}

void connection_state_changed(esp_a2d_connection_state_t state, void *ptr){
  Serial.println(a2dp_sink.to_str(state));
}

void setup() {
  // start a2dp in ESP_BT_MODE_BTDM mode
  Serial.begin(115200);
  a2dp_sink.set_default_bt_mode(ESP_BT_MODE_BTDM);
  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.start("A2DP Bluetooth Client");
  Serial.println("A2DP Started!");

  // start BLE
  BLEDevice::init("Bluetooth A2DP Client");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);

  pCharacteristic->setValue(title);
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(
      0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  delay(1000); // do nothing
}