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

shared_ptr<const Page> Page::insert (Cursor cursor, const string& text) const {
  validateCursorForInsert (cursor);

  Page::Builder builder;
  shared_ptr<string> line;
  if (cursor.line == m_editLineIndex) {
    line.reset (new string (*m_editLine));
    line->insert (cursor.column, text);
  } else {
    if (cursor.line >= m_lineCount) {
      line.reset (new string (text));
    } else {
      line.reset (new string (*m_lines.get ()[cursor.line]));
      line->insert (cursor.column, text);
    }
    if (m_editLineIndex != NO_INDEX) {
      builder.setLine (m_editLineIndex, m_editLine);
    }
  }
  return builder.editLine (cursor.line, line).lines (m_lines, m_lineCount).build ();
}

shared_ptr<const string> Page::lineAt (int line) const {
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
