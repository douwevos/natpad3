#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <natpad/Editor.h>

int main (int argc, char** args, char** env) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create (argc, args);

  Gtk::Window window;
  //window.set_default_size (1024, 768);
  window.set_default_size (800, 600);
  window.set_title ("Tekst tekenen");


  Gtk::ScrolledWindow scrolled_window;
  scrolled_window.set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
  window.add(scrolled_window);
  Gtk::Allocation alloc;
  alloc.set_x(0);
  alloc.set_y(0);
  alloc.set_width(5000);
  alloc.set_height(5000);
  scrolled_window.size_allocate(alloc);

  Editor editor;
  scrolled_window.add(editor);

  window.show_all();

  Gtk::Scrollbar *scrollbar = scrolled_window.get_vscrollbar();
  scrollbar = scrolled_window.get_hscrollbar();
  scrollbar->get_adjustment()->set_upper(8000);
  scrollbar->get_adjustment()->set_page_size(100);

  return app->run (window);
}
