#include "carton.h"
#include <QPainter>

Carton::Carton(QObject *parent)
    : QObject(parent)
{
	setTransform(defaultTransform());
	setImage(Front, defaultImage(Front, QSize(m_defaultWidth, m_defaultHeight)));
}

void Carton::paint(QPaintDevice *paintDevice)
{
	QPainter painter(paintDevice);
	painter.save();
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setTransform(m_transform * QTransform().translate(paintDevice->width() / 4, paintDevice->height() / 4));
	painter.drawImage(0, 0, m_faceImages[Front]);
	painter.restore();
}

void Carton::setTransform(QTransform transform)
{
	m_transform = transform;
}

QTransform Carton::transform() const
{
	return m_transform;
}

void Carton::setImage(Faces face, QImage image)
{
	m_faceImages[face] = image;
}

QImage Carton::image(Faces face) const
{
	return m_faceImages[face];
}

QTransform Carton::defaultTransform()
{
	QTransform result;
	result.rotate(-30.0, Qt::YAxis);
	return result;
}

QImage Carton::defaultImage(Faces face, QSize size)
{
	QImage result(size, QImage::Format_RGB32);
	QPainter painter(&result);
	painter.setBrush(Qt::black);
	painter.drawRect(result.rect());
	painter.setBrush(Qt::white);
	painter.drawRect(result.rect().adjusted(3, 3, -4, -4));
	QString faceCaption = face==Front?"Front":"Blah";
	painter.drawText(result.rect(), Qt::AlignCenter, faceCaption);
	return result;
}
