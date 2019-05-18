#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <natpad/Editor.h>
#include <natpad/textmodel/TextModel.h>

static shared_ptr<const TextModel> createTextModel (void);

int main (int argc, char** args, char** env) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create (argc, args);

  Gtk::Window window;
  window.set_default_size (1024, 768);
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
  editor.getView ()->setTextModel (createTextModel ());

  window.show_all();

  editor.getView()->setLayoutHeight(3000);

  Gtk::Scrollbar *scrollbar = scrolled_window.get_vscrollbar();
  scrollbar = scrolled_window.get_hscrollbar();
  scrollbar->get_adjustment()->set_upper(8000);
  scrollbar->get_adjustment()->set_page_size(100);

  return app->run (window);
}

static shared_ptr<const TextModel> createTextModel (void) {
  TextModel emptyModel;
  return emptyModel.insert (Cursor (), "Uit 'de vrol\304\263ke wetenschap' van Friedrich Nietzsche:\n\n1. Einladung.\n\nWagt's mit meiner Ko\305\277t, ihr E\305\277\305\277er!\nMorgen \305\277chmeckt \305\277ie euch \305\277chon be\305\277\305\277er\nUnd \305\277chon \303\274bermorgen gut!\nWollt ihr dann noch mehr,- \305\277o machen\nMeine alten \305\277ieben Sachen\nMir zu \305\277ieben neuen Muth.\n\n2. Mein Gl\303\274ck.\n\nSeit ich des Suchens m\303\274de ward,\nErlernte ich das Finden.\nSeit mir ein Wind hielt Widerpart,\nSegl' ich mit allen Winden.\n\n3. Unverzagt.\n\nWo du \305\277teh\305\277t, grab tief hinein!\nDrunten i\305\277t die Quelle!\nLa\303\237 die dunklen M\303\244nner schrein:\n\342\200\236Stets i\305\277t drunten - H\303\266lle!\342\200\234\n\n7. Vademecum - Vadetecum\n\nEs lockt dich meine Art und Sprach',\nDu folge\305\277t mir, du geh\305\277t mir nach?\nGeh nur dir \305\277elber treulich nach:-\nSo folg\305\277t du mir - gemach! gemach!\n\n\316\221\316\222\316\223\316\224\n\320\220\320\221\320\222\320\223\320\224\n1\n12\n123\n1234\n1243\nabcd\nABCD\n");
}
