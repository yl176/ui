#include "update_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

int g_upgrade_flag = 0;

void UpdateWidget::translateLanguage()
{

}
void UpdateWidget::get_firmware_update_per(char *value, uint32 len_buf)
{
    uint32 per;
	static uint32 last_per = 0;
	uint32 len;
	enno_result ret = ENNO_OK;
	
    if (NULL == value)
    {
        qDebug()<<"qterr null value, get_firmware_update_per";
    }

	ret = enno_decode_uint(value, &per, &len, len_buf);
	if (ret)
	{
        qDebug()<<"qterr enno_decode_uint, get_firmware_update_per";
	}

	perBar->setValue(per);

	if (last_per != per)
	{
	    g_upgrade_flag = 1;
		last_per = per;
		timer_update->start();
	}
		
	status->setText(tr(" updating...%1%") .arg(per));
	if (per >= 99)
	{
	    status->setText(tr("ready to restart"));
	}
	update();
}

UpdateWidget::UpdateWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);
	
	//right_center_function_widget = new QWidget(this);
	//function_label = new QLabel();
	//right_center_function_widget->raise();
	//this->show();
	status = new QLabel(this);
	status->setText(tr("updating..."));
	status->setStyleSheet("font-size:30px;color:rgb(255,255,255);" \
				"font-weight:bold;text-align:center;background-color:rgb(0,0,0);");
	status->setAlignment(Qt::AlignCenter);
	status->setGeometry(QRect(0, 0, 320, 30));
	
	perBar = new QProgressBar(this);
	perBar->setFormat(QString::fromLocal8Bit(""));
	//perBar->setStyleSheet("background-color:rgb(109, 109, 109); color:rgb(90, 224, 255);border-radius:10px");
	perBar->setGeometry(QRect(0, 142, 320, 36));
	perBar->setRange(0, 100-1);
	perBar->setValue(0);
	perBar->setObjectName("updatePorgress");

	timer_update = new QTimer();
	//timer_off->setInterval(60000);
    timer_update->setInterval(600*1000);
    timer_update->stop();
	
	connect(timer_update, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/

void UpdateWidget::onTimerUpdate()
{
    g_upgrade_flag = 0;
	timer_update->stop();
}

void UpdateWidget::changeLanguage()
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

UpdateWidget_Pic::UpdateWidget_Pic(QWidget *parent)
{
   logo_enno = new QLabel(this);
   logo_enno->setGeometry(QRect(56, 56, 207, 207));
   QPixmap pixmap(LOGO_PIC);
   logo_enno->setPixmap(pixmap);   
}


