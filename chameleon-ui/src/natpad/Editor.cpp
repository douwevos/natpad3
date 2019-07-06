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
#include <natpad/keybindings/EmacsKeyBindings.h>

static shared_ptr<const TextModel> createTextModel (void) {
  TextModel emptyModel;
  return emptyModel.insert (Cursor (), "Uit 'de vrol\304\263ke wetenschap' van Friedrich Nietzsche:\n\n1. Einladung.\n\nWagt's mit meiner Ko\305\277t, ihr E\305\277\305\277er!\nMorgen \305\277chmeckt \305\277ie euch \305\277chon be\305\277\305\277er\nUnd \305\277chon \303\274bermorgen gut!\nWollt ihr dann noch mehr,- \305\277o machen\nMeine alten \305\277ieben Sachen\nMir zu \305\277ieben neuen Muth.\n\n2. Mein Gl\303\274ck.\n\nSeit ich des Suchens m\303\274de ward,\nErlernte ich das Finden.\nSeit mir ein Wind hielt Widerpart,\nSegl' ich mit allen Winden.\n\n3. Unverzagt.\n\nWo du \305\277teh\305\277t, grab tief hinein!\nDrunten i\305\277t die Quelle!\nLa\303\237 die dunklen M\303\244nner \305\277chrein:\n\342\200\236Stets i\305\277t drunten - H\303\266lle!\342\200\234\n\n7. Vademecum - Vadetecum\n\nEs lockt dich meine Art und Sprach',\nDu folge\305\277t mir, du geh\305\277t mir nach?\nGeh nur dir \305\277elber treulich nach:-\nSo folg\305\277t du mir - gemach! gemach!\n\n\316\221\316\222\316\223\316\224\n\320\220\320\221\320\222\320\223\320\224\n1\n12\n123\n1234\n1243\nabcd\nABCD\n\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n61\n62\n63\n64\n65\n66\n67\n68\n69\n70\n71\n72\n73\n74\n75\n76\n77\n78\n79\n80\n81\n82\n83\n84\n85\n86\n87\n88\n89\n90\n91\n92\n93\n94\n95\n96\n97\n98\n99\n");
}

Editor::Editor (void) :
    Gtk::Scrollable (), Glib::ObjectBase (typeid(Gtk::Widget)),
        m_hadjustment (*this, "hadjustment"),
        m_vadjustment (*this, "vadjustment"),
        m_hscroll_policy (*this, "hscroll-policy", Gtk::SCROLL_NATURAL),
        m_vscroll_policy (*this, "vscroll-policy", Gtk::SCROLL_NATURAL),
        m_textDocument (new TextDocument),
        m_keyBindings (new EmacsKeyBindings),
        m_cursorColumn (0)
{
  property_vadjustment ().signal_changed ().connect (sigc::mem_fun (*this, &Editor::on_property_value_vadjustment));
  set_app_paintable ();
  set_can_focus ();
  show ();
}

Editor::~Editor (void) {
  delete m_keyBindings;
}

void Editor::on_size_allocate (Gtk::Allocation& allocation) {
  if (m_view)
    m_view->setDimensions (allocation.get_width (), allocation.get_height ());
  Gtk::Widget::on_size_allocate (allocation);
}

shared_ptr<TextDocument> Editor::getTextDocument (void) {
  return m_textDocument;
}

bool Editor::on_draw (const Cairo::RefPtr<Cairo::Context>& cr) {
  m_view->draw (cr);
  return true;
}

