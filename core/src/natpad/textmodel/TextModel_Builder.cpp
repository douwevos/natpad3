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
  if (m_setIndex < NO_INDEX || m_setIndex > m_pageCount)
    throw std::out_of_range ("Cannot set or add a page at the specified index.");
  if (m_setIndex == m_pageCount)
    m_addPage = true;

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
