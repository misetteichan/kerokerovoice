// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include <M5Unified.h>
#include <Avatar.h>
#include <M5ModuleLLM.h>
#include "kerokero.hpp"

static KeroKero kerokero;
static m5avatar::Avatar avatar;
static M5ModuleLLM  llm;
static String work_id;

class OnseiDuration {
 public:
  void init() {
    buffer = static_cast<int16_t*>(heap_caps_malloc(size * sizeof(int16_t), MALLOC_CAP_8BIT));
    memset(buffer, 0 , size * sizeof(int16_t));
  }

  uint32_t get() {
    if (is_speaking()) {
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
  const size_t size = 200;
  int16_t* buffer;
  bool measuring = false;
  uint32_t start_time = 0;
  uint32_t last_time = 0;

  bool is_speaking() {
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
} duration;

void lip_sync(void* args) {
  auto ctx = static_cast<m5avatar::DriveContext*>(args);
  auto avatar = ctx->getAvatar();
  for (;;) {
    auto ratio = kerokero.level() / 100.0f;
    avatar->setMouthOpenRatio(ratio);
    delay(50);
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.setTimeout(0);
  M5.Speaker.setVolume(120);
  M5.Speaker.end();
  M5.Mic.begin();  
  duration.init();

  avatar.init();
  avatar.addTask(lip_sync, "lipSync");

  if (!kerokero.init()) {
    M5_LOGE("error");
  }

  switch (M5.getBoard()) {
  case m5::board_t::board_M5Stack:
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    break;
  case m5::board_t::board_M5StackCore2:
    Serial2.begin(115200, SERIAL_8N1, 13, 14);
    break;
  case m5::board_t::board_M5StackCoreS3:
    Serial2.begin(115200, SERIAL_8N1, 18, 17);
    break;
  default:
    M5_LOGE("not support..");
    break;
  }

  m5avatar::ColorPalette cp;
  auto set = [&cp](uint16_t color) {
    cp.set(COLOR_BACKGROUND, color);
    avatar.setColorPalette(cp);
  };

  set(ORANGE);
  llm.begin(&Serial2);
  while (!llm.checkConnection()) {
    ;
  }
  set(ORANGE);
  llm.sys.reset();

  set(RED);
  work_id = llm.llm.setup();

  set(BLACK);
  avatar.setExpression(m5avatar::Expression::Sleepy);
}

String randomHiragana(int length) {
  static const char* hiragana =
    "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん";

  String result = "";
  int hiraganaLength = strlen(hiragana) / 3;
  for (int i = 0; i < length; ++i) {
      int index = esp_random() % hiraganaLength;
      result += String(hiragana).substring(index * 3, (index + 1) * 3);
  }
  return result;
}


void loop() {
  auto t = duration.get();
  if (t > 300) {
    M5.Mic.end();
    M5.Speaker.begin();

    avatar.setExpression(m5avatar::Expression::Doubt);

    auto input = randomHiragana(t / 50);
    Serial.printf("<< %s\r\n", input.c_str());

    llm.llm.inferenceAndWaitResult(work_id, input.c_str(), [](String& result) {
      Serial.printf(">> %s\r\n", result.c_str());
      kerokero.play(result, M5.Speaker);
    });
    avatar.setExpression(m5avatar::Expression::Sleepy);
    M5.Speaker.end();
    M5.Mic.begin();
  }
}
