#include "UiColor.hxx"

#include <format>

std::string ui_image(std::string_view name, UiColor const color)
{
  switch (color) {
  case UiColor::Blue:
    return std::format("blue_{}.png", name);
  case UiColor::Green:
    return std::format("green_{}.png", name);
  case UiColor::Grey:
    return std::format("grey_{}.png", name);
  case UiColor::Red:
    return std::format("red_{}.png", name);
  case UiColor::Yellow:
    return std::format("yellow_{}.png", name);
  }
}
