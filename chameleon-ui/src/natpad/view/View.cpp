/*
 * View.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <natpad/view/View.h>

View::View (void) {
	layout_height = 100;
}


void View::setAdjustments(Glib::RefPtr<Gtk::Adjustment> vertical_adjustment) {
	printf("setAdjustments:%p\n", vertical_adjustment.get());
	this->vertical_adjustment = vertical_adjustment;
}
void View::setLayoutHeight(long long int height) {
	  printf ("View::setLayoutHeight: w=%lld\n", height);

	this->layout_height = height;
	if (vertical_adjustment.get()) {
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
