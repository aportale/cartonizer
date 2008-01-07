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
	along with PosteRazor; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "carton.h"
#include <QPainter>
#include <QtDebug>
#include <QMetaEnum>
#include <math.h>

const qreal Carton::m_defaultWidth = 140.;
const qreal Carton::m_defaultHeight = 200.;
const qreal Carton::m_defaultDepth = 60.;
const QHash<Carton::Faces, QVector<Carton::Vertices> > Carton::m_facesVerticesHash = facesVerticesHash();
const qreal Carton::PI = 3.14159265358979323846; // Source: http://en.wikipedia.org/wiki/Pi

Carton::Carton(QObject *parent)
    : QObject(parent)
	, m_xOffset(300)
	, m_yOffset(300)
	, m_xRotation(-2.7)
	, m_yRotation(0.3)
	, m_observerHeight(-200)
	, m_boxWidth(m_defaultWidth)
	, m_boxHeight(m_defaultHeight)
	, m_boxDepth(m_defaultDepth)
	, m_reflectionSize(0.4)
{
}

void Carton::paint(QPaintDevice *paintDevice)
{
	QPainter painter(paintDevice);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//	painter.setOpacity(0.5);
//	paintVertices(&painter);

	paintFace(&painter, Front);
	paintFace(&painter, Left);
//	paintFace(&painter, Right);
	paintFace(&painter, Top);
	paintFace(&painter, FrontReflection);
	paintFace(&painter, LeftReflection);
//	paintFace(&painter, RightReflection);

	painter.restore();
}

void Carton::paintFace(QPainter *painter, Faces face)
{
	painter->save();
	QTransform faceTransform(transform(face) * QTransform().translate(m_xOffset, m_yOffset));
	painter->setTransform(faceTransform);
	switch (face) {
		case FrontReflection:
		case BackReflection:
		case LeftReflection:
		case RightReflection:
			paintFaceReflectionTexture(painter, face);
			break;
		default:
			paintFaceTexture(painter, face);
	}
	painter->restore();
}

void Carton::paintFaceReflectionTexture(QPainter *painter, Faces face)
{
	Faces emittingFace =
		face == FrontReflection?Front
		:face == BackReflection?Back
		:face == LeftReflection?Left
		:/* face == RightReflection? */Right;

	QSizeF faceSize(this->faceSize(face));
	QImage blendImage(faceSize.toSize(), QImage::Format_ARGB32);
	QPainter blendPainter(&blendImage);
	paintFaceTexture(&blendPainter, emittingFace);
	blendPainter.end();

	QImage alphaImage(faceSize.toSize(), QImage::Format_ARGB32);
	QPainter alphaPainter(&alphaImage);
	QLinearGradient alphaGradient(0, faceSize.height() * (1 - m_reflectionSize), 0, faceSize.height());
	alphaGradient.setColorAt(0, Qt::black);
	alphaGradient.setColorAt(1, Qt::lightGray);
	alphaPainter.setPen(Qt::NoPen);
	alphaPainter.fillRect(QRectF(0, 0, faceSize.width(), faceSize.height()), alphaGradient);
	blendImage.setAlphaChannel(alphaImage);
	alphaPainter.end();

	painter->save();
	painter->drawImage(0, 0, blendImage);
	painter->restore();
}

QSizeF Carton::faceSize(Faces face) const
{
	qreal width;
	qreal height;

	// width
	switch (face) {
		case Left:
		case Right:
		case LeftReflection:
		case RightReflection:
			width = m_boxDepth;
			break;
		default:
			width = m_boxWidth;
	}

	// height
	switch (face) {
		case Top:
		case Bottom:
			height = m_boxDepth;
			break;
		default:
			height = m_boxHeight;
	}

	return QSizeF(width, height);
}

QTransform Carton::transform(Faces face) const
{
	QTransform result;

	QSizeF originalSize(faceSize(face));
	QPolygonF original(4);
	original[0] = QPointF(0,                    0);                     // TopLeft
	original[1] = QPointF(originalSize.width(), 0);                     // TopRight
	original[2] = QPointF(originalSize.width(), originalSize.height()), // BottomRight
	original[3] = QPointF(0,                    originalSize.height()); // BottomLeft

	bool possible = QTransform::quadToQuad(original, face2d(face), result);
	Q_ASSERT(possible);

	return result;
}

