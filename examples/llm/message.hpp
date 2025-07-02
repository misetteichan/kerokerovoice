//  Copyright (c) 2025 misetteichan
//  Licensed under the MIT License. See LICENSE file for details.

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <Avatar.h>

// Control the speechText of the avatar.
class Message {
  m5avatar::Avatar& avatar;
  uint32_t _last;
  bool _enable = false;
 public:
  explicit Message(m5avatar::Avatar& o) : avatar(o) {
  }

  void set(const char* text) {
    if (_enable) {
      avatar.setSpeechText(text);
      _last = M5.millis();
    }
  }
  void update() {
    if (_enable && _last > 0 && (M5.millis() - _last) > 2000) {
      avatar.setSpeechText("");
    }
  }

  bool toggle() {
    _enable = !_enable;
    _last = 0;
    if (!_enable) {
      avatar.setSpeechText("");
    }
    return _enable;
  }
};

#endif // MESSAGE_HPP
