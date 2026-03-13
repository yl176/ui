#include "start_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QGraphicsBlurEffect>
void StartWidget::translateLanguage()
{

}
static float delay_show = 1.0;
void StartWidget::TimerTimeOut()
{
	effect_label->setOpacity(delay_show);
	please_label->setGraphicsEffect(effect_label);
	delay_show-=0.05;
	if(delay_show<=0)
	{
		printf("m_timer->stop()\r\n");
		m_timer->stop();
	}
}

StartWidget::StartWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);
	
	m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
	m_timer->stop();
	//right_center_function_widget = new QWidget(this);
	//function_label = new QLabel();
	//right_center_function_widget->raise();
	//this->show();
	please_label = new QLabel(this);
	please_label->setText(tr("Use enno app to config the pannel please\n"));
	//no_curtain->setStyleSheet("font-size:30px;color: white;border-width:2px;border-style:solid;border-color:black;font-weight:bold;text-align:left;");
	//no_curtain->setGeometry(QRect(0, 0, 320, 120));
	please_label->setObjectName("start");
	please_label->setGeometry(QRect(0, 80, 320, 60*4)); //ËÄ±¶ÐÐ¾à 
	please_label->setWordWrap(true); 
	please_label->setAlignment(Qt::AlignTop); 

	effect_label = new QGraphicsOpacityEffect();
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void StartWidget::changeLanguage()
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

void StartWidget::end_show()
{
    m_timer->start(50);
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
void StartTitleWidget::translateLanguage()
{

}

StartTitleWidget::StartTitleWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void StartTitleWidget::changeLanguage()
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

