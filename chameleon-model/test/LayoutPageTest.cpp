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

#include <natpad/textmodel/Line.h>
#include "LayoutPageTest.h"

#define protected public
#include "../src/natpad/textmodel/LayoutPage.h"
#undef protected

#include "../src/natpad/textmodel/BasicPage.h"

#define L(x) L ## x

using Test = void (LayoutPageTest::*) (void);

static const Test tests[] = {
  &LayoutPageTest::testConstructor
};

LayoutPageTest::LayoutPageTest (void) : TestCase ("LayoutPageTest") {
}

unique_ptr<LayoutPageTest> LayoutPageTest::create (void) {
  return unique_ptr<LayoutPageTest> (new LayoutPageTest);
}

int LayoutPageTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* LayoutPageTest::getTests (void) {
  return (TestCase::Test*) tests;
}

static shared_ptr<const Page> createBasicPage (void) {
  shared_ptr<Line>* lineArray = new shared_ptr<Line>[7];
  lineArray[0].reset (new Line (shared_ptr<String> (new String (L("Regel één.")))));
  lineArray[1].reset (new Line (shared_ptr<String> (new String (L("Deze langste regel wordt gevolgd door een lege regel.")))));
  lineArray[2].reset (new Line (shared_ptr<String> (new String (L("")))));
  lineArray[3].reset (new Line (shared_ptr<String> (new String (L("Vierde regeltje.")))));
  lineArray[4].reset (new Line (shared_ptr<String> (new String (L("Vĳfde regeltje.")))));
  lineArray[5].reset (new Line (shared_ptr<String> (new String (L("Zesde regel")))));
  lineArray[6].reset (new Line (shared_ptr<String> (new String (L("De laatste regel van een oninteressant stuk tekst.")))));

  BasicPage::Builder builder;
  return builder
      .lines (shared_ptr<shared_ptr<Line>> (lineArray, [] (shared_ptr<Line>* array) { delete[] array; }), 7)
      .build ();
}

void LayoutPageTest::testConstructor (void) {
  setTestName (__func__);

  const LayoutPage page (createBasicPage (), 15);
  assertEquals (14, page.lineCount ());
  assertEquals (L("Regel één."), *page.lineAt (0)->text ());
  assertEquals (L("Deze langste re"), *page.lineAt (1)->text ());
  assertEquals (L("gel wordt gevol"), *page.lineAt (2)->text ());
  assertEquals (L("gd door een leg"), *page.lineAt (3)->text ());
  assertEquals (L("e regel."), *page.lineAt (4)->text ());
  assertEquals (L(""), *page.lineAt (5)->text ());
  assertEquals (L("Vierde regeltje"), *page.lineAt (6)->text ());
  assertEquals (L("."), *page.lineAt (7)->text ());
  assertEquals (L("Vĳfde regeltje."), *page.lineAt (8)->text ());
  assertEquals (L("Zesde regel"), *page.lineAt (9)->text ());
  assertEquals (L("De laatste rege"), *page.lineAt (10)->text ());
  assertEquals (L("l van een onint"), *page.lineAt (11)->text ());
  assertEquals (L("eressant stuk t"), *page.lineAt (12)->text ());
  assertEquals (L("ekst."), *page.lineAt (13)->text ());
}
