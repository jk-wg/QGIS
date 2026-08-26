/***************************************************************************
    qgsrasternumericformatutils.h
    -----------------------------
    begin                : August 2026
    copyright            : (C) 2026
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSRASTERNUMERICFORMATUTILS_H
#define QGSRASTERNUMERICFORMATUTILS_H

#include "qgsdoublespinbox.h"
#include "qgsguiutils.h"

#include <limits>

namespace QgsRasterNumericFormatUtils
{
  constexpr int DefaultDecimals = std::numeric_limits<double>::digits10 + 1;
  constexpr double DefaultStep = 1.0;

  inline void configureRasterNumericSpinBox( QgsDoubleSpinBox *spinBox, const int decimals = DefaultDecimals )
  {
    spinBox->setShowClearButton( false );
    spinBox->setMinimum( std::numeric_limits<double>::lowest() );
    spinBox->setMaximum( std::numeric_limits<double>::max() );
    spinBox->setDecimals( decimals );
    spinBox->setSingleStep( DefaultStep );
    spinBox->setClearValueMode( QgsDoubleSpinBox::MinimumValue );
    spinBox->setSpecialValueText( QString() );
  }

  inline void setDataTypePrecision( QgsDoubleSpinBox *spinBox, const Qgis::DataType dataType )
  {
    spinBox->setDecimals( QgsGuiUtils::significantDigits( dataType ) );
  }
}

#endif // QGSRASTERNUMERICFORMATUTILS_H
