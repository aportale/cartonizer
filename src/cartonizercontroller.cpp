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

CartonizerController::CartonizerController(QObject *parent)
	: QObject(parent)
	, m_model(NULL)
	, m_view(NULL)
{
	connect(UndoStack::instance(), SIGNAL(indexChanged(int)), this, SIGNAL(cartonChanged()));
}

void CartonizerController::setModelAndView(QObject *model, QWidget *view)
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
		view->setProperty(propertyName, view->setProperty(propertyName, model->property(propertyName)));
	}
	view->setProperty(CartonizerProperties::selectAndFocus, viewSelectsAndFocusses);
}

void CartonizerController::handleViewPropertyChanged(const char *name, const QVariant &value)
{
	PropertyCommand *command = new PropertyCommand(m_model, name, value);
	connect(command, SIGNAL(propertyCanged(const char*, const QVariant&)), m_view, SLOT(updatePropery(const char*, const QVariant&)));
	UndoStack::instance()->push(command);
}
