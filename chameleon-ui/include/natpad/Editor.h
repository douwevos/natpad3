/*
 * Editor.h
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#ifndef NATPAD_EDITOR_H_
#define NATPAD_EDITOR_H_

#include <gtkmm/widget.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/adjustment.h>
#include <glibmm.h>
#include <natpad/textmodel/TextDocument.h>
#include <natpad/view/View.h>

using std::shared_ptr;

class Editor : public Gtk::Scrollable, public Gtk::Widget {
public:
  Editor (void);
  Editor (const Editor&) = delete;
  Editor& operator= (const Editor&) = delete;
  Editor (Editor&&) = delete;
  Editor& operator= (Editor&&) = delete;

  shared_ptr<TextDocument> getTextDocument (void);

  void on_property_value_hadjustment ();
  void on_property_value_vadjustment ();

protected:

  Glib::Property<Glib::RefPtr<Gtk::Adjustment>> m_hadjustment, m_vadjustment;
  Glib::Property<Gtk::ScrollablePolicy> m_hscroll_policy, m_vscroll_policy;

  sigc::connection m_co;

  bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;

  void on_size_allocate (Gtk::Allocation& allocation) override;

  void on_realize () override;
  //  void on_unrealize() override;

private:
  shared_ptr<View> m_view;
  shared_ptr<TextDocument> m_textDocument;

  Glib::RefPtr<Gtk::Adjustment> m_vertical_adjustment;
  sigc::connection m_s_vertical_adjustment;

  void l_set_vadjustment ();
};

#endif /* NATPAD_EDITOR_H_ */
