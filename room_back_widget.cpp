#include "room_back_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

void RoomBackWidget::translateLanguage()
{

}
void RoomBackWidget::showTheRoom(int index, QString room_name)
{
    qDebug()<<"name"<<room_name;

	QFontMetrics fontWidth(return_name_button->font());//
	QString elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, 200);//	
#if 0
	bool b = elideNote.contains(QRegExp("[\\x4e00-\\x9fa5]+"));  
	if (b)
	{
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK")); 
		QString data = QString::fromLocal8Bit("回到") + elideNote;//??μ?êy?Y?a?Dμúi??×???μ??μ
		//return_name_button->setText(QString::fromLocal8Bit("回到"));
	   // room_name_button->setText(elideNote);//
		return_name_button->setText(data);
		//room_name_button->setText(" ");
	}
	else
	{
		QString data1 = QString::fromLocal8Bit("return") + elideNote;
		return_name_button->setText(data1);
	//	room_name_button->setText(" ");
	   // return_name_button->setText("return ");
	   // room_name_button->setText(elideNote);//
	}
#endif
	QString data1 = tr("return to ") + elideNote;
	return_name_button->setText(data1);
	//room_name_button->setText(tr("nonono"));
//	room_name_button->setToolTip(room_name);//

    //room_name_button->setText(room_name);
	update();
}

RoomBackWidget::RoomBackWidget(QWidget *parent)
{
	QSize pic_size(60,60);

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(640,320);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);	
	
	//right_center_function_widget = new QWidget(this);
	//function_label = new QLabel();
	//right_center_function_widget->raise();
	//this->show();
	return_name_button = new QPushButton();
	return_name_button->setText("return\n");
	//no_curtain->setStyleSheet("font-size:30px;color: white;border-width:2px;border-style:solid;border-color:black;font-weight:bold;text-align:left;");
	//no_curtain->setGeometry(QRect(0, 0, 320, 120));
	return_name_button->setObjectName("room_name");
	//return_name_button->setGeometry(QRect(0, 50, 200, 60));
	
	
	button_back = new QPushButton();
	button_back->setIcon(QPixmap("other/room_back.png"));
	button_back->setIconSize(pic_size);	
	//button_back->setGeometry(QRect(135, 110, 50, 51));


	QVBoxLayout *v_layout = new QVBoxLayout();
	v_layout->addStretch();
	v_layout->addWidget(return_name_button);
	v_layout->addWidget(button_back);

	v_layout->addStretch();
	
	connect(button_back, SIGNAL(clicked()), this, SLOT(return_room_slot()));
	connect(return_name_button, SIGNAL(clicked()), this, SLOT(return_room_slot()));
	return_name_button->raise();
	button_back->raise();

	this->setLayout(v_layout);
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void RoomBackWidget::changeLanguage()
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

void RoomBackWidget::return_room_slot()
{
	if(g_upSreenTurning||g_BottomSreenTurning)
    {
        return;
    }

    emit return_room_singal();
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

