/*
 * ChameleonEditor.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <cstdio>
#include <natpad/ChameleonEditor.h>
#include <glibmm.h>
#include <glibmm/objectbase.h>

ChameleonEditorBase::ChameleonEditorBase()
{

}


ChameleonEditor::ChameleonEditor (void) :
		Gtk::Scrollable(), Glib::ObjectBase(typeid(ChameleonEditorBase)),
	      hadjustment_(*this, "hadjustment"),
	      vadjustment_(*this, "vadjustment"),
	      hscroll_policy_(*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
	      vscroll_policy_(*this, "vscroll-policy", Gtk::SCROLL_NATURAL)

	// register Gtk::Scrollbar properties with the object and give them names
//	Glib::Property(*this, "hadjustment"),
//	Glib::Property(*this, "vadjustment"),
//	Glib::Property(*this, "hscroll-policy"),
//	Glib::Property(*this, "vscroll-policy")
{
  /*
  Gtk::Allocation alloc = get_allocation ();
  int width = alloc.get_width ();
  int height = alloc.get_height ();
  printf ("Tekstoppervlak::Tekstoppervlak: w=%d, h=%d\n", width, height);
  */

//	property_hadjustment().signal_changed().connect(sigc::mem_fun(*this, &my_widget::adjustment_changed<false>));

	hadjustment_.

	view = shared_ptr<View>(new View());

	set_app_paintable(TRUE);
	set_can_focus(TRUE);
	show();
}

shared_ptr<View> ChameleonEditor::getView() {
	return view;
}


bool ChameleonEditor::on_draw (const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation alloc = get_allocation ();
  int width = alloc.get_width ();
  int height = alloc.get_height ();
  printf ("ChameleonEditor::on_draw: w=%d, h=%d\n", width, height);


  cr->set_source_rgb (1.0, 0.0, 0.0);
  cr->move_to(0, 0);
  cr->line_to(1000, 3000);
  cr->stroke();


//  drawRect (cr, width, height);
//  drawText (cr, width, height);

  return true;
}
//
//void Tekstoppervlak::drawRect (const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
//  cr->set_source_rgb (0.0, 0.0, 0.0);
//  cr->rectangle (0, 0, width, height);
//  cr->fill ();
//}
//
//void Tekstoppervlak::drawText (const Cairo::RefPtr<Cairo::Context>& cr, int rectWidth, int rectHeight) {
//  Pango::FontDescription font;
//  //font.set_family ("Monospace");
//  font.set_family ("Courier New");
//  font.set_weight (Pango::WEIGHT_NORMAL);
//  font.set_size (20000);
//  printf ("font size: %d\n", font.get_size ());
//
//  Glib::RefPtr<Pango::Layout> layout = create_pango_layout ("Hallo wereld.");
//  layout->set_font_description (font);
//
//  int w, h;
//  layout->get_pixel_size (w, h);
//  printf ("Tekstgrootte: %d x %d\n", w, h);
//
//
//  cr->set_source_rgb (1.0, 0.8, 0.0);
//  cr->move_to ((rectWidth - w) / 2, (rectHeight - h) / 2);
//  layout->show_in_cairo_context (cr);
//}


