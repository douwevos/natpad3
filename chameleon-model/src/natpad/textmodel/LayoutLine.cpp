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

#include "LayoutLine.h"

LayoutLine::LayoutLine (void) : m_lineIndex (-1), m_startIndex (-1), m_length (0) {
}

LayoutLine::LayoutLine (const shared_ptr<const String>& text, int lineIndex, int startIndex, int length) :
    Line (text),
    m_lineIndex (lineIndex),
    m_startIndex (startIndex),
    m_length (length) {
}

int LayoutLine::length (void) const {
  return m_length;
}

shared_ptr<const String> LayoutLine::text (void) const {
  if (!m_text || m_text->length () == m_length) {
    return m_text;
  }
  return shared_ptr<const String> (new String (*m_text, m_startIndex, m_length));
}
