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

#ifndef __NATPAD_UTIL_STRING_UTILS_INCLUDED
#define __NATPAD_UTIL_STRING_UTILS_INCLUDED

#include <memory>
#include <vector>
#include <natpad/io/Reader.h>
#include <natpad/util/string.h>

using std::shared_ptr;
using std::vector;

namespace StringUtils {
  vector<shared_ptr<String>> getLines (Reader& stream);
  vector<shared_ptr<String>> getLines (const String& text);
}

#endif
