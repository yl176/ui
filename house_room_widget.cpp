#include "house_room_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

void HouseRoomWidget::translateLanguage()
{

}
#define MOVT_TIME 1000
#define A_X 15
#define A_Y 0
#define B_X 0
#define B_Y 105
#define C_X 15
#define C_Y 230
#define S_SIZE_W 290
#define S_SIZE_H  90
#define B_SIZE_W OLED_PIX_X
#define B_SIZE_H 110
void HouseRoomWidget::show_move()
{
	if(room_num<0)
		return;
	switch(room_num)
	{
	case 0:
		return;
		break;
		
	case 1:
	{
		QPropertyAnimation *anim_a = new QPropertyAnimation(room_button[0], "geometry");
		anim_a->setDuration(MOVT_TIME);
		anim_a->setKeyValueAt(0, QRect(145, 40,S_SIZE_W/10 , S_SIZE_H/10));
		anim_a->setKeyValueAt(1, QRect(A_X, A_Y, S_SIZE_W, S_SIZE_H));
		anim_a->start(QAbstractAnimation::DeleteWhenStopped);
		break;
	}

	case 2:
	{
		QPropertyAnimation *anim_a = new QPropertyAnimation(room_button[0], "geometry");
		QPropertyAnimation *anim_b = new QPropertyAnimation(room_button[1], "geometry");
		//group_buttom_scale = new QParallelAnimationGroup;
		group_buttom_scale->clear();
		anim_a->setDuration(MOVT_TIME);
		anim_a->setKeyValueAt(0, QRect(145, 40,S_SIZE_W/10 , S_SIZE_H/10));
		anim_a->setKeyValueAt(1, QRect(A_X, A_Y, S_SIZE_W, S_SIZE_H));

		anim_b->setDuration(MOVT_TIME);
		anim_b->setKeyValueAt(0, QRect(145, 40,B_SIZE_W/10 , B_SIZE_H/10));
		anim_b->setKeyValueAt(1, QRect(B_X, B_Y, B_SIZE_W, B_SIZE_H));

		group_buttom_scale->addAnimation(anim_a);
		group_buttom_scale->addAnimation(anim_b);
	    group_buttom_scale->start();

		break;
	}
	default:
	{

		QPropertyAnimation *anim_a = new QPropertyAnimation(room_button[0], "geometry");
		QPropertyAnimation *anim_b = new QPropertyAnimation(room_button[1], "geometry");
		QPropertyAnimation *anim_c = new QPropertyAnimation(room_button[2], "geometry");
		group_buttom_scale->clear();
		
		anim_a->setDuration(MOVT_TIME);
		anim_a->setKeyValueAt(0, QRect(145, 40,S_SIZE_W/10 , S_SIZE_H/10));
		anim_a->setKeyValueAt(1, QRect(A_X, A_Y, S_SIZE_W, S_SIZE_H));

		anim_b->setDuration(MOVT_TIME);
		anim_b->setKeyValueAt(0, QRect(145, 154,B_SIZE_W/10 , B_SIZE_H/10));
		anim_b->setKeyValueAt(1, QRect(B_X, B_Y, B_SIZE_W, B_SIZE_H));

		anim_c->setDuration(MOVT_TIME);
		anim_c->setKeyValueAt(0, QRect(145, 270,S_SIZE_W/10 , S_SIZE_H/10));
		anim_c->setKeyValueAt(1, QRect(C_X, C_Y, S_SIZE_W, S_SIZE_H));
		group_buttom_scale->addAnimation(anim_a);
		group_buttom_scale->addAnimation(anim_b);
		group_buttom_scale->addAnimation(anim_c);
	    group_buttom_scale->start();
		break;
	}


	}

}
extern int g_current_room;

