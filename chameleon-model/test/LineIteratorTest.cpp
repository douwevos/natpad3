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
#include "LineIteratorTest.h"
#include <natpad/textmodel/TextModel.h>

using Test = void (LineIteratorTest::*) (void);

static const Test tests[] = {
  &LineIteratorTest::testIterateOverAllLines,
  &LineIteratorTest::testDifferentStartingPoint
};

static std::string createLine (int number) {
  char buf[8];
  sprintf (buf, "%05d", number + 1);
  return buf;
}

static std::string createLines (int count) {
  std::string result;
  result.reserve (6 * count);
  for (int i = 0; i < count; ++i) {
    result += createLine (i);
    result += '\n';
  }
  if (count > 0)
    result.pop_back ();
  return result;
}

LineIteratorTest::LineIteratorTest (void) : TestCase ("LineIteratorTest") {
}

unique_ptr<LineIteratorTest> LineIteratorTest::create (void) {
  return unique_ptr<LineIteratorTest> (new LineIteratorTest);
}

int LineIteratorTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* LineIteratorTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void LineIteratorTest::testDifferentStartingPoint (void) {
  setTestName (__func__);

  StringConvert convert;
  std::string lines = createLines (4073);
  TextModel emptyModel;
  shared_ptr<const TextModel> model = emptyModel.insert (Cursor (), lines);

  TextModel::LineIterator iter = model->lineIterator (3000);
  for (int i = 0; i < 1073; ++i) {
    shared_ptr<Line> line = *iter;
    int n = std::stoi (convert.to_bytes (*line->text ()));
    assertEquals (i + 3001, n);
    ++iter;
  }

  try {
    *iter;
    FAIL ("out_of_range exception expected.");
  } catch (std::out_of_range& x) {
  }
}

void LineIteratorTest::testIterateOverAllLines (void) {
  setTestName (__func__);

  StringConvert convert;
  std::string lines = createLines (4073);
  TextModel emptyModel;
  shared_ptr<const TextModel> model = emptyModel.insert (Cursor (), lines);

  TextModel::LineIterator iter = model->lineIterator ();
  for (int i = 0; i < 4073; ++i) {
    shared_ptr<Line> line = *iter;
    int n = std::stoi (convert.to_bytes (*line->text ()));
    assertEquals (i + 1, n);
    iter++;
  }

  try {
    *iter;
    FAIL ("out_of_range exception expected.");
  } catch (std::out_of_range& x) {
  }
}
