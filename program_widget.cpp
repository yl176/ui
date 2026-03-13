#include "program_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

void ProgramWidget::translateLanguage()
{

}

ProgramWidget::ProgramWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);
	
	//right_center_function_widget = new QWidget(this);
	dis_label = new QLabel(this);
	
	dis_label->setText("start program...");
	/*dis_label->setStyleSheet("font-size:20px;letter-spacing:6px;background-color: white;"\
	                          "border-style:none;color:black;"\
	                          "font-weight:normal;text-align:left;");*/
	dis_label->setObjectName("normal_label");
	//right_center_function_widget->raise();
	//this->show();
}

void ProgramWidget::changeLanguage()
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

