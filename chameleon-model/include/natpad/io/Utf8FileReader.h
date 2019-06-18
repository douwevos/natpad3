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

#ifndef __NATPAD_IO_UTF8_FILE_READER_INCLUDED
#define __NATPAD_IO_UTF8_FILE_READER_INCLUDED

#include <fstream>
#include <string>
#include <natpad/io/Reader.h>

class Utf8FileReader : public Reader {
private:
  std::wifstream m_stream;

public:
  explicit Utf8FileReader (const std::string& filename);

  int get (void) override;
};

#endif
