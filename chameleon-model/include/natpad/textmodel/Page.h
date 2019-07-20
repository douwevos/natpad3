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

#ifndef __NATPAD_TEXTMODEL_PAGE_INCLUDED
#define __NATPAD_TEXTMODEL_PAGE_INCLUDED

#include <natpad/textmodel/Cursor.h>
#include <natpad/textmodel/Line.h>

class Page {
protected:
  shared_ptr<shared_ptr<Line>> m_lines;
  shared_ptr<Line> m_editLine;
  int m_editLineIndex;
  int m_lineCount;

public:
  Page (void);
  virtual ~Page (void);

  /* Constructs a new Page from this one, with the specified text inserted at the
     the position of the specified cursor; cursor will change value to the position
     where it ends up after insertion.  The new page with the inserted text is
     returned.  */
  virtual shared_ptr<const Page> insert (Cursor& cursor, const String& text) const = 0;
  shared_ptr<Line> lineAt (int line) const;
  int lineCount (void) const;

protected:
  void validateCursorForInsert (const Cursor& cursor) const;
};

#endif
