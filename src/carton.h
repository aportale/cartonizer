#ifndef CARTON_H
#define CARTON_H

#include <QObject>
#include <QImage>
#include <QHash>

class QPainter;

class Carton : public QObject
{
	Q_OBJECT

public:
	enum Faces{Front, Side, Top};

    Carton(QObject *parent = 0);
	void paint(QPaintDevice *paintDevice);

	void setImage(Faces face, QImage image);
	QImage image(Faces face) const;

	static QImage defaultImage(Faces face, QSize size);

	static const int m_defaultWidth = 300;
	static const int m_defaultHeight = 400;
	static const int m_defaultDepth = 100;

private:
	qreal m_xOffset;
	qreal m_yOffset;
	qreal m_yRotation;
	QHash<Faces, QImage> m_faceImages;
};

#endif
