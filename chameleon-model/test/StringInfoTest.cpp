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

#include <string>
#include "StringInfoTest.h"
#include <natpad/util/StringInfo.h>

using Test = void (StringInfoTest::*) (void);

static const Test tests[] = {
  &StringInfoTest::testAll_emptyString,
  &StringInfoTest::testAll_simpleString,
  &StringInfoTest::testAll_complexString
};

StringInfoTest::StringInfoTest (void) : TestCase ("StringInfoTest") {
}

unique_ptr<StringInfoTest> StringInfoTest::create (void) {
  return unique_ptr<StringInfoTest> (new StringInfoTest);
}

int StringInfoTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* StringInfoTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void StringInfoTest::testAll_emptyString (void) {
  setTestName (__func__);

  StringInfo info ("");
  assertEquals (0, info.length ());
  assertEquals (0, info.charCount ());
  assertEquals (0, info.byteIndex (0));
}

void StringInfoTest::testAll_simpleString (void) {
  setTestName (__func__);

  string s = "0123456789";
  StringInfo info (s);
  assertEquals (s.length (), info.length ());
  assertEquals (10, info.charCount ());
  for (int i = 0; i <= info.charCount (); ++i) {
    assertEquals (i, info.byteIndex (i));
  }
}

void StringInfoTest::testAll_complexString (void) {
  setTestName (__func__);

  string s = u8"abßΔД♑";
  StringInfo info (s);
  assertEquals (11, s.length ());
  assertEquals (s.length (), info.length ());
  assertEquals (6, info.charCount ());
  assertEquals (0, info.byteIndex (0));
  assertEquals (1, info.byteIndex (1));
  assertEquals (2, info.byteIndex (2));
  assertEquals (4, info.byteIndex (3));
  assertEquals (6, info.byteIndex (4));
  assertEquals (8, info.byteIndex (5));
  assertEquals (11, info.byteIndex (6));
}
