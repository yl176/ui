#ifndef TOOlBUTTON_H
#define TOOlBUTTON_H

/*
	功能按钮 包括：体检、查杀木马等
	作者：╰☆奋斗ing❤孩子`
	博客地址：http://blog.sina.com.cn/liang19890820
	QQ：550755606
	Qt分享、交流群：26197884

	注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
*/

#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>

#include "common.h"

class BIL_SHARE ToolButton : public QToolButton
{
	Q_OBJECT

public:

	explicit ToolButton(QString pic_name, QWidget *parent = 0);
	~ToolButton();
	void setMousePress(bool mouse_press);

protected:

	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void painterInfo(int top_color, int middle_color, int bottom_color);

public:
	QString off_pixmap_name;
	QString on_pixmap_name;
	bool button_on;

	bool mouse_over; //鼠标是否移过
	bool mouse_press; //鼠标是否按下
};

#endif //TOOlBUTTON_H
