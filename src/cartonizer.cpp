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
	painter->drawPixmap(0, 0, 
		face == Front || face == Back?m_frontFace
		:face == Left?m_leftFace
		:face == Top?m_topFace
		:/* face == Right? */m_rightFace
		);
}

qreal Cartonizer::xRotation() const
{
	return m_xRotation;
}

qreal Cartonizer::yRotation() const
{
	return m_yRotation;
}

qreal Cartonizer::observerHeight() const
{
	return m_observerHeight;
}

qreal Cartonizer::specularityValue() const
{
	return m_specularityValue;
}

qreal Cartonizer::focalLength() const
{
	return m_focalLength;
}

QPixmap Cartonizer::frontFace() const
{
	return m_frontFace;
}

QPixmap Cartonizer::leftFace() const
{
	return m_leftFace;
}

QPixmap Cartonizer::topFace() const
{
	return m_topFace;
}

QPixmap Cartonizer::rightFace() const
{
	return m_rightFace;
}

QPixmap Cartonizer::combinedFaces() const
{
	return m_combinedFaces;
}

void Cartonizer::setXRotation(qreal rotation)
{
	m_xRotation = rotation;
}

void Cartonizer::setYRotation(qreal rotation)
{
	m_yRotation = rotation;
}

void Cartonizer::setObserverHeight(qreal height)
{
	m_observerHeight = height;
}

void Cartonizer::setSpecularityValue(qreal value)
{
	m_specularityValue = value;
}

void Cartonizer::setFocalLength(qreal length)
{
	m_focalLength = length;
}

void Cartonizer::setFrontFace(const QPixmap &face)
{
	m_frontFace = face;
	m_boxWidth = face.width();
	m_boxHeight = face.height();
}

void Cartonizer::setLeftFace(const QPixmap &face)
{
	m_leftFace = face;
	m_boxDepth = face.width();
}

void Cartonizer::setTopFace(const QPixmap &face)
{
	m_topFace = face;
}

void Cartonizer::setRightFace(const QPixmap &face)
{
	m_rightFace = face;
	m_boxDepth = face.width();
}

void Cartonizer::setCombinedFaces(const QPixmap &faces)
{
	m_combinedFaces = faces;
}
