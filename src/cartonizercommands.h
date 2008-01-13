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

#ifndef CARTONIZERCOMMANDS_H
#define CARTONIZERCOMMANDS_H

#include <QUndoCommand>
#include <QVariant>

class CartonizerCommand : public QObject, public QUndoCommand
{
	Q_OBJECT

public:
	CartonizerCommand(QObject *model, QObject *view);

protected:
	QObject *m_model;
	QObject *m_view;
};

class PropertyCommand : public CartonizerCommand
{
	Q_OBJECT

public:
	PropertyCommand(QObject *model, QObject *view, const char *propertyName, const QVariant &propertyValue);
	void undo();
	void redo();
	int id() const;
	bool mergeWith(const QUndoCommand *command);

private:
	const char *m_propertyName;
	QVariant m_value;
	QVariant m_oldValue;
};

#endif
