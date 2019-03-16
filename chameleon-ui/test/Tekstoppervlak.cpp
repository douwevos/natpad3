#include <cstdio>
#include "Tekstoppervlak.h"

Tekstoppervlak::Tekstoppervlak (void) {
  /*
  Gtk::Allocation alloc = get_allocation ();
  int width = alloc.get_width ();
  int height = alloc.get_height ();
  printf ("Tekstoppervlak::Tekstoppervlak: w=%d, h=%d\n", width, height);
  */
}

bool Tekstoppervlak::on_draw (const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation alloc = get_allocation ();
  int width = alloc.get_width ();
  int height = alloc.get_height ();
  printf ("Tekstoppervlak::on_draw: w=%d, h=%d\n", width, height);

  drawRect (cr, width, height);
  drawText (cr, width, height);

  return true;
}

void Tekstoppervlak::drawRect (const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
  cr->set_source_rgb (0.0, 0.0, 0.0);
  cr->rectangle (0, 0, width, height);
  cr->fill ();
}

void Tekstoppervlak::drawText (const Cairo::RefPtr<Cairo::Context>& cr, int rectWidth, int rectHeight) {
  Pango::FontDescription font;
  //font.set_family ("Monospace");
  font.set_family ("Courier New");
  font.set_weight (Pango::WEIGHT_NORMAL);
  font.set_size (20000);
  printf ("font size: %d\n", font.get_size ());

  Glib::RefPtr<Pango::Layout> layout = create_pango_layout ("Hallo wereld.");
  layout->set_font_description (font);

  int w, h;
  layout->get_pixel_size (w, h);
  printf ("Tekstgrootte: %d x %d\n", w, h);


  cr->set_source_rgb (1.0, 0.8, 0.0);
  cr->move_to ((rectWidth - w) / 2, (rectHeight - h) / 2);
  layout->show_in_cairo_context (cr);
}
