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

#include <gdk/gdkkeysyms.h>
#include <natpad/keybindings/CuaKeyBindings.h>

KeyCommand CuaKeyBindings::getAltCommand (int key, bool shift) {
  return KeyCommand::none;
}

KeyCommand CuaKeyBindings::getCtrlAltCommand (int key, bool shift) {
  return KeyCommand::none;
}

KeyCommand CuaKeyBindings::getCtrlCommand (int key, bool shift) {
  switch (key) {
  case GDK_KEY_End:
  case GDK_KEY_KP_End:
    return KeyCommand::cursorTextEnd;

  case GDK_KEY_Home:
  case GDK_KEY_KP_Home:
    return KeyCommand::cursorTextStart;
  }
  return KeyCommand::none;
}
