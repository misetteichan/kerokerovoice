# <center>🐸 KEROKERO VOICE 🐸</center>

[English](README.md)  |  [KeroKero](README_kero.md)

ケロケロボイスでおしゃべり！

## 使用例

YouTube  
https://youtu.be/gGdDbJI6MwA

## インストール

### PlatformIO

`platformio.ini`の`lib_deps`に`kerokerovoice`を追加します。

```ini
lib_deps = https://github.com/misetteichan/kerokerovoice.git
```

### Arduino IDE

1. このリポジトリをZIPでダウンロード
2. 「スケッチ」→「ライブラリをインクルード」→「.ZIP形式のライブラリをインストール」

## 使い方

```cpp
#include <M5Unified.h>
#include <kerokerovoice.h>

KeroKeroVoice kero;

void setup() {
  M5.begin();
  M5.Display.setFont(&fonts::efontJA_16);

  // KeroKeroVoiceの初期化
  kero.init(M5.Speaker);
}

void loop() {
  M5.update();
  
  if (M5.BtnA.wasPressed()) {
    // テキストをケロケロ声で再生
    kero.play("こんにちは", 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
  if (M5.BtnB.wasPressed()) {
    // ランダムな文字列をケロケロ声で再生
    kero.random(10, 1.8, [](const String& c) {
      M5.Display.printf("%s", c);
    });
    M5.Display.println();
  }
}
```

### メソッド一覧

- `void init(m5::Speaker_Class& speaker)`
  - スピーカーを初期化します
  - 必ず最初に呼び出してください

- `void play(const String& text, double rate, void (*callback)(const String&) = nullptr)`
  - テキストをケロケロ声で再生します (ブロッキング)
  - `text`: 再生するテキスト（ひらがな・カタカナ推奨）
  - `rate`: 再生速度（1.0が通常速度）
  - `callback`: 再生完了時に呼ばれるコールバック関数

- `void random(int length, double rate, void (*callback)(const String&) = nullptr)`
  - ランダムなケロケロ声を再生します (ブロッキング)
  - `length`: 音声の長さ（文字数）
  - `rate`: 再生速度（1.0が通常速度）
  - `callback`: 再生完了時に呼ばれるコールバック関数

## 依存ライブラリ

- [M5Unified](https://github.com/m5stack/M5Unified) - M5Stackデバイス用の統一ライブラリ

## ライセンス

MITライセンスです。詳しくは[LICENSE](LICENSE)を参照してください。

## 音声について

このライブラリには、HTS Voice "Mei" を用いて合成した音声データが含まれています。  
音声の合成には OpenJTalk を使用しました。

HTS Voice "Mei"  
released by MMDAgent Project Team  
http://www.mmdagent.jp/  
Copyright (c) 2009–2018  Nagoya Institute of Technology Department of Computer Science  

Licensed under the Creative Commons Attribution 3.0 license.  
See: https://creativecommons.org/licenses/by/3.0/ for details
