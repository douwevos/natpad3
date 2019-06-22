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

#ifndef __NATPAD_KEYBINDINGS_KEY_BINDINGS_INCLUDED
#define __NATPAD_KEYBINDINGS_KEY_BINDINGS_INCLUDED

#include <natpad/keybindings/KeyCommand.h>

class KeyBindings {
public:
  typedef struct {
    int key;
    bool ctrl;
    bool alt;
    bool shift;
  } Event;

  virtual ~KeyBindings (void);

  KeyCommand getCommand (Event event);

protected:
  virtual KeyCommand getAltCommand (int key, bool shift) = 0;
  virtual KeyCommand getCtrlAltCommand (int key, bool shift) = 0;
  virtual KeyCommand getCtrlCommand (int key, bool shift) = 0;
  virtual KeyCommand getUnmodifiedCommand (int key, bool shift);
};

#endif
