/*
  Example for KeroKeroVoice
  This example demonstrates how to use the random playback feature
  of the KeroKeroVoice library.

  * Tested on M5Stack Basic.
  * May also work on CORE2 and CoreS3.

  Copyright (c) 2025 misetteichan
  Licensed under the MIT License. See LICENSE file for details.
*/

#include <M5Unified.h>
#include <kerokerovoice.h>

// Create a KeroKeroVoice instance
KeroKeroVoice keroVoice;

void setup() {
  // Initialize M5Stack
  M5.begin();
  M5.Display.setFont(&fonts::efontJA_16);
  // Initialize KeroKeroVoice
  keroVoice.init(M5.Speaker);
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);

    // Play random audio while button A is being held
    keroVoice.random(1.8, []() {
      // Stop playback when button A is released
      M5.update();
      return !M5.BtnA.isPressed();
    }, [](const String& text) {
      // Display the generated text on the screen.
      M5.Display.printf("%s", text.c_str());
    });
  } else if (M5.BtnB.wasPressed()) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    // Play a random 10-character string
    keroVoice.random(10, 1.2, [](const String& text) {
      // Display the generated text on the screen.
      M5.Display.printf("%s", text.c_str());
    });
  }
}
