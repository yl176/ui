#include "flash_start.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

FlashStartWidget::FlashStartWidget(QWidget *parent)
{
	QSize pic_size(60,60);

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);	
	
	label_gif = new QLabel(this);
	movie_on = new QMovie(FLASH_FILE);
	
	label_gif->setMovie(movie_on);
	movie_on->jumpToFrame(0);
	connect(movie_on, SIGNAL(frameChanged(int)), this, SLOT(moveon_close())); 
	movie_on->start();
    	
	label_gif->setGeometry(QRect(0, 0, 320, 320));
}

void FlashStartWidget::moveon_close()
{
    if (movie_on->currentFrameNumber() == movie_on->frameCount() - 1) 
	{
        movie_on->stop();
		create_flash_file();
		emit flash_singal();
    }
    else
    {
        return;
    }
}

void FlashStartWidget::create_flash_file()
{
	int fd = 0x00;
	char flag[2] = {"1"};
	//system("rm /home/user/conf/pannos_hkt.conf");
    //usleep(1000);
    QFile file(START_FLAG_FILE);
	
	if (file.exists())
	{
	    qDebug()<<"file exist";
	}
	else
	{
	    file.open(QIODevice::ReadWrite | QIODevice::Text);
		file.write(flag);
	}

	file.close();
}

FlashTextWidget::FlashTextWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);	

    label_txt = new QLabel(this);
	label_txt->setText(tr("Welcome to use enno P2"));
	//label_txt->setStyleSheet("font-size:50px;color: white;");	
	//label_txt->setGeometry(QRect(0, 40, 320, 60*4)); //ËÄ±¶ÐÐ¾à 
	label_txt->setObjectName("start");
	label_txt->setGeometry(QRect(0, 40, 320, 60*4));
	label_txt->setWordWrap(true); 
	label_txt->setAlignment(Qt::AlignTop); 
}

