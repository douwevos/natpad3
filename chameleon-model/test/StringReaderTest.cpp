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
#include "StringReaderTest.h"

using Test = void (StringReaderTest::*) (void);

static const Test tests[] = {
  &StringReaderTest::testGet,
  &StringReaderTest::testUnget
};

StringReaderTest::StringReaderTest (void) : TestCase ("StringReaderTest") {
}

unique_ptr<StringReaderTest> StringReaderTest::create (void) {
  return unique_ptr<StringReaderTest> (new StringReaderTest);
}

int StringReaderTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* StringReaderTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void StringReaderTest::testGet (void) {
  setTestName (__func__);

  StringConvert convert;
  StringReader reader (convert.from_bytes (u8"ABCß\u20AC\u21B2\u21B3\u2938\u2939\U0001F60E"));
  assertEquals (0x41, reader.get ());
  assertEquals (0x42, reader.get ());
  assertEquals (0x43, reader.get ());
  assertEquals (0xDF, reader.get ());
  assertEquals (0x20AC, reader.get ());
  assertEquals (0x21B2, reader.get ());
  assertEquals (0x21B3, reader.get ());
  assertEquals (0x2938, reader.get ());
  assertEquals (0x2939, reader.get ());
  assertEquals (0x1F60E, reader.get ());
  assertEquals (Reader::eof, reader.get ());
}

void StringReaderTest::testUnget (void) {
  setTestName (__func__);

  StringConvert convert;
  StringReader reader (convert.from_bytes (u8"ABCß\u20AC\u21B2\u21B3\u2938\u2939\U0001F60E"));
  assertEquals (0x41, reader.get ());
  assertEquals (0x42, reader.get ());
  reader.unget ();
  assertEquals (0x42, reader.get ());
  assertEquals (0x43, reader.get ());
  assertEquals (0xDF, reader.get ());
  assertEquals (0x20AC, reader.get ());
  assertEquals (0x21B2, reader.get ());
  reader.unget ();
  assertEquals (0x21B2, reader.get ());
  reader.unget ();
  assertEquals (0x21B2, reader.get ());
  assertEquals (0x21B3, reader.get ());
  assertEquals (0x2938, reader.get ());
  assertEquals (0x2939, reader.get ());
  assertEquals (0x1F60E, reader.get ());
  reader.unget ();
  assertEquals (0x1F60E, reader.get ());
  assertEquals (Reader::eof, reader.get ());
  reader.unget ();
  assertEquals (Reader::eof, reader.get ());
  assertEquals (Reader::eof, reader.get ());
}
