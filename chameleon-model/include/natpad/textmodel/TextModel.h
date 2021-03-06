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

#include <natpad/io/Reader.h>
#include <natpad/textmodel/Page.h>

class TextModel {
private:
  shared_ptr<shared_ptr<const Page>> m_pages;
  shared_ptr<const Page> m_editPage;
  Cursor m_cursor;
  int m_editPageIndex;
  int m_pageCount;
  int m_lineCount;

public:
  TextModel (void);
  TextModel (Reader& stream);

  Cursor cursor (void) const;
  shared_ptr<const TextModel> insert (const Cursor& cursor, const String& text) const;
  shared_ptr<const TextModel> insert (const Cursor& cursor, const std::string& utf8Text) const;
  shared_ptr<const String> lineAt (int line) const;
  int lineCount (void) const;

private:
  class Builder {
  private:
    shared_ptr<shared_ptr<const Page>> m_pages;
    shared_ptr<const Page> m_editPage;
    shared_ptr<const Page> m_setPage;
    Cursor m_cursor;
    int m_editPageIndex;
    int m_pageCount;
    int m_lineCount;
    int m_setIndex;
    bool m_addPage;

  public:
    Builder (void);

    Builder& addPage (int index, const shared_ptr<const Page>& page);
    shared_ptr<const TextModel> build (void);
    Builder& cursor (const Cursor& cursor);
    Builder& editPage (int index, shared_ptr<const Page>& page);
    Builder& lineCount (int lineCount);
    Builder& pages (const shared_ptr<shared_ptr<const Page>>& pages, int pageCount);
    Builder& setPage (int index, const shared_ptr<const Page>& page);
  };

  class PageInfo {
  public:
    const int index;
    const int firstLine;

    PageInfo (int index, int firstLine);
  };

  const shared_ptr<const Page>& pageAt (int index) const;
  int pageCount (void) const;
  PageInfo pageInfoForLine (int line) const;
};

#endif
