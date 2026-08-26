/***************************************************************************
    testqgshistogramwidget.cpp
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

#include "qgshistogramwidget.h"
#include "qgsfeature.h"
#include "qgsgeometry.h"
#include "qgsmarkersymbol.h"
#include "qgsrendererrange.h"
#include "qgstest.h"
#include "qgsvectorlayer.h"

#include <memory>
#include <qwt_plot.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_item.h>

class TestQgsHistogramWidget : public QObject
{
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();

    void noAutoPenForBrightBrush();
    void explicitPenPreserved();
    void noAutoPenForBrightGraduatedRanges();

  private:
    std::unique_ptr<QgsVectorLayer> createLayer() const;
    QList<QwtPlotHistogram *> histogramItems( const QgsHistogramWidget &widget ) const;
};

void TestQgsHistogramWidget::initTestCase()
{
  QgsApplication::initQgis();
}

void TestQgsHistogramWidget::cleanupTestCase()
{
  QgsApplication::exitQgis();
}

std::unique_ptr<QgsVectorLayer> TestQgsHistogramWidget::createLayer() const
{
  auto layer = std::make_unique<QgsVectorLayer>( u"Point?field=value:double"_s, u"test"_s, u"memory"_s );

  QgsFeature f1( layer->fields(), 1 );
  f1.setAttribute( u"value"_s, 1.0 );
  f1.setGeometry( QgsGeometry::fromWkt( u"Point(0 0)"_s ) );

  QgsFeature f2( layer->fields(), 2 );
  f2.setAttribute( u"value"_s, 2.0 );
  f2.setGeometry( QgsGeometry::fromWkt( u"Point(1 1)"_s ) );

  QgsFeature f3( layer->fields(), 3 );
  f3.setAttribute( u"value"_s, 3.0 );
  f3.setGeometry( QgsGeometry::fromWkt( u"Point(2 2)"_s ) );

  if ( !layer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 ) )
    return nullptr;
  return layer;
}

QList<QwtPlotHistogram *> TestQgsHistogramWidget::histogramItems( const QgsHistogramWidget &widget ) const
{
  QList<QwtPlotHistogram *> items;
  QwtPlot *plot = widget.findChild<QwtPlot *>();
  if ( !plot )
    return items;

  const QwtPlotItemList plotItems = plot->itemList();
  for ( QwtPlotItem *item : plotItems )
  {
    if ( auto *histogram = dynamic_cast<QwtPlotHistogram *>( item ) )
      items << histogram;
  }
  return items;
}

void TestQgsHistogramWidget::noAutoPenForBrightBrush()
{
  std::unique_ptr<QgsVectorLayer> layer = createLayer();
  QVERIFY( layer );
  QVERIFY( layer->isValid() );
  QVERIFY( layer->featureCount() == 3 );

  QgsHistogramWidget widget( nullptr, layer.get(), u"value"_s );
  widget.setBrush( QBrush( QColor( 255, 255, 255 ) ) );
  widget.setPen( Qt::NoPen );
  widget.refreshValues();

  const QList<QwtPlotHistogram *> items = histogramItems( widget );
  QCOMPARE( items.count(), 1 );
  QCOMPARE( items.at( 0 )->pen().style(), Qt::NoPen );
}

void TestQgsHistogramWidget::explicitPenPreserved()
{
  std::unique_ptr<QgsVectorLayer> layer = createLayer();
  QVERIFY( layer );
  QVERIFY( layer->isValid() );

  QgsHistogramWidget widget( nullptr, layer.get(), u"value"_s );
  widget.setBrush( QBrush( QColor( 255, 255, 255 ) ) );

  QPen pen( QColor( 255, 0, 0 ) );
  pen.setWidth( 2 );
  widget.setPen( pen );
  widget.refreshValues();

  const QList<QwtPlotHistogram *> items = histogramItems( widget );
  QCOMPARE( items.count(), 1 );
  QCOMPARE( items.at( 0 )->pen(), pen );
}

void TestQgsHistogramWidget::noAutoPenForBrightGraduatedRanges()
{
  std::unique_ptr<QgsVectorLayer> layer = createLayer();
  QVERIFY( layer );
  QVERIFY( layer->isValid() );

  QgsHistogramWidget widget( nullptr, layer.get(), u"value"_s );

  QVariantMap props;
  props.insert( u"color"_s, u"255,255,255"_s );

  QgsRangeList ranges;
  ranges << QgsRendererRange( 0.0, 10.0, QgsMarkerSymbol::createSimple( props ).release(), u"bright"_s );
  widget.setGraduatedRanges( ranges );
  widget.refreshValues();

  const QList<QwtPlotHistogram *> items = histogramItems( widget );
  QCOMPARE( items.count(), 1 );
  QCOMPARE( items.at( 0 )->pen().style(), Qt::NoPen );
}

QGSTEST_MAIN( TestQgsHistogramWidget )
#include "testqgshistogramwidget.moc"
