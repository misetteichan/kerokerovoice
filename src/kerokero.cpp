// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include "kerokero.hpp"
#include <LittleFS.h>
#include <vector>
#include "kerowav.hpp"

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
        code = random(0x3041, 0x3097);
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

}  // namespace

bool KeroKero::init() {
  return true;
}

bool KeroKero::play(const unsigned char* src, m5::Speaker_Class& speaker) {
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
  // uint32_t front = data_len * 0.0;
  // front -= front % header->fmt.blockAlign;
  // uint32_t rear = data_len * 0.0;
  // rear -= rear % header->fmt.blockAlign;
  // data_len -= front + rear;
  // ptr += front;

  size_t idx = 0;
  while (data_len > 0) {
    size_t len = data_len < buf_size ? data_len : buf_size;

    const auto hi = 8000; // 適当に変えて
    auto level = constrain(abs(*(const int16_t*)ptr), 0, hi);
    _level = map((level < 100 ? 0 : level), 0, hi, 0, 100);
  
    memcpy(wav_data[idx], ptr, len);
    ptr += len;
    data_len -= len;
    
    
    speaker.playRaw(
      (const int16_t*)wav_data[idx],
      len >> 1,
      header->fmt.samplesPerSec * 1.8, header->fmt.channels > 1, 1, 0);
    idx += idx < (buf_num - 1) ? 1 : 0;
  }
  return true;
}

void KeroKero::play(const String& text, m5::Speaker_Class& speaker, void (*func)(String&)) {
  for (const auto& pair : split(text)) {
    if (func) {
      auto c = std::get<0>(pair);
      func(c);
    }
    auto p = std::get<1>(pair);
    if (p == nullptr) {
      continue;
    }
    if (!play(p, speaker)) {
      continue;
    }
    while (speaker.isPlaying()) {
      delay(1);
    }
    _level = 0;
  }
}
