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
#include <natpad/keybindings/EmacsKeyBindings.h>

KeyCommand EmacsKeyBindings::getAltCommand (int key, bool shift) {
  switch (key) {
  case GDK_KEY_v:
    return KeyCommand::cursorPageUp;
  }
  return KeyCommand::none;
}

KeyCommand EmacsKeyBindings::getCtrlAltCommand (int key, bool shift) {
  return KeyCommand::none;
}

KeyCommand EmacsKeyBindings::getCtrlCommand (int key, bool shift) {
  switch (key) {
  case GDK_KEY_a:
    return KeyCommand::cursorLineBeginning;

  case GDK_KEY_b:
    return KeyCommand::cursorBack;

  case GDK_KEY_e:
    return KeyCommand::cursorLineEnd;

  case GDK_KEY_f:
    return KeyCommand::cursorForward;

  case GDK_KEY_n:
    return KeyCommand::cursorLineDown;

  case GDK_KEY_p:
    return KeyCommand::cursorLineUp;

  case GDK_KEY_v:
    return KeyCommand::cursorPageDown;
  }
  return KeyCommand::none;
}
