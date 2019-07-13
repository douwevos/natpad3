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

#include <natpad/util/string.h>
#include "PageTest.h"

#define private public
#include <natpad/textmodel/Page.h>
#undef private

#define FIRST_LINE  "eerste regel"
#define SECOND_LINE "tweede regel"
#define THIRD_LINE  "derde regel"
#define FOURTH_LINE "vierde regel"

#define L(x) L ## x

using Test = void (PageTest::*) (void);

static const Test tests[] = {
  &PageTest::testConstructor,
  &PageTest::testLineAt,
  &PageTest::testInsert_emptyPage,
  &PageTest::testInsert_newNonemptyPage,
  &PageTest::testInsert_beginOfLine,
  &PageTest::testInsert_middleOfLine,
  &PageTest::testInsert_endOfLine,
  &PageTest::testInsert_sameLineAsBefore,
  &PageTest::testInsert_differentLineAsBefore,
  &PageTest::testInsert_beforeAddLineNowAddLine,
  &PageTest::testInsert_beforeAddLineNowDifferentExistingLine,
  &PageTest::testInsert_multipleLines
};

static unique_ptr<const Page> createEmptyPage (void) {
  return unique_ptr<const Page> (new Page);
}

static unique_ptr<const Page> createNonemptyPage (void) {
  StringConvert convert;
  shared_ptr<Line>* lines = new shared_ptr<Line>[4];
  lines[0].reset (new Line (shared_ptr<const String> (new String (convert.from_bytes (FIRST_LINE)))));
  lines[1].reset (new Line (shared_ptr<const String> (new String (convert.from_bytes (SECOND_LINE)))));
  lines[2].reset (new Line (shared_ptr<const String> (new String (convert.from_bytes (THIRD_LINE)))));
  lines[3].reset (new Line (shared_ptr<const String> (new String (convert.from_bytes (FOURTH_LINE)))));
  Page* page = new Page;
  page->m_lines.reset (lines, [] (shared_ptr<Line>* array) { delete[] array; });
  page->m_lineCount = 4;
  return unique_ptr<const Page> (page);
}

PageTest::PageTest (void) : TestCase ("PageTest") {
}

unique_ptr<PageTest> PageTest::create (void) {
  return unique_ptr<PageTest> (new PageTest);
}

int PageTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* PageTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void PageTest::testConstructor (void) {
  setTestName (__func__);

  const Page page;
  assertEquals (0, page.lineCount ());
}

void PageTest::testLineAt (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());
  assertEquals (FIRST_LINE, *page->lineAt (0)->text ());
  assertEquals (SECOND_LINE, *page->lineAt (1)->text ());
  assertEquals (THIRD_LINE, *page->lineAt (2)->text ());
  assertEquals (FOURTH_LINE, *page->lineAt (3)->text ());
}

void PageTest::testInsert_emptyPage (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createEmptyPage ();
  assertEquals (0, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, L("Hallo wereld"));
  assertEquals (1, newPage->lineCount ());
  assertEquals ("Hallo wereld", *newPage->lineAt (0)->text ());
}

void PageTest::testInsert_newNonemptyPage (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, L("vijfde regel"));
  assertEquals (5, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (1)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (2)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (3)->text ());
  assertEquals ("vijfde regel", *newPage->lineAt (4)->text ());
}

void PageTest::testInsert_beginOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 1;
  shared_ptr<const Page> newPage = page->insert (cursor, L("De "));
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0)->text ());
  assertEquals ("De tweede regel", *newPage->lineAt (1)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (2)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (3)->text ());
}

void PageTest::testInsert_middleOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 2;
  cursor.column = 6;
  shared_ptr<const Page> newPage = page->insert (cursor, L("en langste "));
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (1)->text ());
  assertEquals ("derde en langste regel", *newPage->lineAt (2)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (3)->text ());
}

void PageTest::testInsert_endOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 3;
  cursor.column = 12;
  shared_ptr<const Page> newPage = page->insert (cursor, L(" met toevoeging op het eind"));
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (1)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (2)->text ());
  assertEquals ("vierde regel met toevoeging op het eind", *newPage->lineAt (3)->text ());
}

void PageTest::testInsert_sameLineAsBefore (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, L("De "));
  assertEquals ("De eerste regel", *newPage->lineAt (0)->text ());
  assertEquals (3, cursor.column);
  assertEquals (0, cursor.line);

  cursor.column = 15;
  newPage = newPage->insert (cursor, L(" Volgende zin."));
  assertEquals ("De eerste regel Volgende zin.", *newPage->lineAt (0)->text ());
  assertEquals (29, cursor.column);
  assertEquals (0, cursor.line);

  cursor.column = 15;
  newPage = newPage->insert (cursor, L("."));
  assertEquals ("De eerste regel. Volgende zin.", *newPage->lineAt (0)->text ());
  assertEquals (16, cursor.column);
  assertEquals (0, cursor.line);
}

