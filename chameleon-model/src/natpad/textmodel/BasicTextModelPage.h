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

#ifndef __NATPAD_TEXTMODEL_BASIC_TEXT_MODEL_PAGE_INCLUDED
#define __NATPAD_TEXTMODEL_BASIC_TEXT_MODEL_PAGE_INCLUDED

#include <natpad/textmodel/Page.h>

class BasicTextModelPage : public Page {
public:
  static const int preferredSize = 1024;

  shared_ptr<const Page> insert (Cursor& cursor, const String& text) const override;

  class Builder {
  private:
    shared_ptr<shared_ptr<Line>> m_lines;
    shared_ptr<const String> m_editLine;
    shared_ptr<Line> m_setLine;
    int m_editLineIndex;
    int m_lineCount;
    int m_setIndex;

  public:
    Builder (void);
    Builder (const Builder&) = delete;
    Builder (Builder&&) = delete;

    Builder& operator= (const Builder&) = delete;
    Builder& operator= (Builder&&) = delete;

    shared_ptr<const BasicTextModelPage> build (void);
    Builder& editLine (int index, const shared_ptr<const String>& line);
    Builder& lines (const shared_ptr<shared_ptr<Line>>& lines, int lineCount);
    Builder& reset (void);
    Builder& setLine (int index, const shared_ptr<Line>& line);

  private:
    void buildSingleOrNoEditLine (BasicTextModelPage* page);
    void buildMultipleEditLines (BasicTextModelPage* page);
  };
};

#endif
