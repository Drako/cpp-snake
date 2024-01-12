#include "UiColor.hxx"

#include <sstream>

std::string ui_image(std::string_view name, UiColor const color)
{
  std::ostringstream strm;

  switch (color) {
  case UiColor::Blue:
    strm << "blue_";
    break;
  case UiColor::Green:
    strm << "green_";
    break;
  default:
    strm << "grey_";
    break;
  case UiColor::Red:
    strm << "red_";
    break;
  case UiColor::Yellow:
    strm << "yellow_";
    break;
  }

  strm << name << ".png";
  return strm.str();
}
