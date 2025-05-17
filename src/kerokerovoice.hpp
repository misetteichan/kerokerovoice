// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#ifndef KEROKEROVOICE_HPP
#define KEROKEROVOICE_HPP

#include <WString.h>
#include <utility/Speaker_Class.hpp>

class KeroKeroVoice {
  int _level = 0;
  m5::Speaker_Class* _speaker = nullptr;
 public:
  void init(m5::Speaker_Class& speaker);
  int level() const { return _level; }
  void play(const String& text, double rate, void (*callback)(const String&) = nullptr);
  void random(int length, double rate, void (*callback)(const String&) = nullptr);
 private:
  bool play(const unsigned char* src, double rate, m5::Speaker_Class& speaker);
};

#endif // KEROKEROVOICE_HPP