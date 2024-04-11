//Documentation for when things go wrong
//https://pschatzmann.github.io/ESP32-A2DP/html/class_bluetooth_a2_d_p_source.html#a403b0e21db02c81b948534258e5ff37b

#include "BluetoothA2DPSource.h"

BluetoothA2DPSource a2dpSource;

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
        data[sample].channel1 = m_amplitude * sin(angle);
        data[sample].channel2 = data[sample].channel1;
        m_time += m_deltaTime;
    }

  delay(1);

  return frameCount;
}

void connection_state_changed(esp_a2d_connection_state_t state, void *ptr){
  Serial.println(a2dpSource.to_str(state));
}

void audio_state_changed(esp_a2d_audio_state_t state, void *ptr)
{
  Serial.println(a2dpSource.to_str(state));
}

void setup()
{
  Serial.begin(115200);
  a2dpSource.set_local_name("A2DP Bluetooth Server");
  a2dpSource.set_default_bt_mode(ESP_BT_MODE_BTDM);
  a2dpSource.set_on_connection_state_changed(connection_state_changed);
  a2dpSource.set_on_audio_state_changed(audio_state_changed);
  a2dpSource.start("A2DP Bluetooth Client", generateSoundData);
}

void loop()
{
  // The example did this
  delay(1000);
}