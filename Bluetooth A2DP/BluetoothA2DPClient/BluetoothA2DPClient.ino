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

int dataRecieved = 0;

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

void avrc_playback_rn_playstatus_changed(esp_avrc_playback_stat_t state, void *ptr)
{
  Serial.println(a2dp_sink.to_str(state));
}

void audio_state_changed(esp_a2d_audio_state_t state, void *ptr)
{
  Serial.println(a2dp_sink.to_str(state));
}

void data_recieved()
{
  dataRecieved++;
}

void setup() {
  // start a2dp in ESP_BT_MODE_BTDM mode
  Serial.begin(115200);
  a2dp_sink.set_default_bt_mode(ESP_BT_MODE_BTDM);
  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  //a2dp_sink.set_avrc_rn_playstatus_callback(avrc_playback_rn_playstatus_changed);
  a2dp_sink.set_on_audio_state_changed(audio_state_changed);
  a2dp_sink.set_on_data_received (data_recieved);
  a2dp_sink.start("A2DP Bluetooth Client");
  Serial.println("A2DP Started!");

  
}

void loop() {
  delay(1000); // do nothing
  Serial.print("Data sent ");
  Serial.print(dataRecieved);
  Serial.println(" times");
}