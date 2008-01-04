#include "carton.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QtDebug>

class PainterWidget : public QWidget
{
public:
	PainterWidget(QWidget *parent = 0)
		: QWidget(parent)
	{
		m_carton = new Carton(this);
		QImage frontImage("");
	}

	void paintEvent(QPaintEvent *event)
	{
		m_carton->paint(this);
	}

private:
	Carton *m_carton;
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qDebug() << Carton::verticesOfFace(Carton::Top);
	PainterWidget pw;
	pw.show();
	return a.exec();
}
