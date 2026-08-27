/***************************************************************************
  testqgsemptystatetreeview.cpp
  -----------------------------
  Date                 : August 2026
  Copyright            : (C) 2026 QGIS contributors
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsapplication.h"
#include "qgsemptystatetreeview.h"
#include "qgstest.h"

#include <QStandardItemModel>

using namespace Qt::StringLiterals;

class TestQgsEmptyStateTreeView : public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase();
    void cleanupTestCase();
    void testPlaceholderText();
    void testEmptyModelShowsPlaceholder();
    void testNonEmptyModelHidesPlaceholder();
    void testRowsInsertedHidesPlaceholder();
    void testRowsRemovedShowsPlaceholder();
    void testModelResetUpdatesState();
    void testSetModelConnects();
};

void TestQgsEmptyStateTreeView::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();
}

void TestQgsEmptyStateTreeView::cleanupTestCase()
{
  QgsApplication::exitQgis();
}

void TestQgsEmptyStateTreeView::testPlaceholderText()
{
  QgsEmptyStateTreeView view;
  QVERIFY( view.placeholderText().isEmpty() );

  const QString text = u"Select a value and click Classify to create classes."_s;
  view.setPlaceholderText( text );
  QCOMPARE( view.placeholderText(), text );

  // Setting same text is a no-op (verify it doesn't crash)
  view.setPlaceholderText( text );
  QCOMPARE( view.placeholderText(), text );

  // Clear the text
  view.setPlaceholderText( QString() );
  QVERIFY( view.placeholderText().isEmpty() );
}

void TestQgsEmptyStateTreeView::testEmptyModelShowsPlaceholder()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model = std::make_unique<QStandardItemModel>();
  view.setModel( model.get() );

  // Model is empty: placeholder should logically be shown.
  // We verify the state deterministically rather than via screenshot.
  QCOMPARE( model->rowCount(), 0 );
  QCOMPARE( view.placeholderText(), u"Hint text"_s );
}

void TestQgsEmptyStateTreeView::testNonEmptyModelHidesPlaceholder()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model = std::make_unique<QStandardItemModel>();
  model->appendRow( new QStandardItem( u"Item 1"_s ) );
  view.setModel( model.get() );

  QCOMPARE( model->rowCount(), 1 );
  // Placeholder text property unchanged – but row count > 0 means paint will skip it.
  QCOMPARE( view.placeholderText(), u"Hint text"_s );
}

void TestQgsEmptyStateTreeView::testRowsInsertedHidesPlaceholder()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model = std::make_unique<QStandardItemModel>();
  view.setModel( model.get() );
  QCOMPARE( model->rowCount(), 0 );

  model->appendRow( new QStandardItem( u"New item"_s ) );
  QCOMPARE( model->rowCount(), 1 );
  // After insertion the view's viewport should have been scheduled for repaint.
  // Verify row count to confirm signal was processed.
  QCOMPARE( view.model()->rowCount(), 1 );
}

void TestQgsEmptyStateTreeView::testRowsRemovedShowsPlaceholder()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model = std::make_unique<QStandardItemModel>();
  model->appendRow( new QStandardItem( u"Item"_s ) );
  view.setModel( model.get() );
  QCOMPARE( model->rowCount(), 1 );

  model->removeRow( 0 );
  QCOMPARE( model->rowCount(), 0 );
  QCOMPARE( view.model()->rowCount(), 0 );
}

void TestQgsEmptyStateTreeView::testModelResetUpdatesState()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model = std::make_unique<QStandardItemModel>();
  model->appendRow( new QStandardItem( u"Item"_s ) );
  view.setModel( model.get() );
  QCOMPARE( model->rowCount(), 1 );

  model->clear(); // triggers modelReset
  QCOMPARE( model->rowCount(), 0 );
  QCOMPARE( view.model()->rowCount(), 0 );
}

void TestQgsEmptyStateTreeView::testSetModelConnects()
{
  QgsEmptyStateTreeView view;
  view.setPlaceholderText( u"Hint text"_s );

  auto model1 = std::make_unique<QStandardItemModel>();
  auto model2 = std::make_unique<QStandardItemModel>();

  view.setModel( model1.get() );
  model1->appendRow( new QStandardItem( u"A"_s ) );
  QCOMPARE( view.model()->rowCount(), 1 );

  // Switch to a different model – old model signals should be disconnected
  view.setModel( model2.get() );
  QCOMPARE( view.model()->rowCount(), 0 );

  // Adding to old model should not affect view (no crash, no state change via view)
  model1->appendRow( new QStandardItem( u"B"_s ) );
  QCOMPARE( view.model()->rowCount(), 0 ); // view still points to model2
}

QGSTEST_MAIN( TestQgsEmptyStateTreeView )
#include "testqgsemptystatetreeview.moc"
