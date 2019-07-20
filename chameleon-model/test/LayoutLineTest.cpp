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

#include "../src/natpad/textmodel/LayoutLine.h"
#include "LayoutLineTest.h"

#define L(x) L ## x

using Test = void (LayoutLineTest::*) (void);

static const Test tests[] = {
  &LayoutLineTest::testLength,
  &LayoutLineTest::testLength_emptyLine,
  &LayoutLineTest::testText_emptyLine,
  &LayoutLineTest::testText_shortLine,
  &LayoutLineTest::testText_longLine
};

LayoutLineTest::LayoutLineTest (void) : TestCase ("LayoutLineTest") {
}

unique_ptr<LayoutLineTest> LayoutLineTest::create (void) {
  return unique_ptr<LayoutLineTest> (new LayoutLineTest);
}

int LayoutLineTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* LayoutLineTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void LayoutLineTest::testLength (void) {
  setTestName (__func__);

  shared_ptr<const String> text (new String (L("regel")));
  LayoutLine line (text, 0, 0, 4);
  assertEquals (4, line.length ());
}

void LayoutLineTest::testLength_emptyLine (void) {
  setTestName (__func__);

  LayoutLine line;
  assertEquals (0, line.length ());
}

void LayoutLineTest::testText_emptyLine (void) {
  setTestName (__func__);

  LayoutLine line;
  assertTrue (!line.text ());
}

void LayoutLineTest::testText_shortLine (void) {
  setTestName (__func__);

  shared_ptr<const String> text (new String (L("korte regel")));
  LayoutLine line (text, 0, 0, 11);
  assertEquals ("korte regel", *line.text ());
}

void LayoutLineTest::testText_longLine (void) {
  setTestName (__func__);

  shared_ptr<const String> text (new String (L("Dit is een lange regel met tekst.")));
  LayoutLine line (text, 0, 20, 10);
  assertEquals ("el met tek", *line.text ());
}
