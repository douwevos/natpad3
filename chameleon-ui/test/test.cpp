#include <cstdio>
#include <gtkmm.h>

int main (int argc, char** args, char** env) {
  auto app = Gtk::Application::create (argc, args, "org.gtkmm.examples.base");

  Gtk::Window window;
  window.set_default_size (200, 200);

  return app->run (window);
}
