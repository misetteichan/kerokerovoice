//  Copyright (c) 2025 misetteichan
//  Licensed under the MIT License. See LICENSE file for details.

#ifndef SPEECH_DURATION_HPP
#define SPEECH_DURATION_HPP

#include <Avatar.h>

// Measure the duration of speech.
// It measures the duration of speech by measuring the level of the microphone.
class SpeechDuration {
 public:
  explicit SpeechDuration(m5avatar::Avatar& o) : avatar(o) {}

  void init() {
    buffer = static_cast<int16_t*>(heap_caps_malloc(size * sizeof(int16_t), MALLOC_CAP_8BIT));
    memset(buffer, 0 , size * sizeof(int16_t));
  }

  uint32_t get() {
    if (isSpeaking()) {
      if (!measuring) {
        avatar.setExpression(m5avatar::Expression::Neutral);
        start_time = millis();
        measuring = true;
      }
      last_time = millis();
    } else if (measuring && (millis() - last_time > 400)) {
      measuring = false;
      auto d = last_time - start_time;
      avatar.setExpression(d < 200 ? m5avatar::Expression::Sleepy : m5avatar::Expression::Neutral);
      return d < 200 ? 0 : d;
    }
    return 0;
  }

 private:
  m5avatar::Avatar& avatar;
  const size_t size = 200;
  int16_t* buffer;
  bool measuring = false;
  uint32_t start_time = 0;
  uint32_t last_time = 0;

  bool isSpeaking() const {
    if (M5.Mic.isEnabled()) {
      if (M5.Mic.record(buffer, size, 16000)) {
        auto ave = 0;
        for (auto i = 0; i < size; i += 2) {
          ave += abs(buffer[i]);
        }
        ave /= size / 2;
        if (ave > 250) {
          return true;
        }
      }
    }
    return false;
  }
};

#endif // SPEECH_DURATION_HPP
