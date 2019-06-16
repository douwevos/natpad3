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

#include <natpad/io/StringReader.h>

StringReader::StringReader (const String& str) : m_buffer (str), m_length (str.length ()), m_index (0) {
}

int StringReader::get (void) {
  if (m_reread) {
    m_reread = false;
  } else if (m_index < m_length) {
    m_read = m_buffer[m_index];
    ++m_index;
  } else {
    m_read = Reader::eof;
  }
  return m_read;
}
