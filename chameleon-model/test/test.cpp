#include "PageTest.h"
#include "StringUtilsTest.h"

int main (int argc, char** args, char** env) {
  StringUtilsTest::create ()->run ();
  PageTest::create ()->run ();
  return 0;
}
