/*
 * View.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <natpad/view/View.h>

View::View (void) {
	view_y = 0;
	layout_height = 100;
}


long long View::set_view_y(long long y_pos) {
	long long result = view_y;
	this->view_y = y_pos;
	return result;
}


void View::setVerticalAdjustment(Glib::RefPtr<Gtk::Adjustment> vertical_adjustment) {
	printf("setAdjustments:%p\n", vertical_adjustment);
	this->vertical_adjustment = vertical_adjustment;
}
void View::setLayoutHeight(long long int height) {
	  printf ("View::setLayoutHeight: w=%lld\n", height);

	this->layout_height = height;
	if (vertical_adjustment) {
		gdouble new_upper = (gdouble) layout_height;
		gdouble old_value = vertical_adjustment->get_value();
		int view_height = vertical_adjustment->get_page_size();
		new_upper = MAX(layout_height, view_height);

//		cat_log_debug("new-upper=%d, priv->vadjustment=%p, view_height=%d, layout_height=%d", (int) new_upper, priv->vadjustment, view_height, layout_height);
		vertical_adjustment->set_upper(new_upper);
		  printf ("View::new_upper: w=%f\n", new_upper);


		gdouble new_value = CLAMP(old_value, 0, new_upper - view_height);
		if (new_value != old_value) {
			vertical_adjustment->set_value(new_value);
		}
	}
}


//void cha_document_view_set_adjustments(ChaDocumentView *document_view, GtkAdjustment *hadjustment, GtkAdjustment *vadjustment) {
//	ChaDocumentViewPrivate *priv = cha_document_view_get_instance_private(document_view);
//	priv->hadjustment = hadjustment;
//	priv->vadjustment = vadjustment;
//
//}


void View::draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	cr->translate(0, -view_y);

	cr->set_source_rgb (1.0, 0.0, 0.0);
	cr->move_to(0, 0);
	cr->line_to(1000, 3000);
	cr->stroke();

}
