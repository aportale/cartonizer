/*
	Cartonizer - Box it beautifully!
	Copyright (C) 2007-2008 by Alessandro Portale
	http://cartonizer.sourceforge.net/

	This file is part of Cartonizer

	Cartonizer is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Cartonizer is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Cartonizer; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "cartonizertools.h"
#include <QSvgRenderer>
#include <QFileInfo>
#include <QPainter>

Q_DECLARE_METATYPE(QPicture)

QPicture CartonizerTools::pictureFromFile(const QString &fileName)
{
	QPicture result;

	QFileInfo imageFileInfo(fileName);
	QString imageFileSuffix = imageFileInfo.suffix();
	QPainter facePainter(&result);
	if (imageFileSuffix == QLatin1String("svg")) {
		QSvgRenderer svgRenderer(fileName);
		svgRenderer.render(&facePainter, svgRenderer.viewBox());
	} else {

	}

	return result;
}

QVariant CartonizerTools::pictureVariantFromFile(const QString &fileName)
{
	return qVariantFromValue(pictureFromFile(fileName));
}
