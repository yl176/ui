#include "push_button.h"

PushButton::PushButton(QWidget *parent)
	:QPushButton(parent)
{	
	//设置文本颜色
	QPalette text_palette = palette();
	text_palette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
	setPalette(text_palette);

	//设置文本粗体
	QFont &text_font = const_cast<QFont &>(font());
	text_font.setWeight(QFont::Bold);

	status = NORMAL;
	mouse_press = false;
}

PushButton::~PushButton()
{

}

void PushButton::setPicName(QString pic_name)
{
	this->pic_name = pic_name;
	setFixedSize(QPixmap(pic_name).size());
}

void PushButton::enterEvent(QEvent *)
{
	status = ENTER;
	update();
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
	//若点击鼠标左键
	if(event->button() == Qt::LeftButton)
	{
		mouse_press = true;
		status = PRESS;
		update();
	}
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
	//若点击鼠标左键
	if(mouse_press  && this->rect().contains(event->pos()))
	{
		mouse_press = false;
		status = ENTER;
		update();
		emit clicked();
	}
}

void PushButton::leaveEvent(QEvent *)
{
	status = NORMAL;
	update();
}

void PushButton::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QPixmap pixmap;
	switch(status)
	{
	case NORMAL:
		{
			pixmap.load(pic_name);
			break;
		}    
	case ENTER:
		{
			pixmap.load(pic_name + QString("_hover"));
			break;
		} 
	case PRESS:
		{
			pixmap.load(pic_name + QString("_pressed"));	
			break;
		}   
	case NOSTATUS:
		{
			pixmap.load(pic_name);	
			break;
		}   
	default:
		pixmap.load(pic_name);
	}
	
	painter.drawPixmap(rect(), pixmap);
}
