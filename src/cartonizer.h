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
#include <QPicture>

class Cartonizer : public Carton
{
	Q_OBJECT
	Q_PROPERTY(qreal xRotation        READ xRotation        WRITE setXRotation)
	Q_PROPERTY(qreal yRotation        READ yRotation        WRITE setYRotation)
	Q_PROPERTY(qreal observerHeight   READ observerHeight   WRITE setObserverHeight)
	Q_PROPERTY(qreal specularityValue READ specularityValue WRITE setSpecularityValue)
	Q_PROPERTY(qreal focalLength      READ focalLength      WRITE setFocalLength)
	Q_PROPERTY(QPicture frontFace     READ frontFace        WRITE setFrontFace)
	Q_PROPERTY(QPicture leftFace      READ leftFace         WRITE setLeftFace)
	Q_PROPERTY(QPicture topFace       READ topFace          WRITE setTopFace)
	Q_PROPERTY(QPicture rightFace     READ rightFace        WRITE setRightFace)
	Q_PROPERTY(QPicture combinedFaces READ combinedFaces    WRITE setCombinedFaces)

public:
	Cartonizer(QObject *parent = 0);

	void paintFaceTexture(QPainter *painter, Faces face);
	qreal xRotation() const;
	qreal yRotation() const;
	qreal observerHeight() const;
	qreal specularityValue() const;
	qreal focalLength() const;
	QPicture frontFace() const;
	QPicture leftFace() const;
	QPicture topFace() const;
	QPicture rightFace() const;
	QPicture combinedFaces() const;

public slots:
	void setXRotation(qreal rotation);
	void setYRotation(qreal rotation);
	void setObserverHeight(qreal height);
	void setSpecularityValue(qreal value);
	void setFocalLength(qreal length);
	void setFrontFace(const QPicture &face);
	void setLeftFace(const QPicture &face);
	void setTopFace(const QPicture &face);
	void setRightFace(const QPicture &face);
	void setCombinedFaces(const QPicture &faces);

private:
	QPicture m_frontFace;
	QPicture m_leftFace;
	QPicture m_topFace;
	QPicture m_rightFace;
	QPicture m_combinedFaces;
};

#endif
