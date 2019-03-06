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

#define NO_INDEX -1

TextModel::TextModel (void) : m_pageCount (0), m_editPageIndex (NO_INDEX), m_lineCount (0) {
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

shared_ptr<const TextModel> TextModel::insert (const Cursor& cursor, const string& text) const {
  if (cursor.line < 0 || cursor.line > m_lineCount)
    throw std::out_of_range ("Cursor line out of range.");

  TextModel::Builder builder;
  if (cursor.line < m_lineCount) {

    TextModel::PageInfo pageInfo = pageInfoForLine (cursor.line);
    if (pageInfo.index == m_editPageIndex) {

      int oldLC = m_editPage->lineCount ();
      shared_ptr<const Page> newEditPage = m_editPage->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);
      builder.editPage (m_editPageIndex, newEditPage)
             .lineCount (m_lineCount + newEditPage->lineCount () - oldLC);
      /* TODO: Liever nieuwe pagina inserten indien deze editPage te groot wordt?  */

    } else if (m_editPageIndex != NO_INDEX) {

      /* Nieuw TextModel met kopie van m_pages, met op m_editPageIndex de huidige m_editPage;
         de nieuwe m_editPageIndex wordt pageInfo.index en de nieuwe m_editPage de pagina in het huidige array op die index.  */
      int oldLC = m_pages.get ()[pageInfo.index]->lineCount ();
      shared_ptr<const Page> newEditPage = m_pages.get ()[pageInfo.index]->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);
      builder.setPage (m_editPageIndex, m_editPage) /* Kan m_editPageIndex ook gelijk zijn aan m_pageCount? Dat betekent addPage i.p.v. setPage... */
             .editPage (pageInfo.index, newEditPage)
             .lineCount (m_lineCount + newEditPage->lineCount () - oldLC);

    } else {

      /* Er was nog geen m_editPage:
         het nieuwe TextModel heeft hetzelfde array van m_pages (als dit model), maar met een m_editPage (aangegeven door pageInfo.index)...  */
      shared_ptr<const Page> newEditPage = m_pages.get ()[pageInfo.index]->insert (Cursor (cursor.line - pageInfo.firstLine, cursor.column), text);

    }

  } else {
    /* cursor.line == m_lineCount:
       nieuwe Page of een regel toevoegen aan de laatste Page.  */
  }

  return builder.pages (m_pages, m_pageCount).build ();
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


TextModel::Builder::Builder (void) : m_editPageIndex (NO_INDEX), m_pageCount (0), m_lineCount (0), m_setIndex (NO_INDEX), m_addPage (false) {
}

TextModel::Builder& TextModel::Builder::addPage (int index, const shared_ptr<const Page>& page) {
  setPage (index, page);
  m_addPage = true;
  return *this;
}

shared_ptr<const TextModel> TextModel::Builder::build (void) {
  if (m_pages.use_count () == 0)
    throw std::runtime_error ("No pages set.");
  if (m_setIndex < NO_INDEX || m_setIndex >= m_pageCount + m_addPage)
    throw std::out_of_range ("Cannot set or add a page at the specified index.");

  TextModel* textModel = new TextModel ();
  textModel->m_pageCount = m_pageCount + m_addPage;
  if (m_setIndex != NO_INDEX) {
    shared_ptr<const Page>* newPageArray = new shared_ptr<const Page>[textModel->m_pageCount];
    for (int i = 0; i < m_setIndex; ++i) {
      newPageArray[i] = m_pages.get ()[i];
    }
    newPageArray[m_setIndex] = m_setPage;
    for (int i = m_setIndex + 1 - m_addPage; i < m_pageCount; ++i) {
      newPageArray[i + m_addPage] = m_pages.get ()[i];
    }
    textModel->m_pages.reset (newPageArray, [](shared_ptr<const Page>* p) { delete[] p; });
  } else {
    textModel->m_pages = m_pages;
  }

  if (m_editPageIndex != NO_INDEX) {
    textModel->m_editPageIndex = m_editPageIndex;
    textModel->m_editPage = m_editPage;
  }
  textModel->m_lineCount = m_lineCount;

  return shared_ptr<const TextModel> (textModel);
}

TextModel::Builder& TextModel::Builder::editPage (int index, shared_ptr<const Page>& page) {
  m_editPageIndex = index;
  m_editPage = page;
  return *this;
}

TextModel::Builder& TextModel::Builder::lineCount (int lineCount) {
  m_lineCount = lineCount;
  return *this;
}

TextModel::Builder& TextModel::Builder::pages (const shared_ptr<shared_ptr<const Page>>& pages, int pageCount) {
  m_pageCount = pageCount;
  m_pages = pages;
  return *this;
}

TextModel::Builder& TextModel::Builder::setPage (int index, const shared_ptr<const Page>& page) {
  if (m_setIndex != NO_INDEX)
    throw std::runtime_error ("Modified page already set.");
  m_setIndex = index;
  m_setPage = page;
  return *this;
}