bool Editor::on_key_press_event (GdkEventKey* keyEvent) {
  bool ctrl = (keyEvent->state & GDK_CONTROL_MASK) != 0;
  bool alt = (keyEvent->state & GDK_MOD1_MASK) != 0;
  bool shift = (keyEvent->state & GDK_SHIFT_MASK) != 0;
  int key = keyEvent->keyval;

  if (key >= 0xFFE1 && key <= 0xFFEA) {
    return true;
  }

  KeyCommand command = m_keyBindings->getCommand ({key, ctrl, alt, shift});
  switch (command) {
  case KeyCommand::cursorBack:
    moveCursorBack ();
    break;

  case KeyCommand::cursorForward:
    moveCursorForward ();
    break;

  case KeyCommand::cursorLineDown:
    moveCursorDown ();
    break;

  case KeyCommand::cursorLineEnd:
    moveCursorLineEnd ();
    break;

  case KeyCommand::cursorLineStart:
    moveCursorLineStart ();
    break;

  case KeyCommand::cursorLineUp:
    moveCursorUp ();
    break;

  case KeyCommand::cursorPageDown:
    moveCursorPageDown ();
    break;

  case KeyCommand::cursorPageUp:
    moveCursorPageUp ();
    break;

  case KeyCommand::cursorTextEnd:
    moveCursorTextEnd ();
    break;

  case KeyCommand::cursorTextStart:
    moveCursorTextStart ();
    break;
  }

  if (!(command == KeyCommand::cursorLineUp ||
        command == KeyCommand::cursorLineDown ||
        command == KeyCommand::none)) {
    m_cursorColumn = 0;
  }

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
  window->set_events (window->get_events () | Gdk::EventMask::KEY_PRESS_MASK); //I seem to get key events whether I do this or not...

  if (!m_view) {
    m_view.reset (new View (*this, 24));
    m_textDocument->addListener (m_view);
  }
  m_view->setLayoutHeight (3000);
  m_view->setDimensions (window->get_width (), window->get_height ());
  m_textDocument->postTextModel (createTextModel ());
  m_view->setCursor (Cursor ());

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

  int64_t aval = property_vadjustment ().get_value ()->get_value ();
  aval -= aval % m_view->getLineHeight ();

  int64_t old = m_view->set_view_y (aval);
  int dy = old - aval;
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

void Editor::scrollTo (int lineIndex) {
  int viewY = lineIndex * m_view->getLineHeight ();
  property_vadjustment ().get_value ()->set_value (viewY); /* This will trigger a call to Editor::l_set_vadjustment().  */
}


void Editor::moveCursorBack (void) {
  Cursor cursor = m_view->getCursor ();
  if (cursor.column == 0) {
    if (cursor.line == 0)
      return;
    --cursor.line;
    shared_ptr<const String> line = m_textDocument->getTextModel ()->lineAt (cursor.line);
    cursor.column = line->length ();
  } else {
    --cursor.column;
  }
  m_view->setCursor (cursor);
}

void Editor::moveCursorDown (void) {
  Cursor cursor = m_view->getCursor ();
  shared_ptr<const TextModel> textModel = m_textDocument->getTextModel ();
  if (cursor.line >= textModel->lineCount () - 1) {
    return;
  }

  if (m_cursorColumn > 0) {
    cursor.column = m_cursorColumn;
  } else {
    m_cursorColumn = cursor.column;
  }
  ++cursor.line;
  shared_ptr<const String> line = textModel->lineAt (cursor.line);
  if (cursor.column > line->length()) {
    cursor.column = line->length ();
  }
  m_view->setCursor (cursor);
}

void Editor::moveCursorForward (void) {
  Cursor cursor = m_view->getCursor ();
  shared_ptr<const TextModel> textModel = m_textDocument->getTextModel ();
  shared_ptr<const String> line = textModel->lineAt (cursor.line);
  if (cursor.column == line->length ()) {
    if (cursor.line == textModel->lineCount () - 1) {
      return;
    }
    ++cursor.line;
    cursor.column = 0;
  } else {
    ++cursor.column;
  }
  m_view->setCursor (cursor);
}

void Editor::moveCursorLineEnd (void) {
  Cursor cursor = m_view->getCursor ();
  shared_ptr<const String> line = m_textDocument->getTextModel ()->lineAt (cursor.line);
  cursor.column = line->length ();
  m_view->setCursor (cursor);
}

void Editor::moveCursorLineStart (void) {
  Cursor cursor = m_view->getCursor ();
  cursor.column = 0;
  m_view->setCursor (cursor);
}

void Editor::moveCursorPageDown (void) {
  const int maxLineIndex = m_textDocument->getTextModel ()->lineCount () - 1;
  const int lineHeight = m_view->getLineHeight ();
  const int viewHeight = m_view->getHeight ();
  const int64_t viewY = m_view->getViewY ();
  int firstLineIndex = viewY / lineHeight;
  int linesOnPageMinusTwo = viewHeight / lineHeight - 2;
  if (linesOnPageMinusTwo < 1) {
    linesOnPageMinusTwo = 1;
  }

  firstLineIndex += linesOnPageMinusTwo;
  if (firstLineIndex > maxLineIndex) {
    return;
  }
  scrollTo (firstLineIndex);

  int lastLineIndex = firstLineIndex + viewHeight / lineHeight;
  Cursor cursor = m_view->getCursor ();
  if (cursor.line < firstLineIndex || cursor.line >= lastLineIndex) {
    cursor.line = firstLineIndex;
    cursor.column = 0;
    m_view->setCursor (cursor);
  }
}

void Editor::moveCursorPageUp (void) {
  const int lineHeight = m_view->getLineHeight ();
  const int viewHeight = m_view->getHeight ();
  const int64_t viewY = m_view->getViewY ();
  const int oldFirstLineIndex = viewY / lineHeight;
  int linesOnPageMinusTwo = viewHeight / lineHeight - 2;
  if (linesOnPageMinusTwo < 1) {
    if (linesOnPageMinusTwo < 0) {
      moveCursorUp ();
      return;
    }
    linesOnPageMinusTwo = 1;
  }
  if (oldFirstLineIndex == 0) {
    return;
  }

  int delta = 0;
  int firstLineIndex = oldFirstLineIndex - linesOnPageMinusTwo;
  if (firstLineIndex < 0) {
    delta = -firstLineIndex;
    firstLineIndex = 0;
  }
  scrollTo (firstLineIndex);

  int lastLineIndex = firstLineIndex + viewHeight / lineHeight;
  Cursor cursor = m_view->getCursor ();
  if (cursor.line < firstLineIndex || cursor.line >= lastLineIndex) {
    cursor.line = oldFirstLineIndex + delta;
    cursor.column = 0;
    m_view->setCursor (cursor);
  }
}

void Editor::moveCursorTextEnd (void) {
  shared_ptr<const TextModel> textModel = m_textDocument->getTextModel ();
  int lineIndex = textModel->lineCount () - 1;
  shared_ptr<const String> line = textModel->lineAt (lineIndex);
  m_view->setCursor (Cursor (lineIndex, line->length ()));
}

void Editor::moveCursorTextStart (void) {
  m_view->setCursor (Cursor ());
}

void Editor::moveCursorUp (void) {
  Cursor cursor = m_view->getCursor ();
  if (cursor.line == 0)
    return;

  if (m_cursorColumn > 0) {
    cursor.column = m_cursorColumn;
  } else {
    m_cursorColumn = cursor.column;
  }
  --cursor.line;
  shared_ptr<const String> line = m_textDocument->getTextModel ()->lineAt (cursor.line);
  if (cursor.column > line->length()) {
    cursor.column = line->length ();
  }
  m_view->setCursor (cursor);
}
