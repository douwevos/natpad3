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

#include <cstdio>
#include <stdexcept>
#include <testutils/TestCase.h>

#undef assertEquals
#undef assertTrue

TestCase::TestCase (const char* testClassName) : m_testClassName (testClassName) {
}

TestCase::~TestCase (void) {
}

void TestCase::run (void) {
  const Test* tests = getTests ();
  const int testCount = getTestCount ();
  for (int i = 0; i < testCount; ++i) {
    try {
      (this->*tests[i]) ();
      printf ("\x1B[A\x1B[38;5;34m%s\x1B[0m\n", m_testName.c_str ());
    } catch (std::exception& x) {
      printf ("\x1B[A\x1B[38;5;160m%s: %s\x1B[0m\n", m_testName.c_str (), x.what ());
    }
    m_testName = "";
  }
}

void TestCase::assertEquals (int lineNo, int expected, int actual) {
  if (expected != actual) {
    string str = "assertion failed (line " + std::to_string (lineNo);
    str += "):\nexpected: " + std::to_string (expected);
    str += "\n  actual: " +  std::to_string (actual);
    throw std::runtime_error (str);
  }
}

void TestCase::assertEquals (int lineNo, const string& expected, const string& actual) {
  if (expected != actual) {
    string str = "assertion failed (line " + std::to_string (lineNo);
    str += "):\nexpected: " + expected;
    str += "\n  actual: " +  actual;
    throw std::runtime_error (str);
  }
}

void TestCase::assertEquals (int lineNo, const void* expected, const void* actual) {
  if (expected != actual) {
    string str = "assertion failed (line " + std::to_string (lineNo) + "): pointers are not equal";
    throw std::runtime_error (str);
  }
}

void TestCase::assertTrue (int lineNo, bool condition) {
  if (!condition) {
    string str = "assertion failed (line " + std::to_string (lineNo) + ")";
    throw std::runtime_error (str);
  }
}

void TestCase::setTestName (const char* name) {
  m_testName += m_testClassName;
  m_testName += "::";
  m_testName += name;
  printf ("%s\n", m_testName.c_str ());
}
