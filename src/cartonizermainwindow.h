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

#ifndef CARTONIZERMAINWINDOW_H
#define CARTONIZERMAINWINDOW_H

#include "ui_cartonizermainwindow.h"

class CartonizerMainWindow : public QMainWindow, public Ui::CartonizerMainWindow
{
	Q_OBJECT

public:
	CartonizerMainWindow(QWidget *parent = 0);

signals:
	void propertyChanged(const char *name, const QVariant &value);
	void needsPreviewPaint(QPaintDevice *paintDevice);

public slots:
	void updatePreview();

private slots:
	void on_xRotationSpinBox_valueChanged(double rotation);
	void on_yRotationSpinBox_valueChanged(double rotation);
	void on_observerHeightSpinBox_valueChanged(double height);
	void on_focalLengthSpinbox_valueChanged(double length);
	void on_specularityValueSpinBox_valueChanged(double value);
};

#endif
