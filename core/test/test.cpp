#include <cstdio>
#include <fstream>
#include <natpad/textmodel/TextModel.h>
#include <natpad/util/StringUtils.h>

static void testGetLines (void);
static void testTextModel (void);

int main (int argc, char** args, char** env) {
  testTextModel ();
  return 0;
}

static void testGetLines (void) {
  std::ifstream stream ("../src/natpad/util/StringUtils.cpp");
  vector<shared_ptr<string>> lines = StringUtils::getLines (stream);
  for (auto line : lines) {
    printf ("%s\n", line->c_str ());
  }
}

static void printLines (shared_ptr<const TextModel> textModel) {
  int lc = textModel->lineCount ();
  printf ("----- TextModel, lines: %d -----\n", lc);
  for (int i = 0; i < lc; ++i) {
    printf ("%s\n", textModel->lineAt (i)->c_str ());
  }
  printf ("----------\n");
}

static void testTextModel (void) {
  shared_ptr<const TextModel> textModel (new TextModel ());
  Cursor cursor;
  printLines (textModel);
  textModel = textModel->insert (cursor, "allo");
  printLines (textModel);
  textModel = textModel->insert (cursor, "H");
  cursor.column = 5;
  textModel = textModel->insert (cursor, " wereld!");
  cursor.line = 1;
  cursor.column = 0;
  textModel = textModel->insert (cursor, "Dit is een test.");
  printLines (textModel);
}
