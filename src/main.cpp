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
#include "cartonizercontroller.h"
#include "cartonizermainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QVariant>
#include <QtDebug>

class PainterWidget : public QWidget
{
public:
	PainterWidget(QWidget *parent = 0)
		: QWidget(parent)
	{
		m_cartonizer = new Cartonizer(this);
		m_cartonizer->setProperty("xRotation", -2.);
		m_cartonizer->setProperty("yRotation", 1.2);
		m_cartonizer->setProperty("reflectionSize", .5);
		QImage frontImage("");
	}

	void paintEvent(QPaintEvent *event)
	{
		m_cartonizer->paint(this);
	}

private:
	Cartonizer *m_cartonizer;
};

void savePng()
{
	Cartonizer cartonizer;
	QImage pngImage(800, 800, QImage::Format_ARGB32);
	pngImage.fill(qRgba(255, 255, 255, 0));
	cartonizer.paint(&pngImage);
	pngImage.save("carton.png");
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	PainterWidget pw;
//	pw.show();
//	savePng();
	Cartonizer cartonizer;
	CartonizerController controller;
	CartonizerMainWindow w;
	controller.setModelAndView(&cartonizer, &w);
	w.show();
	return a.exec();
}
