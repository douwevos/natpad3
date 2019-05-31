/*
 * View.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <utility>
#include <natpad/view/View.h>
#include <natpad/Editor.h>

View::View (Editor& owningEditor) : m_editor (owningEditor), m_viewHeight (0) {
  m_view_y = 0;
  m_layout_height = 100;
  Cairo::Matrix fontMatrix (m_fontSize, 0, 0, m_fontSize, 0, 0);
  Cairo::Matrix ctmMatrix (1, 0, 0, 1, 0, 0);
  Cairo::RefPtr<Cairo::ToyFontFace> fontface = Cairo::ToyFontFace::create ("Courier New", Cairo::FONT_SLANT_NORMAL,
      Cairo::FONT_WEIGHT_NORMAL);
  m_font = Cairo::ScaledFont::create (fontface, fontMatrix, ctmMatrix);
}

int64_t View::set_view_y (int64_t y_pos) {
  int64_t result = m_view_y;
  m_view_y = y_pos;
  invalidateLines ();
  return result;
}

void View::setVerticalAdjustment (Glib::RefPtr<Gtk::Adjustment> vertical_adjustment) {
  this->m_vertical_adjustment = vertical_adjustment;
}

/* @Douwe: Waar is deze voor?  */
void View::setLayoutHeight (int64_t height) {
  printf ("View::setLayoutHeight: w=%ld\n", height);

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
  cr->rectangle (0, m_view_y, 1024, m_viewHeight);
  cr->fill ();

  if (m_textmodel) {
    printf ("View::draw: Drawing %d lines.\n", m_lineImages.length ());

    const double delta = m_fontSize;
    double y = m_view_y + 3;

    for (int i = 0; i < m_lineImages.length (); ++i) {
      cr->set_source (m_lineImages[i].surface (), 0, y);
      cr->rectangle (0, y, m_lineImages[i].width (), m_lineImages[i].height ());
      cr->fill ();
      y += delta;
    }
  }
}

void View::invalidateLines (void) {
  int firstLineIndex = m_view_y / m_fontSize;
  int lastLineIndex = (m_view_y + m_viewHeight) / m_fontSize;
  if ((m_view_y + m_viewHeight) % m_fontSize > 0) {
    ++lastLineIndex;
  }

  if (firstLineIndex >= m_textmodel->lineCount ()) {
    UniqueArray<LineImage> lineImages;
    m_lineImages = std::move (lineImages);
    return;
  }

  if (lastLineIndex > m_textmodel->lineCount ()) {
    lastLineIndex = m_textmodel->lineCount ();
  }

  Colour textColour (0.7, 0.7, 0.7);
  const int lineCount = lastLineIndex - firstLineIndex;
  UniqueArray<LineImage> lineImages (lineCount);
  for (int i = 0; i < lineCount; ++i) {
    int lineIndex = i + firstLineIndex;
    initLineImage (lineImages[i], m_textmodel->lineAt (lineIndex), textColour);
    //m_editor.queue_draw_area (0, lineIndex * m_fontSize, lineImages[i].width (), lineImages[i].height ());
  }
  m_lineImages = std::move (lineImages);
}

void View::initLineImage (LineImage& lineImage,
    shared_ptr<const string> line,
    const Colour& textColour) {
  Cairo::TextExtents extent;
  m_font->text_extents (*line, extent);
  const double width = extent.x_advance;
  const double height = m_fontSize;

  Glib::RefPtr<Gdk::Window> window = m_editor.get_window ();
  Cairo::RefPtr<Cairo::Surface> surface;
  if (window) {
    surface = window->create_similar_surface (Cairo::CONTENT_COLOR, width, height);
  } else {
    surface = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32, width, height);
  }

  Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create (surface);
  context->set_scaled_font (m_font);
  context->set_source_rgb (textColour.red (), textColour.green (), textColour.blue ());
  context->move_to (0, height - 6);
  context->show_text (*line);

  lineImage.set (surface, width, height);
}

void View::setHeight (int64_t height) {
  m_viewHeight = height;
  invalidateLines ();
}

void View::setTextModel (shared_ptr<const TextModel> textmodel) {
  m_textmodel = textmodel;
  invalidateLines ();
}
