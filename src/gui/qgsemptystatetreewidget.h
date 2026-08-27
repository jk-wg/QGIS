/***************************************************************************
  qgsemptystatetreewidget.h
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

#ifndef QGSEMPTYSTATETREEWIDGET_H
#define QGSEMPTYSTATETREEWIDGET_H

#include "qgis_gui.h"
#include "qgis_sip.h"

#include <QTreeWidget>

/**
 * \ingroup gui
 * \brief A QTreeWidget subclass that paints configurable placeholder text when
 * the widget contains no top-level items.
 *
 * The placeholder is rendered using the active Qt palette so it works across
 * themes and scaling.  The text can be configured via the \c placeholderText
 * Qt property, making it usable directly in Designer UI files.
 *
 * \since QGIS 3.42
 */
class GUI_EXPORT QgsEmptyStateTreeWidget : public QTreeWidget
{
    Q_OBJECT

    /**
     * \brief Text to show when the widget has no top-level items.
     */
    Q_PROPERTY( QString placeholderText READ placeholderText WRITE setPlaceholderText )

  public:
    //! Constructor for QgsEmptyStateTreeWidget
    explicit QgsEmptyStateTreeWidget( QWidget *parent SIP_TRANSFERTHIS = nullptr );

    /**
     * Returns the placeholder text shown when the widget is empty.
     */
    QString placeholderText() const;

    /**
     * Sets the placeholder \a text shown when the widget is empty.
     */
    void setPlaceholderText( const QString &text );

  protected:
    void paintEvent( QPaintEvent *event ) override;

  private:
    QString mPlaceholderText;
};

#endif // QGSEMPTYSTATETREEWIDGET_H
