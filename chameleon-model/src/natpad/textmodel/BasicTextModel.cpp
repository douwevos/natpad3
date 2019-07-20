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

#include <natpad/textmodel/BasicTextModel.h>
#include <natpad/util/StringUtils.h>
#include "BasicPage.h"

BasicTextModel::BasicTextModel (void) {
}

BasicTextModel::BasicTextModel (Reader& stream) {
  vector<shared_ptr<String>> lines = StringUtils::getLines (stream);
  if (!lines.empty ()) {
    m_lineCount = lines.size ();
    m_pageCount = m_lineCount / BasicPage::preferredSize;
    int remainder = m_lineCount % BasicPage::preferredSize;
    if (remainder != 0)
      ++m_pageCount;
    else
      remainder = BasicPage::preferredSize;
    shared_ptr<const Page>* pages = new shared_ptr<const Page>[m_pageCount];

    int i;
    BasicPage::Builder pageBuilder;
    for (i = 0; i < m_pageCount - 1; ++i) {
      shared_ptr<Line>* lineArray = new shared_ptr<Line>[BasicPage::preferredSize];
      for (int j = 0; j < BasicPage::preferredSize; ++j) {
        lineArray[j].reset (new Line (lines[BasicPage::preferredSize * i + j]));
      }
      pages[i] =
          pageBuilder.lines (shared_ptr<shared_ptr<Line>> (lineArray, [] (shared_ptr<Line>* array) { delete[] array; }), BasicPage::preferredSize)
                     .build ();
      pageBuilder.reset ();
    }

    shared_ptr<Line>* lineArray = new shared_ptr<Line>[remainder];
    for (int j = 0; j < remainder; ++j) {
      lineArray[j].reset (new Line (lines[BasicPage::preferredSize * i + j]));
    }
    pages[i] =
        pageBuilder.lines (shared_ptr<shared_ptr<Line>> (lineArray, [] (shared_ptr<Line>* array) { delete[] array; }), remainder)
                   .build ();

    m_pages.reset (pages, [](shared_ptr<const Page>* p) { delete[] p; });
  }
}

unique_ptr<const Page> BasicTextModel::createEmptyPage (void) const {
  return unique_ptr<const Page> (new BasicPage);
}

shared_ptr<const TextModel> BasicTextModel::insert (const Cursor& cursor, const String& text) const {
  BasicTextModel::Builder builder;
  return insert (builder, cursor, text);
}


/* BasicTextModel::Builder: */

shared_ptr<const TextModel> BasicTextModel::Builder::build (void) {
  BasicTextModel* textModel = new BasicTextModel ();
  return TextModel::Builder::build (textModel);
}
