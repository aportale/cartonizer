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

#ifndef CARTON_H
#define CARTON_H

#include "cartonizerproperties.h"

#include <QObject>
#include <QImage>
#include <QHash>

#if QT_VERSION >= 0x040400
QT_FORWARD_DECLARE_CLASS(QPainter)
#else
class QPainter;
#endif

class Carton : public QObject
{
	Q_OBJECT
	Q_ENUMS(Vertices)
	Q_ENUMS(Faces)

public:

	// Left means the observers left, not the "Boxes left".
	enum Vertices {
		LeftTopFront,
		RightTopFront,
		LeftBottomFront,
		RightBottomFront,
		LeftEffectiveSubFront,
		RightEffectiveSubFront,
		LeftSubFront,
		RightSubFront,
		LeftTopBack,
		RightTopBack,
		LeftBottomBack,
		RightBottomBack,
		LeftEffectiveSubBack,
		RightEffectiveSubBack,
		LeftSubBack,
		RightSubBack
	};

	enum Faces {
		Front,
		Left,
		Right,
		Back,
		Top,
		Bottom,
		FrontReflection,
		LeftReflection,
		RightReflection,
		BackReflection
	};

	Carton(QObject *parent = 0);
	void paintFace(QPainter *painter, Faces face, const QPointF &translation, qreal scaling);
	virtual void paintFaceTexture(QPainter *painter, Faces face) = 0;
	void paintFaceReflectionTexture(QPainter *painter, Faces face);

	QSizeF faceSize(Faces face) const;
	QTransform transform(Faces face, const QPointF &translation, qreal scaling) const;
	void boxVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const;
	void rotatedVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const;
	QPointF vertex2d(Vertices vertex) const;
	QPolygonF face2d(Faces face) const;
	bool isFaceVisibleFromFront(Faces face) const;
	QPolygonF outline() const;
	QRectF boundingRect() const;

	static const QVector<Vertices> verticesOfFace(Faces face);

	static const qreal m_defaultWidth;
	static const qreal m_defaultHeight;
	static const qreal m_defaultDepth;
	static const QHash<Carton::Faces, QVector<Carton::Vertices> > &m_facesVerticesHash;
	static const qreal PI;

public slots:
	void paint(QPaintDevice *paintDevice, const QRectF &rect, CartonizerEnums::paintQuality quality);

protected:
	qreal m_xRotation;
	qreal m_yRotation;
	qreal m_observerHeight;
	qreal m_focalLength;
	qreal m_specularityValue;

	qreal m_boxWidth;
	qreal m_boxHeight;
	qreal m_boxDepth;

private:
	static const QHash<Carton::Faces, QVector<Carton::Vertices> > &facesVerticesHash();
};

#endif
