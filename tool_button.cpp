#include "tool_button.h"
#include "content_widget.h"

ToolButton::ToolButton(QString pic_name, QWidget *parent)
	:QToolButton(parent)
{
	//设置文本颜色
	QPalette text_palette = palette();
	text_palette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
	setPalette(text_palette);

	//设置文本粗体
	QFont &text_font = const_cast<QFont &>(font());
	text_font.setWeight(QFont::Bold);

	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	off_pixmap_name = pic_name;
	button_on = false;

	//设置图标
	QPixmap pixmap(pic_name);
	setIcon(pixmap);
	setIconSize(pixmap.size());

	//设置大小
	setFixedSize(pixmap.width()+50, pixmap.height()+35);
	setAutoRaise(true);
	setObjectName("transparentToolButton");

	mouse_over = false;
	mouse_press = false;
}

ToolButton::~ToolButton()
{

}

void ToolButton::enterEvent(QEvent *)
{
	mouse_over = true;
}

void ToolButton::leaveEvent(QEvent *)
{
	mouse_over = false;
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
   // printf("mouse press toolbutton,x=%d,y=%d\n", event->pos().x(), event->pos().y());
	QString map[2] = {"contentWidget/first_add", "contentWidget/recovery"};
	static int i = 0;

    QObject *object = QObject::sender();
    ToolButton *tool_button = qobject_cast<ToolButton *>(object);

    if (button_on == 0)
    {
        //cWidget->scene_off();
    }
	button_on ^= 1;

	//printf("button_on=%d\n", button_on);
	//QPixmap pixmap(map[i++%2]);
	if (button_on && (on_pixmap_name!=NULL))
	{
	    QPixmap pixmap(on_pixmap_name);
		setIcon(pixmap);
	    setIconSize(pixmap.size());
	}
	else if (off_pixmap_name!=NULL)
	{
	    QPixmap pixmap(off_pixmap_name);
		setIcon(pixmap);
	    setIconSize(pixmap.size());
	}
	
	
	//if(event->button() == Qt::LeftButton)
	//{
	//	emit clicked();
	//}
}

void ToolButton::setMousePress(bool mouse_press)
{
   // printf("setMousePress\n");
	this->mouse_press = mouse_press;
	if (off_pixmap_name!=NULL)
	{
		QPixmap pixmap(off_pixmap_name);
	    setIcon(pixmap);
		setIconSize(pixmap.size());
	}
	update();
}

void ToolButton::paintEvent(QPaintEvent *event)
{
   
    //printf("*tool button paint\n");
	 #if 0
	if(mouse_over)
	{
		//绘制鼠标移到按钮上的按钮效果
		painterInfo(0, 100, 150);
	}
	else
	{
		if(mouse_press)
		{
			painterInfo(0, 100, 150);
		}
	}
#endif

	QToolButton::paintEvent(event);
	
}

void ToolButton::painterInfo(int top_color, int middle_color, int bottom_color)
{
	QPainter painter(this);
	QPen pen(Qt::NoBrush, 1);
	painter.setPen(pen);
	QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
	linear.setColorAt(0, QColor(230, 230, 230, top_color));
	linear.setColorAt(0.5, QColor(230, 230, 230, middle_color));
	linear.setColorAt(1, QColor(230, 230, 230, bottom_color));
	painter.setBrush(linear);
	painter.drawRect(rect());
}
