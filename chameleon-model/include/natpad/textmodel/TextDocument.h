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

#ifndef __NATPAD_TEXTMODEL_TEXT_DOCUMENT_INCLUDED
#define __NATPAD_TEXTMODEL_TEXT_DOCUMENT_INCLUDED

#include <vector>
#include <natpad/textmodel/DocumentListener.h>

using std::vector;
using std::weak_ptr;

class TextDocument {
private:
  vector<shared_ptr<const TextModel>> m_versions;
  vector<weak_ptr<DocumentListener>> m_listeners;

public:
  /* Creates a text document with one empty line.  */
  TextDocument (void);

  explicit TextDocument (const shared_ptr<const TextModel>& textModel);

  /* No copies.  */
  TextDocument (const TextDocument&) = delete;
  TextDocument& operator= (const TextDocument&) = delete;

  /* No moves.  */
  TextDocument (TextDocument&&) = delete;
  TextDocument& operator= (TextDocument&&) = delete;

  void addListener (const shared_ptr<DocumentListener>& listener);
  void removeListener (const shared_ptr<DocumentListener>& listener);

  shared_ptr<const TextModel> getTextModel (void);
  void postTextModel (shared_ptr<const TextModel> textModel);
};

#endif
