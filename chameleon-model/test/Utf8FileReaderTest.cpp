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

#include <natpad/io/IOException.h>
#include <natpad/io/Utf8FileReader.h>
#include "Utf8FileReaderTest.h"

using Test = void (Utf8FileReaderTest::*) (void);

static const Test tests[] = {
  &Utf8FileReaderTest::testConstructor,
  &Utf8FileReaderTest::testGet,
  &Utf8FileReaderTest::testUnget
};

Utf8FileReaderTest::Utf8FileReaderTest (void) : TestCase ("Utf8FileReaderTest") {
}

unique_ptr<Utf8FileReaderTest> Utf8FileReaderTest::create (void) {
  return unique_ptr<Utf8FileReaderTest> (new Utf8FileReaderTest);
}

int Utf8FileReaderTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* Utf8FileReaderTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void Utf8FileReaderTest::testConstructor (void) {
  setTestName (__func__);

  try {
    Utf8FileReader reader ("nonexistent.txt");
    FAIL ("IOException expected");
  } catch (IOException& x) {
  }
}

void Utf8FileReaderTest::testGet (void) {
  setTestName (__func__);

  Utf8FileReader reader ("tekens.txt");
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

void Utf8FileReaderTest::testUnget (void) {
  setTestName (__func__);


  Utf8FileReader reader ("tekens.txt");
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
