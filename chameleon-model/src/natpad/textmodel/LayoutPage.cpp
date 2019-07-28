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

#include "LayoutLine.h"
#include "LayoutPage.h"

#define NO_INDEX -1

LayoutPage::LayoutPage (int width) : m_width (width) {
}

LayoutPage::LayoutPage (const shared_ptr<const Page>& page, int width) :
    m_page (page), m_width (width) {
  /* Determine length of array to create.  */
  int arrayLength = 0;
  int count = page->lineCount ();
  for (int i = 0; i < count; ++i) {
    int lineLength = page->lineAt (i)->length ();
    arrayLength += partCount (lineLength);
  }

  if (arrayLength > 0) {
    shared_ptr<Line>* lines = new shared_ptr<Line>[arrayLength];
    int d = 0;
    for (int s = 0; s < count; ++s) {
      shared_ptr<const String> text = page->lineAt (s)->text ();
      int textLength = text->length ();
      int partCountMinusOne = partCount (textLength) - 1;
      for (int i = 0; i < partCountMinusOne; ++i) {
        lines[d].reset (new LayoutLine (text, s, i * width, width));
        ++d;
      }
      int startIndex = partCountMinusOne * width;
      lines[d].reset (new LayoutLine (text, s, startIndex, textLength - startIndex));
      ++d;
    }

    m_lines.reset (lines, [] (shared_ptr<Line>* lines) { delete[] lines; });
    m_lineCount = arrayLength;
  }
}

/* srcArray and dstArray should not overlap.  */
void LayoutPage::copyLines (shared_ptr<Line>* dstArray, int dstIdx, shared_ptr<Line>* srcArray, int srcIdx, int length) {
  for (int i = 0; i < length; ++i) {
    dstArray[dstIdx] = srcArray[srcIdx];
    ++dstIdx;
    ++srcIdx;
  }
}

shared_ptr<const Page> LayoutPage::insert (Cursor& cursor, const String& text) const {
  validateCursorForInsert (cursor);

  if (cursor.line == m_editLineIndex) {
    return insertInEditLine (cursor, text);
  }

  if (cursor.line >= m_lineCount) {
  } else {
  }


  /* return newpage; */
}

/* PRECONDITIONS:
   1. cursor.line == m_editLineIndex
   2. If cursor.line is not the last line, then:
      lineAt (cursor.line + 1)->lineIndex () == lineAt (cursor.line)->lineIndex () + 1
      That is, the next line refers to a different line in the managed page.
   Condition 2 can only be met if we make sure that any m_editLine only refers to the
   last part of a line in the managed page.
 */
shared_ptr<const Page> LayoutPage::insertInEditLine (Cursor& cursor, const String& text) const {
  LayoutPage* newPage = new LayoutPage (m_width);

  LayoutLine* line = static_cast<LayoutLine*> (m_editLine.get ());
  const int lineIndex = line->lineIndex ();
  int startIndex = line->startIndex ();

  Cursor cursorManagedPage (lineIndex, cursor.column + startIndex);
  shared_ptr<const Page> newManagedPage = m_page->insert (cursorManagedPage, text);
  newPage->m_page = newManagedPage;

  if (cursorManagedPage.line == lineIndex) {
    /* Number of lines managed page remains constant.  */
    insertInEditLine_linesManagedPageConstant (newPage, lineIndex, startIndex);

  } else {
    /* Number of lines managed page increased.  */
    int deltaLineCountManagedPage = cursorManagedPage.line - lineIndex;
    insertInEditLine_linesManagedPageIncreased (newPage, lineIndex, startIndex, deltaLineCountManagedPage);
  }

  /* TODO:
     Determine new cursor and, if newPage->m_editLineIndex == NO_INDEX, an new m_editLine if possible.
   */


  /* Vergeet niet de binnenkomende cursor bij te werken!  */
  return shared_ptr<const Page> (newPage);
}

