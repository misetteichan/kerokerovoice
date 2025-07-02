/*
  Example for KeroKeroVoice
  This example combines the KeroKeroVoice Library with the M5Stack ModuleLLM.
  
  * This example has been tested on the M5Stack CoreS3.
  * It may also work on the Basic and CORE2, but some adjustments may be necessary.

  Dependencies
  - M5ModuleLLM (v1.0.0 for the initial lot, v1.3.0 or later for later lots)
  - M5Stack_Avatar v0.10.0

  Copyright (c) 2025 misetteichan
  Licensed under the MIT License. See LICENSE file for details.
*/

#include <Avatar.h>
#include <kerokerovoice.h>
#include <M5ModuleLLM.h>
#include "message.hpp"
#include "speechduration.hpp"

KeroKeroVoice keroVoice;
m5avatar::Avatar avatar;
M5ModuleLLM  llm;
String work_id;

SpeechDuration duration(avatar);
Message message(avatar);

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.setTimeout(0);
  M5.Speaker.setVolume(120);
  M5.Speaker.end();
  M5.Mic.begin();  
  
  duration.init();
  
  keroVoice.init(M5.Speaker);

  avatar.init();
  avatar.addTask(lip_sync, "lipSync");
  avatar.setSpeechFont(&fonts::efontJA_16);

  m5avatar::ColorPalette cp;
  auto setColor = [&cp](uint16_t color) {
    cp.set(COLOR_BACKGROUND, color);
    avatar.setColorPalette(cp);
  };

  // setup ModuleLLM
  setColor(ORANGE);
  const auto rx = M5.getPin(m5::pin_name_t::port_c_rxd);
  const auto tx = M5.getPin(m5::pin_name_t::port_c_txd);
  Serial2.begin(115200, SERIAL_8N1, rx, tx);
  llm.begin(&Serial2);
  while (!llm.checkConnection()) {
    ;
  }
  setColor(ORANGE);
  llm.sys.reset();
  setColor(RED);
  work_id = llm.llm.setup();

  // Setup completed
  setColor(BLACK);
  avatar.setExpression(m5avatar::Expression::Sleepy);
}

void loop() {
  M5.update();
  message.update();
  
  // Touch the panel or press ButtonA to toggle the speech bubble display.
  if ((M5.Touch.isEnabled() && M5.Touch.getCount() > 0) || M5.BtnA.wasPressed()) {
    M5.Mic.end();
    M5.Speaker.begin();
    M5.Speaker.tone(message.toggle() ? 1000 : 400, 100);
    delay(100);
    while ((M5.Touch.isEnabled() && M5.Touch.getCount() > 0) || M5.BtnA.wasPressed()) {
      M5.update();
    }
    M5.Speaker.end();
    M5.Mic.begin();
  }

  auto t = duration.get();
  // Send a message to LLM if a certain period of time has elapsed since the voice was detected.
  if (t > 300) {
    M5.Mic.end();
    M5.Speaker.begin();

    avatar.setExpression(m5avatar::Expression::Doubt);

    auto input = randomHiragana(t / 50);
    Serial.printf("<< %s\r\n", input.c_str());

    // Send a message to LLM and wait for the result.
    llm.llm.inferenceAndWaitResult(work_id, input.c_str(), [](String& result) {
      Serial.printf(">> %s\r\n", result.c_str());
      message.set(result.c_str());
      // Play the response with KeroKeroVoice.
      keroVoice.play(result, 1.8);
    });
    avatar.setExpression(m5avatar::Expression::Sleepy);
    M5.Speaker.end();
    M5.Mic.begin();
  }
}

// control the mouth of the avatar.
void lip_sync(void* args) {
  auto ctx = static_cast<m5avatar::DriveContext*>(args);
  auto avatar = ctx->getAvatar();
  for (;;) {
    auto ratio = keroVoice.level() / 100.0f;
    avatar->setMouthOpenRatio(ratio);
    delay(50);
  }
}

// generate random hiragana.
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
