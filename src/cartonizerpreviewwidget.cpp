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

#include "cartonizerpreviewwidget.h"

#include <QPainter>
#include <QTime>
#include <QMouseEvent>
#include <QtDebug>

CartonizerPreviewWidget::CartonizerPreviewWidget(QWidget *parent)
    : QWidget(parent)
    , m_background(Qt::black)
    , m_cartonChangeIsPainted(false)
    , m_antiAliasTimerID(0)
{
}

void CartonizerPreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.fillRect(rect(), m_background);
    p.end();
    QTime time;
    time.start();
    emit needsPaint(this, rect(), m_cartonChangeIsPainted ? CartonizerEnums::Aliased : CartonizerEnums::Antialiased);
    const int elapsedMilliseconds = time.elapsed();
    if (m_cartonChangeIsPainted) {
        m_cartonChangeIsPainted = false;
        killTimer(m_antiAliasTimerID);
        m_antiAliasTimerID = startTimer(elapsedMilliseconds*1.5);
    }
}

void CartonizerPreviewWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    m_cartonChangeIsPainted = false;
    update();
    killTimer(m_antiAliasTimerID);
}

void CartonizerPreviewWidget::mousePressEvent(QMouseEvent *event)
{
    m_previousMousePosition = event->pos();
}

void CartonizerPreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    const int mouseXOffset = event->x() - m_previousMousePosition.x();
    const int mouseYOffset = event->y() - m_previousMousePosition.y();
    emit mouseManipulation(mouseXOffset, mouseYOffset);
    m_previousMousePosition = event->pos();
}

void CartonizerPreviewWidget::updatePreview()
{
    m_cartonChangeIsPainted = true;
    update();
}