void Carton::boxVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const
{
	// x coordinate
	switch (vertex) {
		case LeftTopFront:
		case LeftBottomFront:
		case LeftSubFront:
		case LeftTopBack:
		case LeftBottomBack:
		case LeftSubBack:
			x = -(m_boxWidth / 2);
			break;
		default:
			x = m_boxWidth / 2;
	}

	// y coordinate
	switch (vertex) {
		case LeftTopFront:
		case LeftTopBack:
		case RightTopFront:
		case RightTopBack:
			y = m_boxHeight;
			break;
		case LeftBottomFront:
		case LeftBottomBack:
		case RightBottomFront:
		case RightBottomBack:
			y = 0;
			break;
		default:
			y = -m_boxHeight;
	}

	// z coordinate
	switch (vertex) {
		case LeftTopBack:
		case LeftBottomBack:
		case LeftSubBack:
		case RightTopBack:
		case RightBottomBack:
		case RightSubBack:
			z = -(m_boxDepth / 2);
			break;
		default:
			z = m_boxDepth / 2;
	}
}

void Carton::rotatedVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const
{
	boxVertex3d(vertex, x, y, z);

	qreal xRotation = (m_xRotation + 180) / PI / 18;
	qreal yRotation = m_yRotation / PI / 18;

	// Rotate vertices
	// from http://sfx.co.nz/tamahori/thought/shock_3d_howto.html#transforming
	qreal temp_z = z * cos(yRotation) - x      * sin(yRotation);
	x =            z * sin(yRotation) + x      * cos(yRotation);
	z =            y * sin(xRotation) + temp_z * cos(xRotation);
	y =            y * cos(xRotation) - temp_z * sin(xRotation);
}

QPointF Carton::vertex2d(Vertices vertex) const
{
	QPointF result;
	qreal x, y, z;
	rotatedVertex3d(vertex, x, y, z);

	// Project 3D point onto 2D plane
	// from http://sfx.co.nz/tamahori/thought/shock_3d_howto.html#displaying
	qreal d = 900.;
	qreal scalar = 1. / ((z / d) + 1.);
	qreal voodoo = 20. - qMax(qMin(38-24*scalar, 17.), 10.);
	x *= scalar;
	y *= scalar;

	result.setX(x);
	result.setY(y);

	return result;
}

QPolygonF Carton::face2d(Faces face) const
{
	QPolygonF result;
	foreach (const Vertices vertex, m_facesVerticesHash[face])
		result << vertex2d(vertex);
	return result;
}

QHash<Carton::Faces, QVector<Carton::Vertices> > Carton::facesVerticesHash()
{
	static const struct {
		Faces face;
		Vertices vertices[4]; //topLeft, topRight, bottomRight, BottomLeft
	} verticesOfFaces[] = {
		{Front,           {LeftTopFront, RightTopFront, RightBottomFront, LeftBottomFront}},
		{Left,            {LeftTopBack, LeftTopFront, LeftBottomFront, LeftBottomBack}},
		{Right,           {RightTopFront, RightTopBack, RightBottomBack, RightBottomFront}},
		{Back,            {RightTopBack, LeftTopBack, LeftBottomBack, RightBottomBack}},
		{Top,             {LeftTopBack, RightTopBack, RightTopFront, LeftTopFront}},
		{Bottom,          {LeftBottomBack, RightBottomBack, RightBottomFront, LeftBottomFront}},
		{FrontReflection, {LeftSubFront, RightSubFront, RightBottomFront, LeftBottomFront}},
		{LeftReflection,  {LeftSubBack, LeftSubFront, LeftBottomFront, LeftBottomBack}},
		{RightReflection, {RightSubFront, RightSubBack, RightBottomBack, RightBottomFront}},
		{BackReflection,  {RightSubBack, LeftSubBack, LeftBottomBack, RightBottomBack}}
	};
	static const size_t facesCount = sizeof(verticesOfFaces)/sizeof(verticesOfFaces[0]);
	static const size_t verticesPerFaceCount = sizeof(verticesOfFaces[0].vertices)/sizeof(verticesOfFaces[0].vertices[0]);

	static QHash<Faces, QVector<Vertices> > result;
	if (result.empty()) {
		for (size_t faceIndex = 0; faceIndex < facesCount; faceIndex++) {
			QVector<Vertices> vertices(verticesPerFaceCount);
			for (size_t vertexIndex = 0; vertexIndex < verticesPerFaceCount; vertexIndex++)
				vertices[vertexIndex] = verticesOfFaces[faceIndex].vertices[vertexIndex];
			result[verticesOfFaces[faceIndex].face] = vertices;
		}
	}

	return result;
}

QVector<Carton::Vertices> Carton::verticesOfFace(Faces face)
{
	Q_ASSERT(m_facesVerticesHash.contains(face));
	return m_facesVerticesHash[face];
}
