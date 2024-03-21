#include "BluetoothA2DPSource.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
  
BluetoothA2DPSource a2dpSource;

BLEServer *pServer;
BLECharacteristic *pCharacteristic;

// Taken from this example
//https://github.com/pschatzmann/ESP32-A2DP/blob/main/examples/bt_music_sender/bt_music_sender.ino
int32_t generateSoundData(Frame *data, int32_t frameCount)
{
  static float m_time = 0.0;
    float m_amplitude = 10000.0;  // -32,768 to 32,767
    float m_deltaTime = 1.0 / 44100.0;
    float m_phase = 0.0;
    float pi_2 = PI * 2.0;
    float c3_frequency = 130.81;
    // fill the channel data
    for (int sample = 0; sample < frameCount; ++sample) {
        float angle = pi_2 * c3_frequency * m_time + m_phase;
        frame[sample].channel1 = m_amplitude * sin(angle);
        frame[sample].channel2 = frame[sample].channel1;
        m_time += m_deltaTime;
    }

  delay(1);

  return frameCount
}

void setup()
{
  a2dpSource.start("A2DP Bluetooth Server", generateSoundData);
}

void loop()
{
  // The example did this
  delay(1000);
}