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

#ifndef __NATPAD_TEXTMODEL_LAYOUT_PAGE_INCLUDED
#define __NATPAD_TEXTMODEL_LAYOUT_PAGE_INCLUDED

#include <natpad/textmodel/Page.h>

class LayoutPage : public Page {
private:
  const int m_width;
  shared_ptr<const Page> m_page;

public:
  LayoutPage (int width);
  LayoutPage (const shared_ptr<const Page>& page, int width);

  shared_ptr<const Page> insert (Cursor& cursor, const String& text) const override;

private:
  int addNewLinesFromManagedPage (
      shared_ptr<Line>* newLineArray,
      int dstIndex,
      shared_ptr<const Page> newManagedPage,
      int srcIndex,
      int deltaLineCountManagedPage) const;

  shared_ptr<const Page> appendNewLines (Cursor& cursor, const String& text) const;

  static void copyLines (shared_ptr<Line>* dstArray, int dstIdx, shared_ptr<Line>* srcArray, int srcIdx, int length);

  void copyLines (shared_ptr<Line>* dstArray, int dstIdx, int srcIdx, int length);

  void determineNewCursorAndEditLine (
      Cursor& cursor,
      LayoutPage* newPage,
      const Cursor& cursorManagedPage,
      int possibleNewEditLineIndex) const;

  shared_ptr<const Page> insertInEditLine (Cursor& cursor, const String& text) const;

  int insertInEditLine_linesManagedPageConstant (
      LayoutPage* newPage,
      int lineIndex,
      int startIndex) const;

  int insertInEditLine_linesManagedPageIncreased (
      LayoutPage* newPage,
      int lineIndex,
      int startIndex,
      int deltaLineCountManagedPage) const;

  shared_ptr<const Page> insertInOtherLine (Cursor& cursor, const String& text) const;

  int partCount (int length) const;

  int partLength (int length, int startIndex) const;
};

#endif
