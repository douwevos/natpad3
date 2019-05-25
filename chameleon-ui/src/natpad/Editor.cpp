/*
 * Editor.cpp
 *
 *  Created on: Mar 17, 2019
 *      Author: dvos
 */

#include <cstdio>
#include <glibmm.h>
#include <glibmm/objectbase.h>
#include <natpad/Editor.h>
#include <natpad/textmodel/TextModel.h>

Editor::Editor (void) :
    Gtk::Scrollable (), Glib::ObjectBase (typeid(Gtk::Widget)),
        m_hadjustment (*this, "hadjustment"),
        m_vadjustment (*this, "vadjustment"),
        m_hscroll_policy (*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
        m_vscroll_policy (*this, "vscroll-policy", Gtk::SCROLL_NATURAL)
{
  property_vadjustment ().signal_changed ().connect (sigc::mem_fun (*this, &Editor::on_property_value_vadjustment));
  m_view = shared_ptr<View> (new View (*this));
  set_app_paintable ();
  set_can_focus ();
  show ();
}

shared_ptr<View> Editor::getView () {
  return m_view;
}

bool Editor::on_draw (const Cairo::RefPtr<Cairo::Context>& cr) {
  m_view->draw (cr);
  return true;
}

void Editor::on_realize () {

  set_realized ();

  Gtk::Allocation allocation = get_allocation ();

  // @Gerard doe ik dat goed hier ?
  Glib::RefPtr<Gdk::Visual> visual = get_visual ();
  enum Gdk::EventMask events = get_events ();

  GdkWindowAttr attributes;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = allocation.get_x ();
  attributes.y = allocation.get_y ();
  attributes.width = allocation.get_width ();
  attributes.height = allocation.get_height ();
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = visual->gobj ();
  attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
      | GDK_EXPOSURE_MASK
      | events;

  gint attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

  Glib::RefPtr<Gdk::Window> window = Gdk::Window::create (get_parent_window (), &attributes, attributes_mask);
  set_window (window);
  register_window (window);

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
//	view->setAdjustments(vertical_adjustment);
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
//void Editor::on_unrealize() {
//	gtk_im_context_set_client_window(priv->im_context, NULL);
//	super->unrealize (widget);
//}
//

void Editor::on_property_value_hadjustment () {
}

void l_vadjustment_value_changed () {
  printf ("got here\n");

}

void Editor::on_property_value_vadjustment () {
  Glib::RefPtr<Gtk::Adjustment> vertical_adjustment = property_vadjustment ().get_value ();
  if (vertical_adjustment) {
    m_co = vertical_adjustment->signal_value_changed ().connect (sigc::mem_fun (*this, &Editor::l_set_vadjustment));
    vertical_adjustment->set_upper (8000);
    vertical_adjustment->set_page_size (80);
  }
  if (m_view) {
    m_view->setVerticalAdjustment (vertical_adjustment);
  }
}

void Editor::l_set_vadjustment () {
  printf ("Editor::l_set_vadjustment:\n");

  glong aval = (glong) property_vadjustment ().get_value ()->get_value ();

  long long old = m_view->set_view_y (aval);
  long long dy = old - aval;
  printf ("scroll-v:%d, %d\n", (int) dy, (int) aval);

//    if (priv->depreacated_scrolling) {
//    	l_deprecated_scroll(editor, 0, dy);
//    } else {
  Glib::RefPtr<Gdk::Window> window = this->get_window ();
//		cha_document_view_set_in_scroll(priv->document_view, TRUE);
//		if (!cha_document_view_check_cache_boundaries(priv->document_view)) {
//			cha_document_view_invalidate_lines(priv->document_view);
//		}
  window->scroll (0, dy);
  window->process_updates (true);
//		cha_document_view_set_in_scroll(priv->document_view, FALSE);
//	  }
}
