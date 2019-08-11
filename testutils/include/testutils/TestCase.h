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

#include "../../../chameleon-model/include/natpad/util/string.h"

class TestCase {
private:
  std::string m_testClassName;
  std::string m_testName;

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
  void assertEquals (int lineNo, const std::string& expected, const std::string& actual);
  void assertEquals (int lineNo, const std::string& expected, const String& actual);
  void assertEquals (int lineNo, const String& expected, const String& actual);
  void assertEquals (int lineNo, const void* expected, const void* actual);
  void assertFalse (int lineNo, bool condition);
  void assertNotNull (int lineNo, const void* ptr);
  void assertNull (int lineNo, const void* ptr);
  void assertTrue (int lineNo, bool condition);
  void fail (int lineNo, const std::string& message);
};

#define assertEquals(expected, actual) assertEquals (__LINE__, expected, actual)
#define assertFalse(condition) assertFalse (__LINE__, condition)
#define assertNotNull(ptr) assertNotNull (__LINE__, ptr)
#define assertNull(ptr) assertNull (__LINE__, ptr)
#define assertTrue(condition) assertTrue (__LINE__, condition)
#define FAIL(message) fail (__LINE__, message)

#endif
