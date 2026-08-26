/***************************************************************************
  qgsemptystatetreeview.cpp
  -------------------------
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

#include "qgsemptystatetreeview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QAbstractItemModel>


QgsEmptyStateTreeView::QgsEmptyStateTreeView( QWidget *parent )
  : QTreeView( parent )
{
}

QString QgsEmptyStateTreeView::placeholderText() const
{
  return mPlaceholderText;
}

void QgsEmptyStateTreeView::setPlaceholderText( const QString &text )
{
  if ( mPlaceholderText == text )
    return;
  mPlaceholderText = text;
  viewport()->update();
}

void QgsEmptyStateTreeView::setModel( QAbstractItemModel *model )
{
  disconnectModel( mConnectedModel );
  QTreeView::setModel( model );
  connectModel( model );
  viewport()->update();
}

void QgsEmptyStateTreeView::paintEvent( QPaintEvent *event )
{
  QTreeView::paintEvent( event );

  if ( mPlaceholderText.isEmpty() )
    return;

  const QAbstractItemModel *m = model();
  if ( m && m->rowCount( rootIndex() ) > 0 )
    return;

  // Draw placeholder text centred in the viewport
  QPainter painter( viewport() );
  const QPalette &pal = palette();
  painter.setPen( pal.color( QPalette::Disabled, QPalette::Text ) );

  const QRect r = viewport()->rect();
  painter.drawText( r, Qt::AlignCenter | Qt::TextWordWrap, mPlaceholderText );
}

void QgsEmptyStateTreeView::onModelReset()
{
  viewport()->update();
}

void QgsEmptyStateTreeView::onRowsInserted()
{
  viewport()->update();
}

void QgsEmptyStateTreeView::onRowsRemoved()
{
  viewport()->update();
}

void QgsEmptyStateTreeView::connectModel( QAbstractItemModel *model )
{
  if ( !model )
    return;
  mConnectedModel = model;
  connect( model, &QAbstractItemModel::modelReset, this, &QgsEmptyStateTreeView::onModelReset );
  connect( model, &QAbstractItemModel::rowsInserted, this, &QgsEmptyStateTreeView::onRowsInserted );
  connect( model, &QAbstractItemModel::rowsRemoved, this, &QgsEmptyStateTreeView::onRowsRemoved );
}

void QgsEmptyStateTreeView::disconnectModel( QAbstractItemModel *model )
{
  if ( !model )
    return;
  disconnect( model, &QAbstractItemModel::modelReset, this, &QgsEmptyStateTreeView::onModelReset );
  disconnect( model, &QAbstractItemModel::rowsInserted, this, &QgsEmptyStateTreeView::onRowsInserted );
  disconnect( model, &QAbstractItemModel::rowsRemoved, this, &QgsEmptyStateTreeView::onRowsRemoved );
  mConnectedModel = nullptr;
}
