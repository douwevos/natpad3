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

#ifndef __TESTUTILS_TEST_CASE_INCLUDED
#define __TESTUTILS_TEST_CASE_INCLUDED

#include <string>

using std::string;

class TestCase {
private:
  string m_testClassName;
  string m_testName;

protected:
  explicit TestCase (const char* testClassName);

public:
  using Test = void (TestCase::*) (void);

  virtual ~TestCase (void);

  void run (void);

protected:
  virtual int getTestCount (void) = 0;
  virtual const Test* getTests (void) = 0;

  void setTestName (const char* name);

  void assertEquals (int lineNo, int expected, int actual);
  void assertEquals (int lineNo, const string& expected, const string& actual);
  void assertEquals (int lineNo, const void* expected, const void* actual);
  void assertTrue (int lineNo, bool condition);
};

#define assertEquals(expected, actual) assertEquals (__LINE__, expected, actual)
#define assertTrue(condition) assertTrue (__LINE__, condition)

#endif