void ChameleonEditor::on_realize() {

	set_realized(TRUE);

	Gtk::Allocation allocation = get_allocation();

	// @Gerard doe ik dat goed hier ?
	Glib::RefPtr<Gdk::Visual> visual = get_visual();
	enum Gdk::EventMask events = get_events();

	GdkWindowAttr attributes;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.get_x();
	attributes.y = allocation.get_y();
	attributes.width = allocation.get_width();
	attributes.height = allocation.get_height();
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = visual.get()->gobj();
	attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
			| GDK_EXPOSURE_MASK
			| events;

	gint attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;


	Glib::RefPtr<Gdk::Window> window = Gdk::Window::create(get_parent_window(), &attributes, attributes_mask);
	set_window(window);
	register_window(window);

// for later user
//
//	priv->pango_context = gtk_widget_create_pango_context(widget);
//
//	if (priv->document_view==NULL) {
//		ChaEditorClass *editor_class = CHA_EDITOR_GET_CLASS(widget);
//		if (editor_class->initializeDocumentView) {
//			priv->document_view = editor_class->initializeDocumentView((ChaEditor *) widget, priv->document, priv->pango_context, widget);
//		} else {
//			priv->document_view = cha_document_view_new(priv->document, priv->pango_context, widget);
//		}
//		if (priv->a_preferences) {
//			cha_document_view_set_preferences(priv->document_view, priv->a_preferences);
//		}
//		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, priv->vadjustment);
//	}
	view->setAdjustments(vertical_adjustment);
//
//	if (priv->cursor==NULL) {
//		priv->cursor = gdk_cursor_new_for_display(gdk_window_get_display(window), GDK_XTERM);
//	}
//
//	gdk_window_set_cursor(gtk_widget_get_window(widget), priv->cursor);
//
//
//	gtk_im_context_set_client_window(priv->im_context, window);
}

// for later use
//void ChameleonEditor::on_unrealize() {
//	gtk_im_context_set_client_window(priv->im_context, NULL);
//	super->unrealize (widget);
//}
//
//void ChameleonEditor::set_property_value(const Glib::ustring& property_name, const Glib::ValueBase& value) {
//
//	switch(property_name) {
//		case "hadjustment":
//			void *val = value;
//			hadjustment = val;
//			break;
////		case PROP_VADJUSTMENT:
////			g_value_set_object(value, priv->vadjustment);
////			break;
////		case PROP_HSCROLL_POLICY:
////			g_value_set_enum(value, priv->hscroll_policy);
////			break;
////		case PROP_VSCROLL_POLICY:
////			g_value_set_enum(value, priv->vscroll_policy);
////			break;
////		default:
////			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
////			break;
//	}
//
//}


void ChameleonEditor::set_property_value(const Glib::ustring& property_name, const Glib::ValueBase& value) {

}



void ChameleonEditor::l_set_vadjustment() {

	printf("ChameleonEditor::l_set_vadjustment:\n");

	Glib::RefPtr<Gtk::Adjustment> adjustment = Glib::RefPtr<Gtk::Adjustment>(vadjustment_.get_value().get());
	if (adjustment.get()!=NULL) {

	}
	if (adjustment.get() && vertical_adjustment.get()->gobj() == adjustment->gobj()) {
		printf("ChameleonEditor::l_set_vadjustment:exit:197\n");
	    return;
	}

	if (!s_vertical_adjustment.empty()) {
		s_vertical_adjustment.disconnect();
	}

	if (vertical_adjustment.get() != NULL) {
////		g_signal_handlers_disconnect_by_func(vertical_adjustment, l_vadjustment_value_changed, editor);
////		g_object_unref(vertical_adjustment);
	}
////
//
	if (adjustment.get() == NULL) {
		printf("ChameleonEditor::creating at 212\n");
	    adjustment = Gtk::Adjustment::create(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}
//
//
	vertical_adjustment = adjustment;


	s_vertical_adjustment = vertical_adjustment->signal_changed().connect(sigc::mem_fun(*this, &ChameleonEditor::l_set_vadjustment));

	if (view.get()) {
		printf("ChameleonEditor::calling view->setAdj223\n");
		view->setAdjustments(vertical_adjustment);
	}

////
////	g_signal_connect(adjustment, "value-changed", G_CALLBACK(l_vadjustment_value_changed), editor);
////	vertical_adjustment = cat_ref_sink_ptr(adjustment);
////	if (priv->document_view) {
////		cha_document_view_set_adjustments(priv->document_view, priv->hadjustment, vertical_adjustment);
////	}
////	l_set_vadjustment_values(editor);
////	g_object_notify(G_OBJECT(editor), "vadjustment");
}
