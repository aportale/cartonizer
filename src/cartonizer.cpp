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

#include "cartonizer.h"
#include <QPainter>

Cartonizer::Cartonizer(QObject *parent)
: Carton(parent)
{
}

void Cartonizer::paintFaceTexture(QPainter *painter, Faces face)
{
	QRect faceRect(QPoint(0, 0), faceSize(face).toSize());
	painter->save();
	painter->setBrush(Qt::black);
	painter->drawRect(faceRect);
	painter->setBrush(Qt::white);
	painter->drawRect(faceRect.adjusted(3, 3, -4, -4));
	QString faceCaption =
		(face==Front||face==Back)?"Front"
		:(face==Left||face==Right)?"Side"
		:"Top";
	QFont font;
	font.setPixelSize(20);
	painter->setFont(font);
	painter->drawText(faceRect, Qt::AlignCenter, faceCaption);
	painter->restore();
}

qreal Cartonizer::xRotation() const
{
	return m_xRotation;
}

qreal Cartonizer::yRotation() const
{
	return m_yRotation;
}

qreal Cartonizer::reflectionSize() const
{
	return m_reflectionSize;
}

void Cartonizer::setXRotation(qreal rotation)
{
	m_xRotation = rotation;
}

void Cartonizer::setYRotation(qreal rotation)
{
	m_yRotation = rotation;
}

void Cartonizer::setReflectionSize(qreal size)
{
	m_reflectionSize = size;
}
