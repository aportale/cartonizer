#include "carton.h"
#include <QPainter>

Carton::Carton(QObject *parent)
    : QObject(parent)
	, m_xOffset(400)
	, m_yOffset(500)
	, m_yRotation(-45)
{
	setImage(Front, defaultImage(Front, QSize(m_defaultWidth, m_defaultHeight)));
	setImage(Side, defaultImage(Side, QSize(m_defaultDepth, m_defaultHeight)));
}

void Carton::paint(QPaintDevice *paintDevice)
{
	QPainter painter(paintDevice);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//	painter.setOpacity(0.5);

	QTransform frontTransform;
	frontTransform.rotate(m_yRotation, Qt::YAxis);
	frontTransform.translate(0, -m_defaultHeight/2);
	frontTransform *= QTransform().translate(m_xOffset, m_yOffset);
	painter.setTransform(frontTransform);
	painter.drawImage(0, 0, m_faceImages[Front]);

	QTransform sideTransform;
	sideTransform.rotate(m_yRotation + 90, Qt::YAxis);
	sideTransform.translate(-m_faceImages[Side].width(), -m_defaultHeight/2);
	sideTransform *= QTransform().translate(m_xOffset, m_yOffset);
	painter.setTransform(sideTransform);
	painter.drawImage(0, 0, m_faceImages[Side]);

	painter.restore();
}

void Carton::setImage(Faces face, QImage image)
{
	m_faceImages[face] = image;
}

QImage Carton::image(Faces face) const
{
	return m_faceImages[face];
}

QImage Carton::defaultImage(Faces face, QSize size)
{
	QImage result(size, QImage::Format_RGB32);
	QPainter painter(&result);
	painter.setBrush(Qt::black);
	painter.drawRect(result.rect());
	painter.setBrush(Qt::white);
	painter.drawRect(result.rect().adjusted(3, 3, -4, -4));
	QString faceCaption =
		face==Front?"Front"
		:face==Side?"Side"
		:"Top";
	painter.drawText(result.rect(), Qt::AlignCenter, faceCaption);
	return result;
}
