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

#include <codecvt>
#include <locale>
#include <natpad/io/IOException.h>
#include <natpad/io/Utf8FileReader.h>

Utf8FileReader::Utf8FileReader (const std::string& filename) : m_stream (filename) {
  if (m_stream.fail ()) {
    std::string error = "Could not open file '";
    error += filename;
    error += '\'';
    throw IOException (error);
  }
  m_stream.imbue (std::locale (std::locale (), new std::codecvt_utf8<wchar_t>));
}

int Utf8FileReader::get (void) {
  if (m_reread) {
    m_reread = false;
    return m_read;
  }
  m_read = m_stream.get ();
  if (m_read == WEOF) {
    m_read = Reader::eof;
  }
  return m_read;
}
