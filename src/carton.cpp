#include "carton.h"
#include <QPainter>

Carton::Carton(QObject *parent)
    : QObject(parent)
	, m_xOffset(400)
	, m_yOffset(200)
	, m_xRotation(-10)
	, m_yRotation(-45)
//	, m_observerHeight(m_defaultHeight/2)
	, m_observerHeight(-200)
{
	setImage(Front, defaultImage(Front, QSize(m_defaultWidth, m_defaultHeight)));
	setImage(Left, defaultImage(Left, QSize(m_defaultDepth, m_defaultHeight)));
	setImage(Top, defaultImage(Top, QSize(m_defaultWidth, m_defaultDepth)));
}

void Carton::paint(QPaintDevice *paintDevice)
{
	QPainter painter(paintDevice);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.setOpacity(0.5);

	painter.setTransform(transform(Front));	painter.drawImage(0, 0, m_faceImages[Front]);
	painter.setTransform(transform(Left));	painter.drawImage(0, 0, m_faceImages[Left]);
	painter.setTransform(transform(Top));	painter.drawImage(0, 0, m_faceImages[Top]);

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

QTransform Carton::transform(Faces face) const
{
	QTransform result;
	switch (face) {
		case Front:
			result.rotate(m_xRotation, Qt::XAxis);
			result.rotate(m_yRotation, Qt::YAxis);
			result.translate(0, -m_observerHeight);
			break;
		case Left:
			result.rotate(m_xRotation, Qt::XAxis);
			result.rotate(m_yRotation + 90, Qt::YAxis);
			result.translate(-m_faceImages[Left].width(), -m_observerHeight);
			break;
		case Top:
			result.rotate(88, Qt::XAxis);
//			result *= QTransform().rotate(-30, Qt::YAxis);
			result.translate(0, -m_faceImages[Top].height());
			break;
	}
	result *= QTransform().translate(m_xOffset, m_yOffset);
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
	QString faceCaption =
		face==Front?"Front"
		:face==Left?"Side"
		:"Top";
	QFont font;
	font.setPixelSize(30);
	painter.setFont(font);
	painter.drawText(result.rect(), Qt::AlignCenter, faceCaption);
	return result;
}

QVector<Carton::Vertices> Carton::verticesOfFace(Faces face)
{
	static const struct {
		Faces face;
		Vertices vertices[4]; //topLeft, topRight, bottomRight, BottomLeft
	} verticesOfFaces[] = {
		{Front,           {LeftTopFront, RightTopFront, RightBottomFront, LeftBottomFront}},
		{Left,            {LeftTopBack, LeftTopFront, LeftBottomFront, LeftBottomBack}},
		{Right,           {RightTopFront, RightTopBack, RightBottomBack, RightBottomFront}},
		{Back,            {RightTopBack, LeftTopBack, LeftBottomBack, RightBottomBack}},
		{Top,             {LeftTopBack, RightTopBack, RightTopFront, LeftTopFront}},
		{Bottom,          {LeftBottomBack, RightBottomBack, RightBottomFront, LeftBottomFront}},
		{FrontReflection, {LeftBottomFront, RightBottomFront, RightSubFront, LeftSubFront}},
		{LeftReflection,  {LeftBottomBack, LeftBottomFront, LeftSubFront, LeftSubBack}},
		{RightReflection, {RightBottomFront, RightBottomBack, RightSubBack, LeftSubBack}},
		{BackReflection,  {RightBottomBack, LeftBottomBack, LeftSubBack, RightSubBack}}
	};
	static const size_t facesCount = sizeof(verticesOfFaces)/sizeof(verticesOfFaces[0]);
	static const size_t verticesPerFaceCount = sizeof(verticesOfFaces[0].vertices)/sizeof(verticesOfFaces[0].vertices[0]);

	static QHash<Faces, QVector<Vertices> > verticesOfFaceHash;
	if (verticesOfFaceHash.empty()) {
		for (int faceIndex = 0; faceIndex < facesCount; faceIndex++) {
			QVector<Vertices> vertices(verticesPerFaceCount);
			for (int verticeIndex = 0; verticeIndex < verticesPerFaceCount; verticeIndex++)
				vertices[verticeIndex] = verticesOfFaces[faceIndex].vertices[verticeIndex];
			verticesOfFaceHash[verticesOfFaces[faceIndex].face] = vertices;
		}
	}

	Q_ASSERT(verticesOfFaceHash.contains(face));
	return verticesOfFaceHash[face];
}

