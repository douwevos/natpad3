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

#ifndef TEXT_MODEL_TEST_INCLUDED
#define TEXT_MODEL_TEST_INCLUDED

#include <memory>
#include <testutils/TestCase.h>

using std::unique_ptr;

class TextModelTest : public TestCase {
public:
  TextModelTest (void);

  static unique_ptr<TextModelTest> create (void);

  int getTestCount (void) override;
  const Test* getTests (void) override;

  void testConstructor_void (void);
  void testConstructor_istream_noLines (void);
  void testConstructor_istream_lessThanPreferredPageSizeLines (void);
  void testConstructor_istream_exactlyPreferredPageSizeLines (void);
  void testConstructor_istream_moreThanPreferredPageSizeLines (void);
  void testPageAt (void);
  void testPageInfoForLine (void);
  void testLineAt (void);
  void testInsert_emptyModel (void);
  void testInsert_modifyLineOrAddAtEnd (void);
};

#endif
