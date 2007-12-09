#ifndef CARTON_H
#define CARTON_H

#include <QObject>
#include <QImage>
#include <QTransform>
#include <QHash>

class QPainter;

class Carton : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QTransform transform READ transform WRITE setTransform)

public:
	enum Faces{Front, Right, Top, Left};

    Carton(QObject *parent = 0);
	void paint(QPaintDevice *paintDevice);

	void setTransform(QTransform transform);
	QTransform transform() const;

	void setImage(Faces face, QImage image);
	QImage image(Faces face) const;

	static QTransform defaultTransform();
	static QImage defaultImage(Faces face, QSize size);

	static const int m_defaultWidth = 300;
	static const int m_defaultHeight = 400;
	static const int m_defaultDepth = 100;

private:
	QTransform m_transform;
	QHash<Faces, QImage> m_faceImages;
};

#endif
