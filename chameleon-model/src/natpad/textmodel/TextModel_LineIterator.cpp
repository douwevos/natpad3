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

TextModel::LineIterator::LineIterator (const TextModel* textModel, int pageIndex, int lineIndex) :
    m_textModel (textModel),
    m_pageIndex (pageIndex),
    m_lineIndex (lineIndex) {
  int pageCount = textModel->pageCount ();
  if (pageIndex < pageCount) {
    m_page = textModel->pageAt (pageIndex);

#ifdef DEBUG_MODE
    if (m_page->lineCount () == 0) {
      throw std::runtime_error ("TextModel::LineIterator: page has no lines.");
    }
#endif
  }
}

TextModel::LineIterator::LineIterator (const LineIterator& other) :
    m_textModel (other.m_textModel),
    m_pageIndex (other.m_pageIndex),
    m_lineIndex (other.m_lineIndex),
    m_page (other.m_page) {
}

TextModel::LineIterator& TextModel::LineIterator::operator= (const LineIterator& other) {
  if (&other == this) {
    return *this;
  }
  m_textModel = other.m_textModel;
  m_pageIndex = other.m_pageIndex;
  m_lineIndex = other.m_lineIndex;
  m_page = other.m_page;
  return *this;
}

TextModel::LineIterator::LineIterator (TextModel::LineIterator&& other) :
    m_textModel (other.m_textModel),
    m_pageIndex (other.m_pageIndex),
    m_lineIndex (other.m_lineIndex),
    m_page (other.m_page) {
  other.m_textModel = nullptr;
  other.m_page.reset ();
}

TextModel::LineIterator& TextModel::LineIterator::operator= (TextModel::LineIterator&& other) {
  if (&other == this) {
    return *this;
  }
  m_textModel = other.m_textModel;
  m_pageIndex = other.m_pageIndex;
  m_lineIndex = other.m_lineIndex;
  m_page = other.m_page;
  other.m_textModel = nullptr;
  other.m_page.reset ();
  return *this;
}

TextModel::LineIterator& TextModel::LineIterator::operator++ (void) {
  if (m_page) {
    ++m_lineIndex;
    if (m_lineIndex == m_page->lineCount ()) {
      m_lineIndex = 0;
      ++m_pageIndex;
      int pageCount = m_textModel->pageCount ();
      if (m_pageIndex < pageCount) {
        m_page = m_textModel->pageAt (m_pageIndex);

#ifdef DEBUG_MODE
        if (m_page->lineCount () == 0) {
          throw std::runtime_error ("TextModel::LineIterator: moving to page without lines.");
        }
#endif

      } else {
        m_page.reset ();
      }
    }
  }
  return *this;
}

TextModel::LineIterator TextModel::LineIterator::operator++ (int dummy) {
  TextModel::LineIterator result = *this;
  ++*this;
  return result;
}

shared_ptr<Line> TextModel::LineIterator::operator* (void) const {
  if (!m_page) {
    throw std::out_of_range ("TextModel::LineIterator: no more lines available.");
  }
  return m_page->lineAt (m_lineIndex);
}
