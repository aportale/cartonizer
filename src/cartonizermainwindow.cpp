#include "cartonizermainwindow.h"

CartonizerMainWindow::CartonizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	connect(m_xRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleXRotationChanged(double)));
	connect(m_yRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleYRotationChanged(double)));
	connect(m_previewWidget, SIGNAL(needsPaint(QPaintDevice *)), this, SIGNAL(needsPreviewPaint(QPaintDevice *)));
}

void CartonizerMainWindow::updatePreview()
{
	m_previewWidget->update();
}

void CartonizerMainWindow::handleXRotationChanged(double rotation)
{
	emit propertyChanged("xRotation", rotation);
}

void CartonizerMainWindow::handleYRotationChanged(double rotation)
{
	emit propertyChanged("yRotation", rotation);
}
