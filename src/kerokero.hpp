// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <WString.h>
#include <utility/Speaker_Class.hpp>

class KeroKero {
  int _level = 0;
 public:
  bool init();
  void play(const String& text, m5::Speaker_Class& speaker, void (*func)(String&) = nullptr);
  int level() const { return _level; }
 private:
  bool play(const unsigned char* src, m5::Speaker_Class& speaker);
};
