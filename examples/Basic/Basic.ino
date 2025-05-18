/*
  Basic Example for KeroKeroVoice
  This example demonstrates the basic usage of the KeroKeroVoice library.
*/

#include <M5Unified.h>
#include <kerokerovoice.h>

// Create an instance of KeroKeroVoice
KeroKeroVoice keroVoice;

void setup()
{
  // Initialize M5Stack
  M5.begin();
  // Initialize KeroKeroVoice
  keroVoice.init(M5.Speaker);
  keroVoice.play("こんにちは", 1.8);
}

void loop() {
}
