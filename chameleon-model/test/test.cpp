#include "LineIteratorTest.h"
#include "BasicTextModelPageTest.h"
#include "StringReaderTest.h"
#include "StringUtilsTest.h"
#include "TextDocumentTest.h"
#include "TextModelTest.h"
#include "Utf8FileReaderTest.h"

int main (int argc, char** args, char** env) {
  StringUtilsTest::create ()->run ();
  Utf8FileReaderTest::create ()->run ();
  StringReaderTest::create ()->run ();
  BasicTextModelPageTest::create ()->run ();
  TextModelTest::create ()->run ();
  TextDocumentTest::create ()->run ();
  LineIteratorTest::create ()->run ();
  return 0;
}
