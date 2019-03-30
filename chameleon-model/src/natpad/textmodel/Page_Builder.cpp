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
#include <natpad/textmodel/Page.h>

#define NO_INDEX -1

Page::Builder::Builder (void) : m_editLineIndex (NO_INDEX), m_lineCount (0), m_setIndex (NO_INDEX), m_addLine (false) {
}

Page::Builder& Page::Builder::addLine (int index, const shared_ptr<const string>& line) {
  setLine (index, line);
  m_addLine = true;
  return *this;
}

shared_ptr<const Page> Page::Builder::build (void) {
  if (m_setIndex < NO_INDEX || m_setIndex > m_lineCount)
    throw std::out_of_range ("Page::Builder::build: Cannot set or add a line at the specified index.");
  if (m_setIndex == m_lineCount)
    m_addLine = true;

  Page* page = new Page;
  page->m_lineCount = m_lineCount + m_addLine;
  if (m_setIndex != NO_INDEX) {
    shared_ptr<const string>* newLineArray = new shared_ptr<const string>[page->m_lineCount];
    for (int i = 0; i < m_setIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    newLineArray[m_setIndex] = m_setLine;
    for (int i = m_setIndex + 1 - m_addLine; i < m_lineCount; ++i) {
      newLineArray[i +  m_addLine] = m_lines.get ()[i];
    }
    page->m_lines.reset (newLineArray, [](shared_ptr<const string>* array) { delete[] array; });
  } else {
    page->m_lines = m_lines;
  }
  if (m_editLineIndex != NO_INDEX) {
    page->m_editLineIndex = m_editLineIndex;
    page->m_editLine = m_editLine;
  }

  return shared_ptr<const Page> (page);
}

Page::Builder& Page::Builder::editLine (int index, const shared_ptr<const string>& line) {
  m_editLineIndex = index;
  m_editLine = line;
  return *this;
}

Page::Builder& Page::Builder::lines (const shared_ptr<shared_ptr<const string>>& lines, int lineCount) {
  m_lines = lines;
  m_lineCount = lineCount;
  return *this;
}

Page::Builder& Page::Builder::reset (void) {
  m_lines.reset ();
  m_editLine.reset ();
  m_setLine.reset ();
  m_editLineIndex = NO_INDEX;
  m_lineCount = 0;
  m_setIndex = NO_INDEX;
  m_addLine = false;
  return *this;
}

Page::Builder& Page::Builder::setLine (int index, const shared_ptr<const string>& line) {
  if (m_setIndex != NO_INDEX)
    throw std::runtime_error ("Page::Builder::setLine: Modified line already set.");
  m_setIndex = index;
  m_setLine = line;
  return *this;
}
