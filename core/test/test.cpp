#include <cstdio>
#include <fstream>
#include <natpad/util/StringUtils.h>

static void testGetLines (void);

int main (int argc, char** args, char** env) {
  testGetLines ();
  return 0;
}

static void testGetLines (void) {
  std::ifstream stream ("../src/natpad/util/StringUtils.cpp");
  vector<shared_ptr<string>> lines = StringUtils::getLines (stream);
  for (auto line : lines) {
    printf ("%s\n", line->c_str ());
  }
}
