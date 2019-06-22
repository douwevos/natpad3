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

#include <natpad/textmodel/TextDocument.h>

using std::function;

void TextDocument::Listeners::add (const shared_ptr<DocumentListener>& listener) {
  m_listeners.push_back (weak_ptr<DocumentListener> (listener));
}

void TextDocument::Listeners::forEach (const function<void(shared_ptr<DocumentListener>&)>& consumer) {
  for (weak_ptr<DocumentListener> listener : m_listeners) {
    if (shared_ptr<DocumentListener> aliveListener = listener.lock ()) {
      consumer (aliveListener);
    }
  }
}

void TextDocument::Listeners::remove (const shared_ptr<DocumentListener>& listener) {
  for (vector<weak_ptr<DocumentListener>>::iterator iter = m_listeners.begin (); iter != m_listeners.end (); ++iter) {
    if (shared_ptr<DocumentListener> otherListener = (*iter).lock ()) {
      if (otherListener.get () == listener.get ()) {
        m_listeners.erase (iter);
        return;
      }
    }
  }
}
