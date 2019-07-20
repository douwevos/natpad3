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

#include "BasicPage.h"

#define NO_INDEX -1

shared_ptr<const Page> BasicPage::insert (Cursor& cursor, const String& text) const {
  validateCursorForInsert (cursor);

  BasicPage::Builder builder;
  shared_ptr<String> line;
  int cursorDistanceToLineEnd;
  if (cursor.line == m_editLineIndex) {
    line.reset (new String (*m_editLine->text ()));
    cursorDistanceToLineEnd = line->length () - cursor.column;
    line->insert (cursor.column, text);
  } else {
    if (cursor.line >= m_lineCount) {
      line.reset (new String (text));
      cursorDistanceToLineEnd = 0;
    } else {
      line.reset (new String (*m_lines.get ()[cursor.line]->text ()));
      cursorDistanceToLineEnd = line->length () - cursor.column;
      line->insert (cursor.column, text);
    }
    if (m_editLineIndex != NO_INDEX) {
      builder.setLine (m_editLineIndex, m_editLine);
    }
  }
  shared_ptr<const BasicPage> resultPage =
      builder.editLine (cursor.line, line)
          .lines (m_lines, m_lineCount)
          .build ();
  cursor.line = resultPage->m_editLineIndex;
  cursor.column = resultPage->m_editLine->length () - cursorDistanceToLineEnd;
  return resultPage;
}
