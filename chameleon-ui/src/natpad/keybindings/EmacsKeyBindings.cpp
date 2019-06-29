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

EmacsKeyBindings::EmacsKeyBindings (void) : m_prefixKey (PrefixKey::none) {
}

KeyCommand EmacsKeyBindings::getAltCommand (int key, bool shift) {
  switch (key) {
  case GDK_KEY_greater:
    return KeyCommand::cursorTextEnd;

  case GDK_KEY_less:
    return KeyCommand::cursorTextStart;

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
    return KeyCommand::cursorLineStart;

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

  case GDK_KEY_KP_End:
    return KeyCommand::cursorTextEnd;

  case GDK_KEY_KP_Home:
    return KeyCommand::cursorTextStart;
  }
  return KeyCommand::none;
}

KeyCommand EmacsKeyBindings::getUnmodifiedCommand (int key, bool shift) {
  if (m_prefixKey == PrefixKey::none) {
    if (key == GDK_KEY_Escape) {
      m_prefixKey = PrefixKey::esc;
      return KeyCommand::none;
    }
    return KeyBindings::getUnmodifiedCommand (key, shift);
  }

  switch (m_prefixKey) {
  case PrefixKey::ctrl_x:
    break;

  case PrefixKey::ctrl_x_8:
    break;

  case PrefixKey::esc:
    m_prefixKey = PrefixKey::none;
    return getAltCommand (key, shift);

  case PrefixKey::meta_g:
    break;
  }
  return KeyCommand::none;
}