void PageTest::testInsert_differentLineAsBefore (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, L("De "));
  cursor.line = 3;
  cursor.column = 7;
  newPage = newPage->insert (cursor, L("en laatste "));
  assertEquals ("De eerste regel", *newPage->lineAt (0)->text ());
  assertEquals ("vierde en laatste regel", *newPage->lineAt (3)->text ());
}

void PageTest::testInsert_beforeAddLineNowAddLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, L("vijfde regel"));
  assertEquals (12, cursor.column);
  assertEquals (4, cursor.line);

  ++cursor.line;
  cursor.column = 0;
  newPage = newPage->insert (cursor, L("zesde regel"));
  assertEquals (6, newPage->lineCount ());
  assertEquals ("vijfde regel", *newPage->lineAt (4)->text ());
  assertEquals ("zesde regel", *newPage->lineAt (5)->text ());
  assertEquals (11, cursor.column);
  assertEquals (5, cursor.line);
}

void PageTest::testInsert_beforeAddLineNowDifferentExistingLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, L("vijfde regel"));
  --cursor.line;
  cursor.column = 12;
  newPage = newPage->insert (cursor, L(" is niet meer de laatste."));
  assertEquals (5, newPage->lineCount ());
  assertEquals ("vierde regel is niet meer de laatste.", *newPage->lineAt (3)->text ());
  assertEquals ("vijfde regel", *newPage->lineAt (4)->text ());
}

void PageTest::testInsert_multipleLines (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor (0, 6);
  shared_ptr<const Page> newPage = page->insert (cursor, L(" test\r\nmet een nieuwe"));
  assertEquals (5, newPage->lineCount ());
  assertEquals ("eerste test", *newPage->lineAt (0)->text ());
  assertEquals ("met een nieuwe regel", *newPage->lineAt (1)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (2)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (3)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (4)->text ());
  assertEquals (1, newPage->m_editLineIndex);
  assertEquals (14, cursor.column);
  assertEquals (1, cursor.line);

  cursor.line = 5;
  cursor.column = 0;
  newPage = newPage->insert (cursor, L("\nUit 'de vrolijke wetenschap'\nvan Friedrich Nietzsche:\n---\n"));
  assertEquals (10, newPage->lineCount ());
  assertEquals ("eerste test", *newPage->lineAt (0)->text ());
  assertEquals ("met een nieuwe regel", *newPage->lineAt (1)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (2)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (3)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (4)->text ());
  assertEquals ("", *newPage->lineAt (5)->text ());
  assertEquals ("Uit 'de vrolijke wetenschap'", *newPage->lineAt (6)->text ());
  assertEquals ("van Friedrich Nietzsche:", *newPage->lineAt (7)->text ());
  assertEquals ("---", *newPage->lineAt (8)->text ());
  assertEquals ("", *newPage->lineAt (9)->text ());
  assertEquals (9, newPage->m_editLineIndex);
  assertEquals (0, cursor.column);
  assertEquals (9, cursor.line);

  cursor.line = 7;
  cursor.column = 24;
  newPage = newPage->insert (cursor, L("\n\nVademecum - Vadetecum\n\nEs lockt dich meine Art und Sprach',\nDu folgest mir, du gehst mir nach?\nGeh nur dir selber treulich nach:-\nSo folgst du mir - gemach! gemach!"));
  assertEquals (17, newPage->lineCount ());
  assertEquals ("eerste test", *newPage->lineAt (0)->text ());
  assertEquals ("met een nieuwe regel", *newPage->lineAt (1)->text ());
  assertEquals (SECOND_LINE, *newPage->lineAt (2)->text ());
  assertEquals (THIRD_LINE, *newPage->lineAt (3)->text ());
  assertEquals (FOURTH_LINE, *newPage->lineAt (4)->text ());
  assertEquals ("", *newPage->lineAt (5)->text ());
  assertEquals ("Uit 'de vrolijke wetenschap'", *newPage->lineAt (6)->text ());
  assertEquals ("van Friedrich Nietzsche:", *newPage->lineAt (7)->text ());
  assertEquals ("", *newPage->lineAt (8)->text ());
  assertEquals ("Vademecum - Vadetecum", *newPage->lineAt (9)->text ());
  assertEquals ("", *newPage->lineAt (10)->text ());
  assertEquals ("Es lockt dich meine Art und Sprach',", *newPage->lineAt (11)->text ());
  assertEquals ("Du folgest mir, du gehst mir nach?", *newPage->lineAt (12)->text ());
  assertEquals ("Geh nur dir selber treulich nach:-", *newPage->lineAt (13)->text ());
  assertEquals ("So folgst du mir - gemach! gemach!", *newPage->lineAt (14)->text ());
  assertEquals ("---", *newPage->lineAt (15)->text ());
  assertEquals ("", *newPage->lineAt (16)->text ());
  assertEquals (14, newPage->m_editLineIndex);
  assertEquals (34, cursor.column);
  assertEquals (14, cursor.line);
}
