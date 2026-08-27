/***************************************************************************
  qgsemptystatetreewidget.cpp
  ---------------------------
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

#include "qgsemptystatetreewidget.h"

#include <QPainter>
#include <QPaintEvent>


QgsEmptyStateTreeWidget::QgsEmptyStateTreeWidget( QWidget *parent )
  : QTreeWidget( parent )
{
}

QString QgsEmptyStateTreeWidget::placeholderText() const
{
  return mPlaceholderText;
}

void QgsEmptyStateTreeWidget::setPlaceholderText( const QString &text )
{
  if ( mPlaceholderText == text )
    return;
  mPlaceholderText = text;
  viewport()->update();
}

void QgsEmptyStateTreeWidget::paintEvent( QPaintEvent *event )
{
  QTreeWidget::paintEvent( event );

  if ( mPlaceholderText.isEmpty() || topLevelItemCount() > 0 )
    return;

  QPainter painter( viewport() );
  painter.setPen( palette().color( QPalette::Disabled, QPalette::Text ) );
  painter.drawText( viewport()->rect(), Qt::AlignCenter | Qt::TextWordWrap, mPlaceholderText );
}
