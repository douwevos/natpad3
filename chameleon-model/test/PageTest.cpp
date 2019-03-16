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
#include "PageTest.h"

#define private public
#include <natpad/textmodel/Page.h>
#undef private

#define FIRST_LINE  "eerste regel"
#define SECOND_LINE "tweede regel"
#define THIRD_LINE  "derde regel"
#define FOURTH_LINE "vierde regel"

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
  &PageTest::testInsert_beforeAddLineNowDifferentExistingLine
};

static unique_ptr<const Page> createEmptyPage (void) {
  return unique_ptr<const Page> (new Page);
}

static unique_ptr<const Page> createNonemptyPage (void) {
  shared_ptr<const string>* lines = new shared_ptr<const string>[4];
  lines[0] = shared_ptr<const string> (new string (FIRST_LINE));
  lines[1] = shared_ptr<const string> (new string (SECOND_LINE));
  lines[2] = shared_ptr<const string> (new string (THIRD_LINE));
  lines[3] = shared_ptr<const string> (new string (FOURTH_LINE));
  Page* page = new Page;
  page->m_lines.reset (lines, [] (shared_ptr<const string>* array) { delete[] array; });
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
  assertEquals (FIRST_LINE, *page->lineAt (0));
  assertEquals (SECOND_LINE, *page->lineAt (1));
  assertEquals (THIRD_LINE, *page->lineAt (2));
  assertEquals (FOURTH_LINE, *page->lineAt (3));
}

void PageTest::testInsert_emptyPage (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createEmptyPage ();
  assertEquals (0, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, "Hallo wereld");
  assertEquals (1, newPage->lineCount ());
  assertEquals ("Hallo wereld", *newPage->lineAt (0));
}

void PageTest::testInsert_newNonemptyPage (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, "vijfde regel");
  assertEquals (5, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0));
  assertEquals (SECOND_LINE, *newPage->lineAt (1));
  assertEquals (THIRD_LINE, *newPage->lineAt (2));
  assertEquals (FOURTH_LINE, *newPage->lineAt (3));
  assertEquals ("vijfde regel", *newPage->lineAt (4));
}

void PageTest::testInsert_beginOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 1;
  shared_ptr<const Page> newPage = page->insert (cursor, "De ");
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0));
  assertEquals ("De tweede regel", *newPage->lineAt (1));
  assertEquals (THIRD_LINE, *newPage->lineAt (2));
  assertEquals (FOURTH_LINE, *newPage->lineAt (3));
}

void PageTest::testInsert_middleOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 2;
  cursor.column = 6;
  shared_ptr<const Page> newPage = page->insert (cursor, "en langste ");
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0));
  assertEquals (SECOND_LINE, *newPage->lineAt (1));
  assertEquals ("derde en langste regel", *newPage->lineAt (2));
  assertEquals (FOURTH_LINE, *newPage->lineAt (3));
}

void PageTest::testInsert_endOfLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 3;
  cursor.column = 12;
  shared_ptr<const Page> newPage = page->insert (cursor, " met toevoeging op het eind");
  assertEquals (4, newPage->lineCount ());
  assertEquals (FIRST_LINE, *newPage->lineAt (0));
  assertEquals (SECOND_LINE, *newPage->lineAt (1));
  assertEquals (THIRD_LINE, *newPage->lineAt (2));
  assertEquals ("vierde regel met toevoeging op het eind", *newPage->lineAt (3));
}

void PageTest::testInsert_sameLineAsBefore (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, "De ");
  assertEquals ("De eerste regel", *newPage->lineAt (0));
  cursor.column = 15;
  newPage = newPage->insert (cursor, " Volgende zin.");
  assertEquals ("De eerste regel Volgende zin.", *newPage->lineAt (0));
  newPage = newPage->insert (cursor, ".");
  assertEquals ("De eerste regel. Volgende zin.", *newPage->lineAt (0));
}

void PageTest::testInsert_differentLineAsBefore (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  shared_ptr<const Page> newPage = page->insert (cursor, "De ");
  cursor.line = 3;
  cursor.column = 7;
  newPage = newPage->insert (cursor, "en laatste ");
  assertEquals ("De eerste regel", *newPage->lineAt (0));
  assertEquals ("vierde en laatste regel", *newPage->lineAt (3));
}

void PageTest::testInsert_beforeAddLineNowAddLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, "vijfde regel");
  ++cursor.line;
  newPage = newPage->insert (cursor, "zesde regel");
  assertEquals (6, newPage->lineCount ());
  assertEquals ("vijfde regel", *newPage->lineAt (4));
  assertEquals ("zesde regel", *newPage->lineAt (5));
}

void PageTest::testInsert_beforeAddLineNowDifferentExistingLine (void) {
  setTestName (__func__);

  unique_ptr<const Page> page = createNonemptyPage ();
  assertEquals (4, page->lineCount ());

  Cursor cursor;
  cursor.line = 4;
  shared_ptr<const Page> newPage = page->insert (cursor, "vijfde regel");
  --cursor.line;
  cursor.column = 12;
  newPage = newPage->insert (cursor, " is niet meer de laatste.");
  assertEquals (5, newPage->lineCount ());
  assertEquals ("vierde regel is niet meer de laatste.", *newPage->lineAt (3));
  assertEquals ("vijfde regel", *newPage->lineAt (4));
}
