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

#include "cartonizercontroller.h"
#include "undostack.h"
#include "cartonizercommands.h"
#include "cartonizertools.h"
#include "cartonizer.h"
#include <QMetaProperty>
#include <QPixmap>
#include <QPainter>
#include <QVariant>
#include <QFileDialog>

CartonizerController::CartonizerController(QObject *parent)
	: QObject(parent)
	, m_model(NULL)
	, m_view(NULL)
	, m_cartonSaveSize(400, 400)
{
	connect(UndoStack::instance(), SIGNAL(indexChanged(int)), SIGNAL(cartonChanged()));
}

void CartonizerController::setModelAndView(QObject *model, QObject *view)
{
	m_model = model;
	m_view = view;
	connect(view, SIGNAL(needsPreviewPaint(QPaintDevice *, const QRectF &, CartonizerEnums::paintQuality)), model, SLOT(paint(QPaintDevice *, const QRectF &, CartonizerEnums::paintQuality)));
	connect(this, SIGNAL(needsCartonPaint(QPaintDevice *, const QRectF &, CartonizerEnums::paintQuality)), model, SLOT(paint(QPaintDevice *, const QRectF &, CartonizerEnums::paintQuality)));
	connect(view, SIGNAL(propertyChanged(const char*, const QVariant&)), SLOT(handleViewPropertyChanged(const char*, const QVariant&)));
	connect(view, SIGNAL(saveCartonRequested()), SLOT(saveCarton()));
	connect(this, SIGNAL(cartonChanged()), view, SLOT(updatePreview()));
	const bool viewSelectsAndFocusses = view->property(CartonizerProperties::selectAndFocus).toBool();
	view->setProperty(CartonizerProperties::selectAndFocus, QVariant(false));
	for (int i = 0; i < model->metaObject()->propertyCount(); i++) {
		const char* propertyName = model->metaObject()->property(i).name();
		if (!strcmp(propertyName, "objectName") == 0)
			view->setProperty(propertyName, model->property(propertyName));
	}
	view->setProperty(CartonizerProperties::selectAndFocus, viewSelectsAndFocusses);
}

void CartonizerController::handleViewPropertyChanged(const char *name, const QVariant &value)
{
	const QVariant propertyValue = transformViewToModelProperty(name, value);
	PropertyCommand *command = new PropertyCommand(m_model, m_view, name, propertyValue);
	UndoStack::instance()->push(command);
}

bool CartonizerController::saveCarton()
{
	const QString fileName = 
		QFileDialog::getSaveFileName(
		NULL, tr("Save File"), "untitled.png", tr("Png (*.png)"));
	return !fileName.isEmpty()?saveCarton(fileName):false;
}

bool CartonizerController::saveCarton(const QString &fileName)
{
	QImage image(m_cartonSaveSize, QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	emit needsCartonPaint(&image, image.rect(), CartonizerEnums::HighQuality);
	return image.save(fileName);
}


QVariant CartonizerController::transformViewToModelProperty(const char *propertyName, const QVariant &viewValue) const
{
	QVariant result;
	if (strcmp(propertyName, "frontFace") == 0
		|| strcmp(propertyName, "leftFace") == 0
		|| strcmp(propertyName, "topFace") == 0
		|| strcmp(propertyName, "rightFace") == 0
		|| strcmp(propertyName, "combinedFaces") == 0) {
		const QString &imageFileName = viewValue.toString();
		result = QPixmap(imageFileName);
	} else {
		result = viewValue;
	}
	return result;
}
