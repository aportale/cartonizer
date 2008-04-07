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

#include "carton.h"
#include <QPainter>
#include <QtDebug>
#include <math.h>

const qreal Carton::m_defaultWidth = 140.;
const qreal Carton::m_defaultHeight = 200.;
const qreal Carton::m_defaultDepth = 60.;
const QHash<Carton::Faces, QVector<Carton::Vertices> > &Carton::m_facesVerticesHash = facesVerticesHash();
const qreal Carton::PI = 3.14159265358979323846; // Source: http://en.wikipedia.org/wiki/Pi

Carton::Carton(QObject *parent)
	: QObject(parent)
	, m_xRotation(0)
	, m_yRotation(0)
	, m_observerHeight(0)
	, m_focalLength(900)
	, m_specularityValue(40)
	, m_boxWidth(m_defaultWidth)
	, m_boxHeight(m_defaultHeight)
	, m_boxDepth(m_defaultDepth)
{
}

void Carton::paint(QPaintDevice *paintDevice, const QRectF &rect, CartonizerEnums::paintQuality quality)
{
	if (quality == CartonizerEnums::HighQuality) {
		const int highResolutionFactor = 3;
		QSizeF highResolutionImageSize(rect.size().width() * highResolutionFactor, rect.size().height() * highResolutionFactor);
		const QRectF highResolutionImageRect(QPointF(), highResolutionImageSize);
		QImage highResolutionImage(highResolutionImageSize.toSize(), QImage::Format_ARGB32);
		highResolutionImage.fill(Qt::transparent);
		paint(&highResolutionImage, highResolutionImageRect, CartonizerEnums::Antialiased);
		QPainter painter(paintDevice);
		painter.drawImage(0, 0, highResolutionImage.scaled(rect.size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		return;
	}

	QPainter painter(paintDevice);
	painter.setRenderHint(QPainter::Antialiasing, quality == CartonizerEnums::Antialiased);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, quality == CartonizerEnums::Antialiased);

	const QRectF boundingRect(this->boundingRect());
	const qreal scaling = qMin(rect.width() / boundingRect.width(), rect.height() / boundingRect.height());
	const QRectF scaledBoundingRect(boundingRect.left() * scaling, boundingRect.top() * scaling,
		boundingRect.width() * scaling, boundingRect.height() * scaling);
	const QPointF translation(rect.center() - scaledBoundingRect.center());

	if (isFaceVisibleFromFront(Top))
		paintFace(&painter, Top, translation, scaling);
	if (isFaceVisibleFromFront(Front)) {
		paintFace(&painter, FrontReflection, translation, scaling);
		paintFace(&painter, Front, translation, scaling);
	} else if (isFaceVisibleFromFront(Back)) {
		paintFace(&painter, BackReflection, translation, scaling);
		paintFace(&painter, Back, translation, scaling);
	}
	if (isFaceVisibleFromFront(Left)) {
		paintFace(&painter, LeftReflection, translation, scaling);
		paintFace(&painter, Left, translation, scaling);
	} else if (isFaceVisibleFromFront(Right)) {
		paintFace(&painter, Right, translation, scaling);
		paintFace(&painter, RightReflection, translation, scaling);
	}
}

void Carton::paintFace(QPainter *painter, Faces face, const QPointF &translation, qreal scaling)
{
	painter->save();
	const QTransform faceTransform(transform(face, translation, scaling));
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
	if (m_specularityValue <= 0)
		return;

	const Faces emittingFace =
		face == FrontReflection?Front
		:face == BackReflection?Back
		:face == LeftReflection?Left
		:/* face == RightReflection? */Right;

	const QSizeF faceSize(this->faceSize(face));
	const qreal faceWith = faceSize.width();
	const qreal faceHeight = faceSize.height();
	const qreal reflectionHeight = faceHeight/100 * m_specularityValue;
	const qreal remainingFaceHeight = faceHeight - reflectionHeight;
	QImage blendImage(QSize(faceWith, reflectionHeight), QImage::Format_ARGB32);
	QPainter blendPainter(&blendImage);
	blendPainter.translate(0, -remainingFaceHeight);
	paintFaceTexture(&blendPainter, emittingFace);
	blendPainter.end();

	QImage alphaImage(QSize(faceWith, reflectionHeight), QImage::Format_ARGB32);
	QPainter alphaPainter(&alphaImage);
	QLinearGradient alphaGradient(0, 0, 0, faceHeight);
	alphaGradient.setColorAt(0, Qt::black);
	alphaGradient.setColorAt(1, Qt::lightGray);
	alphaPainter.setPen(Qt::NoPen);
	alphaPainter.fillRect(QRectF(0, 0, faceWith, reflectionHeight), alphaGradient);
	blendImage.setAlphaChannel(alphaImage);
	alphaPainter.end();

	painter->save();
	painter->drawImage(0, remainingFaceHeight, blendImage);
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

QTransform Carton::transform(Faces face, const QPointF &translation, qreal scaling) const
{
	QTransform result;

	const QSizeF originalSize(faceSize(face));
	QPolygonF original(4);
	original[0] = QPointF(0,                    0);                     // TopLeft
	original[1] = QPointF(originalSize.width(), 0);                     // TopRight
	original[2] = QPointF(originalSize.width(), originalSize.height()), // BottomRight
	original[3] = QPointF(0,                    originalSize.height()); // BottomLeft

	QPolygonF targetFace = face2d(face);
	QPolygonF::iterator i;
	for (i = targetFace.begin(); i != targetFace.end(); ++i) {
		(*i).setX((*i).x() * scaling + translation.x());
		(*i).setY((*i).y() * scaling + translation.y());
	}

	const bool possible = QTransform::quadToQuad(original, targetFace, result);
	Q_ASSERT(possible);

	return result;
}

void Carton::boxVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const
{
	// x coordinate
	switch (vertex) {
		case LeftTopFront:
		case LeftBottomFront:
		case LeftEffectiveSubFront:
		case LeftSubFront:
		case LeftTopBack:
		case LeftBottomBack:
		case LeftEffectiveSubBack:
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
		case LeftEffectiveSubFront:
		case LeftEffectiveSubBack:
		case RightEffectiveSubFront:
		case RightEffectiveSubBack:
			y = -m_boxHeight/100 * m_specularityValue;
			break;
		default:
			y = -m_boxHeight;
	}

	// z coordinate
	switch (vertex) {
		case LeftTopBack:
		case LeftBottomBack:
		case LeftEffectiveSubBack:
		case LeftSubBack:
		case RightTopBack:
		case RightBottomBack:
		case RightEffectiveSubBack:
		case RightSubBack:
			z = -(m_boxDepth / 2);
			break;
		default:
			z = m_boxDepth / 2;
	}

	y -= m_boxHeight * m_observerHeight / 100;
}

void Carton::rotatedVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const
{
	boxVertex3d(vertex, x, y, z);

	const qreal xRotation = (m_xRotation - 180) * PI / 180.;
	const qreal yRotation = m_yRotation * PI / 180.;

	// Rotate vertices
	// from http://sfx.co.nz/tamahori/thought/shock_3d_howto.html#transforming
	const qreal temp_z = z * cos(yRotation) - x      * sin(yRotation);
	x =                  z * sin(yRotation) + x      * cos(yRotation);
	z =                  y * sin(xRotation) + temp_z * cos(xRotation);
	y =                  y * cos(xRotation) - temp_z * sin(xRotation);
}

QPointF Carton::vertex2d(Vertices vertex) const
{
	QPointF result;
	qreal x, y, z;
	rotatedVertex3d(vertex, x, y, z);

	// Project 3D point onto 2D plane
	// from http://sfx.co.nz/tamahori/thought/shock_3d_howto.html#displaying
	const qreal scalar = 1. / ((z / m_focalLength) + 1.);
	x *= scalar;
	y *= scalar;

	result.setX(x);
	result.setY(y);

	return result;
}

QPolygonF Carton::face2d(Faces face) const
{
	QPolygonF result;
	foreach (const Vertices &vertex, m_facesVerticesHash.value(face))
		result.append(vertex2d(vertex));
	return result;
}

bool Carton::isFaceVisibleFromFront(Faces face) const
{
	// 2D Polygon Backface Culling
	const QPolygonF vertices2d(face2d(face));
	const qreal
		x1 = vertices2d.at(0).x(),
		y1 = vertices2d.at(0).y(),
		x2 = vertices2d.at(1).x(),
		y2 = vertices2d.at(1).y(),
		x3 = vertices2d.at(2).x(),
		y3 = vertices2d.at(2).y();

	// code line from http://www.cgafaq.info/wiki/2D_Polygon_Backface_Culling
	return (x1-x2)*(y3-y2)-(y1-y2)*(x3-x2) < 0;
}

QPolygonF Carton::outline() const
{
#if QT_VERSION >= 0x040400
	QPainterPath painterPath;

	if (isFaceVisibleFromFront(Top))
		painterPath.addPolygon(face2d(Top));
	if (isFaceVisibleFromFront(Left))
		painterPath.addPolygon(face2d(Left));
	else if (isFaceVisibleFromFront(Right))
		painterPath.addPolygon(face2d(Right));
	if (isFaceVisibleFromFront(Front))
		painterPath.addPolygon(face2d(Front));
	else if (isFaceVisibleFromFront(Back))
		painterPath.addPolygon(face2d(Back));

	return painterPath.simplified().toFillPolygon();
#else
	return QPolygonF();
#endif
}

QRectF Carton::boundingRect() const
{
	QPolygonF vertices(8);
	static const Vertices boundingVertices[] =	{
		LeftTopBack, RightTopBack, RightTopFront, LeftTopFront, 
		LeftEffectiveSubBack, RightEffectiveSubBack, RightEffectiveSubFront, LeftEffectiveSubFront
	};
	const int boundingVerticesCount = sizeof(boundingVertices)/sizeof(boundingVertices[0]);
	for (int i = 0; i < boundingVerticesCount; i++)
		vertices.append(vertex2d(boundingVertices[i]));
	return vertices.boundingRect();
}

const QHash<Carton::Faces, QVector<Carton::Vertices> > &Carton::facesVerticesHash()
{
	static const struct {
		const Faces face;
		const Vertices vertices[4]; //topLeft, topRight, bottomRight, BottomLeft
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

const QVector<Carton::Vertices> Carton::verticesOfFace(Faces face)
{
	Q_ASSERT(m_facesVerticesHash.contains(face));
	return m_facesVerticesHash.value(face);
}
