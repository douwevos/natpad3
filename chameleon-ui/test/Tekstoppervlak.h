/* Zie:
   https://developer.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en */

#ifndef TEKSTOPPERVLAK_INCLUDED
#define TEKSTOPPERVLAK_INCLUDED

#include <gtkmm/drawingarea.h>

class Tekstoppervlak : public Gtk::DrawingArea {
public:
  Tekstoppervlak (void);

protected:
  bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  void drawRect (const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
  void drawText (const Cairo::RefPtr<Cairo::Context>& cr, int rectWidth, int rectHeight);
};

#endif
