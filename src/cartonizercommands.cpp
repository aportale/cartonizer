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

#include "cartonizercommands.h"

CartonizerCommand::CartonizerCommand(QObject *model, QObject *view)
	: m_model(model)
	, m_view(view)
{
}

PropertyCommand::PropertyCommand(QObject *model, QObject *view, const char *propertyName, const QVariant &propertyValue)
	: CartonizerCommand(model, view)
	, m_propertyName(propertyName)
	, m_value(propertyValue)
	, m_oldValue(model->property(propertyName))
{
}

void PropertyCommand::undo()
{
	m_model->setProperty(m_propertyName, m_oldValue);
	m_view->setProperty(m_propertyName, m_oldValue);
}

void PropertyCommand::redo()
{
	m_model->setProperty(m_propertyName, m_value);
	m_view->setProperty(m_propertyName, m_value);
}

int PropertyCommand::id() const
{
	return 10000 + m_model->metaObject()->indexOfProperty(m_propertyName);
}

bool PropertyCommand::mergeWith(const QUndoCommand *command)
{
	const PropertyCommand *otherCommand = static_cast<const PropertyCommand*>(command);
	setText(otherCommand->text());
	m_propertyName = otherCommand->m_propertyName;
	m_value = otherCommand->m_value;
	return true;
}

