/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2019 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef INCLUDE_NATPAD_VIEW_LINEIMAGE_H_
#define INCLUDE_NATPAD_VIEW_LINEIMAGE_H_

#include <string>
#include <cairomm/context.h>

class LineImage {
private:
  Cairo::RefPtr<Cairo::Surface> m_surface;
  std::string m_text;
  double m_width;
  double m_height;
  int m_lineIndex;

public:
  double height (void) const;
  int lineIndex (void) const;
  void set (Cairo::RefPtr<Cairo::Surface> surface, double width, double height, const std::string& text, int lineIndex);
  void setLineIndex (int lineIndex);
  Cairo::RefPtr<Cairo::Surface> surface (void) const;
  const std::string& text (void) const;
  double width (void) const;
};

#endif
