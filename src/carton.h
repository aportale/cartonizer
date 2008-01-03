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

	// Left means the observers left, not the "Boxes left".
	enum Vertices {
		LeftTopFront,
		RightTopFront,
		LeftBottomFront,
		RightBottomFront,
		LeftSubFront,
		RightSubFront,
		LeftTopBack,
		RightTopBack,
		LeftBottomBack,
		RightBottomBack,
		LeftSubBack,
		RightSubBack
	};

	enum Faces {
		Front,
		Left,
		Right,
		Back,
		Top,
		Bottom,
		FrontReflection,
		LeftReflection,
		RightReflection,
		BackReflection
	};

    Carton(QObject *parent = 0);
	void paint(QPaintDevice *paintDevice);

	void setImage(Faces face, QImage image);
	QImage image(Faces face) const;

	QTransform transform(Faces face) const;

	static QImage defaultImage(Faces face, QSize size);

	static QVector<Vertices> verticesOfFace(Faces face);

	static const int m_defaultWidth = 300;
	static const int m_defaultHeight = 400;
	static const int m_defaultDepth = 100;

private:
	qreal m_xOffset;
	qreal m_yOffset;
	qreal m_xRotation;
	qreal m_yRotation;
	qreal m_observerHeight;
	QHash<Faces, QImage> m_faceImages;
};

#endif
