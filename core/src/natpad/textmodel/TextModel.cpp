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
#include <natpad/util/StringUtils.h>

#define NO_EDIT_PAGE -1

TextModel::TextModel (void) : m_pageCount (0), m_editPageIndex (NO_EDIT_PAGE), m_lineCount (0) {
}

TextModel::TextModel (istream& stream) : TextModel () {
  vector<shared_ptr<string>> lines = StringUtils::getLines (stream);
  if (!lines.empty ()) {
    m_lineCount = lines.size ();
    m_pageCount = m_lineCount / Page::preferredSize;
    int remainder = m_lineCount % Page::preferredSize;
    if (remainder != 0)
      ++m_pageCount;
    else
      remainder = Page::preferredSize;
    shared_ptr<const Page>* pages = new shared_ptr<const Page>[m_pageCount];

    int i;
    Page::Builder pageBuilder;
    for (i = 0; i < m_pageCount - 1; ++i) {
      pageBuilder.prepareBuildingNewPage (Page::preferredSize);
      for (int j = 0; j < Page::preferredSize; ++j) {
        pageBuilder.addLine (lines[Page::preferredSize * i + j]);
      }
      pages[i] = pageBuilder.build ();
    }
    pageBuilder.prepareBuildingNewPage (remainder);
    for (int j = 0; j < remainder; ++j) {
      pageBuilder.addLine (lines[Page::preferredSize * i + j]);
    }
    pages[i] = pageBuilder.build ();

    m_pages.reset (pages, [](shared_ptr<const Page>* p) { delete[] p; });
  }
}

TextModel::TextModel (shared_ptr<shared_ptr<const Page>>& pages, int pageCount) {
  /* TODO: Implement or delete this one.  */
  throw std::runtime_error ("Unsupported Operation.");
}

shared_ptr<const TextModel> TextModel::insert (const Cursor& cursor, const string& text) const {
  if (cursor.line < 0 || cursor.line > m_lineCount)
    throw std::out_of_range ("Cursor line out of range.");

  if (cursor.line < m_lineCount) {

    TextModel::PageInfo pageInfo = pageInfoForLine (cursor.line);
    if (pageInfo.index == m_editPageIndex) {
      /* Nieuw TextModel met zelfde array van m_pages, maar met newPage als m_editPage.  */
      shared_ptr<const Page> newEditPage = m_editPage->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);
    } else if (m_editPageIndex != NO_EDIT_PAGE) {
      /* Nieuw TextModel met kopie van m_pages, met op m_editPageIndex de huidige m_editPage;
         de nieuwe m_editPageIndex wordt pageInfo.index en de nieuwe m_editPage de pagina in het huidige array op die index.  */
      shared_ptr<const Page> newEditPage = m_pages.get ()[pageInfo.index]->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);
    } else {
      /* Er was nog geen m_editPage:
         het nieuwe TextModel heeft hetzelfde array van m_pages (als dit model), maar met een m_editPage (aangegeven door pageInfo.index)...  */
      shared_ptr<const Page> newEditPage = m_pages.get ()[pageInfo.index]->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);
    }

  } else {
    /* cursor.line == m_lineCount:
       nieuwe Page of een regel toevoegen aan de laatste Page.  */
  }


  throw std::runtime_error ("Deze methode (TextModel::insert) is nog niet klaar.");

  return shared_ptr<const TextModel> (nullptr); /* Deze moet dus nog even worden vervangen!!!!  */
}

shared_ptr<const string> TextModel::lineAt (int line) const {
  if (line < 0 || line >= m_lineCount)
    throw std::out_of_range ("Specified line number out of range.");

  TextModel::PageInfo pageInfo = pageInfoForLine (line);
  return m_pages.get ()[pageInfo.index]->lineAt (line - pageInfo.firstLine);
}

int TextModel::lineCount (void) const {
  return m_lineCount;
}

/* NB: Alleen aanroepen als 0 <= line < m_lineCount !!  */
TextModel::PageInfo TextModel::pageInfoForLine (int line) const {
  shared_ptr<const Page>* pages = m_pages.get ();
  int i = 0;
  int minLine = 0;
  int maxLine = pages[0]->lineCount ();
  while (line >= maxLine) {
    minLine = maxLine;
    maxLine += pages[++i]->lineCount ();
  }
  return TextModel::PageInfo (i, minLine);
}

TextModel::PageInfo::PageInfo (int index, int firstLine) : index (index), firstLine (firstLine) {
}
