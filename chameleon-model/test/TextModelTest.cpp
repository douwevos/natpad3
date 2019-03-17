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
#include "TextModelTest.h"

#include <string>
#include <istream>
#include <natpad/textmodel/Page.h>
#define private public
#include <natpad/textmodel/TextModel.h>
#undef private

using Test = void (TextModelTest::*) (void);

static const Test tests[] = {
  &TextModelTest::testConstructor_void,
  &TextModelTest::testConstructor_istream_noLines,
  &TextModelTest::testConstructor_istream_lessThanPreferredPageSizeLines,
  &TextModelTest::testConstructor_istream_exactlyPreferredPageSizeLines,
  &TextModelTest::testConstructor_istream_moreThanPreferredPageSizeLines,
  &TextModelTest::testPageAt,
  &TextModelTest::testPageInfoForLine,
  &TextModelTest::testLineAt,
  &TextModelTest::testInsert_emptyModel,
  &TextModelTest::testInsert_modifyLineOrAddAtEnd
};

static string createLine (int number) {
  char buf[8];
  sprintf (buf, "%05d", number + 1);
  string result = buf;
  return result;
}

static string createLines (int count) {
  string result;
  result.reserve (6 * count);
  for (int i = 0; i < count; ++i) {
    result += createLine (i);
    result += '\n';
  }
  return result;
}

static shared_ptr<const Page> createPage (int lineCount, int index = 0) {
  Page::Builder builder;
  builder.prepareBuildingNewPage (lineCount);
  for (int i = 0; i < lineCount; ++i) {
    builder.addLine (shared_ptr<const string> (new string (createLine (index++))));
  }
  return builder.build ();
}

TextModelTest::TextModelTest (void) : TestCase ("TextModelTest") {
}

unique_ptr<TextModelTest> TextModelTest::create (void) {
  return unique_ptr<TextModelTest> (new TextModelTest);
}

int TextModelTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* TextModelTest::getTests (void) {
  return (TestCase::Test*) tests;
}

void TextModelTest::testConstructor_void (void) {
  setTestName (__func__);

  const TextModel model;
  assertEquals (0, model.lineCount ());
  assertEquals (0, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);
}

void TextModelTest::testConstructor_istream_noLines (void) {
  setTestName (__func__);

  string s = "";
  std::stringstream stream (s);

  const TextModel model (stream);
  assertEquals (0, model.lineCount ());
  assertEquals (0, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);
}

void TextModelTest::testConstructor_istream_lessThanPreferredPageSizeLines (void) {
  setTestName (__func__);

  assertTrue (Page::preferredSize > 10);
  const int expectedLineCount = Page::preferredSize - 1;

  string s = createLines (expectedLineCount);
  std::stringstream stream (s);

  const TextModel model (stream);
  const int lineCount = model.lineCount ();
  assertEquals (expectedLineCount, lineCount);
  assertEquals (1, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);

  shared_ptr<const Page> page = model.m_pages.get ()[0];
  for (int i = 0; i < lineCount; ++i) {
    assertEquals (createLine (i), *page->lineAt (i));
  }
}

void TextModelTest::testConstructor_istream_exactlyPreferredPageSizeLines (void) {
  setTestName (__func__);

  const int expectedLineCount = Page::preferredSize;

  string s = createLines (expectedLineCount);
  std::stringstream stream (s);

  const TextModel model (stream);
  const int lineCount = model.lineCount ();
  assertEquals (expectedLineCount, lineCount);
  assertEquals (1, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);

  shared_ptr<const Page> page = model.m_pages.get ()[0];
  for (int i = 0; i < lineCount; ++i) {
    assertEquals (createLine (i), *page->lineAt (i));
  }
}

