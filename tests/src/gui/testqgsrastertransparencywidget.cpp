/***************************************************************************
    testqgsrastertransparencywidget.cpp
     --------------------------------------
    Date                 : August 2026
    Copyright            : (C) 2026
    Email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsapplication.h"
#include "qgsdoublespinbox.h"
#include "qgsrasterdataprovider.h"
#include "qgsrasterlayer.h"
#include "qgsrastertransparencywidget.h"
#include "qgstest.h"

#include <QObject>
#include <memory>

class TestQgsRasterTransparencyWidget : public QObject
{
    Q_OBJECT

  private:
    std::unique_ptr<QgsRasterLayer> mRasterLayer;

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void testAdditionalNoDataValue();
};

void TestQgsRasterTransparencyWidget::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();

  const QString testDataDir = QStringLiteral( TEST_DATA_DIR ) + '/';
  mRasterLayer = std::make_unique<QgsRasterLayer>( testDataDir + QStringLiteral( "landsat.tif" ), QStringLiteral( "landsat" ) );
  QVERIFY( mRasterLayer->isValid() );
}

void TestQgsRasterTransparencyWidget::cleanupTestCase()
{
  mRasterLayer.reset();
}

void TestQgsRasterTransparencyWidget::testAdditionalNoDataValue()
{
  QgsRasterDataProvider *provider = mRasterLayer->dataProvider();
  QVERIFY( provider );

  QgsRasterTransparencyWidget widget( mRasterLayer.get(), nullptr );
  QVERIFY( qobject_cast<QgsDoubleSpinBox *>( widget.leNoDataValue ) );

  widget.leNoDataValue->clear();
  widget.applyToRasterProvider( provider );
  QVERIFY( provider->userNoDataValues( 1 ).isEmpty() );

  widget.leNoDataValue->setValue( 123.5 );
  widget.applyToRasterProvider( provider );

  const QgsRasterRangeList noDataRangeList = provider->userNoDataValues( 1 );
  QCOMPARE( noDataRangeList.size(), 1 );
  QCOMPARE( noDataRangeList.at( 0 ).min(), 123.5 );
  QCOMPARE( noDataRangeList.at( 0 ).max(), 123.5 );

  widget.leNoDataValue->clear();
  widget.applyToRasterProvider( provider );
  QVERIFY( provider->userNoDataValues( 1 ).isEmpty() );
}

QGSTEST_MAIN( TestQgsRasterTransparencyWidget )
#include "testqgsrastertransparencywidget.moc"
