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
#include "undostack.h"
#include <QApplication>
#include <QImage>
#include <QtDebug>

void savePng(Cartonizer *cartonizer)
{
	QImage pngImage(800, 800, QImage::Format_ARGB32);
	pngImage.fill(qRgba(255, 255, 255, 0));
	cartonizer->paint(&pngImage, QRectF(0, 0, 800, 800), CartonizerEnums::Antialiased);
	pngImage.save("carton.png");
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Cartonizer cartonizer;
	CartonizerController controller;
	CartonizerMainWindow w;
	controller.setModelAndView(&cartonizer, &w);
	controller.handleViewPropertyChanged("frontFace", ":/faces/front.png");
	controller.handleViewPropertyChanged("leftFace", ":/faces/left.png");
	controller.handleViewPropertyChanged("rightFace", ":/faces/left.png");
	controller.handleViewPropertyChanged("topFace", ":/faces/top.png");
	UndoStack::instance()->clear();
	w.show();
//	savePng(&cartonizer);
	return a.exec();
}
