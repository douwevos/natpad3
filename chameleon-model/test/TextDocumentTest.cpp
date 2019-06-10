/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2019 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "TextDocumentTest.h"
#include <natpad/textmodel/TextDocument.h>

class TestDocumentListener : public DocumentListener {
private:
  shared_ptr<const TextModel> m_textModel;

public:
  void onNewTextModel (shared_ptr<const TextModel> textModel) override {
    m_textModel = textModel;
  }

  shared_ptr<const TextModel> getTextModel (void) {
    return m_textModel;
  }
};

using Test = void (TextDocumentTest::*) (void);

static const Test tests[] = {
  &TextDocumentTest::testConstructor_void,
  &TextDocumentTest::testConstructor_TextModel,
  &TextDocumentTest::testAddListenerAndPostTextModel,
  &TextDocumentTest::testRemoveListener
};

TextDocumentTest::TextDocumentTest (void) : TestCase ("TextDocumentTest") {
}

unique_ptr<TextDocumentTest> TextDocumentTest::create (void) {
  return unique_ptr<TextDocumentTest> (new TextDocumentTest);
}

int TextDocumentTest::getTestCount (void) {
  return sizeof (tests) / sizeof (Test);
}

const TestCase::Test* TextDocumentTest::getTests (void) {
  return (TestCase::Test*) tests;
}

static shared_ptr<const TextModel> createTextModel (void) {
  TextModel emptyModel;
  return emptyModel.insert (Cursor (), "eerste regel\ntweede regel");
}

void TextDocumentTest::testConstructor_void (void) {
  setTestName (__func__);

  TextDocument doc;
  shared_ptr<const TextModel> model = doc.getTextModel ();
  assertEquals (1, model->lineCount ());
  assertEquals ("", *model->lineAt (0));
}

void TextDocumentTest::testConstructor_TextModel (void) {
  setTestName (__func__);

  TextDocument doc (createTextModel ());
  shared_ptr<const TextModel> model = doc.getTextModel ();
  assertEquals (2, model->lineCount ());
  assertEquals ("eerste regel", *model->lineAt (0));
  assertEquals ("tweede regel", *model->lineAt (1));
}

void TextDocumentTest::testAddListenerAndPostTextModel (void) {
  setTestName (__func__);

  TextDocument doc;
  shared_ptr<const TextModel> model = createTextModel ();

  shared_ptr<TestDocumentListener> listener1 (new TestDocumentListener);
  doc.addListener (listener1);
  {
    shared_ptr<TestDocumentListener> listener2 (new TestDocumentListener);
    doc.addListener (listener2);
    assertEquals (1, listener2.use_count ());
    assertEquals (1, model.use_count ());
    doc.postTextModel (model);
    assertEquals (4, model.use_count ());
    assertEquals (model.get (), doc.getTextModel ().get ());
    assertEquals (model.get (), listener1->getTextModel ().get ());
    assertEquals (model.get (), listener2->getTextModel ().get ());
  }
  assertEquals (3, model.use_count ());

  shared_ptr<const TextModel> modifiedModel = model->insert (Cursor (2, 0), "derde regel\nvierde regel");
  doc.postTextModel (modifiedModel);
  assertEquals (2, model.use_count ());
  assertEquals (3, modifiedModel.use_count ());
  assertEquals (modifiedModel.get (), doc.getTextModel ().get ());
  assertEquals (modifiedModel.get (), listener1->getTextModel ().get ());
}

void TextDocumentTest::testRemoveListener (void) {
  setTestName (__func__);

  TextDocument doc;
  shared_ptr<const TextModel> model = createTextModel ();
  shared_ptr<TestDocumentListener> listener1 (new TestDocumentListener);
  shared_ptr<TestDocumentListener> listener2 (new TestDocumentListener);
  shared_ptr<TestDocumentListener> listener3 (new TestDocumentListener);
  doc.addListener (listener1);
  doc.addListener (listener2);
  doc.addListener (listener3);

  doc.removeListener (listener2);

  doc.postTextModel (model);
  assertEquals (model.get (), listener1->getTextModel ().get ());
  assertEquals (nullptr, listener2->getTextModel ().get ());
  assertEquals (model.get (), listener3->getTextModel ().get ());
}
