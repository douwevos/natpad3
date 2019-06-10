/*
 * View.h
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#ifndef NATPAD_VIEW_VIEW_H_
#define NATPAD_VIEW_VIEW_H_

#include <cstdint>
#include <glibmm-2.4/glibmm.h>
#include <gtkmm.h>
#include <natpad/textmodel/DocumentListener.h>
#include <natpad/textmodel/TextModel.h>
#include <natpad/view/Colour.h>
#include <natpad/view/LineImage.h>
#include <natpad/util/UniqueArray.h>

class Editor;

class View : public DocumentListener {
public:
  View (Editor& owningEditor, int fontSize);

  void setVerticalAdjustment (Glib::RefPtr<Gtk::Adjustment> vertical_adjustment);

  void setLayoutHeight (int64_t height);

  int64_t set_view_y (int64_t y_pos);

  void setHeight (int64_t height);

  void draw (const Cairo::RefPtr<Cairo::Context>& cr);

  void onNewTextModel (shared_ptr<const TextModel> textModel) override;

private:
  int64_t m_view_y;
  int64_t m_viewHeight;
  int64_t m_layout_height;

  Glib::RefPtr<Gtk::Adjustment> m_vertical_adjustment;
  shared_ptr<const TextModel> m_textmodel;
  Editor& m_editor;
  Cairo::RefPtr<Cairo::ScaledFont> m_font;
  UniqueArray<LineImage> m_lineImages;
  Cursor m_cursor;

  int m_fontSize;
  int m_charWidth;

  void drawCursor (const Cairo::RefPtr<Cairo::Context>& cr);
  int findIndexOfLineImage (const string& text);
  void initLineImage (LineImage& lineImage,
      shared_ptr<const string> line,
      const Colour& textColour,
      int lineIndex);
  void invalidateLines (void);
};

#endif /* NATPAD_VIEW_VIEW_H_ */
