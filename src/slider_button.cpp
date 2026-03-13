#include "slider_button.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

/*
SliderButton::SliderButton(Qt::Orientation orientation, QWidget *parent=0) : QSlider(orientation, parent)
{

}
*/

void SliderButton::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}


