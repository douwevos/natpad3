#include "PageTest.h"
#include "StringReaderTest.h"
#include "StringUtilsTest.h"
#include "TextDocumentTest.h"
#include "TextModelTest.h"
#include "Utf8FileReaderTest.h"

int main (int argc, char** args, char** env) {
  StringUtilsTest::create ()->run ();
  Utf8FileReaderTest::create ()->run ();
  StringReaderTest::create ()->run ();
  PageTest::create ()->run ();
  TextModelTest::create ()->run ();
  TextDocumentTest::create ()->run ();
  return 0;
}
