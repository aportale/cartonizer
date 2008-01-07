#ifndef CARTONIZERPREVIEWWIDGET_H
#define CARTONIZERPREVIEWWIDGET_H

#include <QWidget>

class CartonizerPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    CartonizerPreviewWidget(QWidget *parent = 0);
	void paintEvent(QPaintEvent *event);

signals:
	void needsPaint(QPaintDevice *paintDevice);
};

#endif
