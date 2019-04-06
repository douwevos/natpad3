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
  shared_ptr<const string> m_editLine;
  int m_editLineIndex;
  int m_lineCount;

public:
  static const int preferredSize = 1024;

  Page (void);

  shared_ptr<const Page> insert (Cursor cursor, const string& text) const;
  shared_ptr<const string> lineAt (int line) const;
  int lineCount (void) const;

  class Builder {
  private:
    shared_ptr<shared_ptr<const string>> m_lines;
    shared_ptr<const string> m_editLine;
    shared_ptr<const string> m_setLine;
    int m_editLineIndex;
    int m_lineCount;
    int m_setIndex;

  public:
    Builder (void);
    Builder (const Builder&) = delete;
    Builder (Builder&&) = delete;

    Builder& operator= (const Builder&) = delete;
    Builder& operator= (Builder&&) = delete;

    shared_ptr<const Page> build (void);
    Builder& editLine (int index, const shared_ptr<const string>& line);
    Builder& lines (const shared_ptr<shared_ptr<const string>>& lines, int lineCount);
    Builder& reset (void);
    Builder& setLine (int index, const shared_ptr<const string>& line);

  private:
    void buildSingleOrNoEditLine (Page* page);
    void buildMultipleEditLines (Page* page);
  };

private:
  void validateCursorForInsert (const Cursor& cursor) const;
};

#endif
