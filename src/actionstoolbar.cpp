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

#include "actionstoolbar.h"
#include "undostack.h"
#include <QIcon>
#include <QAction>

ActionsToolbar::ActionsToolbar(QWidget *parent)
	: QToolBar(parent)
{
	m_undoAction = UndoStack::instance()->createUndoAction(this);
	m_undoAction->setIcon(QIcon(":/icons/undo.png"));
	addAction(m_undoAction);
	m_redoAction = UndoStack::instance()->createRedoAction(this);
	m_redoAction->setIcon(QIcon(":/icons/redo.png"));
	addAction(m_redoAction);
	m_saveAction = new QAction(this);
	m_saveAction->setIcon(QIcon(":/icons/save.png"));
	connect(m_saveAction, SIGNAL(triggered()), this, SIGNAL(save()));
	addAction(m_saveAction);
}
