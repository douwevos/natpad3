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

#ifndef __NATPAD_TEXTMODEL_TEXT_MODEL_INCLUDED
#define __NATPAD_TEXTMODEL_TEXT_MODEL_INCLUDED

#include <istream>
#include <natpad/textmodel/Page.h>

using std::istream;

class TextModel {
private:
  shared_ptr<shared_ptr<const Page>> m_pages;
  shared_ptr<const Page> m_editPage;
  int m_editPageIndex;
  int m_pageCount;
  int m_lineCount;

public:
  TextModel (void);
  TextModel (istream& stream);
  TextModel (shared_ptr<shared_ptr<const Page>>& pages, int pageCount);

  shared_ptr<const TextModel> insert (const Cursor& cursor, const string& text) const;
  shared_ptr<const string> lineAt (int line) const;
  int lineCount (void) const;

private:
  class PageInfo {
  public:
    const int index;
    const int firstLine;

    PageInfo (int index, int firstLine);
  };

  PageInfo pageInfoForLine (int line) const;
};

#endif
