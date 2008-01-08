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

#ifndef CARTONIZER_H
#define CARTONIZER_H

#include "carton.h"

class Cartonizer : public Carton
{
    Q_OBJECT
	Q_PROPERTY(qreal xRotation READ xRotation WRITE setXRotation)
	Q_PROPERTY(qreal yRotation READ yRotation WRITE setYRotation)
	Q_PROPERTY(qreal reflectionSize READ reflectionSize WRITE setReflectionSize)
	Q_PROPERTY(qreal focalLength READ focalLength WRITE setFocalLength)

public:
    Cartonizer(QObject *parent = 0);

	void paintFaceTexture(QPainter *painter, Faces face);
	qreal xRotation() const;
	qreal yRotation() const;
	qreal reflectionSize() const;
	qreal focalLength() const;

public slots:
	void setXRotation(qreal rotation);
	void setYRotation(qreal rotation);
	void setReflectionSize(qreal size);
	void setFocalLength(qreal length);
};

#endif
