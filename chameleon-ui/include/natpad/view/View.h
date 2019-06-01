/*
 * View.h
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#ifndef NATPAD_VIEW_VIEW_H_
#define NATPAD_VIEW_VIEW_H_

#include <glibmm-2.4/glibmm.h>
#include <gtkmm.h>
#include <natpad/textmodel/TextModel.h>
#include <natpad/view/Colour.h>
#include <natpad/view/LineImage.h>

class Editor;

class View {
public:
  View (Editor& owningEditor);

  void setVerticalAdjustment (Glib::RefPtr<Gtk::Adjustment> vertical_adjustment);

  void setLayoutHeight (long long int height);

  long long set_view_y (long long y_pos);

  void draw (const Cairo::RefPtr<Cairo::Context>& cr);

  void setTextModel (shared_ptr<const TextModel> textmodel);

private:
  static constexpr double m_fontSize = 24;

  long long m_view_y;
  long long int m_layout_height;

  Glib::RefPtr<Gtk::Adjustment> m_vertical_adjustment;
  shared_ptr<const TextModel> m_textmodel;
  Editor& m_editor;

  void initLineImage (LineImage& lineImage,
      shared_ptr<const string> line,
      Cairo::RefPtr<Cairo::ScaledFont> font,
      const Colour& textColour);
  void setFont (const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif /* NATPAD_VIEW_VIEW_H_ */