void LayoutPage::insertInEditLine_linesManagedPageConstant (
                                                            LayoutPage* newPage,
                                                            int lineIndex,
                                                            int startIndex) const {
  shared_ptr<Line> lineManagedPage = newPage->m_page->lineAt (lineIndex);
  if (lineManagedPage->length () - startIndex <= m_width) {

    newPage->m_lines = m_lines;
    newPage->m_editLine.reset (new LayoutLine (lineManagedPage->text (), lineIndex, startIndex, lineManagedPage->length () - startIndex));
    newPage->m_editLineIndex = m_editLineIndex;
    newPage->m_lineCount = m_lineCount;

  } else {

    int deltaLineCount = partCount (lineManagedPage->length ()) - partCount (m_page->lineAt (lineIndex)->length ());
    newPage->m_lineCount = m_lineCount + deltaLineCount;
    shared_ptr<Line>* newLineArray = new shared_ptr<Line>[newPage->m_lineCount];
    newPage->m_lines.reset (newLineArray, [] (shared_ptr<Line>* lines) { delete[] lines; });

    copyLines (newLineArray, 0, m_lines.get (), 0, m_editLineIndex);

    for (int i = 0; i < deltaLineCount; ++i) {
      newLineArray[m_editLineIndex + i].reset (new LayoutLine (lineManagedPage->text (), lineIndex, startIndex, m_width));
      startIndex += m_width;
    }
    newLineArray[m_editLineIndex + deltaLineCount].reset (new LayoutLine (lineManagedPage->text (), lineIndex, startIndex, lineManagedPage->length () - startIndex));

    copyLines (newLineArray, m_editLineIndex + 1 + deltaLineCount, m_lines.get (), m_editLineIndex + 1, m_lineCount - (m_editLineIndex + 1));

  }
}

void LayoutPage::insertInEditLine_linesManagedPageIncreased (
                                                             LayoutPage* newPage,
                                                             int lineIndex,
                                                             int startIndex,
                                                             int deltaLineCountManagedPage) const {
  int deltaLineCount = partCount (newPage->m_page->lineAt (lineIndex)->length ()) - partCount (m_page->lineAt (lineIndex)->length ());
  for (int i = 0; i < deltaLineCountManagedPage; ++i) {
    deltaLineCount += partCount (newPage->m_page->lineAt (lineIndex + 1 + i)->length ());
  }
  newPage->m_lineCount = m_lineCount + deltaLineCount;
  shared_ptr<Line>* newLineArray = new shared_ptr<Line>[newPage->m_lineCount];
  newPage->m_lines.reset (newLineArray, [] (shared_ptr<Line>* lines) { delete[] lines; });

  copyLines (newLineArray, 0, m_lines.get (), 0, m_editLineIndex);

  shared_ptr<Line> lineManagedPage = newPage->m_page->lineAt (lineIndex);
  int pc = partCount (lineManagedPage->length ()) - startIndex / m_width;
  int d = 0;
  for (int i = 0; i < pc; ++i) {
    int length = partLength (lineManagedPage->length (), startIndex);
    newLineArray[m_editLineIndex + d].reset (new LayoutLine (lineManagedPage->text (), lineIndex, startIndex, length));
    startIndex += m_width;
    ++d;
  }
  ++lineIndex;

  for (int i = 0; i < deltaLineCountManagedPage; ++i) {
    lineManagedPage = newPage->m_page->lineAt (lineIndex);
    pc = partCount (lineManagedPage->length ());
    startIndex = 0;
    for (int j = 0; j < pc; ++j) {
      int length = partLength (lineManagedPage->length (), startIndex);
      newLineArray[m_editLineIndex + d].reset (new LayoutLine (lineManagedPage->text (), lineIndex, startIndex, length));
      startIndex += m_width;
      ++d;
    }
    ++lineIndex;
  }

  shared_ptr<Line>* oldLineArray = m_lines.get ();
  int remainingLinesCount = m_lineCount - (m_editLineIndex + 1);
  for (int i = 0; i < remainingLinesCount; ++i) {
    LayoutLine* oldLine = static_cast<LayoutLine*> (oldLineArray[m_editLineIndex + 1 + i].get ());
    newLineArray[m_editLineIndex + d].reset (
        new LayoutLine (
            oldLine->text (),
            oldLine->lineIndex () + deltaLineCountManagedPage,
            oldLine->startIndex (),
            oldLine->length ()));
    ++d;
  }
}

int LayoutPage::partCount (int length) const {
  int result = length / m_width;
  if (length % m_width > 0) {
    ++result;
  }
  return result;
}

int LayoutPage::partLength (int length, int startIndex) const {
  int result = length - startIndex;
  if (result > m_width) {
    result = m_width;
  }
  return result;
}
