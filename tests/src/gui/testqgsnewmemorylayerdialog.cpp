/***************************************************************************
    testqgsnewmemorylayerdialog.cpp
     --------------------------------------
    Date                 : August 2026
    Copyright            : (C) 2026
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgscoordinatereferencesystem.h"
#include "qgsfields.h"
#include "qgsmemoryproviderutils.h"
#include "qgsnewmemorylayerdialog.h"
#include "qgsproject.h"
#include "qgstest.h"
#include "qgsvectorlayer.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

using namespace Qt::StringLiterals;

class TestQgsNewMemoryLayerDialog : public QObject
{
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void emptyNameIsValidAfterGeometrySelection();
    void firstGeneratedPointName();
    void existingPointNameIncrementsSuffix();
    void numberingIndependentPerGeometryType();
    void firstAvailableGapIsUsed();
    void explicitUserNameIsPreserved();
    void noGeometryUsesDeterministicGeneratedName();

  private:
    void addProjectLayer( const QString &name, Qgis::WkbType geometryType );
};

void TestQgsNewMemoryLayerDialog::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();
}

void TestQgsNewMemoryLayerDialog::cleanupTestCase()
{
  QgsApplication::exitQgis();
}

void TestQgsNewMemoryLayerDialog::init()
{
  QgsProject::instance()->clear();
}

void TestQgsNewMemoryLayerDialog::cleanup()
{
  QgsProject::instance()->clear();
}

void TestQgsNewMemoryLayerDialog::addProjectLayer( const QString &name, Qgis::WkbType geometryType )
{
  QgsVectorLayer *layer = QgsMemoryProviderUtils::createMemoryLayer( name, QgsFields(), geometryType, QgsCoordinateReferenceSystem() );
  QVERIFY( layer );
  QgsProject::instance()->addMapLayer( layer );
}

void TestQgsNewMemoryLayerDialog::emptyNameIsValidAfterGeometrySelection()
{
  QgsNewMemoryLayerDialog dialog;

  QLineEdit *nameLineEdit = dialog.findChild<QLineEdit *>( u"mNameLineEdit"_s );
  QVERIFY( nameLineEdit );
  QCOMPARE( nameLineEdit->text(), QString() );
  QCOMPARE( nameLineEdit->placeholderText(), QgsNewMemoryLayerDialog::tr( "[New scratch layer]" ) );

  QDialogButtonBox *buttonBox = dialog.findChild<QDialogButtonBox *>( u"mButtonBox"_s );
  QVERIFY( buttonBox );
  QPushButton *okButton = buttonBox->button( QDialogButtonBox::Ok );
  QVERIFY( okButton );
  QVERIFY( !okButton->isEnabled() );

  QWidget *crsSelector = dialog.findChild<QWidget *>( u"mCrsSelector"_s );
  QVERIFY( crsSelector );
  QVERIFY( !crsSelector->isEnabled() );

  QComboBox *geometryTypeBox = dialog.findChild<QComboBox *>( u"mGeometryTypeBox"_s );
  QVERIFY( geometryTypeBox );

  nameLineEdit->setText( QStringLiteral( "Custom" ) );
  QVERIFY( !okButton->isEnabled() );
  QVERIFY( !crsSelector->isEnabled() );

  const int pointIndex = geometryTypeBox->findData( static_cast<quint32>( Qgis::WkbType::Point ) );
  QVERIFY( pointIndex >= 0 );
  geometryTypeBox->setCurrentIndex( pointIndex );

  nameLineEdit->clear();
  QVERIFY( okButton->isEnabled() );
  QVERIFY( crsSelector->isEnabled() );
}

void TestQgsNewMemoryLayerDialog::firstGeneratedPointName()
{
  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::Point, QgsProject::instance() ), QStringLiteral( "Point_1" ) );
}

void TestQgsNewMemoryLayerDialog::existingPointNameIncrementsSuffix()
{
  addProjectLayer( QStringLiteral( "Point_1" ), Qgis::WkbType::Point );

  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::Point, QgsProject::instance() ), QStringLiteral( "Point_2" ) );
}

void TestQgsNewMemoryLayerDialog::numberingIndependentPerGeometryType()
{
  addProjectLayer( QStringLiteral( "Point_1" ), Qgis::WkbType::Point );

  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::Point, QgsProject::instance() ), QStringLiteral( "Point_2" ) );
  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::MultiPolygon, QgsProject::instance() ), QStringLiteral( "MultiPolygon_1" ) );
}

void TestQgsNewMemoryLayerDialog::firstAvailableGapIsUsed()
{
  addProjectLayer( QStringLiteral( "Point_1" ), Qgis::WkbType::Point );
  addProjectLayer( QStringLiteral( "Point_3" ), Qgis::WkbType::Point );

  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::Point, QgsProject::instance() ), QStringLiteral( "Point_2" ) );
}

void TestQgsNewMemoryLayerDialog::explicitUserNameIsPreserved()
{
  const QString explicitName = QStringLiteral( "  My Scratch Layer  " );
  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( explicitName, Qgis::WkbType::Point, QgsProject::instance() ), explicitName );
}

void TestQgsNewMemoryLayerDialog::noGeometryUsesDeterministicGeneratedName()
{
  const QString expectedName = QStringLiteral( "%1_1" ).arg( QgsWkbTypes::translatedDisplayString( Qgis::WkbType::NoGeometry ) );
  QCOMPARE( QgsNewMemoryLayerDialog::resolvedLayerName( QString(), Qgis::WkbType::NoGeometry, QgsProject::instance() ), expectedName );
}

QGSTEST_MAIN( TestQgsNewMemoryLayerDialog )
#include "testqgsnewmemorylayerdialog.moc"
