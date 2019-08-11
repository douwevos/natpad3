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
  &LayoutPageTest::testConstructor,
  &LayoutPageTest::testInsert_el_1,
  &LayoutPageTest::testInsert_el_2,
  &LayoutPageTest::testInsert_el_3,
  &LayoutPageTest::testInsert_el_4
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

/*
1. Adding to 'edit line'.
2. Number of lines page constant.
3. Number of lines managed page constant.
 */
void  LayoutPageTest::testInsert_el_1 (void) {
  setTestName (__func__);

  LayoutPage page (createBasicPage (), 15);
  page.m_editLineIndex = 9;
  page.m_editLine = page.m_lines.get ()[9];

  Cursor cursor (9, 3);
  shared_ptr<const Page> newPage = page.insert (cursor, L("-"));
  assertEquals (14, newPage->lineCount ());
  assertEquals (9, cursor.line);
  assertEquals (4, cursor.column);
  assertEquals (L("Zes-de regel"), *newPage->lineAt (9)->text ());
  assertEquals (page.m_lines.get (), newPage->m_lines.get ());
}

/*
1. Adding to 'edit line'.
2. Number of lines page increases.
3. Number of lines managed page constant.
4. New page also has m_editLine.
 */
void  LayoutPageTest::testInsert_el_2 (void) {
  setTestName (__func__);

  LayoutPage page (createBasicPage (), 15);
  page.m_editLineIndex = 0;
  page.m_editLine = page.m_lines.get ()[0];

  Cursor cursor (0, 9);
  shared_ptr<const Page> newPage = page.insert (cursor, L(" is langer gemaakt"));
  assertEquals (15, newPage->lineCount ());
  assertEquals (1, cursor.line);
  assertEquals (12, cursor.column);
  assertEquals (L("Regel één is la"), *newPage->lineAt (0)->text ());
  assertEquals (L("nger gemaakt."), *newPage->lineAt (1)->text ());
  assertEquals (L("Deze langste re"), *newPage->lineAt (2)->text ());
  assertNotNull (newPage->m_editLine.get ());
  assertEquals (1, newPage->m_editLineIndex);
}

/*
1. Adding to 'edit line'.
2. Number of lines page increases.
3. Number of lines managed page constant.
4. New page doesn't have m_editLine.
 */
void  LayoutPageTest::testInsert_el_3 (void) {
  setTestName (__func__);

  LayoutPage page (createBasicPage (), 15);
  page.m_editLineIndex = 4;
  page.m_editLine = page.m_lines.get ()[4];

  Cursor cursor (4, 1);
  shared_ptr<const Page> newPage = page.insert (cursor, L(" (opvulling voor deze regel)"));
  assertEquals (16, newPage->lineCount ());
  assertEquals (5, cursor.line);
  assertEquals (14, cursor.column);
  assertEquals (L("Deze langste re"), *newPage->lineAt (1)->text ());
  assertEquals (L("gel wordt gevol"), *newPage->lineAt (2)->text ());
  assertEquals (L("gd door een leg"), *newPage->lineAt (3)->text ());
  assertEquals (L("e (opvulling vo"), *newPage->lineAt (4)->text ());
  assertEquals (L("or deze regel) "), *newPage->lineAt (5)->text ());
  assertEquals (L("regel."), *newPage->lineAt (6)->text ());
  assertEquals (L(""), *newPage->lineAt (7)->text ());
  assertEquals (L("Vierde regeltje"), *newPage->lineAt (8)->text ());
  assertNull (newPage->m_editLine.get ());
}

/*
1. Adding to 'edit line'.
2. Number of lines page increases.
3. Number of lines managed page increases.
4. Number of parts old original edit line increases.
5. New page also has m_editLine.
 */
void  LayoutPageTest::testInsert_el_4 (void) {
  setTestName (__func__);

  LayoutPage page (createBasicPage (), 15);
  page.m_editLineIndex = 4;
  page.m_editLine = page.m_lines.get ()[4];

  Cursor cursor (4, 7);
  shared_ptr<const Page> newPage = page.insert (cursor, L(", maar niet onmiddelijk meer.\nExtra regel in de beheerde pagina.\n\nDerde extra"));
  assertEquals (21, newPage->lineCount ());
  assertEquals (11, cursor.line);
  assertEquals (11, cursor.column);
  assertEquals (L("Deze langste re"), *newPage->lineAt (1)->text ());
  assertEquals (L("gel wordt gevol"), *newPage->lineAt (2)->text ());
  assertEquals (L("gd door een leg"), *newPage->lineAt (3)->text ());
  assertEquals (L("e regel, maar n"), *newPage->lineAt (4)->text ());
  assertEquals (L("iet onmiddelijk"), *newPage->lineAt (5)->text ());
  assertEquals (L(" meer."), *newPage->lineAt (6)->text ());
  assertEquals (L("Extra regel in "), *newPage->lineAt (7)->text ());
  assertEquals (L("de beheerde pag"), *newPage->lineAt (8)->text ());
  assertEquals (L("ina."), *newPage->lineAt (9)->text ());
  assertEquals (L(""), *newPage->lineAt (10)->text ());
  assertEquals (L("Derde extra."), *newPage->lineAt (11)->text ());
  assertEquals (L(""), *newPage->lineAt (12)->text ());
  assertEquals (L("Vierde regeltje"), *newPage->lineAt (13)->text ());
  assertEquals (L("."), *newPage->lineAt (14)->text ());
  assertEquals (L("Vĳfde regeltje."), *newPage->lineAt (15)->text ());
  assertNotNull (newPage->m_editLine.get ());
  assertEquals (11, newPage->m_editLineIndex);
}
