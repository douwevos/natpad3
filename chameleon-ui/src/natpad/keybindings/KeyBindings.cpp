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
#include <natpad/keybindings/KeyBindings.h>

KeyBindings::~KeyBindings (void) {
}

KeyCommand KeyBindings::getCommand (Event event) {
  if (event.ctrl) {
    if (event.alt) {
      return getCtrlAltCommand (event.key, event.shift);
    }
    return getCtrlCommand (event.key, event.shift);
  } else if (event.alt) {
    return getAltCommand (event.key, event.shift);
  }
  return getUnmodifiedCommand (event.key, event.shift);
}

KeyCommand KeyBindings::getUnmodifiedCommand (int key, bool shift) {
  switch (key) {
  case GDK_KEY_Down:
    return KeyCommand::cursorLineDown;

  case GDK_KEY_Left:
    return KeyCommand::cursorBack;

  case GDK_KEY_Right:
    return KeyCommand::cursorForward;

  case GDK_KEY_Up:
    return KeyCommand::cursorLineUp;

  case GDK_KEY_KP_End:
    return KeyCommand::cursorLineEnd;

  case GDK_KEY_KP_Home:
    return KeyCommand::cursorLineStart;

  case GDK_KEY_KP_Page_Down:
    return KeyCommand::cursorPageDown;

  case GDK_KEY_KP_Page_Up:
    return KeyCommand::cursorPageUp;
  }
  return KeyCommand::none;
}
