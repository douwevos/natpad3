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

#include <stdexcept>
#include <natpad/util/StringInfo.h>

static int determineCharLength (unsigned char c) {
  int mask = 0x80;
  int result = 0;
  while ((c & mask) != 0) {
    mask >>= 1;
    ++result;
  }
  if (result == 1 || result > 4) {
    throw std::runtime_error ("utf8 encoding error.");
  }
  if (result == 0) {
    ++result;
  }
  return result;
}

StringInfo::StringInfo (const string& utf8) {
  m_indices.push_back (0);
  const int len = utf8.length ();
  if (len > 0) {
    m_indices.reserve (len + 1);
    int i = 0;
    while (i < len) {
      int charLen = determineCharLength (utf8[i]);
      i += charLen;
      m_indices.push_back (i);
    }
    //m_indices.shrink_to_fit ();
  }
}

int StringInfo::byteIndex (int charIndex) const {
  return m_indices.at (charIndex);
}

int StringInfo::charCount (void) const {
  return m_indices.size () - 1;
}

int StringInfo::length (void) const {
  return m_indices.back ();
}
