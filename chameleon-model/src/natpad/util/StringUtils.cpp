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
#include <natpad/util/StringUtils.h>

static String* getNextLine (Reader& stream, int c) {
  String* result = new String ();
  while (!(c == Reader::eof || c == '\n')) {
    if (c != '\r')
      *result += (Wchar) c;
    c = stream.get ();
  }
  stream.unget ();
  return result;
}

vector<shared_ptr<String>> StringUtils::getLines (Reader& stream) {
  vector<shared_ptr<String>> list;
  int c;
  do {
    c = stream.get ();
    shared_ptr<String> line (getNextLine (stream, c));
    list.push_back (line);
    c = stream.get ();
  } while (c != Reader::eof);
  return list;
}

vector<shared_ptr<String>> StringUtils::getLines (const String& text) {
  StringReader stream (text);
  return getLines (stream);
}
