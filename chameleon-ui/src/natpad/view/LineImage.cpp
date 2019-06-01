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

#include <natpad/view/LineImage.h>

double LineImage::height (void) const {
  return m_height;
}

void LineImage::set (Cairo::RefPtr<Cairo::Surface> surface, double width, double height) {
  m_surface = surface;
  m_height = height;
  m_width = width;
}

Cairo::RefPtr<Cairo::Surface> LineImage::surface (void) const {
  return m_surface;
}

double LineImage::width (void) const {
  return m_width;
}
