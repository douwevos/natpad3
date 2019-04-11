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

	void setVerticalAdjustment(Glib::RefPtr<Gtk::Adjustment> vertical_adjustment);

	void setLayoutHeight(long long int height);

	long long set_view_y(long long y_pos);

	void draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:

	long long view_y;
	long long int layout_height;

	Glib::RefPtr<Gtk::Adjustment> vertical_adjustment;

};

#endif /* NATPAD_VIEW_VIEW_H_ */
