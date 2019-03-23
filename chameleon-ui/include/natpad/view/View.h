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

class View : public Glib::Object {
public:
	View (void);

	void setAdjustments(Glib::RefPtr<Gtk::Adjustment> vertical_adjustment);

	void setLayoutHeight(long long int height);

//protected:
//  bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr) override;
//
//  void on_realize() override;
////  void on_unrealize() override;
//
//
//
private:

	long long int layout_height;

	Glib::RefPtr<Gtk::Adjustment> vertical_adjustment;


//  void drawRect (const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
//  void drawText (const Cairo::RefPtr<Cairo::Context>& cr, int rectWidth, int rectHeight);
};

#endif /* NATPAD_VIEW_VIEW_H_ */
