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

#include <map>
#include <sstream>
#include <stdexcept>
#include "TextModelTest.h"

#include <string>
#include <istream>
#include <natpad/textmodel/Page.h>
#define private public
#include <natpad/textmodel/TextModel.h>
#undef private

using std::map;

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
  if (count > 0)
    result.pop_back ();
  return result;
}

static shared_ptr<const Page> createPage (int lineCount, int index = 0) {
  shared_ptr<const string>* lines = new shared_ptr<const string>[lineCount];
  for (int i = 0; i < lineCount; ++i) {
    lines[i] = shared_ptr<const string> (new string (createLine (index++)));
  }
  Page::Builder builder;
  return builder.lines (shared_ptr<shared_ptr<const string>> (lines, [] (shared_ptr<const string>* array) { delete[] array; }), lineCount)
                .build ();
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
  assertEquals (1, model.lineCount ());
  assertEquals (1, model.m_pageCount);
  assertTrue (model.m_editPageIndex < 0);
  assertEquals ("", *model.m_pages.get ()[0]->lineAt (0));
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

  shared_ptr<const Page>* pages = new shared_ptr<const Page>[3];
  pages[0] = createPage (Page::preferredSize);
  pages[1] = createPage (Page::preferredSize);
  pages[2] = createPage (Page::preferredSize, 2 * Page::preferredSize);

  TextModel model;
  model.m_pages.reset (pages, [] (shared_ptr<const Page>* array) { delete[] array; });
  model.m_pageCount = 3;
  model.m_lineCount = 3 * Page::preferredSize;
  model.m_editPage = createPage (Page::preferredSize, Page::preferredSize);
  model.m_editPageIndex = 1;

  for (int i = 0; i < model.m_lineCount; ++i) {
    shared_ptr<const string> line = model.lineAt (i);
    int n = std::stoi (*line);
    assertEquals (i + 1, n);
  }

  try {
    model.lineAt (model.m_lineCount);
    fail ("out_of_range exception expected.");
  } catch (std::out_of_range& x) {
  }

  try {
    model.lineAt (-1);
    fail ("out_of_range exception expected.");
  } catch (std::out_of_range& x) {
  }
}

void TextModelTest::testInsert_emptyModel (void) {
  setTestName (__func__);

  shared_ptr<const TextModel> textModel = shared_ptr<const TextModel> (new TextModel);
  textModel = textModel->insert (Cursor (), "hoi");
  assertEquals (1, textModel->lineCount ());
  assertEquals ("hoi", *textModel->lineAt (0));
}

/* Auxiliary to TextModelTest::testInsert_modifyLineOrAddAtEnd.  */
static void updateChangedLinesMap (map<int, string>& changedLines, int lineNo, const string& appendedText) {
  string& line = changedLines[lineNo];
  if (line.empty () && lineNo < 3 * Page::preferredSize - 1)
    line = createLine (lineNo);
  line += appendedText;
}

void TextModelTest::testInsert_modifyLineOrAddAtEnd (void) {
  setTestName (__func__);

  string s = createLines (3 * Page::preferredSize - 1);
  std::stringstream stream (s);

  map<int, string> changedLines;
  shared_ptr<const TextModel> textModel = shared_ptr<const TextModel> (new TextModel (stream));
  assertEquals (3, textModel->m_pageCount);

  /* Updates an existing line in a model with no m_editPage selected: */
  textModel = textModel->insert (Cursor (Page::preferredSize + 4, 5), "A");
  updateChangedLinesMap (changedLines, Page::preferredSize + 4, "A");

  /* Updates an existing line on the same m_editPage: */
  textModel = textModel->insert (Cursor (Page::preferredSize + 2, 5), "B");
  updateChangedLinesMap (changedLines, Page::preferredSize + 2, "B");

  /* Update the same line: */
  textModel = textModel->insert (Cursor (Page::preferredSize + 2, 6), "C");
  updateChangedLinesMap (changedLines, Page::preferredSize + 2, "C");

  /* Update a line on different page: */
  textModel = textModel->insert (Cursor (2 * Page::preferredSize + 3, 5), "D");
  updateChangedLinesMap (changedLines, 2 * Page::preferredSize + 3, "D");

  /* Add a line to that page: */
  textModel = textModel->insert (Cursor (3 * Page::preferredSize - 1, 0), "E");
  updateChangedLinesMap (changedLines, 3 * Page::preferredSize - 1, "E");
  assertEquals (2, textModel->m_editPageIndex);

  /* Add another line, on a new page: */
  textModel = textModel->insert (Cursor (3 * Page::preferredSize, 0), "F");
  updateChangedLinesMap (changedLines, 3 * Page::preferredSize, "F");
  assertEquals (3, textModel->m_editPageIndex);

  int expectedLineCount = 3 * Page::preferredSize + 1;
  assertEquals (expectedLineCount, textModel->lineCount ());
  for (int i = 0; i < expectedLineCount; ++i) {
    string str;
    map<int, string>::iterator iter = changedLines.find (i);
    if (iter != changedLines.end ()) {
      str = iter->second;
    } else {
      str = createLine (i);
    }
    assertEquals (str, *textModel->lineAt (i));
  }
}
