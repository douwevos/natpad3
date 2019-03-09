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

#include <sstream>
#include <natpad/util/StringUtils.h>
#include "StringUtilsTest.h"

using Test = void (StringUtilsTest::*) (void);

const Test tests[] = {
  &StringUtilsTest::testGetLines
};

StringUtilsTest::StringUtilsTest (void) : TestCase ("StringUtilsTest") {
}

unique_ptr<StringUtilsTest> StringUtilsTest::create (void) {
  return unique_ptr<StringUtilsTest> (new StringUtilsTest);
}

int StringUtilsTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* StringUtilsTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void StringUtilsTest::testGetLines (void) {
  setTestName (__func__);

  string data = "eerste regel\r\n\nderde regel \n  vierde regel";
  std::stringstream stream (data);

  vector<shared_ptr<string>> lines = StringUtils::getLines (stream);
  assertEquals (4, lines.size ());
  assertEquals ("eerste regel", *lines[0]);
  assertEquals ("", *lines[1]);
  assertEquals ("derde regel ", *lines[2]);
  assertEquals ("  vierde regel", *lines[3]);
}
