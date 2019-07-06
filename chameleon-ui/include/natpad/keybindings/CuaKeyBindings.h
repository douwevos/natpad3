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

#ifndef __NATPAD_KEYBINDINGS_CUA_KEY_BINDINGS_INCLUDED
#define __NATPAD_KEYBINDINGS_CUA_KEY_BINDINGS_INCLUDED

#include <natpad/keybindings/KeyBindings.h>

class CuaKeyBindings : public KeyBindings {
protected:
  KeyCommand getAltCommand (int key, bool shift) override;
  KeyCommand getCtrlAltCommand (int key, bool shift) override;
  KeyCommand getCtrlCommand (int key, bool shift) override;
};

#endif
