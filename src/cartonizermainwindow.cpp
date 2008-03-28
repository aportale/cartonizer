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
#include "actionstoolbar.h"
#include "cartonizerproperties.h"

#include <QFileDialog>

CartonizerMainWindow::CartonizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_selectAndFocus(true)
{
	setupUi(this);
	ActionsToolbar *toolBar = new ActionsToolbar(this);
	addToolBar(toolBar);
	connect(toolBar, SIGNAL(save()), SLOT(do_save()));
	connect(m_previewWidget, SIGNAL(needsPaint(QPaintDevice *, const QRectF&, bool)), this, SIGNAL(needsPreviewPaint(QPaintDevice *, const QRectF&, bool)));
}

qreal CartonizerMainWindow::xRotation() const
{
	return xRotationSpinBox->value();
}

qreal CartonizerMainWindow::yRotation() const
{
	return yRotationSpinBox->value();
}

qreal CartonizerMainWindow::observerHeight() const
{
	return observerHeightSpinBox->value();
}

qreal CartonizerMainWindow::specularityValue() const
{
	return specularityValueSpinBox->value();
}

qreal CartonizerMainWindow::focalLength() const
{
	return focalLengthSpinBox->value();
}

bool CartonizerMainWindow::selectAndFocus() const
{
	return m_selectAndFocus;
}

void CartonizerMainWindow::setXRotation(qreal rotation)
{
	updateSpinBoxValue(xRotationSpinBox, rotation);
}

void CartonizerMainWindow::setYRotation(qreal rotation)
{
	updateSpinBoxValue(yRotationSpinBox, rotation);
}

void CartonizerMainWindow::setObserverHeight(qreal height)
{
	updateSpinBoxValue(observerHeightSpinBox, height);
}

void CartonizerMainWindow::setSpecularityValue(qreal value)
{
	updateSpinBoxValue(specularityValueSpinBox, value);
}

void CartonizerMainWindow::setFocalLength(qreal length)
{
	updateSpinBoxValue(focalLengthSpinBox, length);
}

void CartonizerMainWindow::setSelectAndFocus(bool selectAndFocus)
{
	m_selectAndFocus = selectAndFocus;
}

void CartonizerMainWindow::updatePreview()
{
	m_previewWidget->update();
}

void CartonizerMainWindow::updatePropery(const char *name, const QVariant &value)
{
	setProperty(name, value);
}

void CartonizerMainWindow::on_xRotationSpinBox_valueChanged(double rotation)
{
	emit propertyChanged(CartonizerProperties::xRotation, rotation);
}

void CartonizerMainWindow::on_yRotationSpinBox_valueChanged(double rotation)
{
	emit propertyChanged(CartonizerProperties::yRotation, rotation);
}

void CartonizerMainWindow::on_observerHeightSpinBox_valueChanged(double height)
{
	emit propertyChanged(CartonizerProperties::observerHeight, height);
}

void CartonizerMainWindow::on_focalLengthSpinBox_valueChanged(double length)
{
	emit propertyChanged(CartonizerProperties::focalLength, length);
}

void CartonizerMainWindow::on_specularityValueSpinBox_valueChanged(double value)
{
	emit propertyChanged(CartonizerProperties::specularityValue, value);
}

void CartonizerMainWindow::do_save()
{
	const QString fileName = 
		QFileDialog::getSaveFileName(
			this, tr("Save File"), "untitled.png", tr("Images (*.png *.xpm *.jpg)"));
	if (!fileName.isEmpty())
		emit saveImage(fileName, rect().size());
}

void CartonizerMainWindow::updateSpinBoxValue(QDoubleSpinBox *spinBox, double value)
{
	const bool signalsOriginallyBlocked = spinBox->blockSignals(true);
	spinBox->setValue(value);
	spinBox->blockSignals(signalsOriginallyBlocked);
	if (m_selectAndFocus) {
		spinBox->selectAll();
		spinBox->setFocus();
	}
}
