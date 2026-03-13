#include "light_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include "status_xml.h"

void LightWidget::translateLanguage()
{

}

void LightWidget::get_dim_value(char *value, uint32 len_buf)
{
    uint32 step;
	uint32 len_use;
	int aul_dim[7] = {0, 10, 5, 3, -3, -5, -10}; //调速模式

	enno_decode_uint(value, &step, &len_use, len_buf);

    if (step >= 7)
    {
        return;
    }
    light_value += aul_dim[step];
    if (light_value >= 100)
    {
        light_value = 100;
    }
	else if (light_value <= 0)
	{
	    light_value = 0;
	}

    //qDebug()<<"step="<<step<<", light="<<light_value;
	
	bar->setValue(light_value);
	update();
}


LightWidget::LightWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	widget_light = new QWidget(this);

	widget_light->move(0, 200);

	widget_light->setFixedSize(320,120);
	bar = new QProgressBar(widget_light);
	bar->setFormat(QString::fromLocal8Bit(""));
	bar->setStyleSheet("background-color:rgb(109, 109, 109); color:rgb(255, 255, 255);");
	bar->setGeometry(QRect(0, 90, OLED_PIX_X, 30));
	bar->setRange(0, 100-1);
	bar->setValue(50);

	light_value = 50;
	//right_center_function_widget = new QWidget(this);
	//function_label = new QLabel();
	//right_center_function_widget->raise();
	//this->show();

	
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/

void LightWidget::paintEvent(QPaintEvent *e)
{

    //qDebug()<<"LightWidget paintEvent";
	QImage image = QPixmap("curtainWidget/curtain_dim.png").toImage();			
	QPainter paint(this);
	//qDebug()<<"cur_button=["<<cur_button<<"]"<<endl;
	QPainter painter(&image);	
	//设置画刷模式
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	//改变画笔和字体
	QPen pen = painter.pen();
	pen.setColor(Qt::white);
	QFont font = painter.font();
	font.setBold(false);//加粗
	font.setPixelSize(80);//改变字体大小
	painter.setPen(pen);
	painter.setFont(font);	

	QRect target(30.0, -30.0, 261.0, 261.0); //建立目标矩形，该区域是显示图像的目的地
	QRect source(0.0, 0.0, 261.0, 261.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域

	//将文字绘制在图片中心位置
	QString str;
	str.sprintf("%d%", light_value);

	painter.drawText(image.rect(), Qt::AlignCenter, str);

	paint.drawImage(target,image,source);

}

void LightWidget::changeLanguage()
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

