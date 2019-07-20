#include "LayoutLineTest.h"
#include "LineIteratorTest.h"
#include "BasicPageTest.h"
#include "StringReaderTest.h"
#include "StringUtilsTest.h"
#include "TextDocumentTest.h"
#include "TextModelTest.h"
#include "Utf8FileReaderTest.h"

int main (int argc, char** args, char** env) {
  StringUtilsTest::create ()->run ();
  Utf8FileReaderTest::create ()->run ();
  StringReaderTest::create ()->run ();
  BasicPageTest::create ()->run ();
  TextModelTest::create ()->run ();
  TextDocumentTest::create ()->run ();
  LineIteratorTest::create ()->run ();
  LayoutLineTest::create ()-> run ();
  return 0;
}
