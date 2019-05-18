/*
 * View.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <natpad/view/View.h>

using std::unique_ptr;

View::View (void) {
  m_view_y = 0;
  m_layout_height = 100;
}

long long View::set_view_y (long long y_pos) {
  long long result = m_view_y;
  this->m_view_y = y_pos;
  return result;
}

void View::setVerticalAdjustment (Glib::RefPtr<Gtk::Adjustment> vertical_adjustment) {
  printf ("setAdjustments:%p\n", vertical_adjustment);
  this->m_vertical_adjustment = vertical_adjustment;
}

void View::setLayoutHeight (long long int height) {
  printf ("View::setLayoutHeight: w=%lld\n", height);

  this->m_layout_height = height;
  if (m_vertical_adjustment) {
    gdouble new_upper = (gdouble) m_layout_height;
    gdouble old_value = m_vertical_adjustment->get_value ();
    int view_height = m_vertical_adjustment->get_page_size ();
    new_upper = MAX(m_layout_height, view_height);

//		cat_log_debug("new-upper=%d, priv->vadjustment=%p, view_height=%d, layout_height=%d", (int) new_upper, priv->vadjustment, view_height, layout_height);
    m_vertical_adjustment->set_upper (new_upper);
    printf ("View::new_upper: w=%f\n", new_upper);

    gdouble new_value = CLAMP(old_value, 0, new_upper - view_height);
    if (new_value != old_value) {
      m_vertical_adjustment->set_value (new_value);
    }
  }
}

//void cha_document_view_set_adjustments(ChaDocumentView *document_view, GtkAdjustment *hadjustment, GtkAdjustment *vadjustment) {
//	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
//	priv->hadjustment = hadjustment;
//	priv->vadjustment = vadjustment;
//
//}

void View::draw (const Cairo::RefPtr<Cairo::Context>& cr) {
  cr->translate (0, -m_view_y); /* @Douwe: Deze regel lijkt nodig voor het scrollen... Waarom is dat? */

  cr->set_source_rgb (0.0, 0.0, 0.0);
  cr->rectangle (0, 0, 1024, 5000);
  cr->fill ();


  if (m_textmodel) {
    setFontAndColour (cr);

    const int lineCount = m_textmodel->lineCount ();
    unique_ptr<Cairo::RefPtr<Cairo::ImageSurface>[]> surfaces (new Cairo::RefPtr<Cairo::ImageSurface>[lineCount]);

    for (int i = 0; i < lineCount; ++i) {
      Cairo::TextExtents extent;
      shared_ptr<const string> line = m_textmodel->lineAt (i);
      cr->get_text_extents (*line, extent);
      surfaces[i] = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32, extent.width + 1, m_fontSize);
      Cairo::RefPtr<Cairo::Context> ctxt = Cairo::Context::create (surfaces[i]);
      if (i & 1) {
        ctxt->set_source_rgb (0.5, 0.0, 0.0);
        ctxt->rectangle (0, 0, surfaces[i]->get_width (), surfaces[i]->get_height ());
        ctxt->fill ();
      }
      setFontAndColour (ctxt);
      ctxt->move_to (0, m_fontSize - 6);
      ctxt->show_text (*line);
    }

    const double delta = m_fontSize;
    double y = 3;

    for (int i = 0; i < lineCount; ++i) {
      cr->set_source (surfaces[i], 0, y);
      cr->rectangle (0, y, surfaces[i]->get_width (), m_fontSize);
      cr->fill ();
      y += delta;
    }
  }
}

void View::setFontAndColour (const Cairo::RefPtr<Cairo::Context>& cr) {
  cr->set_source_rgb (0.7, 0.7, 0.7);
  cr->set_font_size (m_fontSize);
  cr->select_font_face ("Courier New", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
}

void View::setTextModel (shared_ptr<const TextModel> textmodel) {
  m_textmodel = textmodel;
}