HouseRoomWidget::HouseRoomWidget(TreeModel *model, QWidget *parent)
{
    int j;
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	wid_room = new QWidget(this);
	wid_room->setFixedSize(OLED_PIX_X, OLED_PIX_Y);
	
    group_buttom_scale = new QParallelAnimationGroup;
	
	QModelIndex title;
	QModelIndex room;
	char cmd_buf[64];
	
	room = model->index(0, 0, title);

	for (j = 0; j < model->rowCount(title); j++)
	{
	    QList<QString> room_value;
		for (int i = 0; i < model->columnCount(room); i++)
		{
			QModelIndex room1 = model->index(j, i, title);
			QVariant columndata1 = model->data(room1, Qt::DisplayRole);
			//qDebug()<<"index["<<room1.column()<<"]room["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			room_value<<columndata1.value<QString>();
		}

		astRoom[j].m_Id = room_value[0];
		astRoom[j].m_name = room_value[1];
	}

	room_num = j;

	 qDebug()<<"HouseRoomWidget---room_num="<<room_num;
	 qDebug()<<"HouseRoomWidget---g_current_room="<<g_current_room;

	  if(g_current_room>=room_num || g_current_room<0)
	  {
		  qDebug()<<"g_current_room>=g_current_room ,so reset g_current_room = 0";
		  g_current_room = 0;
		  snprintf(cmd_buf, 64, "echo %d > /home/user/ui/qwid/current_room_id",g_current_room );
		  system(cmd_buf);
		  usleep(20000);
		  system("sync");

	  }
	 

	 
	QSignalMapper *signal_mapper = new QSignalMapper(this);

	for (int i = 0; i < room_num && i < 3; i++)
	{
	    room_button[i] = new QPushButton(wid_room);
		room_button[i]->setText(astRoom[i].m_name);
		
	    if(i==0)
		{
			room_button[i]->setGeometry(QRect(15, 0, 290, 90));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
						font-size:30px;color:white;font-weight:bold;\
						border-radius: 15px;  border: 2px;");
		}
		else if (i == 1)
		{
		    room_button[i]->setGeometry(QRect(0, 105, OLED_PIX_X, 110));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
					font-size:30px;color:white;font-weight:bold;\
	                border-radius: 15px;  border: 2px;");
		}
		else
		{
			room_button[i]->setGeometry(QRect(15, 230+(i-2)*110, 290, 90));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
						font-size:30px;color:white;font-weight:bold;\
						border-radius: 15px;  border: 2px;");
		}
		
		connect(room_button[i], SIGNAL(clicked()), signal_mapper, SLOT(map()));
		signal_mapper->setMapping(room_button[i], QString::number(i, 10));
	}	
	
	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnIcon(QString)));
	//g_room_num = room_num;
}

static int g_current_index  =0;
void HouseRoomWidget:: show_scene()
{
    qDebug()<<"g_current_index="<<g_current_index;
    emit changeRoom(g_current_index,1);
}

void HouseRoomWidget::turnIcon(QString current_icon)
{
	bool ok;  
	g_current_index = current_icon.toInt(&ok, 10);
	//QPushButton *h_button = button_list_room.at(current_index);
    qDebug()<<"---------emit changeRoom="<<g_current_index;

   /* hrlabel_house->setPixmap(wid_room->grab());
    hrlabel_house->raise();
    hrlabel_house->show();*/

    QPropertyAnimation *animation_house = new QPropertyAnimation(wid_room, "geometry");
	animation_house->setDuration(400);
	animation_house->setKeyValueAt(0, QRect(0,0,320,320));
	animation_house->setKeyValueAt(1, QRect(-OLED_PIX_X, 0, 320, 320));
	animation_house->start(QAbstractAnimation::DeleteWhenStopped);
	//wid_room->hide();
	create_houseflag_file();

	QTimer::singleShot(400, this, SLOT(show_scene()));
}

void HouseRoomWidget::create_houseflag_file()
{
	int fd = 0x00;
	char flag[2] = {"1"};
	//system("rm /home/user/conf/pannos_hkt.conf");
    //usleep(1000);
    QFile file(HOUSE_SET_FILE);
	
	if (file.exists())
	{
	    qDebug()<<HOUSE_SET_FILE<<", file exist";
	}
	else
	{
	    file.open(QIODevice::ReadWrite | QIODevice::Text);
		file.write(flag);
	}

	file.close();
}

void HouseRoomWidget::changeLanguage()
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

