# <center>🐸 🐸🐸🐸🐸 🐸🐸🐸 🐸</center>

[English](README.md)  |  [日本語](README_ja.md)

🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

## 🐸🐸🐸

🐸🐸🐸🐸🐸🐸  
https://youtu.be/gGdDbJI6MwA

## 🐸🐸🐸🐸🐸🐸

### 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

`platformio.ini`🐸lib_deps🐸kerokerovoice🐸🐸🐸🐸🐸🐸

```ini
lib_deps = https://github.com/misetteichan/kerokerovoice.git
```

### 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

🐸. 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸  
🐸. 「🐸🐸🐸🐸🐸」→「🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸」→「.ZIP🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸」

## 🐸🐸🐸

```cpp
#include <M5Unified.h>
#include <kerokerovoice.h>

KeroKeroVoice kero;

void setup() {
  M5.begin();
  M5.Display.setFont(&fonts::efontJA_16);

  // 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸
  kero.init(M5.Speaker);
}

void loop() {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    // 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸
    kero.play("こんにちは", 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
  if (M5.BtnB.wasPressed()) {
    // 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸
    kero.random(10, 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
}
```

### 🐸🐸🐸🐸🐸🐸

- `void init(m5::Speaker_Class& speaker)`
  - 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸
  - 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

- `void play(const String& text, double rate, void (*callback)(const String&) = nullptr)`
  - 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸 (🐸🐸🐸🐸🐸🐸)
  - `text`: 🐸🐸🐸🐸🐸🐸🐸🐸🐸（🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸）
  - `rate`: 🐸🐸🐸🐸🐸🐸🐸（🐸🐸🐸🐸🐸🐸🐸🐸）
  - `callback`: 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

- `void random(int length, double rate, void (*callback)(const String&) = nullptr)`
  - 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸 (🐸🐸🐸🐸🐸🐸)
  - `length`: 🐸🐸🐸🐸🐸🐸（🐸🐸🐸）
  - `rate`: 🐸🐸🐸🐸🐸（🐸🐸🐸🐸🐸🐸🐸🐸）
  - `callback`: 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

## 🐸🐸🐸🐸🐸🐸🐸

- [M5Unified](https://github.com/m5stack/M5Unified) - M5Stack🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸

## 🐸🐸🐸🐸🐸

MIT🐸🐸🐸🐸🐸🐸. 🐸🐸🐸🐸🐸[LICENSE](LICENSE)🐸🐸🐸🐸🐸🐸🐸🐸🐸

## 🐸🐸🐸🐸🐸🐸

🐸🐸🐸🐸🐸🐸🐸🐸🐸、HTS Voice "Mei" 🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸🐸  
🐸🐸🐸🐸🐸🐸🐸 OpenJTalk 🐸🐸🐸🐸🐸🐸🐸🐸🐸

HTS Voice "Mei"  
released by MMDAgent Project Team  
http://www.mmdagent.jp/  
Copyright (c) 2009–2018  Nagoya Institute of Technology Department of Computer Science  

Licensed under the Creative Commons Attribution 3.0 license.  
See: https://creativecommons.org/licenses/by/3.0/ for details
