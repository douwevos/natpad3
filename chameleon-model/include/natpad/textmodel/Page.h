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

#include <memory>
#include <string>
#include <natpad/textmodel/Cursor.h>

using std::shared_ptr;
using std::string;

class Page {
private:
  shared_ptr<shared_ptr<const string>> m_lines;
  int m_lineCount;

public:
  static const int preferredSize = 1024;

  Page (void);
  Page (shared_ptr<shared_ptr<const string>>& lines, int lineCount);

  shared_ptr<const Page> insert (Cursor cursor, const string& text) const;
  shared_ptr<const string> lineAt (int line) const;
  int lineCount (void) const;

  class Builder {
  private:
    Page* m_page;
    int m_lineCount;
    int m_index;

  public:
    Builder (void);
    Builder (const Builder&) = delete;
    Builder (Builder&&) = delete;
    ~Builder (void);

    Builder& operator= (const Builder&) = delete;
    Builder& operator= (Builder&&) = delete;

    Builder& addLine (shared_ptr<const string> line);
    shared_ptr<const Page> build (void);
    Builder& prepareBuildingNewPage (int lineCount);
  };

private:
  int validateCursorForInsert (const Cursor& cursor) const;
};

#endif
