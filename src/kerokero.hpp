// Copyright (c) 2025 misetteichan
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <WString.h>
#include <utility/Speaker_Class.hpp>

class KeroKero {
 public:
  bool init();
  void play(const String& text, m5::Speaker_Class& speaker, void (*func)(String&) = nullptr);
};
