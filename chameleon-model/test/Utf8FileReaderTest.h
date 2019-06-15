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

#ifndef UTF8_FILE_READER_TEST_INCLUDED
#define UTF8_FILE_READER_TEST_INCLUDED

#include <memory>
#include <testutils/TestCase.h>

using std::unique_ptr;

class Utf8FileReaderTest : public TestCase {
public:
  Utf8FileReaderTest (void);

  static unique_ptr<Utf8FileReaderTest> create (void);

  int getTestCount (void) override;
  const Test* getTests (void) override;

  void testConstructor (void);
  void testGet (void);
  void testUnget (void);
};

#endif
