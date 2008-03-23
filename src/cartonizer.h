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
#include <QPixmap>

class Cartonizer : public Carton
{
	Q_OBJECT
	Q_PROPERTY(qreal xRotation        READ xRotation        WRITE setXRotation)
	Q_PROPERTY(qreal yRotation        READ yRotation        WRITE setYRotation)
	Q_PROPERTY(qreal observerHeight   READ observerHeight   WRITE setObserverHeight)
	Q_PROPERTY(qreal specularityValue READ specularityValue WRITE setSpecularityValue)
	Q_PROPERTY(qreal focalLength      READ focalLength      WRITE setFocalLength)
	Q_PROPERTY(QPixmap frontFace      READ frontFace        WRITE setFrontFace)
	Q_PROPERTY(QPixmap leftFace       READ leftFace         WRITE setLeftFace)
	Q_PROPERTY(QPixmap topFace        READ topFace          WRITE setTopFace)
	Q_PROPERTY(QPixmap rightFace      READ rightFace        WRITE setRightFace)
	Q_PROPERTY(QPixmap combinedFaces  READ combinedFaces    WRITE setCombinedFaces)

public:
	Cartonizer(QObject *parent = 0);

	void paintFaceTexture(QPainter *painter, Faces face);
	qreal xRotation() const;
	qreal yRotation() const;
	qreal observerHeight() const;
	qreal specularityValue() const;
	qreal focalLength() const;
	QPixmap frontFace() const;
	QPixmap leftFace() const;
	QPixmap topFace() const;
	QPixmap rightFace() const;
	QPixmap combinedFaces() const;

public slots:
	void setXRotation(qreal rotation);
	void setYRotation(qreal rotation);
	void setObserverHeight(qreal height);
	void setSpecularityValue(qreal value);
	void setFocalLength(qreal length);
	void setFrontFace(const QPixmap &face);
	void setLeftFace(const QPixmap &face);
	void setTopFace(const QPixmap &face);
	void setRightFace(const QPixmap &face);
	void setCombinedFaces(const QPixmap &faces);

private:
	QPixmap m_frontFace;
	QPixmap m_leftFace;
	QPixmap m_topFace;
	QPixmap m_rightFace;
	QPixmap m_combinedFaces;
};

#endif
