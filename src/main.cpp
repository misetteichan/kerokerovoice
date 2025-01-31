// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#include <M5Unified.h>
#include "kerokero.hpp"

static KeroKero kerokero;

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.setTimeout(0);
  M5.Display.clear(WHITE);
  M5.Display.setTextSize(1);
  M5.Display.setTextScroll(true);
  M5.Display.setFont(&fonts::efontJA_16);
  M5.Display.setTextColor(GREEN, WHITE);
  M5.Display.printf("KEROKERO\n");
  M5.Display.setTextColor(BLACK, WHITE);
  M5.Speaker.setVolume(120);

  if (!kerokero.init()) {
    M5_LOGE("error");
  }
}

void func(String& c) {
  M5.Display.printf("%s", c);
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    String input("わんわん、わんだふる");
    kerokero.play(input, M5.Speaker, func);
    M5.Display.println();
    return;
  }
  if (Serial.available() > 0) { 
    String input = Serial.readStringUntil('\n');
    Serial.printf("%s\n", input.c_str());
    kerokero.play(input, M5.Speaker, func);
    M5.Display.println();
  }
}
