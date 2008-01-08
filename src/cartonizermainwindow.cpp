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

#include "cartonizermainwindow.h"

CartonizerMainWindow::CartonizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	connect(m_xRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleXRotationChanged(double)));
	connect(m_yRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleYRotationChanged(double)));
	connect(m_focalLengthSpinbox, SIGNAL(valueChanged(double)), this, SLOT(handleFocalLengthChanged(double)));
	connect(m_previewWidget, SIGNAL(needsPaint(QPaintDevice *)), this, SIGNAL(needsPreviewPaint(QPaintDevice *)));
}

void CartonizerMainWindow::updatePreview()
{
	m_previewWidget->update();
}

void CartonizerMainWindow::handleXRotationChanged(double rotation)
{
	emit propertyChanged("xRotation", rotation);
}

void CartonizerMainWindow::handleYRotationChanged(double rotation)
{
	emit propertyChanged("yRotation", rotation);
}

void CartonizerMainWindow::handleFocalLengthChanged(double length)
{
	emit propertyChanged("focalLength", length);
}
