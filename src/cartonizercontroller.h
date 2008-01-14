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

#ifndef CARTONIZERCONTROLLER_H
#define CARTONIZERCONTROLLER_H

#include <QObject>

class QPaintDevice;

class CartonizerController : public QObject
{
	Q_OBJECT

public:
	CartonizerController(QObject *parent = 0);

	void setModelAndView(QObject *model, QObject *view);

signals:
	void cartonChanged();

public slots:
	void handleViewPropertyChanged(const char *name, const QVariant &value);

private:
	QVariant transformViewToModelProperty(const char *propertyName, const QVariant &viewValue) const;

	QObject *m_model;
	QObject *m_view;
};

#endif
