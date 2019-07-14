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

#ifndef __NATPAD_TEXTMODEL_BASIC_TEXT_MODEL_INCLUDED
#define __NATPAD_TEXTMODEL_BASIC_TEXT_MODEL_INCLUDED

#include <natpad/io/Reader.h>
#include <natpad/textmodel/TextModel.h>

class BasicTextModel : public TextModel {
public:
  using TextModel::insert;

  BasicTextModel (void);
  BasicTextModel (Reader& stream);

  shared_ptr<const TextModel> insert (const Cursor& cursor, const String& text) const override;

protected:
  unique_ptr<const Page> createEmptyPage (void) const override;

private:
  class Builder : public TextModel::Builder {
  public:
    shared_ptr<const TextModel> build (void) override;
  };
};

#endif
