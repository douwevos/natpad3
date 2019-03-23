/*
 * ChameleonEditor.h
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#ifndef NATPAD_CHAMELEONEDITOR_H_
#define NATPAD_CHAMELEONEDITOR_H_

#include <memory>
#include <string>
#include <gtkmm/widget.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/adjustment.h>
#include <glibmm.h>
#include <natpad/view/View.h>

using std::shared_ptr;

class ChameleonEditorBase;
class ChameleonEditorBase : public Gtk::Widget {
  protected:
	ChameleonEditorBase();
  protected:
};


class ChameleonEditor : public Gtk::Scrollable, public ChameleonEditorBase {
public:
	ChameleonEditor (void);

	// TODO only for testing for now.
	shared_ptr<View> getView();

	void set_property_value(const Glib::ustring& property_name, const Glib::ValueBase& value) override;

protected:

    Glib::Property<Glib::RefPtr<Gtk::Adjustment>> hadjustment_, vadjustment_;
    Glib::Property<Gtk::ScrollablePolicy> hscroll_policy_, vscroll_policy_;


  bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;

  void on_realize() override;
//  void on_unrealize() override;




//
//private:
//  void drawRect (const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
//  void drawText (const Cairo::RefPtr<Cairo::Context>& cr, int rectWidth, int rectHeight);

private:
  shared_ptr<View> view;

  Glib::RefPtr<Gtk::Adjustment> vertical_adjustment;
  sigc::connection s_vertical_adjustment;

  void l_set_vadjustment();
};

#endif /* NATPAD_CHAMELEONEDITOR_H_ */
