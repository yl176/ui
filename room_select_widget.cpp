#include "room_select_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QGraphicsBlurEffect>
#include <QTimer>

void RoomSelectWidget::translateLanguage()
{

}





static float delay_show = 0.0;
void RoomSelectWidget::TimerTimeOut()
{
	effect->setOpacity(delay_show);
	room_select_label->setGraphicsEffect(effect);
	delay_show+=0.05;
	if(delay_show>1)
	{
		printf("m_timer->stop1()\r\n");
		m_timer->stop();
	}

}


void RoomSelectWidget:: start_timer()
{
    //m_timer = new QTimer(this);
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
    m_timer->start(50);
}


RoomSelectWidget::RoomSelectWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
	m_timer->stop();

	room_select_label = new QLabel(this);
	room_select_label->setText("select the room please\n");
	room_select_label->setObjectName("room_select");
	room_select_label->setGeometry(QRect(0, 140, 320, 60*4)); //ËÄ±¶ÐÐ¾à
	room_select_label->setWordWrap(true);
	room_select_label->setAlignment(Qt::AlignTop);

	effect = new QGraphicsOpacityEffect();
	effect->setOpacity(delay_show);
	room_select_label->setGraphicsEffect(effect);
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void RoomSelectWidget::changeLanguage()
{
    language_flag ^= 1;
    if (language_flag)
    {
        translator->load(QString("qwid_zh_CN.qm"));
    }
	else
	{
        translator->load(QString("qwid_zh_en.qm"));
	}

	this->translateLanguage();
}


/*void ContentWidget::setTranslator(QTranslator* translator)
{
	this->translator = translator;
}
*/
/*
void ContentWidget::changeSkin(QString skin_name)
{
	Util::writeInit(QString("./user.ini"), QString("skin"), skin_name);
	this->skin_name = skin_name;
	this->setStyleSheet("background:url(skin/17_big)");
    update();
}
*/

