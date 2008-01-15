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
#include "cartonizerproperties.h"
#include <QWidget>
#include <QMetaProperty>
#include <QVariant>
#include <QFileInfo>
#include <QPicture>
#include <QPainter>
#include <QSvgRenderer>

Q_DECLARE_METATYPE(QPicture)

CartonizerController::CartonizerController(QObject *parent)
	: QObject(parent)
	, m_model(NULL)
	, m_view(NULL)
{
	connect(UndoStack::instance(), SIGNAL(indexChanged(int)), this, SIGNAL(cartonChanged()));
}

void CartonizerController::setModelAndView(QObject *model, QObject *view)
{
	m_model = model;
	m_view = view;
	connect(view, SIGNAL(needsPreviewPaint(QPaintDevice *)), model, SLOT(paint(QPaintDevice *)));
	connect(view, SIGNAL(propertyChanged(const char*, const QVariant&)), SLOT(handleViewPropertyChanged(const char*, const QVariant&)));
	connect(this, SIGNAL(cartonChanged()), view, SLOT(updatePreview()));
	bool viewSelectsAndFocusses = view->property(CartonizerProperties::selectAndFocus).toBool();
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
	QVariant propertyValue = transformViewToModelProperty(name, value);
	PropertyCommand *command = new PropertyCommand(m_model, m_view, name, propertyValue);
	UndoStack::instance()->push(command);
}

QVariant CartonizerController::transformViewToModelProperty(const char *propertyName, const QVariant &viewValue) const
{
	QVariant result;
	if (strcmp(propertyName, "frontFace") == 0) {
		QString imageFileName = viewValue.toString();
		QFileInfo imageFileInfo(imageFileName);
		QString imageFileSuffix = imageFileInfo.suffix();
		QPicture facePicture;
		QPainter facePainter(&facePicture);
		if (imageFileSuffix == QLatin1String("svg")) {
			QSvgRenderer svgRenderer(imageFileName);
			svgRenderer.render(&facePainter, svgRenderer.viewBox());
		} else {

		}
		result = qVariantFromValue(facePicture);
	} else {
		result = viewValue;
	}
	return result;
}
