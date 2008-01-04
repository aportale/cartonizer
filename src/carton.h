#ifndef CARTON_H
#define CARTON_H

#include <QObject>
#include <QImage>
#include <QHash>

class QPainter;

class Carton : public QObject
{
	Q_OBJECT
	Q_ENUMS(Vertices)
	Q_ENUMS(Faces)

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
	void paintVertices(QPaintDevice *paintDevice);

	void setImage(Faces face, QImage image);
	QImage image(Faces face) const;

	QTransform transform(Faces face) const;
	void boxVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const;
	void rotatedVertex3d(Vertices vertex, qreal &x, qreal &y, qreal &z) const;
	QPointF vertex2d(Vertices vertex) const;

	static QImage defaultImage(Faces face, QSize size);

	static QVector<Vertices> verticesOfFace(Faces face);

	static const qreal m_defaultWidth;
	static const qreal m_defaultHeight;
	static const qreal m_defaultDepth;
	static const QHash<Carton::Faces, QVector<Carton::Vertices> > m_facesVerticesHash;

private:
	static QHash<Carton::Faces, QVector<Carton::Vertices> > facesVerticesHash();

	qreal m_xOffset;
	qreal m_yOffset;
	qreal m_xRotation;
	qreal m_yRotation;
	qreal m_observerHeight;

	qreal m_boxWidth;
	qreal m_boxHeight;
	qreal m_boxDepth;

	QHash<Faces, QImage> m_faceImages;
};

#endif
