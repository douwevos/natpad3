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

Page::Page (void) : m_lineCount (0) {
}

Page::Page (shared_ptr<shared_ptr<const string>>& lines, int lineCount) : m_lineCount (lineCount), m_lines (lines) {
}

shared_ptr<const Page> Page::insert (Cursor cursor, const string& text) const {
  int lineCount = validateCursorForInsert (cursor);
  shared_ptr<shared_ptr<const string>> lines (new shared_ptr<const string>[lineCount], [](shared_ptr<const string>* p) { delete[] p; });
  for (int i = 0; i < m_lineCount; ++i) {
    lines.get ()[i] = m_lines.get ()[i];
  }
  if (cursor.line == m_lineCount) {
    shared_ptr<string> newLine (new string (text));
    lines.get ()[cursor.line] = newLine;
  } else {
    shared_ptr<string> modifiedLine (new string (*m_lines.get ()[cursor.line]));
    modifiedLine->insert (cursor.column, text);
    lines.get ()[cursor.line] = modifiedLine;
  }

  return shared_ptr<Page> (new Page (lines, lineCount));
}

shared_ptr<const string> Page::lineAt (int line) const {
  if (line < 0 || line >= m_lineCount)
    throw std::out_of_range ("Specified line number out of range.");

  return m_lines.get ()[line];
}

int Page::lineCount (void) const {
  return m_lineCount;
}

int Page::validateCursorForInsert (const Cursor& cursor) const {
  if (cursor.line < 0 || cursor.line > m_lineCount)
    throw std::out_of_range ("Cursor line out of range.");

  if (cursor.line == m_lineCount) {
    if (cursor.column != 0)
      throw std::out_of_range ("Cursor column out of range.");
    return m_lineCount + 1;
  }

  int lineLength = m_lines.get ()[cursor.line]->length ();
  if (cursor.column < 0 || cursor.column > lineLength)
    throw std::out_of_range ("Cursor column out of range.");
  return m_lineCount;
}


/* Page::Builder: */

Page::Builder::Builder (void) : m_page (nullptr) {
}

Page::Builder::~Builder (void) {
  if (m_page != nullptr) {
    delete m_page;
  }
}

Page::Builder& Page::Builder::addLine (shared_ptr<const string> line) {
  if (m_page == nullptr) {
    throw std::runtime_error ("Cannot add a new line: try calling Page::Builder::prepareBuildingNewPage first.");
  }
  if (m_index == m_lineCount) {
    throw std::runtime_error ("Cannot add a new line: page already complete.");
  }
  m_page->m_lines.get ()[m_index++] = line;
  return *this;
}

shared_ptr<const Page> Page::Builder::build (void) {
  if (m_page == nullptr || m_index < m_lineCount) {
    throw std::runtime_error ("Cannot build new page, it was either not prepared (null) or not complete yet.");
  }
  shared_ptr<const Page> result (m_page);
  m_page = nullptr;
  return result;
}

Page::Builder& Page::Builder::prepareBuildingNewPage (int lineCount) {
  if (m_page != nullptr) {
    delete m_page;
  }
  m_lineCount = lineCount;
  m_index = 0;
  m_page = new Page ();
  m_page->m_lineCount = lineCount;
  if (lineCount > 0)
    m_page->m_lines.reset (new shared_ptr<const string>[lineCount], [](shared_ptr<const string>* p) { delete[] p; });
  return *this;
}
