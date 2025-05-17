// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include "kerokerovoice.hpp"
#include <vector>
#include <tuple>
#include "wav/kerowav.hpp"

namespace {

constexpr size_t buf_num = 3;
constexpr size_t buf_size = 1024;
uint8_t wav_data[buf_num][buf_size];

struct __attribute__((packed)) wav_header_t {
  char RIFF[4];
  uint32_t chunkSize;
  char WAVE[4];
  struct __attribute__((packed)) fmt_t {
    char fmt_[4];
    uint32_t chunkSize;
    uint16_t formatTag;
    uint16_t channels;
    uint32_t samplesPerSec;
    uint32_t aveBytesPerSec;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    bool isOK() const {
      if (memcmp(fmt_, "fmt ", 4)) {
        return false;
      }
      if (formatTag != 1 || bitsPerSample < 8 || bitsPerSample > 16 || channels == 0 || channels > 2) {
        return false;
      }      
      return true;
    }
  } fmt;

  bool isOK() const {
    if (memcmp(RIFF, "RIFF", 4)) {
      return false;
    }
    if (memcmp(WAVE, "WAVE", 4)) {
      return false;
    }
    if (!fmt.isOK()) {
      return false;
    }
    return true;
  }
};

struct __attribute__((packed)) chunk_t {
  char chunkId[4];
  uint32_t chunkSize;
};

const unsigned char* find(const char* kana) {
  for (const auto& pair : mojiarray) {
    if (strcmp(pair.first, kana) == 0) {
      return pair.second;
    }
  }
  return nullptr;
}

std::vector<std::tuple<String, const unsigned char*>> split(const String& text) {
  std::vector<std::tuple<String, const unsigned char*>> r;
  for (auto i = 0; i < text.length();) {
    uint16_t code = text[i] & 0xff;
    size_t sz = 1;
    if ((code & 0x80) == 0x00) {
    }
    else if ((code & 0xe0) == 0xc0) {
      sz = 2;
      code = ((text[i] & 0x1f) << 6) | (text[i+1] & 0x3f);
    }
    else if ((code & 0xf0) == 0xe0) {
      sz = 3;
      code = ((text[i] & 0x0f) << 12) |
             ((text[i+1] & 0x3f) << 6) |
             (text[i+2] & 0x3f);
    }
    else if ((code & 0xf8) == 0xf0) {
      sz = 4;
      code = ((text[i] & 0x07) << 18) |
             ((text[i+1] & 0x3f) << 12) | 
             ((text[i+2] & 0x3f) << 6) |
             (text[i+3] & 0x3f);
    }

    const unsigned char* ptr = nullptr;
    if (code >= 0x3040 && code <= 0x309f) {
      // ひらがなだよ
      ptr = find(text.substring(i, i+sz).c_str());
    }
    else {
      if (code >= 0x30a0 && code <= 0x30ff) {
        // カタカナだからひらがなにおきかえ
        code -= 0x60;
      }
      else {
        // その他・・とりあえずランダムで
        code = 0x3041 + (rand() % (0x3097 - 0x3041 + 1));
      }
      ptr = find(
        (String((char)(0xe0 | ((code >> 12) & 0x0f))) +
         String((char)(0x80 | ((code >> 6) & 0x3f))) +
         String((char)(0x80 | (code & 0x3f)))).c_str());
    }
    r.push_back(std::tuple<String, const unsigned char*>(text.substring(i, i+sz), ptr));
    i += sz;
  }
  return r;
}

template <typename T>
T constrain(T value, T min, T max) {
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}

template <typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

}  // namespace

void KeroKeroVoice::init(m5::Speaker_Class& speaker) {
  _level = 0;
  _speaker = &speaker;
}

bool KeroKeroVoice::play(const unsigned char* src, double rate, m5::Speaker_Class& speaker) {
  if (!src) {
    return false;
  }
  auto ptr = src;
  auto header = reinterpret_cast<const wav_header_t*>(ptr);
  ptr += sizeof(wav_header_t);
  if (!header->isOK() || header->fmt.bitsPerSample != 16) {
    return false;
  }
  auto chunk = reinterpret_cast<const chunk_t*>(ptr);
  ptr += sizeof(chunk_t);
  //TODO: 範囲外チェック
  while (memcmp(chunk->chunkId, "data", 4)) {
    ptr += chunk->chunkSize;
    chunk = reinterpret_cast<const chunk_t*>(ptr);
    ptr += sizeof(chunk_t);
  }
  auto data_len = chunk->chunkSize;
  size_t idx = 0;
  while (data_len > 0) {
    size_t len = data_len < buf_size ? data_len : buf_size;

    const auto hi = 8000;
    auto level = constrain(abs(*(const int16_t*)ptr), 0, hi);
    _level = map((level < 100 ? 0 : level), 0, hi, 0, 100);
  
    memcpy(wav_data[idx], ptr, len);
    ptr += len;
    data_len -= len;

    speaker.playRaw(
      (const int16_t*)wav_data[idx],
      len >> 1,
      header->fmt.samplesPerSec * rate, header->fmt.channels > 1, 1, 0);
    idx += idx < (buf_num - 1) ? 1 : 0;
  }
  return true;
}

void KeroKeroVoice::play(const String& text, double rate, void (*callback)(const String&)) {
  if (_speaker == nullptr) {
    return;
  }
  for (const auto& pair : split(text)) {
    if (callback) {
      auto c = std::get<0>(pair);
      callback(c);
    }
    auto p = std::get<1>(pair);
    if (p == nullptr) {
      continue;
    }
    if (!play(p, rate, *_speaker)) {
      continue;
    }
    while (_speaker->isPlaying()) {
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    _level = 0;
  }
}

void KeroKeroVoice::random(int length, double rate, void (*callback)(const String&)) {
  String randomText;
  for (int i = 0; i < length; ++i) {
    uint16_t code = 0x3041 + (rand() % (0x3097 - 0x3041 + 1));
    randomText += String((char)(0xe0 | ((code >> 12) & 0x0f))) +
                  String((char)(0x80 | ((code >> 6) & 0x3f))) +
                  String((char)(0x80 | (code & 0x3f)));
  }
  play(randomText, rate, callback);
}
