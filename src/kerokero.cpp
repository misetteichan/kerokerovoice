// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include "kerokero.hpp"
#include <LittleFS.h>
#include <vector>

namespace {

struct __attribute__((packed)) riff_t {
  char RIFF[4];
  uint32_t chunkSize;
  char WAVE[4];
};

struct __attribute__((packed)) chunk_t {
  char chunkId[4];
  uint32_t chunkSize;
};

struct __attribute__((packed)) format_t {
  uint16_t formatTag;
  uint16_t channels;
  uint32_t samplesPerSec;
  uint32_t aveBytesPerSec;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
};

constexpr size_t buf_num = 3;
constexpr size_t buf_size = 1024;
uint8_t wav_data[buf_num][buf_size];

bool playWave(fs::File& file, m5::Speaker_Class& speaker) {
  if (!file) {
    return false;
  }
  riff_t riff;
  if (file.read(reinterpret_cast<uint8_t*>(&riff), sizeof(riff_t)) != sizeof(riff_t)) {
    return false;
  }
  if (memcmp(riff.RIFF,"RIFF",4) || memcmp(riff.WAVE, "WAVE",4)) {
    return false;
  }
  chunk_t chunk = {0};
  while (memcmp(chunk.chunkId, "fmt ", 4) != 0) {
    if (!file.seek(chunk.chunkSize, SeekMode::SeekCur)) {
      return false;
    }
    if (file.read(reinterpret_cast<uint8_t*>(&chunk), sizeof(chunk_t)) != sizeof(chunk_t)) {
      return false;
    }
  }
  if (chunk.chunkSize != sizeof(format_t)) {
    return false;
  }
  format_t format;
  if (file.read(reinterpret_cast<uint8_t*>(&format), sizeof(format_t)) != sizeof(format_t)) {
    return false;
  }
  if (format.formatTag != 1 || format.bitsPerSample < 8 || format.bitsPerSample > 16 ||
      format.channels == 0 || format.channels > 2) {
      return false;
  }
  chunk.chunkSize = 0;
  while (memcmp(chunk.chunkId, "data", 4) != 0) {
    if (!file.seek(chunk.chunkSize, SeekMode::SeekCur)) {
      return false;
    }
    if (file.read(reinterpret_cast<uint8_t*>(&chunk), sizeof(chunk_t)) != sizeof(chunk_t)) {
      return false;
    }
  }
  auto data_len = chunk.chunkSize;
  const auto flg_16bit = format.bitsPerSample >> 4;
  uint32_t front = data_len * 0.2;
  front -= front % format.blockAlign;
  uint32_t rear = data_len * 0.5;
  rear -= rear % format.blockAlign;

  data_len -= front + rear;
  format.samplesPerSec *= 2.0;
  file.seek(front, SeekMode::SeekCur);

  size_t idx = 0;
  while (data_len > 0) {
    size_t len = data_len < buf_size ? data_len : buf_size;
    len = file.read(wav_data[idx], len);
    data_len -= len;

    speaker.playRaw((const int16_t*)wav_data[idx], len >> 1, format.samplesPerSec, format.channels > 1, 1, 0);
    idx += idx < (buf_num - 1) ? 1 : 0;
  }
  return true;
}

constexpr std::array<std::pair<const char*, const char*>, 71> mojis = {{
    {"あ", "a"}, {"い", "i"}, {"う", "u"}, {"え", "e"}, {"お", "o"},
    {"か", "ka"}, {"き", "ki"}, {"く", "ku"}, {"け", "ke"}, {"こ", "ko"},
    {"が", "ga"}, {"ぎ", "gi"}, {"ぐ", "gu"}, {"げ", "ge"}, {"ご", "go"},
    {"さ", "sa"}, {"し", "si"}, {"す", "su"}, {"せ", "se"}, {"そ", "so"},
    {"ざ", "za"}, {"じ", "ji"}, {"ず", "zu"}, {"ぜ", "ze"}, {"ぞ", "zo"},
    {"た", "ta"}, {"ち", "ti"}, {"つ", "tu"}, {"て", "te"}, {"と", "to"},
    {"だ", "da"}, {"ぢ", "ji"}, {"づ", "zu"}, {"で", "de"}, {"ど", "do"},
    {"な", "na"}, {"に", "ni"}, {"ぬ", "nu"}, {"ね", "ne"}, {"の", "no"},
    {"は", "ha"}, {"ひ", "hi"}, {"ふ", "fu"}, {"へ", "he"}, {"ほ", "ho"},
    {"ば", "ba"}, {"び", "bi"}, {"ぶ", "bu"}, {"べ", "be"}, {"ぼ", "bo"},
    {"ぱ", "pa"}, {"ぴ", "pi"}, {"ぷ", "pu"}, {"ぺ", "pe"}, {"ぽ", "po"},
    {"ま", "ma"}, {"み", "mi"}, {"む", "mu"}, {"め", "me"}, {"も", "mo"},
    {"や", "ya"}, {"ゆ", "yu"}, {"よ", "yo"},
    {"ら", "ra"}, {"り", "ri"}, {"る", "ru"}, {"れ", "re"}, {"ろ", "ro"},
    {"わ", "wa"}, {"を", "wo"}, {"ん", "n"},
    // {"ぁ", "a"}, {"ぃ", "i"}, {"ぅ", "u"}, {"ぇ", "e"}, {"ぉ", "o"},
    // {"っ", "tu"},{"ゃ", "ya"}, {"ゅ", "yu"}, {"ょ", "yo"},
}};

const char* find(const char* kana) {
  for (const auto& pair : mojis) {
    if (strcmp(pair.first, kana) == 0) {
      return pair.second;
    }
  }
  return nullptr;
}

std::vector<std::tuple<String, const char*>> split(const String& text) {
  std::vector<std::tuple<String, const char*>> r;
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

    const char* ptr = nullptr;
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
    r.push_back(std::tuple<String, const char*>(text.substring(i, i+sz), ptr));
    i += sz;
  }
  return r;
}

}  // namespace

bool KeroKero::init() {
  if (!LittleFS.begin()) {
    return false;
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
    auto n = "/" + String(p) + ".wav";
    if (!LittleFS.exists(n)) {
      continue;
    }
    auto f = LittleFS.open(n);
    playWave(f, speaker);
    f.close();
    while (speaker.isPlaying()) {
      delay(1);
    }
  }
}
