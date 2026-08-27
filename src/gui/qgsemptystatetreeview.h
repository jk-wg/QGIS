/***************************************************************************
  qgsemptystatetreeview.h
  -----------------------
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

#ifndef QGSEMPTYSTATETREEVIEW_H
#define QGSEMPTYSTATETREEVIEW_H

#include "qgis_gui.h"
#include "qgis_sip.h"

#include <QTreeView>

/**
 * \ingroup gui
 * \brief A QTreeView subclass that paints configurable placeholder text when the
 * model contains no rows.
 *
 * The placeholder is rendered using the active Qt palette so it works across
 * themes and scaling.  The text can be configured via the \c placeholderText
 * Qt property, making it usable directly in Designer UI files.
 *
 * \since QGIS 3.42
 */
class GUI_EXPORT QgsEmptyStateTreeView : public QTreeView
{
    Q_OBJECT

    /**
     * \brief Text to show when the model has zero rows.
     *
     * This property mirrors the built-in QAbstractItemView::placeholderText
     * introduced in Qt 5.15 but is provided here so the class works with
     * older Qt versions and is always available in Designer.
     */
    Q_PROPERTY( QString placeholderText READ placeholderText WRITE setPlaceholderText )

  public:
    //! Constructor for QgsEmptyStateTreeView
    explicit QgsEmptyStateTreeView( QWidget *parent SIP_TRANSFERTHIS = nullptr );

    /**
     * Returns the placeholder text shown when the model is empty.
     */
    QString placeholderText() const;

    /**
     * Sets the placeholder \a text shown when the model is empty.
     */
    void setPlaceholderText( const QString &text );

    void setModel( QAbstractItemModel *model ) override;

  protected:
    void paintEvent( QPaintEvent *event ) override;

  private slots:
    void onModelReset();
    void onRowsInserted();
    void onRowsRemoved();

  private:
    QString mPlaceholderText;
    QAbstractItemModel *mConnectedModel = nullptr;

    void connectModel( QAbstractItemModel *model );
    void disconnectModel( QAbstractItemModel *model );
};

#endif // QGSEMPTYSTATETREEVIEW_H
