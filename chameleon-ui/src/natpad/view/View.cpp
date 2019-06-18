/*
 * View.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <utility>
#include <natpad/view/View.h>
#include <natpad/Editor.h>

#define WINDOW_Y_OFFSET 3

View::View (Editor& owningEditor, int fontSize) :
    m_editor (owningEditor), m_viewHeight (0), m_fontSize (fontSize) {
  m_view_y = 0;
  m_layout_height = 100;
  Cairo::Matrix fontMatrix (m_fontSize, 0, 0, m_fontSize, 0, 0);
  Cairo::Matrix ctmMatrix (1, 0, 0, 1, 0, 0);
  Cairo::RefPtr<Cairo::ToyFontFace> fontface = Cairo::ToyFontFace::create ("Courier New", Cairo::FONT_SLANT_NORMAL,
      Cairo::FONT_WEIGHT_NORMAL);
  m_font = Cairo::ScaledFont::create (fontface, fontMatrix, ctmMatrix);

  Cairo::TextExtents extents;
  m_font->text_extents ("A", extents);
  m_charWidth = extents.x_advance + 0.5;
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
    //printf ("View::draw: Drawing %d lines.\n", m_lineImages.length ());

    const double delta = m_fontSize;
    double y = m_view_y + WINDOW_Y_OFFSET;

    for (int i = 0; i < m_lineImages.length (); ++i) {
      cr->set_source (m_lineImages[i].surface (), 0, y);
      cr->rectangle (0, y, m_lineImages[i].width (), m_lineImages[i].height ());
      cr->fill ();
      y += delta;
    }
    drawCursor (cr);
  }
}

void View::drawCursor (const Cairo::RefPtr<Cairo::Context>& cr) {
  int lineImageCount = m_lineImages.length ();
  Glib::RefPtr<Gdk::Window> window = m_editor.get_window ();
  if (window && lineImageCount > 0) {
    int index = m_cursor.line - m_lineImages[0].lineIndex ();
    if (index >= 0 && index < lineImageCount) {
      int64_t viewY = m_view_y + WINDOW_Y_OFFSET + m_fontSize * index;
      int64_t viewX = m_cursor.column * m_charWidth;

      Cairo::RefPtr<Cairo::Surface> tempSurf = window->create_similar_surface (Cairo::CONTENT_COLOR_ALPHA, m_charWidth, m_fontSize);
      Cairo::RefPtr<Cairo::Context> tempCtxt = Cairo::Context::create (tempSurf);
      tempCtxt->set_source (m_lineImages[index].surface (), -viewX, 0);
      tempCtxt->rectangle (0, 0, m_charWidth, m_fontSize);
      tempCtxt->fill ();

      cr->set_source_rgb (1.0, 1.0, 1.0);
      cr->rectangle (viewX, viewY, m_charWidth, m_fontSize);
      cr->fill ();
      cr->set_source_rgb (0.0, 0.0, 0.0);
      cr->mask (tempSurf, viewX, viewY);
      cr->fill ();
    }
  }
}

void View::invalidateLines (void) {
  int firstLineIndex = m_view_y / m_fontSize;
  int lastLineIndex = (m_view_y + m_viewHeight) / m_fontSize;
  if ((m_view_y + m_viewHeight) % m_fontSize > 0) {
    ++lastLineIndex;
  }

  if (!m_textmodel || firstLineIndex >= m_textmodel->lineCount ()) {
    UniqueArray<LineImage> lineImages;
    m_lineImages = std::move (lineImages);
    return;
  }

  if (lastLineIndex > m_textmodel->lineCount ()) {
    lastLineIndex = m_textmodel->lineCount ();
  }

  Colour textColour (1.0, 1.0, 1.0);
  const int lineCount = lastLineIndex - firstLineIndex;
  UniqueArray<LineImage> lineImages (lineCount);
  for (int i = 0; i < lineCount; ++i) {
    int lineIndex = i + firstLineIndex;
    initLineImage (lineImages[i], m_textmodel->lineAt (lineIndex), textColour, lineIndex);
    m_editor.queue_draw_area (0, lineIndex * m_fontSize + WINDOW_Y_OFFSET, lineImages[i].width (), lineImages[i].height ());
  }
  m_lineImages = std::move (lineImages);
}

int View::findIndexOfLineImage (const std::string& text) {
  for (int i = 0; i < m_lineImages.length (); ++i) {
    if (m_lineImages[i].text () == text) {
      return i;
    }
  }
  return -1;
}

void View::initLineImage (LineImage& lineImage,
    shared_ptr<const String> line,
    const Colour& textColour,
    int lineIndex) {
  StringConvert convert;
  std::string utf8Line = convert.to_bytes (*line);

  int oldImageIndex = findIndexOfLineImage (utf8Line);
  if (oldImageIndex > -1) {
    //printf ("Reusing old image.\n");
    lineImage = m_lineImages[oldImageIndex];
    lineImage.setLineIndex (lineIndex);
    return;
  }
  //printf ("Creating new image.\n");

  Cairo::TextExtents extent;
  m_font->text_extents (utf8Line, extent);
  const double width = extent.x_advance;
  const double height = m_fontSize;

  Glib::RefPtr<Gdk::Window> window = m_editor.get_window ();
  Cairo::RefPtr<Cairo::Surface> surface;
  if (window) {
    surface = window->create_similar_surface (Cairo::CONTENT_COLOR_ALPHA, width, height);
  } else {
    surface = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32, width, height);
  }

  Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create (surface);
  context->set_scaled_font (m_font);
  context->set_source_rgb (textColour.red (), textColour.green (), textColour.blue ());
  context->move_to (0, 0.79 * height);
  context->show_text (utf8Line);

  lineImage.set (surface, width, height, utf8Line, lineIndex);
}

void View::setHeight (int64_t height) {
  m_viewHeight = height;
  invalidateLines ();
}

void View::onNewTextModel (shared_ptr<const TextModel> textmodel) {
  m_textmodel = textmodel;
  m_cursor = textmodel->cursor ();
  invalidateLines ();
}
