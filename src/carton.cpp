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

const qreal Carton::m_defaultWidth = 120.;
const qreal Carton::m_defaultHeight = 200.;
const qreal Carton::m_defaultDepth = 80.;
const QHash<Carton::Faces, QVector<Carton::Vertices> > Carton::m_facesVerticesHash = facesVerticesHash();

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
{
	setImage(Front, defaultImage(Front, QSize(m_boxWidth, m_boxHeight)));
	setImage(Left, defaultImage(Left, QSize(m_boxDepth, m_boxHeight)));
	setImage(Right, defaultImage(Right, QSize(m_boxDepth, m_boxHeight)));
	setImage(Top, defaultImage(Top, QSize(m_boxWidth, m_boxDepth)));
}

void Carton::paint(QPaintDevice *paintDevice)
{
	QPainter painter(paintDevice);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//	painter.setOpacity(0.5);
	paintVertices(&painter);

	paintFace(&painter, Front);
	paintFace(&painter, Left);
	paintFace(&painter, Top);

	painter.restore();
}

void Carton::paintVertices(QPainter *painter)
{
	painter->save();
	painter->translate(m_xOffset, m_yOffset);

	QMetaEnum verticesEnum = metaObject()->enumerator(metaObject()->indexOfEnumerator("Vertices"));
	for (int vertexIndex = 0; vertexIndex < verticesEnum.keyCount(); vertexIndex++) {
		Vertices vertex = (Vertices)verticesEnum.value(vertexIndex);
		painter->drawEllipse(vertex2d(vertex), 1.5, 1.5);
	}

	painter->restore();
}

void Carton::paintFace(QPainter *painter, Faces face)
{
	painter->save();
	painter->setTransform(transform(face) * QTransform().translate(m_xOffset, m_yOffset));
	painter->drawImage(0, 0, m_faceImages[face]);
	painter->restore();
}

void Carton::setImage(Faces face, QImage image)
{
	m_faceImages[face] = image;
}

QImage Carton::image(Faces face) const
{
	return m_faceImages[face];
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

	QPolygonF projected;
	foreach (const Vertices vertex, m_facesVerticesHash[face])
		projected << vertex2d(vertex);
	qDebug() << projected;

	bool possible = QTransform::quadToQuad(original, projected, result);
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

	// Rotate vertices
	// from http://sfx.co.nz/tamahori/thought/shock_3d_howto.html#transforming
	qreal temp_z = z * cos(m_yRotation) - x      * sin(m_yRotation);
	x =            z * sin(m_yRotation) + x      * cos(m_yRotation);
	z =            y * sin(m_xRotation) + temp_z * cos(m_xRotation);
	y =            y * cos(m_xRotation) - temp_z * sin(m_xRotation);
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

QImage Carton::defaultImage(Faces face, QSize size)
{
	QImage result(size, QImage::Format_RGB32);
	QPainter painter(&result);
	painter.setBrush(Qt::black);
	painter.drawRect(result.rect());
	painter.setBrush(Qt::white);
	painter.drawRect(result.rect().adjusted(3, 3, -4, -4));
	QString faceCaption =
		face==Front?"Front"
		:face==Left?"Left"
		:face==Right?"Right"
		:"Top";
	QFont font;
	font.setPixelSize(30);
	painter.setFont(font);
	painter.drawText(result.rect(), Qt::AlignCenter, faceCaption);
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
		{FrontReflection, {LeftBottomFront, RightBottomFront, RightSubFront, LeftSubFront}},
		{LeftReflection,  {LeftBottomBack, LeftBottomFront, LeftSubFront, LeftSubBack}},
		{RightReflection, {RightBottomFront, RightBottomBack, RightSubBack, LeftSubBack}},
		{BackReflection,  {RightBottomBack, LeftBottomBack, LeftSubBack, RightSubBack}}
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
