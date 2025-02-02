// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include <M5Unified.h>
#include <Avatar.h>
#include "kerokero.hpp"

static KeroKero kerokero;
static m5avatar::Avatar avatar;

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

  if (!kerokero.init()) {
    M5_LOGE("error");
  }
  avatar.init();
  avatar.addTask(lip_sync, "lipSync");
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    String input("うたっておどってファンサして");
    kerokero.play(input, M5.Speaker);
    M5.Display.println();
    return;
  }
  if (Serial.available() > 0) { 
    String input = Serial.readStringUntil('\n');
    Serial.printf("%s\n", input.c_str());
    kerokero.play(input, M5.Speaker);
    M5.Display.println();
  }
}
