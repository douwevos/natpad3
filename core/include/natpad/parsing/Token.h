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

#ifndef __NPPP_TOKEN_INCLUDED
#define __NPPP_TOKEN_INCLUDED

#include <string>

class Token {
private:
  std::string m_text;
  int m_bcol;
  int m_bline;
  int m_ecol;
  int m_eline;
  int m_code;

public:
  Token (const std::string& text, int beginLine, int beginColumn, int endLine, int endColumn);
  Token (const std::string& text, int beginLine, int beginColumn);
  Token (const Token&) = delete;
  Token (Token&&) = delete;
  virtual ~Token (void);

  Token& operator= (const Token&) = delete;
  Token& operator= (Token&&) = delete;

  int beginColumn (void) const;
  int beginLine (void) const;
  int code (void) const;
  int endColumn (void) const;
  int endLine (void) const;
  const std::string& text (void) const;
};

#endif
