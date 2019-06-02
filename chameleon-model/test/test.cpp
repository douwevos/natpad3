#include "PageTest.h"
#include "StringInfoTest.h"
#include "StringUtilsTest.h"
#include "TextModelTest.h"

int main (int argc, char** args, char** env) {
  StringUtilsTest::create ()->run ();
  StringInfoTest::create ()->run ();
  PageTest::create ()->run ();
  TextModelTest::create ()->run ();
  return 0;
}
