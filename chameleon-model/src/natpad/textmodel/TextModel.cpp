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
#include <natpad/textmodel/TextModel.h>

#define NO_INDEX -1

TextModel::TextModel (void) : m_pageCount (0), m_editPageIndex (NO_INDEX), m_lineCount (0) {
}

TextModel::~TextModel (void) {
}

Cursor TextModel::cursor (void) const {
  return m_cursor;
}

shared_ptr<const TextModel> TextModel::insert (TextModel::Builder& builder, const Cursor& cursor, const String& text) const {
  if (cursor.line < 0 || cursor.line > m_lineCount)
    throw std::out_of_range ("Cursor line out of range.");

  TextModel::PageInfo pageInfo = pageInfoForLine (cursor.line);
  Cursor pageCursor (cursor.line - pageInfo.firstLine, cursor.column);

  int oldLC;
  shared_ptr<const Page> newEditPage;

  if (pageInfo.index == m_editPageIndex) {

    oldLC = m_editPage->lineCount ();
    newEditPage = m_editPage->insert (pageCursor, text);

  } else {

    if (pageInfo.index >= m_pageCount) {
      oldLC = 0;
      unique_ptr<const Page> editPage = createEmptyPage ();
      newEditPage = editPage->insert (pageCursor, text);
    } else {
      oldLC = m_pages.get ()[pageInfo.index]->lineCount ();
      newEditPage = m_pages.get ()[pageInfo.index]->insert (pageCursor, text);
    }
    if (m_editPageIndex != NO_INDEX) {
      builder.setPage (m_editPageIndex, m_editPage);
    }

  }

  return builder.pages (m_pages, m_pageCount)
                .editPage (pageInfo.index, newEditPage)
                .lineCount (m_lineCount + newEditPage->lineCount () - oldLC)
                .cursor (Cursor (pageCursor.line + pageInfo.firstLine, pageCursor.column))
                .build ();
}

shared_ptr<const TextModel> TextModel::insert (const Cursor& cursor, const std::string& utf8Text) const {
  StringConvert convert;
  String text = convert.from_bytes (utf8Text);
  return insert (cursor, text);
}

shared_ptr<Line> TextModel::lineAt (int line) const {
  if (line < 0 || line >= m_lineCount)
    throw std::out_of_range ("Specified line number out of range.");

  TextModel::PageInfo pageInfo = pageInfoForLine (line);
  return pageAt (pageInfo.index)->lineAt (line - pageInfo.firstLine);
}

int TextModel::lineCount (void) const {
  return m_lineCount;
}

TextModel::LineIterator TextModel::lineIterator (void) const {
  return TextModel::LineIterator (this, 0, 0);
}

TextModel::LineIterator TextModel::lineIterator (int line) const {
  if (line < 0 || line >= m_lineCount)
    throw std::out_of_range ("Specified line number out of range.");

  TextModel::PageInfo pageInfo = pageInfoForLine (line);
  return TextModel::LineIterator (this, pageInfo.index, line - pageInfo.firstLine);
}

/* Private method: no bounds check. */
const shared_ptr<const Page>& TextModel::pageAt (int index) const {
  if (index == m_editPageIndex)
    return m_editPage;
  return m_pages.get ()[index];
}

int TextModel::pageCount (void) const {
  return m_pageCount + (m_editPageIndex == m_pageCount);
}

TextModel::PageInfo TextModel::pageInfoForLine (int line) const {
  int pageCount = this->pageCount ();
  if (pageCount == 0)
    return TextModel::PageInfo (0, 0);

  int i = 1;
  int minLine = 0;
  int maxLine = pageAt (0)->lineCount ();
  while (i < pageCount && line >= maxLine) {
    minLine = maxLine;
    maxLine += pageAt (i++)->lineCount ();
  }

  --i;
  if (line < maxLine || pageAt (i)->lineCount () < Page::preferredSize)
    return TextModel::PageInfo (i, minLine);

  return TextModel::PageInfo (pageCount, maxLine);
}

TextModel::PageInfo::PageInfo (int index, int firstLine) : index (index), firstLine (firstLine) {
}
