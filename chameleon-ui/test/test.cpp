#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "Tekstoppervlak.h"

int main (int argc, char** args, char** env) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create (argc, args);

  Gtk::Window window;
  window.set_default_size (640, 480);
  window.set_title ("Tekst tekenen");

  Tekstoppervlak area;
  window.add (area);
  area.show ();

  return app->run (window);
}
