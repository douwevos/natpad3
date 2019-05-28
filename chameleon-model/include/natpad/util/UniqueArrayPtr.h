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

#ifndef __NATPAD_UTIL_UNIQUE_ARRAY_PTR_INCLUDED
#define __NATPAD_UTIL_UNIQUE_ARRAY_PTR_INCLUDED

template <typename T>
class UniqueArrayPtr {
private:
  T* m_array;

public:
  UniqueArrayPtr (void) : m_array (nullptr) {
  }

  UniqueArrayPtr (const UniqueArrayPtr&) = delete;
  UniqueArrayPtr& operator= (const UniqueArrayPtr&) = delete;

  UniqueArrayPtr (UniqueArrayPtr&& other) : m_array (other.m_array) {
    other.m_array = nullptr;
  }

  UniqueArrayPtr& operator= (UniqueArrayPtr&& other) {
    if (&other == this) {
      return *this;
    }
    if (m_array != nullptr) {
      delete[] m_array;
    }
    m_array = other.m_array;
    other.m_array = nullptr;
    return *this;
  }

  explicit UniqueArrayPtr (T* array) : m_array (array) {
  }

  virtual ~UniqueArrayPtr (void) {
    if (m_array != nullptr) {
      delete[] m_array;
    }
  }

  operator bool () const {
    return m_array != nullptr;
  }

  T& operator[] (int index) const {
    return m_array[index];
  }

  T& operator* (void) const {
    return m_array[0];
  }
};

#endif
