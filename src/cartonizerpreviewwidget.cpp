#include "cartonizerpreviewwidget.h"

CartonizerPreviewWidget::CartonizerPreviewWidget(QWidget *parent)
    : QWidget(parent)
{
}

void CartonizerPreviewWidget::paintEvent(QPaintEvent *event)
{
	emit needsPaint(this);
}