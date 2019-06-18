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
#include <natpad/util/StringUtils.h>

#define NO_INDEX -1

Page::Builder::Builder (void) : m_editLineIndex (NO_INDEX), m_lineCount (0), m_setIndex (NO_INDEX) {
}

shared_ptr<const Page> Page::Builder::build (void) {
  if (m_setIndex < NO_INDEX || m_setIndex > m_lineCount)
    throw std::out_of_range ("Page::Builder::build: Cannot set or add a line at the specified index.");
  bool appendLine = m_setIndex == m_lineCount;

  Page* page = new Page;
  page->m_lineCount = m_lineCount + appendLine;

  if (m_editLine && m_editLine->find ('\n') != String::npos)
    buildMultipleEditLines (page);
  else
    buildSingleOrNoEditLine (page);

  return shared_ptr<const Page> (page);
}

void Page::Builder::buildMultipleEditLines (Page* page) {
  vector<shared_ptr<String>> linesToInsert = StringUtils::getLines (*m_editLine);
  page->m_editLine = linesToInsert.back ();
  linesToInsert.pop_back ();
  page->m_lineCount += linesToInsert.size ();
  page->m_editLineIndex = m_editLineIndex + linesToInsert.size ();
  shared_ptr<const String>* newLineArray = new shared_ptr<const String>[page->m_lineCount];
  page->m_lines.reset (newLineArray, [](shared_ptr<const String>* array) { delete[] array; });

  if (m_setIndex == NO_INDEX) {

    for (int i = 0; i < m_editLineIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    for (int i = 0; i < linesToInsert.size (); ++i) {
      newLineArray[i + m_editLineIndex] = linesToInsert[i];
    }
    int j = page->m_editLineIndex + 1;
    for (int i = m_editLineIndex + 1; i < m_lineCount; ++i) {
      newLineArray[j++] = m_lines.get ()[i];
    }

  } else if (m_setIndex < m_editLineIndex) {

    for (int i = 0; i < m_setIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    newLineArray[m_setIndex] = m_setLine;
    for (int i = m_setIndex + 1; i < m_editLineIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    for (int i = 0; i < linesToInsert.size (); ++i) {
      newLineArray[i + m_editLineIndex] = linesToInsert[i];
    }
    int j = page->m_editLineIndex + 1;
    for (int i = m_editLineIndex + 1; i < m_lineCount; ++i) {
      newLineArray[j++] = m_lines.get ()[i];
    }

  } else {

    for (int i = 0; i < m_editLineIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    for (int i = 0; i < linesToInsert.size (); ++i) {
      newLineArray[i + m_editLineIndex] = linesToInsert[i];
    }
    int j = page->m_editLineIndex + 1;
    for (int i = m_editLineIndex + 1; i < m_setIndex; ++i) {
      newLineArray[j++] = m_lines.get ()[i];
    }
    newLineArray[j++] = m_setLine;
    for (int i = m_setIndex + 1; i < m_lineCount; ++i) {
      newLineArray[j++] = m_lines.get ()[i];
    }

  }
}

void Page::Builder::buildSingleOrNoEditLine (Page* page) {
  if (m_setIndex != NO_INDEX) {
    shared_ptr<const String>* newLineArray = new shared_ptr<const String>[page->m_lineCount];
    for (int i = 0; i < m_setIndex; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    newLineArray[m_setIndex] = m_setLine;
    for (int i = m_setIndex + 1; i < m_lineCount; ++i) {
      newLineArray[i] = m_lines.get ()[i];
    }
    page->m_lines.reset (newLineArray, [](shared_ptr<const String>* array) { delete[] array; });
  } else {
    page->m_lines = m_lines;
  }
  if (m_editLineIndex != NO_INDEX) {
    page->m_editLineIndex = m_editLineIndex;
    page->m_editLine = m_editLine;
  }
}

Page::Builder& Page::Builder::editLine (int index, const shared_ptr<const String>& line) {
  m_editLineIndex = index;
  m_editLine = line;
  return *this;
}

Page::Builder& Page::Builder::lines (const shared_ptr<shared_ptr<const String>>& lines, int lineCount) {
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
  return *this;
}

Page::Builder& Page::Builder::setLine (int index, const shared_ptr<const String>& line) {
  if (m_setIndex != NO_INDEX)
    throw std::runtime_error ("Page::Builder::setLine: Modified line already set.");
  m_setIndex = index;
  m_setLine = line;
  return *this;
}
