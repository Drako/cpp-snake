#pragma once

#ifndef SNAKE_UICOLOR_HXX
#define SNAKE_UICOLOR_HXX

#include <string>
#include <string_view>

enum class UiColor {
  Blue,
  Green,
  Grey,
  Red,
  Yellow,
};

std::string ui_image(std::string_view name, UiColor color = UiColor::Grey);

#endif // SNAKE_UICOLOR_HXX
