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

#include "carton.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QtDebug>

class PainterWidget : public QWidget
{
public:
	PainterWidget(QWidget *parent = 0)
		: QWidget(parent)
	{
		m_carton = new Carton(this);
		QImage frontImage("");
	}

	void paintEvent(QPaintEvent *event)
	{
		m_carton->paint(this);
	}

private:
	Carton *m_carton;
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qDebug() << Carton::verticesOfFace(Carton::Top);
	PainterWidget pw;
	pw.show();
	return a.exec();
}
