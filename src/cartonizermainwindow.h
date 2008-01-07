#ifndef CARTONIZERMAINWINDOW_H
#define CARTONIZERMAINWINDOW_H

#include "ui_cartonizermainwindow.h"

class CartonizerMainWindow : public QMainWindow, public Ui::CartonizerMainWindow
{
    Q_OBJECT

public:
    CartonizerMainWindow(QWidget *parent = 0);

signals:
	void propertyChanged(const char *name, const QVariant &value);
	void needsPreviewPaint(QPaintDevice *paintDevice);

public slots:
	void updatePreview();

private slots:
	void handleXRotationChanged(double rotation);
	void handleYRotationChanged(double rotation);
};

#endif
