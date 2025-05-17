# <center>🐸 KEROKERO VOICE 🐸</center>

[日本語](README_ja.md)  |  [KeroKero](README_kero.md)

Let's talk with KEROKERO VOICE!

## Usage Example

YouTube  
https://youtu.be/gGdDbJI6MwA

## Installation

### For PlatformIO

Add kerokerovoice to `lib_deps` in your `platformio.ini`

```ini
lib_deps = https://github.com/misetteichan/kerokerovoice.git
```

### For Arduino IDE

1. Download this repository as ZIP
2. In Arduino IDE: Sketch > Include Library > Add .ZIP Library...

## Quick Start

```cpp
#include <M5Unified.h>
#include <kerokerovoice.h>

KeroKeroVoice kero;

void setup() {
  M5.begin();
  M5.Display.setFont(&fonts::efontJA_16);

  // Initialize KeroKeroVoice
  kero.init(M5.Speaker);
}

void loop() {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    // Play text in KeroKero voice
    kero.play("こんにちは", 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
  if (M5.BtnB.wasPressed()) {
    // Play random KeroKero sounds
    kero.random(10, 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
}
```

## API Reference

- `void init(m5::Speaker_Class& speaker)`
  - Initialize the speaker
  - Must be called first

- `void play(const String& text, double rate, void (*callback)(const String&) = nullptr)`
  - Play text in KeroKero voice (blocking)
  - `text`: Text to play (Hiragana/Katakana recommended)
  - `rate`: Playback speed (1.0 = normal speed, 1.8 recommended)
  - `callback`: Function called for each character played

- `void random(int length, double rate, void (*callback)(const String&) = nullptr)`
  - Play random KeroKero sounds (blocking)
  - `length`: Length of sound in characters
  - `rate`: Playback speed (1.0 = normal speed, 1.8 recommended)
  - `callback`: Function called for each character played

## Dependencies

- [M5Unified](https://github.com/m5stack/M5Unified) - Unified library for M5Stack devices

## License

MIT License. See [LICENSE](LICENSE) for details.

## About the Voice Data

This library includes voice data synthesized using HTS Voice "Mei".
OpenJTalk was used for voice synthesis.

HTS Voice "Mei"  
released by MMDAgent Project Team  
http://www.mmdagent.jp/  
Copyright (c) 2009–2018  Nagoya Institute of Technology Department of Computer Science  

Licensed under the Creative Commons Attribution 3.0 license.  
See: https://creativecommons.org/licenses/by/3.0/ for details
