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

#include <stdexcept>
#include <natpad/textmodel/Page.h>

#define NO_INDEX -1

Page::Page (void) : m_lineCount (0), m_editLineIndex (NO_INDEX) {
}

shared_ptr<const Page> Page::insert (Cursor& cursor, const String& text) const {
  validateCursorForInsert (cursor);

  Page::Builder builder;
  shared_ptr<String> line;
  int cursorDistanceToLineEnd;
  if (cursor.line == m_editLineIndex) {
    line.reset (new String (*m_editLine));
    cursorDistanceToLineEnd = line->length () - cursor.column;
    line->insert (cursor.column, text);
  } else {
    if (cursor.line >= m_lineCount) {
      line.reset (new String (text));
      cursorDistanceToLineEnd = 0;
    } else {
      line.reset (new String (*m_lines.get ()[cursor.line]));
      cursorDistanceToLineEnd = line->length () - cursor.column;
      line->insert (cursor.column, text);
    }
    if (m_editLineIndex != NO_INDEX) {
      builder.setLine (m_editLineIndex, m_editLine);
    }
  }
  shared_ptr<const Page> resultPage =
      builder.editLine (cursor.line, line)
          .lines (m_lines, m_lineCount)
          .build ();
  cursor.line = resultPage->m_editLineIndex;
  cursor.column = resultPage->m_editLine->length () - cursorDistanceToLineEnd;
  return resultPage;
}

shared_ptr<const String> Page::lineAt (int line) const {
  if (line < 0 || line >= lineCount ())
    throw std::out_of_range ("Specified line number out of range.");

  if (line == m_editLineIndex)
    return m_editLine;
  return m_lines.get ()[line];
}

int Page::lineCount (void) const {
  return m_lineCount + (m_editLineIndex == m_lineCount);
}

void Page::validateCursorForInsert (const Cursor& cursor) const {
  int lc = lineCount ();
  if (cursor.line < 0 || cursor.line > lc)
    throw std::out_of_range ("Cursor line out of range.");

  if (cursor.line == lc) {
    if (cursor.column != 0)
      throw std::out_of_range ("Cursor column out of range.");
    return;
  }

  int lineLength;
  if (cursor.line == m_editLineIndex)
    lineLength = m_editLine->length ();
  else
    lineLength = m_lines.get ()[cursor.line]->length ();
  if (cursor.column < 0 || cursor.column > lineLength)
    throw std::out_of_range ("Cursor column out of range.");
}
