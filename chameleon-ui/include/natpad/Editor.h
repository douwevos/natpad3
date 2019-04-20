/*
 * Editor.h
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#ifndef NATPAD_EDITOR_H_
#define NATPAD_EDITOR_H_

#include <memory>
#include <string>
#include <gtkmm/widget.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/adjustment.h>
#include <glibmm.h>
#include <natpad/view/View.h>

using std::shared_ptr;

class Editor : public Gtk::Scrollable, public Gtk::Widget {
public:
	Editor (void);

	// TODO only for testing for now.
	shared_ptr<View> getView();

	void on_property_value_hadjustment();
	void on_property_value_vadjustment();

protected:

    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> hadjustment_, vadjustment_;
    Glib::Property<Gtk::ScrollablePolicy> hscroll_policy_, vscroll_policy_;

    sigc::connection co;

  bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;

  void on_realize() override;
//  void on_unrealize() override;



private:
  shared_ptr<View> view;

  Glib::RefPtr<Gtk::Adjustment> vertical_adjustment;
  sigc::connection s_vertical_adjustment;

  void l_set_vadjustment();
};

#endif /* NATPAD_EDITOR_H_ */