void TextModelTest::testConstructor_istream_moreThanPreferredPageSizeLines (void) {
  setTestName (__func__);

  const int expectedLineCount = 3 * Page::preferredSize + 1;

  string s = createLines (expectedLineCount);
  std::stringstream stream (s);

  const TextModel model (stream);
  int lineCount = model.lineCount ();
  assertEquals (expectedLineCount, lineCount);
  assertEquals (4, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);

  for (int i = 0; i < 4; ++i) {
    shared_ptr<const Page> page = model.m_pages.get ()[i];
    lineCount = page->lineCount ();
    if (i < 3)
      assertEquals (Page::preferredSize, lineCount);
    else
      assertEquals (1, lineCount);
    for (int j = 0; j < lineCount; ++j) {
      assertEquals (createLine (i * Page::preferredSize + j), *page->lineAt (j));
    }
  }
}

void TextModelTest::testPageAt (void) {
  setTestName (__func__);

  shared_ptr<const Page>* pages = new shared_ptr<const Page>[4];
  for (int i = 0; i < 4; ++i) {
    pages[i].reset (new Page);
  }

  TextModel model;
  model.m_pages.reset (pages, [] (shared_ptr<const Page>* array) { delete[] array; });
  model.m_pageCount = 4;
  model.m_editPage.reset (new Page);
  model.m_editPageIndex = 2;

  for (int i = 0; i < 4; ++i) {
    if (i != 2)
      assertEquals (pages[i].get (), model.pageAt (i).get ());
    else
      assertEquals (model.m_editPage.get (), model.pageAt (i).get ());
  }
}

void TextModelTest::testPageInfoForLine (void) {
  setTestName (__func__);

  int lc[] = {Page::preferredSize / 3, Page::preferredSize / 2, Page::preferredSize - 1};

  shared_ptr<const Page>* pages = new shared_ptr<const Page>[3];
  for (int i = 0; i < 3; ++i) {
    pages[i] = createPage (lc[i]);
  }

  TextModel model;
  model.m_pages.reset (pages, [] (shared_ptr<const Page>* array) { delete[] array; });
  model.m_pageCount = 3;
  model.m_lineCount = lc[0] + lc[1] + lc[2];

  for (int i = 0; i <= model.m_lineCount; ++i) {
    TextModel::PageInfo info = model.pageInfoForLine (i);
    if (i < lc[0]) {
      assertEquals (0, info.index);
      assertEquals (0, info.firstLine);
    } else if (i < lc[0] + lc[1]) {
      assertEquals (1, info.index);
      assertEquals (lc[0], info.firstLine);
    } else {
      assertEquals (2, info.index);
      assertEquals (lc[0] + lc[1], info.firstLine);
    }
  }

  model.m_editPage = createPage (Page::preferredSize);
  model.m_editPageIndex = 3;
  model.m_lineCount += Page::preferredSize;
  for (int i = 0; i < model.m_lineCount; ++i) {
    TextModel::PageInfo info = model.pageInfoForLine (i);
    if (i < lc[0]) {
      assertEquals (0, info.index);
      assertEquals (0, info.firstLine);
    } else if (i < lc[0] + lc[1]) {
      assertEquals (1, info.index);
      assertEquals (lc[0], info.firstLine);
    } else if (i < lc[0] + lc[1] + lc[2]) {
      assertEquals (2, info.index);
      assertEquals (lc[0] + lc[1], info.firstLine);
    } else {
      assertEquals (3, info.index);
      assertEquals (lc[0] + lc[1] + lc[2], info.firstLine);
    }
  }

  TextModel::PageInfo info = model.pageInfoForLine (model.m_lineCount);
  assertEquals (4, info.index);
  assertEquals (model.m_lineCount, info.firstLine);
}

void TextModelTest::testLineAt (void) {
  setTestName (__func__);

  /* TODO: Implement.  */
  assertTrue (false);
}

void TextModelTest::testInsert_emptyModel (void) {
  setTestName (__func__);

  /* TODO: Implement.  */
  assertTrue (false);
}

void TextModelTest::testInsert_modifyLineOrAddAtEnd (void) {
  setTestName (__func__);

  /* TODO: Implement.  */
  assertTrue (false);
}
