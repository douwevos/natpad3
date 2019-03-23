#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <natpad/ChameleonEditor.h>

int main (int argc, char** args, char** env) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create (argc, args);

  Gtk::Window window;
  window.set_default_size (640, 480);
  window.set_title ("Tekst tekenen");


  Gtk::ScrolledWindow scrolled_window;
  window.add(scrolled_window);


  ChameleonEditor editor;
  scrolled_window.add(editor);

  window.show_all();


  editor.getView()->setLayoutHeight(3000);

  return app->run (window);
}
