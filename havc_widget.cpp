#include "havc_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QApplication>
//#include "sliderpicture.h"
#include <QGraphicsBlurEffect>

#include "wthread.h"
#include "status_xml.h"

#define MAIN_X_SIZE 300

unsigned int g_openBitMap = 0;
extern MainWidget * g_ptr_main_widget;

void HavcWidget::translateLanguage()
{
    for (int i = 0; i < device_num; i++)
	{
		set_button_content(i); 
	/*	if(pLabel_humidity[i]&&stAc[i].dev_type == DEV_RS485_AIR_PURIFIER)
		{
		    pLabel_humidity[i]->setText(tr("Wet"));
		}*/
	}

	if (i_dev_cnt == 0 && content)
    {
		QLabel * label_no_device = content->findChild<QLabel*>("nohvac");
		if(label_no_device)
		{
		    label_no_device->setText(tr("There's no havc here.\n"));
		}
    }
}

void HavcWidget::ac_on_show()
{
    //printf("ac_on_show, current_index=%d,uid=%d\n", current_uid, current_uid);
	//wid_get->hide();
	stAc[current_uid].switch_on = 1;

	//ac_button->setText(tr("AC %1 off") .arg(current_uid+1));
	QPixmap pixmap("havc/ac_off.png");
	//ac_button->setEnabled(false);

    //printf("button clicked!!!icon=%s\n", tool_button->icon());
	ac_button->setIcon(pixmap);
	ac_button->setIconSize(pixmap.size());
	//改变按钮文字
	//button_on->setText("on");
	//tool_button->setText(tr("AC %1 off") .arg(ac_uid+1));
	
	//pLabel_ac_id[current_uid]->setText(tr("AC %1") .arg(current_uid+1));
	
	QString temp;
    temp.sprintf("%d%cC", stAc[current_uid].target_temperature, 0xB0);
	pLabel_ac[current_uid]->setText(temp);
	
	wid_get[current_uid]->raise();
	current_uid++;
}
void HavcWidget::ac_off_show()
{
    //printf("ac_off_show,current_index=%d, current_uid=%d\n", current_uid, current_uid);
	#if 0
	//wid_get->show();
	stAc[ac_uid].switch_on = 0;
    QString str1;
	str1.sprintf("havc/ac_off.png");
	tool_button->setText(tr("AC %1 off") .arg(ac_uid+1));
	//tool_button->show();
	QPixmap pixmap(str1);

    //printf("button clicked!!!icon=%s\n", tool_button->icon());
	tool_button->setIcon(pixmap);
	tool_button->setIconSize(pixmap.size());
	#endif
	tool_button[current_uid]->setText(tr("AC %1 off") .arg(current_uid+1));
	ac_button_on->setText("on");
	current_uid--;
	wid_get[current_uid]->raise();
	ac_button_on->setEnabled(true);
	
}

static bool g_ctrl_creat_complate = 0;
static bool g_purify_creat_complate = 0;
void HavcWidget::set_button_content(int uid)
{
    QPixmap button_on_pixmap;
	QPushButton *button_on = stAc[uid].button_on;
	QLabel *l_name =  stAc[uid].label_name;
	QLabel *l_img = stAc[uid].label_img;
	QSize pic_size(75,75);
	//button_on->setFixedSize(320, 60);
	QString str_name(ast_dev[uid].m_name);
	QString str = str_name.left(6);
	//QString str_space(" ");
	QString str_pad = str;// + str_space;

	if (stAc[uid].switch_on)
	{
		if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER) 
		{	
			//QString str1 = QString(tr("AC%1")).arg(stAc[uid].dev_cnt, 0, 'g', 2);
			//QString str2 = QString(tr("heat%1")).arg(stAc[uid].dev_cnt, 0, 'g', 2);		
		    button_on->setIcon(QPixmap("havc/ac_on.png"));
		    button_on->setIconSize(pic_size);
			//button_on->setStyleSheet(QString("font-size:30px;color:rgb(0,0,0);text-align:center;"));//border-image:url(havc/ac_on.png);
            l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(255,255,255)");
			//button_on->setText(str);
			l_img->setPixmap(QPixmap("havc/ac_on_flag.png"));
			l_name->setText(str);

			if(g_ctrl_creat_complate)
			{
				set_ac_mode_init(uid);
				set_fan_speed_init(uid);
			}	
		}	
		else if (stAc[uid].dev_type == DEV_FLOOR_HEAT)
		{
		    button_on->setIcon(QPixmap("havc/ac_on.png"));
		    button_on->setIconSize(pic_size);
			//button_on->setStyleSheet(QString("font-size:30px;border-image:url(havc/heat_on.png);color:rgb(0,0,0);text-align:center;"));
			//button_on->setText(str);
			l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(255,255,255)");
			l_img->setPixmap(QPixmap("havc/heat_on_flag.png"));
			l_name->setText(str);
		}
		else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
		{
			QString str1 = QString(tr("purify%1")).arg(stAc[uid].dev_cnt, 0, 'g', 2);
			button_on->setIcon(QPixmap("havc/ac_on.png"));
			button_on->setIconSize(pic_size);
			//button_on->setStyleSheet(QString("font-size:30px;border-image:url(havc/purify_on.png);color:rgb(0,0,0);text-align:center;"));
			l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(255,255,255)");
			//button_on->setText(str);
			l_img->setPixmap(QPixmap("havc/purify_on_flag.png"));
			l_name->setText(str);
			if(g_purify_creat_complate)
			{
				set_fan_speed_init(uid);
				//pLabel_humidity[uid]->setText(tr("Wet"));
				set_ap_mode_init(uid);
			}

		}
	}
	else
	{	
		if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER) 
		{
			//QString str1 = QString(tr("AC%1    ")).arg(stAc[uid].dev_cnt, 0, 'g', 2);
			//QString str2 = QString(tr("heat%1    ")).arg(stAc[uid].dev_cnt, 0, 'g', 2);
		    button_on->setIcon(QPixmap("havc/ac_off.png"));
		    button_on->setIconSize(pic_size);
			//button_on->setStyleSheet(QString("font-size:30px;border-image:url(havc/ac_off.png);color:rgb(0,0,0);text-align:right;"));
			//button_on->setText(str_pad);
		    l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");
		//button_on->setText(str);
		    l_img->setPixmap(QPixmap("havc/ac_off_flag.png"));
			l_name->setText(str);

			if(g_ctrl_creat_complate)
			{
				set_ac_mode_off(uid);
				set_fan_speed_off(uid);
			}
		}
		else if (stAc[uid].dev_type == DEV_FLOOR_HEAT)
		{
			button_on->setIcon(QPixmap("havc/ac_off.png"));
			button_on->setIconSize(pic_size);
			//button_on->setStyleSheet(QString("font-size:30px;border-image:url(havc/heat_off.png);color:rgb(0,0,0);text-align:right;"));
			//button_on->setText(str_pad);
			l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");
		//button_on->setText(str);
			l_img->setPixmap(QPixmap("havc/heat_off_flag.png"));
			l_name->setText(str);
		}
		else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
		{
			//QString str1 = QString(tr("purify%1    ")).arg(stAc[uid].dev_cnt, 0, 'g', 2);
			//button_on->setText(str_pad);
		    l_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");
			//button_on->setText(str);
			l_img->setPixmap(QPixmap("havc/purify_off_flag.png"));
			l_name->setText(str);
			button_on->setIcon(QPixmap("havc/ac_off.png"));
			button_on->setIconSize(pic_size);

			if(g_purify_creat_complate)
			{
				set_fan_speed_off(uid);
				//pLabel_humidity[uid]->setText(tr("Wet"));
				set_ap_mode_off(uid);
			}
			//button_on->setStyleSheet(QString("font-size:30px;border-image:url(havc/purify_off.png);color:rgb(0,0,0);text-align:right;"));
		}
	}

}

void HavcWidget::buttonClicked()
{
   // printf("button clicked!!!\n");
	QObject *object = QObject::sender();
    QToolButton *tool_button = qobject_cast<QToolButton *>(object);
#if 1
	ac_button = tool_button;

	//scene_off();
    QString str1;
	if (stAc[current_uid].switch_on == 1)
		return;

	stAc[current_uid].switch_on = !stAc[current_uid].switch_on;

    if (stAc[current_uid].switch_on == 1)
    {
        str1.sprintf("havc/ac_on.png");
		tool_button->setText(tr("AC %1 on") .arg(current_uid+1));
		QTimer::singleShot( 5*100, this, SLOT(ac_on_show()));
		//tool_button->setEnabled(false);
		//update();
		
		//tool_button->show();
		//usleep(2000000);
		//wid_get1->raise();
    }
    else
    {
        return;
		str1.sprintf("havc/ac_off.png");
		tool_button->setText(tr("AC %1 off") .arg(current_uid+1));
		tool_button->show();
    }
	//return;
	QPixmap pixmap(str1);

    //printf("button clicked!!!icon=%s\n", tool_button->icon());
	tool_button->setIcon(pixmap);
	tool_button->setIconSize(pixmap.size());
#endif
	//tool_button->hide();

	tool_button->update();
}
void HavcWidget::scene_off()
{
    QStringList string_list;
	string_list<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop"
		<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext"
		<<"contentWidget/first_add";
    for(int i=0; i<button_list.count()/2; i++)
	{
		QToolButton *tool_button = button_list.at(i);
		QPixmap pixmap(string_list.at(i));
		tool_button->setIcon(pixmap);
		tool_button->setIconSize(pixmap.size());
		tool_button->update();
    }
}
void HavcWidget::ac_init()
{
	int i;
	ac_uid = 0;
	bool ok;
	//ac_num = 2;
	//purify_num = 2;
	//floor_num = 2;
	qDebug()<<"ac_num="<<ac_num;
	qDebug()<<"purify_num="<<purify_num;
	qDebug()<<"floor_num="<<floor_num;
	
	device_num = ac_num + purify_num + floor_num;

	for (i = 0; i < MAX_AC_NUM*3; i++)
	{
	    stAc[i].vaild = 0;
	}

    for (i = 0; i < ac_num; i++)
	{
	    stAc[i].switch_on = ast_dev[i].on;
		if(stAc[i].switch_on)
		{
		    g_openBitMap |= BIT_AC;
		}
		//stAc[i].current_heat_cool_mode = 0; 
		stAc[i].target_heat_cool_mode = (ast_dev[i].m_mode).toInt(&ok,10);
		stAc[i].target_temperature = (ast_dev[i].value).toInt(&ok,10);
		stAc[i].dev_type = DEV_RS485_HEATER_COOLER;
		stAc[i].ac_floor_mode = 0;
		stAc[i].fan_speed = (ast_dev[i].speed).toInt(&ok,10);
		stAc[i].device_id = (ast_dev[i].m_Id).toInt(&ok,10);
		stAc[i].mode = ast_dev[i].mod;
		stAc[i].dev_cnt = i;
		stAc[i].vaild = 1;
    }

	for (i = 0; i < purify_num; i++)
	{
	    stAc[i+ac_num].switch_on = ast_dev[i+ac_num].on;
		stAc[i+ac_num].PM25 = 36;
		stAc[i+ac_num].fan_speed = (ast_dev[i+ac_num].speed).toInt(&ok,10);
		stAc[i+ac_num].dev_type = DEV_RS485_AIR_PURIFIER;
		stAc[i+ac_num].device_id = (ast_dev[i+ac_num].m_Id).toInt(&ok,10);
		stAc[i+ac_num].dev_cnt = i;
		stAc[i+ac_num].vaild = 1;
	}

	for (i = 0; i < floor_num; i++)
	{
		stAc[i+ac_num+purify_num].switch_on = ast_dev[i+ac_num+purify_num].on;
		if(stAc[i+ac_num+purify_num].switch_on)
		{
		    g_openBitMap |= BIT_FLOOR;
		}
		
		stAc[i+ac_num+purify_num].target_temperature = (ast_dev[i+ac_num+purify_num].value).toInt(&ok,10);
		stAc[i+ac_num+purify_num].dev_type = DEV_FLOOR_HEAT;
		stAc[i+ac_num+purify_num].ac_floor_mode = 2;
		stAc[i+ac_num+purify_num].fan_speed = (ast_dev[i+ac_num+purify_num].speed).toInt(&ok,10);
		stAc[i+ac_num+purify_num].device_id = (ast_dev[i+ac_num+purify_num].m_Id).toInt(&ok,10);
		stAc[i+ac_num+purify_num].dev_cnt = i;
		stAc[i+ac_num+purify_num].vaild = 1;
	}
}

HavcWidget::HavcWidget(int room_id, TreeModel *model, QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	this->setMaximumSize(640,320);
	this->setMinimumSize(320,320);

    //return;
    content = new QWidget(this);
	content->setFixedSize(320,300);
	content->raise();
	content->move(0,0);

	group_bits = new QParallelAnimationGroup;
	//group_anima = new QParallelAnimationGroup;
	group_move_widget = new QParallelAnimationGroup;
#if 0
	dot_wid = new QWidget(this);
	dot_wid->setFixedSize(320,20);
	dot_wid->move(0,300);
	dot_wid->hide();
#endif
    i_dev_cnt = 0;
	i_dev_num = 0;
	device_num = 0;
	ac_num = 0;
	purify_num = 0;
	floor_num = 0;

	g_openBitMap = 0;
	
	QModelIndex title;
	QModelIndex room;
	QModelIndex curtain;
	QModelIndex air;
    int i = 0;
	int j;
	
	qDebug()<<"HavcWidget widget"<<endl;
    isAnimation = false;
    duration = 200;
    animation = new QPropertyAnimation(this, QByteArray());
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));

	room = model->index(room_id, 0, title);
	//qDebug() << "model->columnCount(room)="<<model->columnCount(room);
	for (i = 0; i < model->columnCount(room); i++)
	{
		QModelIndex room1 = model->index(0, i, title);
		QVariant columndata1 = model->data(room1, Qt::DisplayRole);
	//	qDebug()<<"index["<<room1.column()<<"]room["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
	}

	curtain = model->index(0, 0, room);
	qDebug()<<"rowCount="<<model->rowCount(room);
	for (j = 0; j < model->rowCount(room); j++)
	{
		QModelIndex ac1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(ac1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
		int i_mode = str_mode.toInt(&ok,10);
		//qDebug()<<"HEATER_COOLER["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl; 
		if (1 == ok 
			&& (    DEV_RS485_HEATER_COOLER == i_mode ))
		{	
			qDebug()<<"room child["<<j<<"] is a HEATER_COOLER"<<endl;
			i_dev_num++;
		}
		else
		{ 
			continue;
		}
		
		QList<QString> ac_value;
		air = model->index(j, 0, room);
		//qDebug()<<"columnCount(air)="<<model->columnCount(air);
		for (int i = 0; i < model->columnCount(air); i++)
		{
			QModelIndex ac1 = model->index(j, i, room);
			columndata1 = model->data(ac1, Qt::DisplayRole);
			//qDebug()<<"index=["<<ac1.column();
			//qDebug()<<"airs["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			ac_value<<columndata1.value<QString>();
		}
		ast_dev[i_dev_cnt].m_Id = ac_value[0];
		ast_dev[i_dev_cnt].m_name = ac_value[1];
		ast_dev[i_dev_cnt].m_mode = ac_value[2];
		ast_dev[i_dev_cnt].value = ac_value[3];
		ast_dev[i_dev_cnt].on = ac_value[4].toInt(&ok,10);
		//ast_dev[i_dev_cnt].target_pos = ac_value[3].toInt(&ok,10);
		ast_dev[i_dev_cnt].mod = ac_value[6].toInt(&ok,10);
		ast_dev[i_dev_cnt].device_id =	(ast_dev[i_dev_cnt].m_Id).toInt(&ok,10);

		i_dev_cnt++;
	}
	ac_num = i_dev_cnt;
	//qDebug()<<"ac_num="<<ac_num;

	for (j = 0; j < model->rowCount(room); j++)
	{
		QModelIndex curtain1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(curtain1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
		int i_mode = str_mode.toInt(&ok,10);
		//qDebug()<<"PURIFY["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl; 
		if (1 == ok && DEV_RS485_AIR_PURIFIER == i_mode)
		{	
			//qDebug()<<"room child["<<j<<"] is a DEV_RS485_AIR_PURIFIER"<<endl;
			i_dev_num++;
		}
		else
		{ 
			continue;
		}
		
		QList<QString> purify_value;
		air = model->index(j, 0, room);
		for (int i = 0; i < model->columnCount(air); i++)
		{
			QModelIndex curtain1 = model->index(j, i, room);
			columndata1 = model->data(curtain1, Qt::DisplayRole);
			//qDebug()<<"index=["<<curtain1.column();
			//qDebug()<<"purify["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			purify_value<<columndata1.value<QString>();
		}
		ast_dev[i_dev_cnt].m_Id = purify_value[0];
		ast_dev[i_dev_cnt].m_name = purify_value[1];
		ast_dev[i_dev_cnt].m_mode = purify_value[2];
		ast_dev[i_dev_cnt].value = purify_value[3];
		ast_dev[i_dev_cnt].on = purify_value[4].toInt(&ok,10);
		//ast_dev[i_dev_cnt].target_pos = purify_value[3].toInt(&ok,10);
		ast_dev[i_dev_cnt].device_id =	(ast_dev[i_dev_cnt].m_Id).toInt(&ok,10);

		i_dev_cnt++;
	}
	purify_num = i_dev_cnt - ac_num;
	
	for (j = 0; j < model->rowCount(room); j++)
	{
		QModelIndex ac1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(ac1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
		int i_mode = str_mode.toInt(&ok,10);		
		//qDebug()<<"FLOOR["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl; 
		//qDebug()<<"i_mode=["<<i_mode<<"]"<<endl;
		if (1 == ok && DEV_FLOOR_HEAT == i_mode)
		{	
			//qDebug()<<"room child["<<j<<"] is a DEV_RS485_SHADES"<<endl;
			i_dev_num++;
		}
		else
		{ 
			continue;
		}
		
		QList<QString> ac_value;
		air = model->index(j, 0, room);
		for (int i = 0; i < model->columnCount(air); i++)
		{
			QModelIndex ac1 = model->index(j, i, room);
			columndata1 = model->data(ac1, Qt::DisplayRole);
			//qDebug()<<"index=["<<ac1.column();
			//qDebug()<<"curtain["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			ac_value<<columndata1.value<QString>();
		}
		ast_dev[i_dev_cnt].m_Id = ac_value[0];
		ast_dev[i_dev_cnt].m_name = ac_value[1];
		ast_dev[i_dev_cnt].m_mode = ac_value[2];
		ast_dev[i_dev_cnt].value = ac_value[3];
		ast_dev[i_dev_cnt].on = ac_value[4].toInt(&ok,10);
		//ast_dev[i_dev_cnt].target_pos = ac_value[3].toInt(&ok,10);
		ast_dev[i_dev_cnt].device_id =	(ast_dev[i_dev_cnt].m_Id).toInt(&ok,10);

		i_dev_cnt++;
	}

	floor_num = i_dev_cnt - ac_num - purify_num;

    if (i_dev_cnt == 0)
    {
		label_no_device = new QLabel(content);
		label_no_device->setText(tr("There's no hvac here.\n"));
		//no_curtain->setStyleSheet("font-size:30px;color: white;border-width:2px;border-style:solid;border-color:black;font-weight:bold;text-align:left;");
		//no_curtain->setGeometry(QRect(0, 0, 320, 120));
		label_no_device->setObjectName("nohvac");
		label_no_device->setGeometry(QRect(0, 80, 320, 60*4)); //4倍行距
		label_no_device->setWordWrap(true); 
		label_no_device->setAlignment(Qt::AlignCenter);
        return;
    }

    ac_init();

	current_uid = 0;

	//qDebug()<<"ac_init room["<<room_id<<"]"<<"ac_num="<<ac_num<<"purify_num="<<purify_num\ 
	//	    <<"floor_num="<<floor_num<<"device_num="<<device_num;
	
    //create_off_widget(wid_get[0]);
    wid_get[0] = new QWidget(content);
	wid_get[0]->setFixedSize(320*(device_num+2),306);
	wid_get[0]->hide();
    for (i = 0; i < device_num; i++)
    {
        current_uid = i;
	   // wid_get[i] = new QWidget(content);
		//wid_get[i]->setFixedSize(320,300);
		//qDebug()<<"create_havc_device,i="<<i;
		group_anima[i] = new QParallelAnimationGroup;
	    if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER)  
		{
		    //qDebug()<<"create_ctrl_widget,i="<<i;
		    create_ctrl_widget(wid_get[0],i);
		}
		else if (stAc[i].dev_type == DEV_RS485_AIR_PURIFIER)
		{
			create_purify_widget(wid_get[0],i);
		}
		else if (stAc[i].dev_type == DEV_FLOOR_HEAT)
		{
		    //qDebug()<<"create_floor_widget,i="<<i;
			create_floor_widget(wid_get[0],i);
		}
		else
		{
		    //qDebug() << "error device mode = " << stAc[current_uid].dev_type;
			//qDebug() << "ac_floor_mode = " << stAc[current_uid].ac_floor_mode;
		}
    }

	
	current_uid = 0;
	wid_get[0]->show();
	wid_get[0]->raise();
	startx = -320;
	starty = 0;
	
	wid_get[0]->move(startx, starty);

	translateLanguage();
}

HavcWidget::~HavcWidget()
{
    delete content;
	
   // for (int i = 0; i < device_num+1; i++)
    {
		delete wid_get[0];
	}
}

void HavcWidget::create_off_widget(QWidget *widget)
{
	//this->setStyleSheet("background:red");
	tool_button[current_uid] = new QToolButton();
	tool_button[current_uid]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	QPixmap button_pixmap("havc/ac_off.png");
	tool_button[current_uid]->setIcon(button_pixmap);
	tool_button[current_uid]->setIconSize(button_pixmap.size());
	tool_button[current_uid]->setFixedSize(button_pixmap.width()+50, button_pixmap.height()+35);
	tool_button[current_uid]->setObjectName("functionButton");

	tool_button[current_uid]->setGeometry(QRect(80, 120, 100, 100));
	tool_button[current_uid]->setText(tr("AC %1 off") .arg(current_uid+1));
	
	QHBoxLayout *h_layout0 = new QHBoxLayout();
	h_layout0->addWidget(tool_button[current_uid]);
	widget->setLayout(h_layout0);
	//wid_get2->setLayout(h_layout0);
	connect(tool_button[current_uid], SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

#define WINDOW_W 140
#define WINDOW_H 125
#define WINDOW_X 100
#define WINDOW_Y 90
#define ADD_H 30
#define FONT_H 95
#define FONT_W 53 //50 

#define D_ABC_x 0
#define B_ABC_x FONT_W
#define A_y -70
#define B_y ADD_H-10
#define C_y ADD_H+FONT_H+10
#define D_x D_ABC_x+WINDOW_W
#define D_y ADD_H-10
#define E_x D_ABC_x-FONT_W
#define E_y ADD_H-10
#define FONT_MOVE_TIME 200
static int g_b_flg[MAX_AC_NUM*3];
static int g_db_flg[MAX_AC_NUM*3];
static int decade_bit[MAX_AC_NUM*3];

void HavcWidget::create_ctrl_widget(QWidget *widget,int index)
{
	int add_offset = 0;
	QPushButton* button_on = new QPushButton(widget);
	QLabel *label_name_local = new QLabel(widget);
	QLabel *label_img = new QLabel(widget);
	memset(g_b_flg,1,MAX_AC_NUM*3);
	memset(g_db_flg,1,MAX_AC_NUM*3);
	memset(decade_bit,0,MAX_AC_NUM*3);

	add_offset = 320*(index+1);

	wid_temp_window[current_uid]=new QWidget(widget);
	wid_temp_window[current_uid]->setFixedSize(WINDOW_W,WINDOW_H);
	wid_temp_window[current_uid]->setStyleSheet("QWidget{background-color:rgb(0, 0, 0);}");
	wid_temp_window[current_uid]->move(WINDOW_X+add_offset, WINDOW_Y);

	stAc[current_uid].label_name = label_name_local;
	stAc[current_uid].button_on = button_on;
	stAc[current_uid].label_img = label_img;

    connect(button_on, SIGNAL(released()), this, SLOT(txtButton()));

	button_down[current_uid] = new QToolButton(widget);
	button_down[current_uid]->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QPixmap button_down_pixmap("havc/down.png");
	button_down[current_uid]->setIcon(button_down_pixmap);
	button_down[current_uid]->setIconSize(button_down_pixmap.size());
	button_down[current_uid]->setFixedSize(button_down_pixmap.width()+0, button_down_pixmap.height()+0);
	
	if (stAc[current_uid].target_temperature < 16)
		stAc[current_uid].target_temperature = 16;
	else if (stAc[current_uid].target_temperature > 32)
		stAc[current_uid].target_temperature = 32;

	pLabel_temp_decade[current_uid] = new QLabel("2", wid_temp_window[current_uid]);
	QString temp;
	pLabel_temp_decade[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");//edit by iovine old:100px
    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
    pLabel_temp_decade[current_uid]->setText(temp);

    pLabel_temp_decade_brother[current_uid] = new QLabel("1", wid_temp_window[current_uid]);
    pLabel_temp_decade_brother[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", 1);
    pLabel_temp_decade_brother[current_uid]->setText(temp);

    pLabel_temp_bits[current_uid] = new QLabel("8", wid_temp_window[current_uid]);
	pLabel_temp_bits[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
    pLabel_temp_bits[current_uid]->setText(temp);

    pLabel_temp_bits_brother[current_uid] = new QLabel("9", wid_temp_window[current_uid]);
    pLabel_temp_bits_brother[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
    pLabel_temp_bits_brother[current_uid]->setText(temp);

    pLabel_temp_unit[current_uid] = new QLabel(wid_temp_window[current_uid]);
  	pLabel_temp_unit[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%c", 0xB0);
    pLabel_temp_unit[current_uid]->setText(temp);

    pLabel_ac[current_uid] = new QLabel("2", widget);//for test

	button_up[current_uid] = new QToolButton(widget);
	button_up[current_uid]->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QPixmap button_up_pixmap("havc/up.png");
	button_up[current_uid]->setIcon(button_up_pixmap);
	button_up[current_uid]->setIconSize(button_up_pixmap.size());
	button_up[current_uid]->setFixedSize(button_up_pixmap.width()+0, button_up_pixmap.height()+0);

#if 0
	QHBoxLayout *h_layout0 = new QHBoxLayout();
	h_layout0->addWidget(label_img);
	h_layout0->addWidget(label_name_local);
	h_layout0->addSpacing(60);
	h_layout0->addWidget(button_on);
	
	QHBoxLayout *h_layout1 = new QHBoxLayout();
	//wid_get1->setGeometry(QRect(90, 140, 100, 100));
	h_layout1->addWidget(button_down[current_uid]);
	h_layout1->addWidget(pLabel_ac[current_uid]);
	h_layout1->addWidget(button_up[current_uid]);
#endif
	tool_button_mode[current_uid] = new QToolButton(widget);
	//tool_button_mode[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:bold;border-image:url(havc/mode_cool.png);"));
	set_ac_mode_init(current_uid);

	tool_button_fan[current_uid] = new QToolButton(widget);
	//tool_button_fan[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:bold;border-image:url(havc/fan_low.png);"));
	//tool_button_fan[current_uid]->setText(tr("    low"));
	set_fan_speed_init(current_uid);


#if 0
	QHBoxLayout *h_layout2 = new QHBoxLayout();
	h_layout2->addWidget(tool_button_mode[current_uid]);
	h_layout2->addWidget(tool_button_fan[current_uid]);

	QVBoxLayout *v_layout = new QVBoxLayout();
	//v_layout->addWidget(button_on);
	v_layout->addLayout(h_layout0);
	v_layout->addLayout(h_layout1);
	v_layout->addLayout(h_layout2);
	
	//v_layout->setSpacing(30);
	//v_layout->setContentsMargins(0, 0, 0, 0);
	v_layout->setMargin(0);
	v_layout->setSpacing(0);
#else
    //fisrt row
    label_img->setGeometry(QRect(0+add_offset, 5, 60, 60));
    label_name_local->setGeometry(QRect(80+add_offset, 5, 160, 60));
	button_on->setGeometry(QRect(245+add_offset, 0, 75, 75));

	//second row
	button_down[current_uid]->setGeometry(QRect(0+add_offset, 120, 75, 75));
	button_up[current_uid]->setGeometry(QRect(245+add_offset, 120, 75, 75));

	pLabel_temp_decade[current_uid]->setGeometry(QRect(D_ABC_x, B_y, FONT_W, FONT_H));
	pLabel_temp_decade_brother[current_uid]->setGeometry(QRect(E_x, E_y, FONT_W, FONT_H));

	pLabel_temp_bits[current_uid]->setGeometry(QRect(B_ABC_x, B_y, FONT_W, FONT_H));
	pLabel_temp_bits_brother[current_uid]->setGeometry(QRect(D_x, D_y, FONT_W, FONT_H));


	pLabel_temp_unit[current_uid]->setGeometry(QRect(FONT_W*2, ADD_H-10, 43, FONT_H));//40

	//third row
	tool_button_mode[current_uid]->setGeometry(QRect(0+add_offset, 235, 155, 64));//edit by iovine  240->235
	tool_button_fan[current_uid]->setGeometry(QRect(164+add_offset, 235, 155, 64));

#endif

	if (stAc[current_uid].switch_on)
	{
		button_down[current_uid]->setVisible(1);
		button_up[current_uid]->setVisible(1);
	}
    else
    {
		button_down[current_uid]->setVisible(0);
		button_up[current_uid]->setVisible(0);

		tool_button_mode[current_uid]->setEnabled(false);
		tool_button_fan[current_uid]->setEnabled(false);

		set_ac_mode_off(current_uid);
		set_fan_speed_off(current_uid);

		pLabel_temp_decade[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		pLabel_temp_bits[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		pLabel_temp_unit[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		g_b_flg[current_uid] = true;
		g_db_flg[current_uid] = true;
    }

	set_button_content(current_uid);

	connect(tool_button_mode[current_uid], SIGNAL(clicked()), this, SLOT(set_ac_mode()));
	connect(tool_button_fan[current_uid], SIGNAL(clicked()), this, SLOT(set_fan_speed()));
	connect(button_down[current_uid], SIGNAL(clicked()), this, SLOT(ac_temp_down()));
	connect(button_up[current_uid], SIGNAL(clicked()), this, SLOT(ac_temp_up()));

	g_ctrl_creat_complate = true;
    //widget->setLayout(v_layout);
}
void HavcWidget::create_purify_widget(QWidget *widget,int index)
{
	int add_offset = 0;
	QPushButton* button_on = new QPushButton(widget);
	QLabel *label_name_local = new QLabel(widget);
	QLabel *label_img = new QLabel(widget);
	add_offset = 320*(index+1);

	stAc[current_uid].label_name = label_name_local;
	stAc[current_uid].label_img = label_img;
	stAc[current_uid].button_on = button_on;
	
	connect(button_on, SIGNAL(released()), this, SLOT(txtButton()));	

	pLabel_purify[current_uid] = new QLabel("PM2.5",widget);
	pLabel_purify[current_uid]->setStyleSheet("font-size:30px;color:rgb(180,180,180);font-weight:bold;background:transparent");
	pLabel_purify[current_uid]->setAlignment(Qt::AlignCenter);

	pLabel_PM25[current_uid] = new QLabel("--",widget);
	pLabel_PM25[current_uid]->setStyleSheet("font-size:78px;color:rgb(180, 180, 180);font-weight:normal;background:transparent");//old 96px edit by iovine 2023.10.07
	pLabel_PM25[current_uid]->setAlignment(Qt::AlignCenter);

	tool_button_mode[current_uid] = new QToolButton(widget);
	//tool_button_mode[current_uid]->setText(tr("    cool"));
	//tool_button_mode[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:bold;border-image:url(havc/mode_cool.png);"));
	set_ac_mode_init(current_uid);

	tool_button_fan[current_uid] = new QToolButton(widget);
	//tool_button_fan[current_uid]->setText(tr("    low"));
	//tool_button_fan[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:bold;border-image:url(havc/fan_low.png);"));
	set_fan_speed_init(current_uid);

    //pLabel_humidity[current_uid] = new QLabel(tr("Wet"),widget);
	//pLabel_humidity[current_uid]->setStyleSheet("font-size:40px;color:white;font-weight:bold");
	
	effect_ac_fan[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_n[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_pm[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_hum[current_uid]= new QGraphicsOpacityEffect();

	if (!stAc[current_uid].switch_on)
	{
		tool_button_fan[current_uid]->setEnabled(false);
		tool_button_mode[current_uid]->setEnabled(false);
		set_fan_speed_off(current_uid);
		set_ap_mode_off(current_uid);
		//pLabel_PM25[current_uid]->setStyleSheet("font-size:96px;color:rgb(180,180,180);font-weight:normal;background:transparent");
		//pLabel_humidity[current_uid]->setStyleSheet("font-size:40px;color:rgb(180,180,180);font-weight:bold");
    }

	connect(tool_button_fan[current_uid], SIGNAL(clicked()), this, SLOT(set_fan_speed()));
	connect(tool_button_mode[current_uid], SIGNAL(clicked()), this, SLOT(set_ap_mode()));

    //fisrt row
    label_img->setGeometry(QRect(0+add_offset, 5, 60, 60));
    label_name_local->setGeometry(QRect(70+add_offset, 5, 160, 60));
	button_on->setGeometry(QRect(245+add_offset, 0, 75, 75));

	//second row
	//pLabel_PM25[current_uid] ->setGeometry(QRect(110, 100, 200, 110));
	//pLabel_purify[current_uid]->setGeometry(QRect(125, 185, 150, 40));

	pLabel_PM25[current_uid] ->setGeometry(QRect(0+add_offset, 100, 320, 90));//h:110
	pLabel_purify[current_uid]->setGeometry(QRect(0+add_offset, 185, 320, 40));

	//third row
	//third row
	tool_button_mode[current_uid]->setGeometry(QRect(0+add_offset, 235, 155, 64));//edit by iovine  240->235
	tool_button_fan[current_uid]->setGeometry(QRect(164+add_offset, 235, 155, 64));
	//tool_button_fan[current_uid]->setGeometry(QRect(0+add_offset, 235, 155, 64));
//	pLabel_humidity[current_uid]->setGeometry(QRect(220+add_offset, 245, 100, 50));
	set_button_content(current_uid);

	g_purify_creat_complate = true;


	if (access(AQI_HIDE_FILE, F_OK) == 0)
	{
		pLabel_purify[current_uid]->setVisible(0);//add  by iovine
		pLabel_PM25[current_uid]->setVisible(0);//add  by iovine
		tool_button_mode[current_uid]->setVisible(0);//add  by iovine
	}

}
void HavcWidget::create_floor_widget(QWidget *widget,int index)
{
	int add_offset = 0;
	QPushButton* button_on = new QPushButton(widget);
	QLabel *label_name_local = new QLabel(widget);
	QLabel *label_img = new QLabel(widget);
	memset(g_b_flg,1,MAX_AC_NUM*3);
	memset(g_db_flg,1,MAX_AC_NUM*3);
	memset(decade_bit,0,MAX_AC_NUM*3);
	add_offset = 320*(index+1);
	wid_temp_window[current_uid]=new QWidget(widget);
	wid_temp_window[current_uid]->setFixedSize(WINDOW_W,WINDOW_H);
	wid_temp_window[current_uid]->setStyleSheet("QWidget{background-color:rgb(0, 0, 0);}");
	wid_temp_window[current_uid]->move(WINDOW_X+add_offset, WINDOW_Y+10);

	stAc[current_uid].label_img = label_img;
	stAc[current_uid].label_name = label_name_local;
	stAc[current_uid].button_on = button_on;

	connect(button_on, SIGNAL(released()), this, SLOT(txtButton()));

	button_down[current_uid] = new QToolButton(widget);
	button_down[current_uid]->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QPixmap button_down_pixmap("havc/down.png");
	button_down[current_uid]->setIcon(button_down_pixmap);
	button_down[current_uid]->setIconSize(button_down_pixmap.size());
	button_down[current_uid]->setFixedSize(button_down_pixmap.width()+0, button_down_pixmap.height()+0);

	if(stAc[current_uid].target_temperature < 16)
		stAc[current_uid].target_temperature = 16;
	else if(stAc[current_uid].target_temperature > 30)
		stAc[current_uid].target_temperature = 30;

	qDebug()<<"1target_temperature="<<stAc[current_uid].target_temperature;

	pLabel_temp_decade[current_uid] = new QLabel("2", wid_temp_window[current_uid]);
	QString temp;
	pLabel_temp_decade[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");//edit by iovine old:100px
    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
    pLabel_temp_decade[current_uid]->setText(temp);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",pLabel_temp_decade, temp="<<temp;

    pLabel_temp_decade_brother[current_uid] = new QLabel("1", wid_temp_window[current_uid]);
    pLabel_temp_decade_brother[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", 1);
    pLabel_temp_decade_brother[current_uid]->setText(temp);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",pLabel_temp_decade_brother, temp="<<temp;

    pLabel_temp_bits[current_uid] = new QLabel("8", wid_temp_window[current_uid]);
	pLabel_temp_bits[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
    pLabel_temp_bits[current_uid]->setText(temp);

	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",pLabel_temp_bits, temp="<<temp;

    pLabel_temp_bits_brother[current_uid] = new QLabel("9", wid_temp_window[current_uid]);
    pLabel_temp_bits_brother[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
    pLabel_temp_bits_brother[current_uid]->setText(temp);
	 
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",pLabel_temp_bits_brother, temp="<<temp;

    pLabel_temp_unit[current_uid] = new QLabel(wid_temp_window[current_uid]);
  	pLabel_temp_unit[current_uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
    temp.sprintf("%c", 0xB0);
    pLabel_temp_unit[current_uid]->setText(temp);

    pLabel_ac[current_uid] = new QLabel("2", widget);//for test


	button_up[current_uid] = new QToolButton(widget);
	button_up[current_uid]->setToolButtonStyle(Qt::ToolButtonIconOnly);
	QPixmap button_up_pixmap("havc/up.png");
	button_up[current_uid]->setIcon(button_up_pixmap);
	button_up[current_uid]->setIconSize(button_up_pixmap.size());
	button_up[current_uid]->setFixedSize(button_up_pixmap.width()+0, button_up_pixmap.height()+0);

    //auto
	tool_button_mode[current_uid] = new QToolButton(widget);
	//tool_button_mode[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:600;border-image:url(havc/mode_auto.png);"));
	//tool_button_mode[current_uid]->setText(tr(" 	 Auto"));

    //man
	tool_button_fan[current_uid] = new QToolButton(widget);
	//tool_button_fan[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:600;border-image:url(havc/mode_auto.png);"));
	//tool_button_fan[current_uid]->setText(tr(" 	     Manu"));
	//set_floor_auto_man_switch(current_uid);
	
	//set_fan_speed_init(current_uid);

    //fisrt row
    label_img->setGeometry(QRect(0+add_offset, 5, 60, 60));
    label_name_local->setGeometry(QRect(70+add_offset, 5, 160, 60));
	button_on->setGeometry(QRect(245+add_offset, 0, 75, 75));

	//second row
	button_down[current_uid]->setGeometry(QRect(0+add_offset, 130, 75, 75));
	button_up[current_uid]->setGeometry(QRect(245+add_offset, 130, 75, 75));

	pLabel_temp_decade[current_uid]->setGeometry(QRect(D_ABC_x, B_y, FONT_W, FONT_H)); 
	pLabel_temp_decade_brother[current_uid]->setGeometry(QRect(E_x, E_y, FONT_W, FONT_H));

	pLabel_temp_bits[current_uid]->setGeometry(QRect(B_ABC_x, B_y, FONT_W, FONT_H)); 
	pLabel_temp_bits_brother[current_uid]->setGeometry(QRect(D_x, D_y, FONT_W, FONT_H));


	pLabel_temp_unit[current_uid]->setGeometry(QRect(FONT_W*2, ADD_H-10, 40, FONT_H));

	tool_button_mode[current_uid]->setGeometry(QRect(0+add_offset, 235, 155, 64));//edit by iovine  240->235
	tool_button_fan[current_uid]->setGeometry(QRect(164+add_offset, 235, 155, 64));

	effect_ac_up[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_down[current_uid]= new QGraphicsOpacityEffect();

	effect_ac_d[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_b[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_u[current_uid]= new QGraphicsOpacityEffect();
	effect_ac_n[current_uid]= new QGraphicsOpacityEffect();

	tool_button_fan[current_uid]->setVisible(0);

	if (stAc[current_uid].switch_on)
	{
		button_down[current_uid]->setVisible(1);
		button_up[current_uid]->setVisible(1);
		
        #ifndef ENNO_HILINK_AUTH
		tool_button_mode[current_uid]->setVisible(1);
		ENNO_LOG_ERR("setVisible = 1\n");
		#endif
		set_floor_auto_man_switch(current_uid);		
	}
    else
    {
		button_down[current_uid]->setVisible(0);
		button_up[current_uid]->setVisible(0);
		pLabel_temp_decade[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		pLabel_temp_bits[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		pLabel_temp_unit[current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		g_b_flg[current_uid] = true;
		g_db_flg[current_uid] = true;

		//ENNO_LOG_ERR("floor_mode all false\n");
		tool_button_mode[current_uid]->setVisible(0);
		
		//ENNO_LOG_ERR("setVisible = 0\n");

		tool_button_mode[current_uid]->setEnabled(false);
		tool_button_fan[current_uid]->setEnabled(false);

		tool_button_mode[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:600;border-image:url(havc/auto_off.png);"));
		tool_button_mode[current_uid]->setText(tr("      Auto"));
		tool_button_fan[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:600;border-image:url(havc/man_off.png);"));
		tool_button_fan[current_uid]->setText(tr("      Manu"));
    }
	set_button_content(current_uid);

	connect(tool_button_mode[current_uid], SIGNAL(clicked()), this, SLOT(set_floor_auto()));
	//connect(tool_button_fan[current_uid], SIGNAL(clicked()), this, SLOT(set_floor_man()));

	connect(button_down[current_uid], SIGNAL(clicked()), this, SLOT(ac_temp_down()));
	connect(button_up[current_uid], SIGNAL(clicked()), this, SLOT(ac_temp_up()));
}


void HavcWidget::set_ac_mode()
{
	//printf("XXXXstAc[current_uid].mode=%d current_uid=%d\r\n",stAc[current_uid].mode,current_uid);
	if (AC_MODE_OFF == stAc[current_uid].mode)
	{
		stAc[current_uid].mode = AC_MODE_HEAT;
	}
	else if (AC_MODE_AUTO == stAc[current_uid].mode)
	{
		stAc[current_uid].mode = AC_MODE_HEAT;
	}
    else if (AC_MODE_HEAT == stAc[current_uid].mode)
    {
        stAc[current_uid].mode = AC_MODE_COOL;
    }
	else if (AC_MODE_COOL == stAc[current_uid].mode)
	{
	    stAc[current_uid].mode = AC_MODE_DEHUMIDIFY;
	}
	else if (AC_MODE_DEHUMIDIFY == stAc[current_uid].mode)
	{
		stAc[current_uid].mode = AC_MODE_FAN;
	}
	else if (AC_MODE_FAN == stAc[current_uid].mode)
	{
		stAc[current_uid].mode = AC_MODE_AUTO;
	}

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
		CHAR_AC_TARGET_HEATING_COOLING_STATE, stAc[current_uid].mode);
	

	//tool_button_mode[current_uid]->update();
}

void HavcWidget::set_ap_mode()
{
	//printf("XXXXstAc[current_uid].mode=%d current_uid=%d\r\n",stAc[current_uid].mode,current_uid);
	if (AP_MODE_IN_CYCLE == stAc[current_uid].mode)
	{
		stAc[current_uid].mode = AP_MODE_OUT_CYCLE;
	}
	else
	{
		stAc[current_uid].mode = AP_MODE_IN_CYCLE;
	}

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
		CHAR_AP_CYCLE_MODE, stAc[current_uid].mode);
	//tool_button_mode[current_uid]->update();
}

void HavcWidget::set_fan_speed()
{
//	QPixmap pixmap;

    if (stAc[current_uid].fan_speed == AC_FAN_LOW)
    {
        stAc[current_uid].fan_speed = AC_FAN_MIDDLE;
    }
	else if (stAc[current_uid].fan_speed == AC_FAN_MIDDLE)
	{
	    stAc[current_uid].fan_speed = AC_FAN_HIGH;
	}
	else if (stAc[current_uid].fan_speed == AC_FAN_HIGH)
	{
	    stAc[current_uid].fan_speed = AC_FAN_AUTO;
	}
	else if (stAc[current_uid].fan_speed == AC_FAN_AUTO)
	{
		stAc[current_uid].fan_speed = AC_FAN_LOW;
	}

    if (DEV_RS485_AIR_PURIFIER == stAc[current_uid].dev_type)
    {
		g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
			CHAR_AP_ROTATION_SPEED, stAc[current_uid].fan_speed);
    }
	else
	{
	    g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
			CHAR_AC_ROTATION_SPEED, stAc[current_uid].fan_speed);
	}

	//tool_button_fan[current_uid]->update();
}


static bool g_move_end_flg = true;
void HavcWidget::font_move_end()
{
	g_move_end_flg = true;
}

void HavcWidget::ac_temp_down()
{
   // printf("ac_temp_down, temp=%d\n", stAc[current_uid].target_temperature );
	if(!stAc[current_uid].switch_on)
		return;
	if(g_move_end_flg == false)
	{
		printf("ac_temp_down, but move is not complete\n");
		return;
	}
	decade_bit[current_uid] = stAc[current_uid].target_temperature/10;
	if (stAc[current_uid].target_temperature > 16)
	{
	    stAc[current_uid].target_temperature--;
	}
	else
	{
	    stAc[current_uid].target_temperature = 16;
	    return;
	}

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
			CHAR_AC_TARGET_TEMPERATURE, stAc[current_uid].target_temperature);

	QString temp;

	QPropertyAnimation *anim_bb = new QPropertyAnimation(pLabel_temp_bits[current_uid], "geometry");
	QPropertyAnimation *anim_bd = new QPropertyAnimation(pLabel_temp_bits_brother[current_uid], "geometry");
	//if(decade_bits>stAc[current_uid].target_temperature/10)
	//{
	QPropertyAnimation *anim_db = new QPropertyAnimation(pLabel_temp_decade[current_uid], "geometry");
	QPropertyAnimation *anim_de = new QPropertyAnimation(pLabel_temp_decade_brother[current_uid], "geometry");
	//}
	//group_bits = new QParallelAnimationGroup;
	group_bits->clear();

	if(g_b_flg[current_uid])
	{
		anim_bb->setDuration(FONT_MOVE_TIME);
		anim_bb->setKeyValueAt(0, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.8, QRect(B_ABC_x, C_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.9, QRect(D_x, C_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(1, QRect(D_x, D_y, FONT_W, FONT_H));

		anim_bd->setDuration(FONT_MOVE_TIME);
		anim_bd->setKeyValueAt(0, QRect(D_x, D_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.1, QRect(D_x, A_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.2, QRect(B_ABC_x, A_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(1, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		g_b_flg[current_uid] = false;
	    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
	    pLabel_temp_bits_brother[current_uid]->setText(temp);

	}
	else
	{
		anim_bd->setDuration(FONT_MOVE_TIME);
		anim_bd->setKeyValueAt(0, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.8, QRect(B_ABC_x, C_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.9, QRect(D_x, C_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(1, QRect(D_x, D_y, FONT_W, FONT_H));

		anim_bb->setDuration(FONT_MOVE_TIME);
		anim_bb->setKeyValueAt(0, QRect(D_x, D_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.1, QRect(D_x, A_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.2, QRect(B_ABC_x, A_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(1, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		g_b_flg[current_uid] = true;
	    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
	    pLabel_temp_bits[current_uid]->setText(temp);
	}

	if(decade_bit[current_uid]>stAc[current_uid].target_temperature/10)
	{
		if(g_db_flg[current_uid])
		{
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(0, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.8, QRect(D_ABC_x, C_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.9, QRect(E_x, C_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));

			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(0, QRect(E_x, E_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.1, QRect(E_x, A_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.2, QRect(D_ABC_x, A_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			g_db_flg[current_uid] = false;
		    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
		    pLabel_temp_decade_brother[current_uid]->setText(temp);
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",temp="<<temp;
		}
		else
		{
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(0, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.8, QRect(D_ABC_x, C_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.9, QRect(E_x, C_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));

			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(0, QRect(E_x, E_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.1, QRect(E_x, A_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.2, QRect(D_ABC_x, A_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			g_db_flg[current_uid] = true;
		    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",temp="<<temp;
		    pLabel_temp_decade[current_uid]->setText(temp);
		}


	}
	else
	{
		if(g_db_flg[current_uid])
		{
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));
		}
		else
		{
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));
		}
	}
	group_bits->addAnimation(anim_db);
	group_bits->addAnimation(anim_de);
	group_bits->addAnimation(anim_bb);
	group_bits->addAnimation(anim_bd);
	group_bits->start();
	g_move_end_flg = false;
	QTimer::singleShot(FONT_MOVE_TIME, this, SLOT(font_move_end()));

}


void HavcWidget::ac_temp_up()
{
    //printf("ac_temp_up, temp=%d\n", stAc[current_uid].target_temperature );
	if(!stAc[current_uid].switch_on)
		return;

	if(g_move_end_flg == false)
	{
		printf("ac_temp_up, but move is not complete\n");
		return;
	}

	decade_bit[current_uid] = stAc[current_uid].target_temperature/10;

	if (stAc[current_uid].target_temperature < MAX_AC_TEMPERATURE)
	{
	    stAc[current_uid].target_temperature++;
	}
	else
	{
	    stAc[current_uid].target_temperature = MAX_AC_TEMPERATURE;
	    return;
	}

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
			CHAR_AC_TARGET_TEMPERATURE, stAc[current_uid].target_temperature);

	QString temp;

	QPropertyAnimation *anim_bb = new QPropertyAnimation(pLabel_temp_bits[current_uid], "geometry");
	QPropertyAnimation *anim_bd = new QPropertyAnimation(pLabel_temp_bits_brother[current_uid], "geometry");
	//if(decade_bit<stAc[current_uid].target_temperature/10)
	//{
		QPropertyAnimation *anim_db = new QPropertyAnimation(pLabel_temp_decade[current_uid], "geometry");
		QPropertyAnimation *anim_de = new QPropertyAnimation(pLabel_temp_decade_brother[current_uid], "geometry");
	//}
	//group_bits = new QParallelAnimationGroup;
	group_bits->clear();

	if(g_b_flg[current_uid])
	{
		anim_bb->setDuration(FONT_MOVE_TIME);
		anim_bb->setKeyValueAt(0, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.8, QRect(B_ABC_x, A_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.9, QRect(D_x, A_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(1, QRect(D_x, D_y, FONT_W, FONT_H));

		anim_bd->setDuration(FONT_MOVE_TIME);
		anim_bd->setKeyValueAt(0, QRect(D_x, D_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.1, QRect(D_x, C_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.2, QRect(B_ABC_x, C_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(1, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		g_b_flg[current_uid] = false;
	    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
	    pLabel_temp_bits_brother[current_uid]->setText(temp);

	}
	else
	{
		anim_bd->setDuration(FONT_MOVE_TIME);
		anim_bd->setKeyValueAt(0, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.8, QRect(B_ABC_x, A_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(0.9, QRect(D_x, A_y, FONT_W, FONT_H));
		anim_bd->setKeyValueAt(1, QRect(D_x, D_y, FONT_W, FONT_H));

		anim_bb->setDuration(FONT_MOVE_TIME);
		anim_bb->setKeyValueAt(0, QRect(D_x, D_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.1, QRect(D_x, C_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(0.2, QRect(B_ABC_x, C_y, FONT_W, FONT_H));
		anim_bb->setKeyValueAt(1, QRect(B_ABC_x, B_y, FONT_W, FONT_H));
		g_b_flg[current_uid] = true;
	    temp.sprintf("%d", stAc[current_uid].target_temperature%10);
	    pLabel_temp_bits[current_uid]->setText(temp);
	}

	if(decade_bit[current_uid]<stAc[current_uid].target_temperature/10)
	{
		if(g_db_flg[current_uid])
		{
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(0, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.8, QRect(D_ABC_x, A_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.9, QRect(E_x, A_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));

			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(0, QRect(E_x, E_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.1, QRect(E_x, C_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.2, QRect(D_ABC_x, C_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			g_db_flg[current_uid] = false;
		    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
		    pLabel_temp_decade_brother[current_uid]->setText(temp);

		}
		else
		{
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(0, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.8, QRect(D_ABC_x, A_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(0.9, QRect(E_x, A_y, FONT_W, FONT_H));
			anim_de->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));

			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(0, QRect(E_x, E_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.1, QRect(E_x, C_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(0.2, QRect(D_ABC_x, C_y, FONT_W, FONT_H));
			anim_db->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			g_db_flg[current_uid] = true;
		    temp.sprintf("%d", stAc[current_uid].target_temperature/10);
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",temp="<<temp;
		    pLabel_temp_decade[current_uid]->setText(temp);
		}


	}
	else
	{
		if(g_db_flg[current_uid])
		{
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));
		}
		else
		{
			anim_de->setDuration(FONT_MOVE_TIME);
			anim_de->setKeyValueAt(1, QRect(D_ABC_x, B_y, FONT_W, FONT_H));
			anim_db->setDuration(FONT_MOVE_TIME);
			anim_db->setKeyValueAt(1, QRect(E_x, E_y, FONT_W, FONT_H));
		}
	}
	group_bits->addAnimation(anim_db);
	group_bits->addAnimation(anim_de);
	group_bits->addAnimation(anim_bb);
	group_bits->addAnimation(anim_bd);
	group_bits->start();
	g_move_end_flg = false;
	QTimer::singleShot(FONT_MOVE_TIME, this, SLOT(font_move_end()));

}
void HavcWidget::set_ac_mode_init(int uid)
{

	if (AC_MODE_AUTO == stAc[uid].mode)
	{
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_auto.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
	}
    else if (AC_MODE_HEAT == stAc[uid].mode)
    {
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_heat.png);"));
		tool_button_mode[uid]->setText(tr("      Heat"));
    }
	else if (AC_MODE_COOL == stAc[uid].mode)
	{
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_cool.png);"));
		tool_button_mode[uid]->setText(tr("      Cool"));
	}
	else if (AC_MODE_DEHUMIDIFY == stAc[uid].mode)
	{
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_dehumidify.png);"));
		tool_button_mode[uid]->setText(tr("      Dry"));
	}
	else if (AC_MODE_FAN == stAc[uid].mode)
	{
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_fan.png);"));
		tool_button_mode[uid]->setText(tr("      Fan"));
	}
	else
	{
		//printf("stAc[current_uid].mode is invalid! so new one \r\n");
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_auto.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
		stAc[uid].mode = AC_MODE_AUTO;
	}

	tool_button_mode[uid]->update();
}


void HavcWidget::set_ac_mode_off(int uid)
{

	if (AC_MODE_AUTO == stAc[uid].mode)
	{
		//tool_button_mode[current_uid]->setText(tr("    heat"));
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/auto_off.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
	}
    else if (AC_MODE_HEAT == stAc[uid].mode)
    {
    	tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/mode_heat_off.png);"));
    	tool_button_mode[uid]->setText(tr("      Heat"));
    }
	else if (AC_MODE_COOL == stAc[uid].mode)
	{
    	tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/mode_cool_off.png);"));
    	tool_button_mode[uid]->setText(tr("      Cool"));
	}
	else if (AC_MODE_DEHUMIDIFY == stAc[uid].mode)
	{
    	tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/mode_dehumidify_off.png);"));
    	tool_button_mode[uid]->setText(tr("      Dry"));
	}
	else if (AC_MODE_FAN == stAc[uid].mode)
	{
    	tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/mode_fan_off.png);"));
    	tool_button_mode[uid]->setText(tr("      Fan"));
	}
	else
	{
		//printf("stAc[current_uid].mode off  is invalid! so new one \r\n");
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/auto_off.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
		stAc[uid].mode = AC_MODE_AUTO;
	}

	tool_button_mode[uid]->update();
}
void HavcWidget::set_ap_mode_off(int uid)
{

	if (AP_MODE_IN_CYCLE == stAc[uid].mode)
	{
		//tool_button_mode[current_uid]->setText(tr("    heat"));
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/ap_in_cycle_off.png);"));
		tool_button_mode[uid]->setText(tr("In Cycle"));
	}
    else
    {
    	tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/ap_out_cycle_off.png);"));
    	tool_button_mode[uid]->setText(tr("Out Cycle"));
    }
	
	tool_button_mode[uid]->update();
}

void HavcWidget::set_ap_mode_init(int uid)
{

	if (AP_MODE_IN_CYCLE == stAc[uid].mode)
	{
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/ap_in_cycle.png);"));
		tool_button_mode[uid]->setText(tr("In Cycle"));
	}
	else
	{
		//printf("stAc[current_uid].mode is invalid! so new one \r\n");
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/ap_out_cycle.png);"));
		tool_button_mode[uid]->setText(tr("Out Cycle"));
		stAc[uid].mode = AP_MODE_OUT_CYCLE;
	}

	tool_button_mode[uid]->update();
}

void HavcWidget::set_floor_auto()
{
	//printf("XXXXstAc[current_uid].mode=%d current_uid=%d\r\n",stAc[current_uid].mode,current_uid);
	stAc[current_uid].floor_ctrl_mode = !stAc[current_uid].floor_ctrl_mode;

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
		CHAR_AC_FLOOR_HEAT_AUTO_MAN_CTRL, stAc[current_uid].floor_ctrl_mode);
	//tool_button_mode[current_uid]->update();
}

void HavcWidget::set_floor_man()
{
	//printf("XXXXstAc[current_uid].mode=%d current_uid=%d\r\n",stAc[current_uid].mode,current_uid);
	stAc[current_uid].floor_ctrl_mode = 1;

	g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
		CHAR_AC_FLOOR_HEAT_AUTO_MAN_CTRL, stAc[current_uid].floor_ctrl_mode);
	//tool_button_mode[current_uid]->update();
}

void HavcWidget::set_floor_auto_man_switch(int uid)
{	
    if (stAc[uid].floor_ctrl_mode == 0)
    {
        //tool_button_mode[uid]->setEnabled(false);
		//tool_button_fan[uid]->setEnabled(true);
		tool_button_mode[uid]->setEnabled(true);
		//ENNO_LOG_ERR("floor_mode man true, auto false\n");

		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_auto.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
		tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/man_off.png);"));
		tool_button_fan[uid]->setText(tr("      Manu"));
    }
	else
	{
	   // tool_button_mode[uid]->setEnabled(true);
		//tool_button_fan[uid]->setEnabled(false);
		
	   tool_button_mode[uid]->setEnabled(true);
		//ENNO_LOG_ERR("floor_mode man false, auto true\n");
		
		tool_button_mode[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/auto_off.png);"));
		tool_button_mode[uid]->setText(tr("      Auto"));
		tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/mode_man.png);"));
		tool_button_fan[uid]->setText(tr("      Manu"));
	}
}

void HavcWidget::set_fan_speed_off(int uid)
{

    if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
    {
	    if (stAc[uid].fan_speed == AC_FAN_LOW)
	    {
	        tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
	        tool_button_fan[uid]->setText(tr("      Low fan"));
	    }
		else if (stAc[uid].fan_speed == AC_FAN_MIDDLE)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Mid fan"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_HIGH)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
			 tool_button_fan[uid]->setText(tr("      High fan"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_AUTO)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_auto_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Auto"));
		}
		else
		{
			//printf("stAc[current_uid].fan_speed off init is invalid so new one\r\n");
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_auto_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Auto"));
	    	stAc[uid].fan_speed =AC_FAN_LOW;
		}
    }
	else
	{
	    if (stAc[uid].fan_speed == AC_FAN_LOW)
	    {
	        tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
	        tool_button_fan[uid]->setText(tr("      Low"));
	    }
		else if (stAc[uid].fan_speed == AC_FAN_MIDDLE)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Mid"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_HIGH)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_off.png);"));
			 tool_button_fan[uid]->setText(tr("      High"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_AUTO)
		{
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_auto_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Auto"));
		}
		else
		{
			//printf("stAc[current_uid].fan_speed off init is invalid so new one\r\n");
			 tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/fan_auto_off.png);"));
			 tool_button_fan[uid]->setText(tr("      Auto"));
	    	stAc[uid].fan_speed =AC_FAN_LOW;
		}
	}
	tool_button_fan[uid]->update();
}

void HavcWidget::set_fan_speed_init(int uid)
{
	if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
	{
	    if (stAc[uid].fan_speed == AC_FAN_LOW)
	    {
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_low.png);"));
	    	tool_button_fan[uid]->setText(tr("      Low fan"));
	    }
		else if (stAc[uid].fan_speed == AC_FAN_MIDDLE)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_mid.png);"));
	    	tool_button_fan[uid]->setText(tr("      Mid fan"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_HIGH)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_high.png);"));
	    	tool_button_fan[uid]->setText(tr("      High fan"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_AUTO)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	    	tool_button_fan[uid]->setText(tr("      Auto"));
		}
		else
		{
			//printf("stAc[current_uid].fan_speed init is invalid so new one\r\n");
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	    	tool_button_fan[uid]->setText(tr("      Auto"));
	    	stAc[uid].fan_speed =AC_FAN_LOW;
		}
	}
	else
	{
	     if (stAc[uid].fan_speed == AC_FAN_LOW)
	    {
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_low.png);"));
	    	tool_button_fan[uid]->setText(tr("      Low"));
	    }
		else if (stAc[uid].fan_speed == AC_FAN_MIDDLE)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_mid.png);"));
	    	tool_button_fan[uid]->setText(tr("      Mid"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_HIGH)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_high.png);"));
	    	tool_button_fan[uid]->setText(tr("      High"));
		}
		else if (stAc[uid].fan_speed == AC_FAN_AUTO)
		{
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	    	tool_button_fan[uid]->setText(tr("      Auto"));
		}
		else
		{
			//printf("stAc[current_uid].fan_speed init is invalid so new one\r\n");
	    	tool_button_fan[uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	    	tool_button_fan[uid]->setText(tr("      Auto"));
	    	stAc[uid].fan_speed =AC_FAN_LOW;
		}
	}
	tool_button_fan[uid]->update();
}



static int glb_current_uid = 0;

void HavcWidget::ac_on_timer_callback()
{
	if (!stAc[glb_current_uid].switch_on)
	{
		return;
	}

	if (stAc[glb_current_uid].dev_type == DEV_RS485_HEATER_COOLER)
	{
		if (stAc[glb_current_uid].ac_floor_mode == 0)
		{
			 tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);
			 tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);
			 button_up[glb_current_uid]->setGraphicsEffect(NULL);
			 button_down[glb_current_uid]->setGraphicsEffect(NULL);
			if(g_db_flg[glb_current_uid])
				pLabel_temp_decade[glb_current_uid]->setGraphicsEffect(NULL);
			else
				pLabel_temp_decade_brother[glb_current_uid]->setGraphicsEffect(NULL);
			if(g_b_flg[glb_current_uid])
				pLabel_temp_bits[glb_current_uid]->setGraphicsEffect(NULL);
			else
				pLabel_temp_bits_brother[glb_current_uid]->setGraphicsEffect(NULL);

			pLabel_temp_unit[glb_current_uid]->setGraphicsEffect(NULL);
			stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);
		}	
	}
    else if (stAc[glb_current_uid].dev_type == DEV_RS485_AIR_PURIFIER)
    {
    	tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);
		pLabel_PM25[glb_current_uid]->setGraphicsEffect(NULL);
		stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);
    }
	else if(stAc[glb_current_uid].dev_type == DEV_FLOOR_HEAT)
	{
	    tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);

        #ifndef ENNO_HILINK_AUTH
		tool_button_mode[glb_current_uid]->setVisible(1);
		ENNO_LOG_ERR("setVisible = 1\n");
		#endif

		set_floor_auto_man_switch(glb_current_uid);
		
		if(g_db_flg[glb_current_uid])
			pLabel_temp_decade[glb_current_uid]->setGraphicsEffect(NULL);
		else
			pLabel_temp_decade_brother[glb_current_uid]->setGraphicsEffect(NULL);
		if(g_b_flg[glb_current_uid])
			pLabel_temp_bits[glb_current_uid]->setGraphicsEffect(NULL);
		else
			pLabel_temp_bits_brother[glb_current_uid]->setGraphicsEffect(NULL);

		pLabel_temp_unit[glb_current_uid]->setGraphicsEffect(NULL);
		stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);
	}
}


void HavcWidget::ac_off_timer_callback()
{
    if (stAc[glb_current_uid].switch_on)
    {
        return;
    }
		
	if (stAc[glb_current_uid].dev_type == DEV_RS485_HEATER_COOLER)
	{
		tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_mode[glb_current_uid]->setEnabled(false);
		tool_button_fan[glb_current_uid]->setEnabled(false);
		set_ac_mode_off(glb_current_uid);
		set_fan_speed_off(glb_current_uid);
		//qDebug()<<"func="<<__func__<<",line=%d"<<__LINE__<<", setVisible(0),["<<glb_current_uid<<"]";
		button_down[glb_current_uid]->setVisible(0);
		button_up[glb_current_uid]->setVisible(0);

		if(g_db_flg[glb_current_uid])
		{
			pLabel_temp_decade[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_decade[glb_current_uid]->setGraphicsEffect(NULL);
		}
		else
		{
			pLabel_temp_decade_brother[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_decade_brother[glb_current_uid]->setGraphicsEffect(NULL);
		}
		if(g_b_flg[glb_current_uid])
		{
			pLabel_temp_bits[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_bits[glb_current_uid]->setGraphicsEffect(NULL);
		}
		else
		{
			pLabel_temp_bits_brother[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_bits_brother[glb_current_uid]->setGraphicsEffect(NULL);
		}

		pLabel_temp_unit[glb_current_uid]->setGraphicsEffect(NULL);
		stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);

		pLabel_temp_unit[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		stAc[glb_current_uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");
		
	}
    else if (stAc[glb_current_uid].dev_type == DEV_RS485_AIR_PURIFIER)
    {
		tool_button_fan[glb_current_uid]->setEnabled(false);
		set_fan_speed_off(glb_current_uid);
		
		tool_button_mode[glb_current_uid]->setEnabled(false);

		tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);
		//pLabel_humidity[glb_current_uid]->setGraphicsEffect(NULL);
		pLabel_PM25[glb_current_uid]->setGraphicsEffect(NULL);
		stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);

		//pLabel_humidity[glb_current_uid]->setStyleSheet("font-size:40px;color:rgb(180,180,180);font-weight:bold");
		stAc[glb_current_uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");
		pLabel_PM25[glb_current_uid]->setStyleSheet("font-size:78px;color:rgb(180,180,180);font-weight:500;background:transparent");

    }
	else if (stAc[glb_current_uid].dev_type == DEV_FLOOR_HEAT)
	{
	
	   //qDebug()<<"func="<<__func__<<",line="<<__LINE__<<", setVisible(0),["<<glb_current_uid<<"]";
		button_down[glb_current_uid]->setVisible(0);
		button_up[glb_current_uid]->setVisible(0);

		//ENNO_LOG_ERR("floor_mode all false\n");

		tool_button_mode[glb_current_uid]->setEnabled(false);
		tool_button_fan[glb_current_uid]->setEnabled(false);

		tool_button_mode[glb_current_uid]->setVisible(0);
		
		//ENNO_LOG_ERR("setVisible = 0\n");


		tool_button_mode[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/auto_off.png);"));
		tool_button_mode[current_uid]->setText(tr("      Auto"));
		tool_button_fan[current_uid]->setStyleSheet(QString("font-size:30px;text-align:center;color:rgb(180,180,180);font-weight:500;border-image:url(havc/man_off.png);"));
		tool_button_fan[current_uid]->setText(tr("      Manu"));
	
		tool_button_mode[glb_current_uid]->setGraphicsEffect(NULL);
		tool_button_fan[glb_current_uid]->setGraphicsEffect(NULL);

		if(g_db_flg[glb_current_uid])
		{
			pLabel_temp_decade[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_decade[glb_current_uid]->setGraphicsEffect(NULL);
		}
		else
		{
			pLabel_temp_decade_brother[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_decade_brother[glb_current_uid]->setGraphicsEffect(NULL);
		}
		if(g_b_flg[glb_current_uid])
		{
			pLabel_temp_bits[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_bits[glb_current_uid]->setGraphicsEffect(NULL);
		}
		else
		{
			pLabel_temp_bits_brother[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
			pLabel_temp_bits_brother[glb_current_uid]->setGraphicsEffect(NULL);
		}

		pLabel_temp_unit[glb_current_uid]->setGraphicsEffect(NULL);
		stAc[glb_current_uid].label_name->setGraphicsEffect(NULL);

		pLabel_temp_unit[glb_current_uid]->setStyleSheet("font-size:95px;color:rgb(180,180,180);font-weight:normal");
		stAc[glb_current_uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:rgb(180,180,180)");

	}
}



#define AC_OPACITY_TIME 500
void HavcWidget::set_on_animation(int uid,uint32 switch_on)
{
#if 1
	 //  m_timer = new QTimer(this);
	 //   connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
	    glb_current_uid = uid;
	    //group_anima = new QParallelAnimationGroup;
	   // qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",uid="<<uid<<",switch_on="<<switch_on;
		//group_anima->stop();
		group_anima[uid]->setCurrentTime(AC_OPACITY_TIME); 
		group_anima[uid]->stop();
		group_anima[uid]->clear();
		if (!switch_on)
		{
		    stAc[uid].switch_on = 0;
			set_button_content(uid);
			if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER)
			{
		        QGraphicsOpacityEffect *ac_mode_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_mode = new QPropertyAnimation(ac_mode_effect,"opacity");
				tool_button_mode[uid]->setGraphicsEffect(ac_mode_effect);
				animation_ac_mode->setDuration(AC_OPACITY_TIME);
				animation_ac_mode->setKeyValueAt(0, 0.99);
				animation_ac_mode->setKeyValueAt(1, 0.7);
				animation_ac_mode->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_mode);

		        QGraphicsOpacityEffect *ac_fan_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_fan = new QPropertyAnimation(ac_fan_effect,"opacity");
				tool_button_fan[uid]->setGraphicsEffect(ac_fan_effect);
				animation_ac_fan->setDuration(AC_OPACITY_TIME);
				animation_ac_fan->setKeyValueAt(0, 0.99);
				animation_ac_fan->setKeyValueAt(1, 0.7);
				animation_ac_fan->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_fan);

		        QGraphicsOpacityEffect *ac_up_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_up = new QPropertyAnimation(ac_up_effect,"opacity");
				button_up[uid]->setGraphicsEffect(ac_up_effect);
				animation_ac_up->setDuration(AC_OPACITY_TIME);
				animation_ac_up->setKeyValueAt(0, 0.99);
				animation_ac_up->setKeyValueAt(1, 0);
				animation_ac_up->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_up);

		        QGraphicsOpacityEffect *ac_down_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_down = new QPropertyAnimation(ac_down_effect,"opacity");
				button_down[uid]->setGraphicsEffect(ac_down_effect);
				animation_ac_down->setDuration(AC_OPACITY_TIME);
				animation_ac_down->setKeyValueAt(0, 0.99);
				animation_ac_down->setKeyValueAt(1, 0);
				animation_ac_down->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_down);

		        QGraphicsOpacityEffect *ac_decade_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_decade = new QPropertyAnimation(ac_decade_effect,"opacity");
				if(g_db_flg[uid])
						pLabel_temp_decade[uid]->setGraphicsEffect(ac_decade_effect);
				else
						pLabel_temp_decade_brother[uid]->setGraphicsEffect(ac_decade_effect);
				animation_ac_decade->setDuration(AC_OPACITY_TIME);
				animation_ac_decade->setKeyValueAt(0, 0.99);
				animation_ac_decade->setKeyValueAt(1, 0.7);
				animation_ac_decade->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_decade);

		        QGraphicsOpacityEffect *ac_bit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_bit = new QPropertyAnimation(ac_bit_effect,"opacity");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setGraphicsEffect(ac_bit_effect);
				else
					pLabel_temp_bits_brother[uid]->setGraphicsEffect(ac_bit_effect);
				animation_ac_bit->setDuration(AC_OPACITY_TIME);
				animation_ac_bit->setKeyValueAt(0, 0.99);
				animation_ac_bit->setKeyValueAt(1, 0.7);
				animation_ac_bit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_bit);

		        QGraphicsOpacityEffect *ac_unit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_unit = new QPropertyAnimation(ac_unit_effect,"opacity");
				pLabel_temp_unit[uid]->setGraphicsEffect(ac_unit_effect);
				animation_ac_unit->setDuration(AC_OPACITY_TIME);
				animation_ac_unit->setKeyValueAt(0, 0.99);
				animation_ac_unit->setKeyValueAt(1, 0.7);
				animation_ac_unit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_unit);

		        QGraphicsOpacityEffect *ac_name_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_name = new QPropertyAnimation(ac_name_effect,"opacity");
				stAc[uid].label_name->setGraphicsEffect(ac_name_effect);
				animation_ac_name->setDuration(AC_OPACITY_TIME);
				animation_ac_name->setKeyValueAt(0, 0.99);
				animation_ac_name->setKeyValueAt(1, 0.7);
				animation_ac_name->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_name);

				group_anima[uid]->start();
				QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_off_timer_callback()));
			}
	        else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
	        {
		        QGraphicsOpacityEffect *air_fan_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_fan = new QPropertyAnimation(air_fan_effect,"opacity");
				tool_button_fan[uid]->setGraphicsEffect(air_fan_effect);
				animation_air_fan->setDuration(AC_OPACITY_TIME);
				animation_air_fan->setKeyValueAt(0, 0.99);
				animation_air_fan->setKeyValueAt(1, 0.7);
				animation_air_fan->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_fan);

		        QGraphicsOpacityEffect *air_humi_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_humi = new QPropertyAnimation(air_humi_effect,"opacity");
				tool_button_mode[uid]->setGraphicsEffect(air_humi_effect);
				animation_air_humi->setDuration(AC_OPACITY_TIME);
				animation_air_humi->setKeyValueAt(0, 0.99);
				animation_air_humi->setKeyValueAt(1, 0.7);
				animation_air_humi->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_humi);

		        QGraphicsOpacityEffect *air_pm_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_pm = new QPropertyAnimation(air_pm_effect,"opacity");
				pLabel_PM25[uid]->setGraphicsEffect(air_pm_effect);
				animation_air_pm->setDuration(AC_OPACITY_TIME);
				animation_air_pm->setKeyValueAt(0, 0.99);
				animation_air_pm->setKeyValueAt(1, 0.7);
				animation_air_pm->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_pm);


		        QGraphicsOpacityEffect *air_name_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_name = new QPropertyAnimation(air_name_effect,"opacity");
				stAc[uid].label_name->setGraphicsEffect(air_name_effect);
				animation_air_name->setDuration(AC_OPACITY_TIME);
				animation_air_name->setKeyValueAt(0, 0.99);
				animation_air_name->setKeyValueAt(1, 0.7);
				animation_air_name->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_name);

				group_anima[uid]->start();
				QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_off_timer_callback()));

	        }
			else if(stAc[uid].dev_type == DEV_FLOOR_HEAT)
		    {
			        QGraphicsOpacityEffect *h_up_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_up = new QPropertyAnimation(h_up_effect,"opacity");
					button_up[uid]->setGraphicsEffect(h_up_effect);
					animation_h_up->setDuration(AC_OPACITY_TIME);
					animation_h_up->setKeyValueAt(0, 0.99);
					animation_h_up->setKeyValueAt(1, 0);
					animation_h_up->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_up);

			        QGraphicsOpacityEffect *h_down_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_down = new QPropertyAnimation(h_down_effect,"opacity");
					button_down[uid]->setGraphicsEffect(h_down_effect);
					animation_h_down->setDuration(AC_OPACITY_TIME);
					animation_h_down->setKeyValueAt(0, 0.99);
					animation_h_down->setKeyValueAt(1, 0);
					animation_h_down->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_down);

			        QGraphicsOpacityEffect *h_decade_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_decade = new QPropertyAnimation(h_decade_effect,"opacity");
					if(g_db_flg[uid])
							pLabel_temp_decade[uid]->setGraphicsEffect(h_decade_effect);
					else
							pLabel_temp_decade_brother[uid]->setGraphicsEffect(h_decade_effect);
					animation_h_decade->setDuration(AC_OPACITY_TIME);
					animation_h_decade->setKeyValueAt(0, 0.99);
					animation_h_decade->setKeyValueAt(1, 0.7);
					animation_h_decade->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_decade);

			        QGraphicsOpacityEffect *h_bit_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_bit = new QPropertyAnimation(h_bit_effect,"opacity");
					if(g_b_flg[uid])
						pLabel_temp_bits[uid]->setGraphicsEffect(h_bit_effect);
					else
						pLabel_temp_bits_brother[uid]->setGraphicsEffect(h_bit_effect);
					animation_h_bit->setDuration(AC_OPACITY_TIME);
					animation_h_bit->setKeyValueAt(0, 0.99);
					animation_h_bit->setKeyValueAt(1, 0.7);
					animation_h_bit->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_bit);

			        QGraphicsOpacityEffect *h_unit_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_unit = new QPropertyAnimation(h_unit_effect,"opacity");
					pLabel_temp_unit[uid]->setGraphicsEffect(h_unit_effect);
					animation_h_unit->setDuration(AC_OPACITY_TIME);
					animation_h_unit->setKeyValueAt(0, 0.99);
					animation_h_unit->setKeyValueAt(1, 0.7);
					animation_h_unit->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_unit);

			        QGraphicsOpacityEffect *h_name_effect= new QGraphicsOpacityEffect(this);
					QPropertyAnimation *animation_h_name = new QPropertyAnimation(h_name_effect,"opacity");
					stAc[uid].label_name->setGraphicsEffect(h_name_effect);
					animation_h_name->setDuration(AC_OPACITY_TIME);
					animation_h_name->setKeyValueAt(0, 0.99);
					animation_h_name->setKeyValueAt(1, 0.7);
					animation_h_name->setEasingCurve(QEasingCurve::Linear);
					group_anima[uid]->addAnimation(animation_h_name);

					group_anima[uid]->start();
					QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_off_timer_callback()));
		    }
		}
		else
		{
		    stAc[uid].switch_on = 1;
			set_button_content(uid);
			if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER)
			{
				tool_button_mode[uid]->setEnabled(true);
				tool_button_fan[uid]->setEnabled(true);
				set_ac_mode_init(uid);
				set_fan_speed_init(uid);

				if(g_db_flg[uid])
					pLabel_temp_decade[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_decade_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_bits_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");

				pLabel_temp_unit[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");




		        QGraphicsOpacityEffect *ac_mode_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_mode = new QPropertyAnimation(ac_mode_effect,"opacity");
				tool_button_mode[uid]->setGraphicsEffect(ac_mode_effect);
				animation_ac_mode->setDuration(AC_OPACITY_TIME);
				animation_ac_mode->setKeyValueAt(0, 0.7);
				animation_ac_mode->setKeyValueAt(1, 0.99);
				animation_ac_mode->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_mode);

		        QGraphicsOpacityEffect *ac_fan_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_fan = new QPropertyAnimation(ac_fan_effect,"opacity");
				tool_button_fan[uid]->setGraphicsEffect(ac_fan_effect);
				animation_ac_fan->setDuration(AC_OPACITY_TIME);
				animation_ac_fan->setKeyValueAt(0, 0.7);
				animation_ac_fan->setKeyValueAt(1, 0.99);
				animation_ac_fan->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_fan);

		        QGraphicsOpacityEffect *ac_up_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_up = new QPropertyAnimation(ac_up_effect,"opacity");
				button_up[uid]->setGraphicsEffect(ac_up_effect);
				animation_ac_up->setDuration(AC_OPACITY_TIME);
				animation_ac_up->setKeyValueAt(0, 0);
				animation_ac_up->setKeyValueAt(1, 0.99);
				animation_ac_up->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_up);

		        QGraphicsOpacityEffect *ac_down_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_down = new QPropertyAnimation(ac_down_effect,"opacity");
				button_down[uid]->setGraphicsEffect(ac_down_effect);
				animation_ac_down->setDuration(AC_OPACITY_TIME);
				animation_ac_down->setKeyValueAt(0, 0);
				animation_ac_down->setKeyValueAt(1, 0.99);
				animation_ac_down->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_down);

		        QGraphicsOpacityEffect *ac_decade_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_decade = new QPropertyAnimation(ac_decade_effect,"opacity");
				if(g_db_flg[uid])
						pLabel_temp_decade[uid]->setGraphicsEffect(ac_decade_effect);
				else
						pLabel_temp_decade_brother[uid]->setGraphicsEffect(ac_decade_effect);
				animation_ac_decade->setDuration(AC_OPACITY_TIME);
				animation_ac_decade->setKeyValueAt(0, 0.7);
				animation_ac_decade->setKeyValueAt(1, 0.99);
				animation_ac_decade->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_decade);

		        QGraphicsOpacityEffect *ac_bit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_bit = new QPropertyAnimation(ac_bit_effect,"opacity");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setGraphicsEffect(ac_bit_effect);
				else
					pLabel_temp_bits_brother[uid]->setGraphicsEffect(ac_bit_effect);
				animation_ac_bit->setDuration(AC_OPACITY_TIME);
				animation_ac_bit->setKeyValueAt(0, 0.7);
				animation_ac_bit->setKeyValueAt(1, 0.99);
				animation_ac_bit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_bit);

		        QGraphicsOpacityEffect *ac_unit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_unit = new QPropertyAnimation(ac_unit_effect,"opacity");
				pLabel_temp_unit[uid]->setGraphicsEffect(ac_unit_effect);
				animation_ac_unit->setDuration(AC_OPACITY_TIME);
				animation_ac_unit->setKeyValueAt(0, 0.7);
				animation_ac_unit->setKeyValueAt(1, 0.99);
				animation_ac_unit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_unit);

		        QGraphicsOpacityEffect *ac_name_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_ac_name = new QPropertyAnimation(ac_name_effect,"opacity");
				stAc[uid].label_name->setGraphicsEffect(ac_name_effect);
				animation_ac_name->setDuration(AC_OPACITY_TIME);
				animation_ac_name->setKeyValueAt(0, 0.7);
				animation_ac_name->setKeyValueAt(1, 0.99);
				animation_ac_name->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_ac_name);

				group_anima[uid]->start();
				QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_on_timer_callback()));

				//qDebug()<<"func="<<__func__<<",line="<<__LINE__<<", setVisible(1),["<<uid<<"]";
				button_down[uid]->setVisible(1);
				button_up[uid]->setVisible(1);
			}
			else if(stAc[uid].dev_type == DEV_FLOOR_HEAT)
		    {
				if(g_db_flg[uid])
					pLabel_temp_decade[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_decade_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_bits_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");


				pLabel_temp_unit[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");


		        QGraphicsOpacityEffect *h_up_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_up = new QPropertyAnimation(h_up_effect,"opacity");
				button_up[uid]->setGraphicsEffect(h_up_effect);
				animation_h_up->setDuration(AC_OPACITY_TIME);
				animation_h_up->setKeyValueAt(0, 0);
				animation_h_up->setKeyValueAt(1, 0.99);
				animation_h_up->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_up);

		        QGraphicsOpacityEffect *h_down_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_down = new QPropertyAnimation(h_down_effect,"opacity");
				button_down[uid]->setGraphicsEffect(h_down_effect);
				animation_h_down->setDuration(AC_OPACITY_TIME);
				animation_h_down->setKeyValueAt(0, 0);
				animation_h_down->setKeyValueAt(1, 0.99);
				animation_h_down->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_down);

		        QGraphicsOpacityEffect *h_decade_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_decade = new QPropertyAnimation(h_decade_effect,"opacity");
				if(g_db_flg[uid])
						pLabel_temp_decade[uid]->setGraphicsEffect(h_decade_effect);
				else
						pLabel_temp_decade_brother[uid]->setGraphicsEffect(h_decade_effect);
				animation_h_decade->setDuration(AC_OPACITY_TIME);
				animation_h_decade->setKeyValueAt(0, 0.7);
				animation_h_decade->setKeyValueAt(1, 0.99);
				animation_h_decade->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_decade);

		        QGraphicsOpacityEffect *h_bit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_bit = new QPropertyAnimation(h_bit_effect,"opacity");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setGraphicsEffect(h_bit_effect);
				else
					pLabel_temp_bits_brother[uid]->setGraphicsEffect(h_bit_effect);
				animation_h_bit->setDuration(AC_OPACITY_TIME);
				animation_h_bit->setKeyValueAt(0, 0.7);
				animation_h_bit->setKeyValueAt(1, 0.99);
				animation_h_bit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_bit);

		        QGraphicsOpacityEffect *h_unit_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_unit = new QPropertyAnimation(h_unit_effect,"opacity");
				pLabel_temp_unit[uid]->setGraphicsEffect(h_unit_effect);
				animation_h_unit->setDuration(AC_OPACITY_TIME);
				animation_h_unit->setKeyValueAt(0, 0.7);
				animation_h_unit->setKeyValueAt(1, 0.99);
				animation_h_unit->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_unit);

		        QGraphicsOpacityEffect *h_name_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_h_name = new QPropertyAnimation(h_name_effect,"opacity");
				stAc[uid].label_name->setGraphicsEffect(h_name_effect);
				animation_h_name->setDuration(AC_OPACITY_TIME);
				animation_h_name->setKeyValueAt(0, 0.7);
				animation_h_name->setKeyValueAt(1, 0.99);
				animation_h_name->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_h_name);

				group_anima[uid]->start();
				QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_on_timer_callback()));

                #ifndef ENNO_HILINK_AUTH
                tool_button_mode[uid]->setVisible(1);
				#endif
				button_down[uid]->setVisible(1);
				button_up[uid]->setVisible(1);
		    }	
	        else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
	        {
				pLabel_PM25[uid]->setStyleSheet("font-size:78px;color:rgb(0,255,0);font-weight:500;background:transparent");
				tool_button_fan[uid]->setEnabled(true);
				tool_button_mode[uid]->setEnabled(true);
				set_fan_speed_init(uid);
				//tool_button_mode[uid]->setStyleSheet("font-size:40px;color:white;font-weight:bold");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");


		        QGraphicsOpacityEffect *air_fan_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_fan = new QPropertyAnimation(air_fan_effect,"opacity");
				tool_button_fan[uid]->setGraphicsEffect(air_fan_effect);
				animation_air_fan->setDuration(AC_OPACITY_TIME);
				animation_air_fan->setKeyValueAt(0, 0.7);
				animation_air_fan->setKeyValueAt(1, 0.99);
				animation_air_fan->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_fan);

		        QGraphicsOpacityEffect *air_humi_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_humi = new QPropertyAnimation(air_humi_effect,"opacity");
				tool_button_mode[uid]->setGraphicsEffect(air_humi_effect);
				animation_air_humi->setDuration(AC_OPACITY_TIME);
				animation_air_humi->setKeyValueAt(0, 0.7);
				animation_air_humi->setKeyValueAt(1, 0.99);
				animation_air_humi->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_humi);

		        QGraphicsOpacityEffect *air_pm_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_pm = new QPropertyAnimation(air_pm_effect,"opacity");
				pLabel_PM25[uid]->setGraphicsEffect(air_pm_effect);
				animation_air_pm->setDuration(AC_OPACITY_TIME);
				animation_air_pm->setKeyValueAt(0, 0.7);
				animation_air_pm->setKeyValueAt(1, 0.99);
				animation_air_pm->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_pm);


		        QGraphicsOpacityEffect *air_name_effect= new QGraphicsOpacityEffect(this);
				QPropertyAnimation *animation_air_name = new QPropertyAnimation(air_name_effect,"opacity");
				stAc[uid].label_name->setGraphicsEffect(air_name_effect);
				animation_air_name->setDuration(AC_OPACITY_TIME);
				animation_air_name->setKeyValueAt(0, 0.7);
				animation_air_name->setKeyValueAt(1, 0.99);
				animation_air_name->setEasingCurve(QEasingCurve::Linear);
				group_anima[uid]->addAnimation(animation_air_name);

				group_anima[uid]->start();
				QTimer::singleShot(AC_OPACITY_TIME, this, SLOT(ac_on_timer_callback()));

	        }
		}

	//	m_timer->start(50);
		//update();
#endif
}

void HavcWidget::set_on_but_no_animation(int uid,uint32 switch_on)
{
#if 1
	 //  m_timer = new QTimer(this);
	 //   connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
	    glb_current_uid = uid;
	    //group_anima = new QParallelAnimationGroup;
	   // qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",uid="<<uid<<",switch_on="<<switch_on;
		//group_anima->stop();
		group_anima[uid]->setCurrentTime(AC_OPACITY_TIME); 
		group_anima[uid]->stop();
		group_anima[uid]->clear();
		if (!switch_on)
		{
		    stAc[uid].switch_on = 0;
			set_button_content(uid);
			if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER)
			{		 	       
				QTimer::singleShot(1, this, SLOT(ac_off_timer_callback()));
			}
	        else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
	        {
				QTimer::singleShot(1, this, SLOT(ac_off_timer_callback()));
	        }
			else if (stAc[uid].dev_type == DEV_FLOOR_HEAT)
		    {
				QTimer::singleShot(1, this, SLOT(ac_off_timer_callback()));
		    }
		}
		else
		{
		    stAc[uid].switch_on = 1;
			set_button_content(uid);
			if (stAc[uid].dev_type == DEV_RS485_HEATER_COOLER)
			{
				tool_button_mode[uid]->setEnabled(true);
				tool_button_fan[uid]->setEnabled(true);
				set_ac_mode_init(uid);
				set_fan_speed_init(uid);

				if(g_db_flg[uid])
					pLabel_temp_decade[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_decade_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_bits_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");

				pLabel_temp_unit[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");//600

				QTimer::singleShot(1, this, SLOT(ac_on_timer_callback()));

				//qDebug()<<"func="<<__func__<<",line="<<__LINE__<<", setVisible(1),["<<uid<<"]";
				button_down[uid]->setVisible(1);
				button_up[uid]->setVisible(1);


			}
	        else if (stAc[uid].dev_type == DEV_RS485_AIR_PURIFIER)
	        {
				//pLabel_PM25[uid]->setStyleSheet("font-size:96px;color:rgb(0,255,0);font-weight:normal;background:transparent");
			    if (stAc[uid].PM25 <= 100)
			    {        
					pLabel_PM25[uid]->setStyleSheet("font-size:78px;color:rgb(159, 217, 65);font-weight:500;background:transparent");
			    }
				else if (stAc[uid].PM25 > 100 && stAc[uid].PM25 <= 200)
				{
				    pLabel_PM25[uid]->setStyleSheet("font-size:78px;color:rgb(238, 154, 0);font-weight:500;background:transparent");
				}
				else if (stAc[uid].PM25 > 200 && stAc[uid].PM25 <= 1000)
				{
				    pLabel_PM25[uid]->setStyleSheet("font-size:78px;color:rgb(255, 54, 106);font-weight:500;background:transparent");
				}
				tool_button_fan[uid]->setEnabled(true);
				set_fan_speed_init(uid);
				
				tool_button_mode[uid]->setEnabled(true);
				set_ap_mode_init(uid);
				//pLabel_humidity[uid]->setStyleSheet("font-size:40px;color:white;font-weight:bold");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");
		    
				QTimer::singleShot(1, this, SLOT(ac_on_timer_callback()));

	        }
			else if (stAc[uid].dev_type == DEV_FLOOR_HEAT)
		    {
				if(g_db_flg[uid])
					pLabel_temp_decade[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_decade_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				if(g_b_flg[uid])
					pLabel_temp_bits[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				else
					pLabel_temp_bits_brother[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");


				pLabel_temp_unit[uid]->setStyleSheet("font-size:95px;color:white;font-weight:normal");
				stAc[uid].label_name->setStyleSheet("font-size:36px;font-weight:500;color:white");
		   
				QTimer::singleShot(1, this, SLOT(ac_on_timer_callback()));

				button_down[uid]->setVisible(1);
			    button_up[uid]->setVisible(1);

				#ifndef ENNO_HILINK_AUTH
				tool_button_mode[uid]->setVisible(1);
				#endif
				set_floor_auto_man_switch(uid);
		    }

                
				//qDebug()<<"func="<<__func__<<",line="<<__LINE__<<", setVisible(1),["<<uid<<"]";
				
		}

	//	m_timer->start(50);
		//update();
#endif
}

void HavcWidget::txtButton()
{
//	stAc[current_uid].switch_on = 0;
	QObject *object = QObject::sender();
    QPushButton *button_on = qobject_cast<QPushButton *>(object);
	//改变按钮文字
	//button_on->setText("off");
//button_on->setEnabled(false);
	//ac_button_on = button_on;
    //qDebug()<<"current uid"<<current_uid;
	//qDebug()<<"dev_type"<<stAc[current_uid].dev_type;
	//qDebug()<<"ac_floor_mode"<<stAc[current_uid].ac_floor_mode;
	#if 1

#ifdef MOTOR_ENABLE
	Util::UI_motor_shake(1);
#endif

	if (stAc[current_uid].switch_on)
	{
		set_on_animation(current_uid,0);
		if (stAc[current_uid].dev_type == DEV_RS485_HEATER_COOLER 
			|| stAc[current_uid].dev_type == DEV_FLOOR_HEAT) 
		{	
			if (g_thread != NULL)
			{
			    g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
					 CHAR_AC_ACTIVE, stAc[current_uid].switch_on);
			}
		}	
        else if (stAc[current_uid].dev_type == DEV_RS485_AIR_PURIFIER)
        {
			if (g_thread != NULL)
			{
			    g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
			    		CHAR_AP_ACTIVE, stAc[current_uid].switch_on);
			}
        }
	}
	else
	{	
		set_on_animation(current_uid,1);

		if (stAc[current_uid].dev_type == DEV_RS485_HEATER_COOLER
			|| stAc[current_uid].dev_type == DEV_FLOOR_HEAT) 
		{
			if (g_thread != NULL)
			{
			    g_thread->msg_dev_ctrl(stAc[current_uid].device_id, 
					 CHAR_AC_ACTIVE, stAc[current_uid].switch_on);
			}
		}	
        else if (stAc[current_uid].dev_type == DEV_RS485_AIR_PURIFIER)
        {
			if (g_thread != NULL)
			{
			    g_thread->msg_dev_ctrl(stAc[current_uid].device_id,
			    		CHAR_AP_ACTIVE, stAc[current_uid].switch_on);
			}
        }
	}
	#endif
	//update();
}

void HavcWidget::changeLanguage()
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




void HavcWidget::set_ac_on_status(int index, uint32 state, QPushButton *button_on)
{      
	if (!state)
	{
		set_on_but_no_animation(index,0);
	}
	else
	{	
		set_on_but_no_animation(index,1);
	}
}
void HavcWidget::set_fan_speed_status(int index)
{
    if ((stAc[index].dev_type == DEV_RS485_AIR_PURIFIER))
    {
	    if (stAc[index].fan_speed == AC_FAN_LOW)
	    {
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_low.png);"));
	        tool_button_fan[index]->setText(tr("      Low fan"));
	    }
		else if (stAc[index].fan_speed == AC_FAN_MIDDLE)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_mid.png);"));
	        tool_button_fan[index]->setText(tr("      Mid fan"));
		}
		else if (stAc[index].fan_speed == AC_FAN_HIGH)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_high.png);"));
	        tool_button_fan[index]->setText(tr("      High fan"));
		}
		else if (stAc[index].fan_speed == AC_FAN_AUTO)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	        tool_button_fan[index]->setText(tr("      Auto"));
		}
		tool_button_fan[index]->update();
    }
	else if ((stAc[index].dev_type == DEV_RS485_HEATER_COOLER))
	{
	    if (stAc[index].fan_speed == AC_FAN_LOW)
	    {
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_low.png);"));
	        tool_button_fan[index]->setText(tr("      Low"));
	    }
		else if (stAc[index].fan_speed == AC_FAN_MIDDLE)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_mid.png);"));
	        tool_button_fan[index]->setText(tr("      Mid"));
		}
		else if (stAc[index].fan_speed == AC_FAN_HIGH)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_high.png);"));
	        tool_button_fan[index]->setText(tr("      High"));
		}
		else if (stAc[index].fan_speed == AC_FAN_AUTO)
		{
	        tool_button_fan[index]->setStyleSheet(QString("font-size:30px;text-align:center;color:white;font-weight:500;border-image:url(havc/fan_auto.png);"));
	        tool_button_fan[index]->setText(tr("      Auto"));
		}
		tool_button_fan[index]->update();
	}
 	else if (stAc[index].dev_type == DEV_FLOOR_HEAT)
	{
	    stAc[index].floor_ctrl_mode = stAc[index].fan_speed;
		set_floor_auto_man_switch(index);
	}
}
void HavcWidget::set_ac_mode_status(int index)
{
	//printf("XXXXstAc[index].mode=%d index=%d\r\n",stAc[index].mode,index);

	if (AC_MODE_HEAT == stAc[index].mode)
	{
		tool_button_mode[index]->setText(tr("      Heat"));
		tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/mode_heat.png);"));
	}
    else if (AC_MODE_COOL == stAc[index].mode)
    {
    	tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/mode_cool.png);"));
    	tool_button_mode[index]->setText(tr("      Cool"));
    }
	else if (AC_MODE_DEHUMIDIFY == stAc[index].mode)
	{
    	tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/mode_dehumidify.png);"));
    	tool_button_mode[index]->setText(tr("      Dry"));
	}
	else if (AC_MODE_FAN == stAc[index].mode)
	{
    	tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/mode_fan.png);"));
    	tool_button_mode[index]->setText(tr("      Fan"));
	}
	else //if (AC_MODE_AUTO == stAc[index].mode)
	{
		stAc[index].mode = AC_MODE_AUTO;
		tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/mode_auto.png);"));
		tool_button_mode[index]->setText(tr("      Auto"));
	}

	tool_button_mode[index]->update();
}
void HavcWidget::set_ap_mode_status(int index)
{
	//printf("XXXXstAc[index].mode=%d index=%d\r\n",stAc[index].mode,index);

	if (AP_MODE_IN_CYCLE == stAc[index].mode)
	{
		tool_button_mode[index]->setText(tr("In Cycle"));
		tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/ap_in_cycle.png);"));

	}
    else
    {
    	tool_button_mode[index]->setStyleSheet(QString("font-size:30px;text-align:right;color:white;font-weight:500;border-image:url(havc/ap_out_cycle.png);"));
    	tool_button_mode[index]->setText(tr("Out Cycle"));

    }
	
	tool_button_mode[index]->update();
}

void HavcWidget::ap_pm25_set_status(int index)
{
    QString pm25;

	if (g_purify_creat_complate != true)
	{
	    return;
	}

	if (!stAc[index].switch_on)
    {
        return;
    }

	if (stAc[index].PM25 >= 500 || stAc[index].PM25 <= 0)
	{
	    qDebug()<<"pm25 error"<<stAc[index].PM25;
	    return;
	}
	
	pm25.sprintf("%d", stAc[index].PM25);
	pLabel_PM25[index]->setText(pm25);

    if (stAc[index].PM25 <= 100)
    {        
		pLabel_PM25[index]->setStyleSheet("font-size:78px;color:rgb(159, 217, 65);font-weight:500;background:transparent");
    }
	else if (stAc[index].PM25 > 100 && stAc[index].PM25 <= 200)
	{
	    pLabel_PM25[index]->setStyleSheet("font-size:78px;color:rgb(238, 154, 0);font-weight:500;background:transparent");
	}
	else if (stAc[index].PM25 > 200 && stAc[index].PM25 <= 1000)
	{
	    pLabel_PM25[index]->setStyleSheet("font-size:78px;color:rgb(255, 54, 106);font-weight:500;background:transparent");
	}
}
void HavcWidget::ac_temp_set_status(int index)
{
/*	QString temp;
    temp.sprintf("%d%c", stAc[index].target_temperature, 0xB0);
	pLabel_ac[index]->setText(temp);	*/

	QString temp;

	if(g_db_flg[index])
	{
	    temp.sprintf("%d", stAc[index].target_temperature/10);
	    pLabel_temp_decade[index]->setText(temp);
	}
	else
	{
	    temp.sprintf("%d", stAc[index].target_temperature/10);
	    pLabel_temp_decade_brother[index]->setText(temp);
	}

	if(g_b_flg[index])
	{
	    temp.sprintf("%d", stAc[index].target_temperature%10);
	    pLabel_temp_bits[index]->setText(temp);
	}
	else
	{
		 temp.sprintf("%d", stAc[index].target_temperature%10);
		 pLabel_temp_bits_brother[index]->setText(temp);
	}

}

enno_result HavcWidget::set_dev_refresh_ui()
{
	QSqlDatabase database;
	char *sql1 = "select * from HeaterCooler WHERE DevID=";
	char *sql2 = "select * from AirPurifer WHERE DevID=";
	char sql[DB_CMD_LEN];
	

	if (QSqlDatabase::contains("qt_sql_default_connection"))
	{
		database = QSqlDatabase::database("qt_sql_default_connection");
	}
	else
	{
		database = QSqlDatabase::addDatabase("QSQLITE");
	}
	
	database.setDatabaseName(OS_ENNO_DB_FILE);
	if (!database.open())
	{
		qDebug() << "Error: Failed to connect database." << database.lastError();
	}
	else
	{
		//qDebug() << "Succeed to connect database." ;
	}
        
    QSqlQuery sql_query;

	for (int i = 0; i < device_num; i++)
	{
	    if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER || stAc[i].dev_type == DEV_FLOOR_HEAT)
	    {
			//ENNO_LOG_ERR("device_id=%d\r\n", stAc[i].device_id);
			snprintf(sql, DB_CMD_LEN, "%s%d", sql1, stAc[i].device_id);
			
	        sql_query.exec(sql);
		    if(!sql_query.exec())
		    {
		        //ENNO_LOG_ERR("device_id=%d, sql_qurey error\r\n");
		        //qDebug()<<sql_query.lastError();
		    }
		    else
		    {
		        while(sql_query.next())
		        {
		            stAc[i].switch_on = sql_query.value(2).toInt();
					stAc[i].target_heat_cool_mode = sql_query.value(3).toInt();
					stAc[i].mode = stAc[i].target_heat_cool_mode;
		            stAc[i].target_temperature = sql_query.value(5).toInt();
					stAc[i].fan_speed = sql_query.value(6).toInt();

					set_ac_on_status(i, stAc[i].switch_on, stAc[i].button_on);
					ac_temp_set_status(i);
					set_ac_mode_status(i);
					set_fan_speed_status(i);
					/*ENNO_LOG_ERR("device_id=%d, on=%d,mode=%d,temp=%d,fan=%d\r\n", 
						sql_query.value(0).toInt(),
						stAc[i].switch_on,
						stAc[i].target_heat_cool_mode,
						stAc[i].target_temperature,
						stAc[i].fan_speed);*/
					if(stAc[i].switch_on)
					{
					   if(stAc[i].dev_type == DEV_RS485_HEATER_COOLER)
					   {
					       g_openBitMap |= BIT_AC;
					   }
					   else 
					   {
	                       g_openBitMap |= BIT_FLOOR;  
					   }				   
					}
					else
					{
					       if(stAc[i].ac_floor_mode == 0 || stAc[i].ac_floor_mode == 1)
						   {
						       if(g_ptr_main_widget&&g_ptr_main_widget->isAllClose(DEV_RS485_HEATER_COOLER))
						       {
						            g_openBitMap &= (~BIT_AC);
						       }
						   }
						   else if(stAc[i].ac_floor_mode == 2)
						   {
						       if(g_ptr_main_widget&&g_ptr_main_widget->isAllClose(DEV_FLOOR_HEAT))
						       {
						            g_openBitMap &= (~BIT_FLOOR);  
						       }	                       
						   }
					}
		        }
		    }
		}
		else if (stAc[i].dev_type == DEV_RS485_AIR_PURIFIER)
		{
		   // ENNO_LOG_ERR("device_id=%d\r\n", stAc[i].device_id);
			snprintf(sql, DB_CMD_LEN, "%s%d", sql2, stAc[i].device_id);
			
	        sql_query.exec(sql);
		    if(!sql_query.exec())
		    {
		       // ENNO_LOG_ERR("device_id=%d, sql_qurey error\r\n");
		        //qDebug()<<sql_query.lastError();
		    }
		    else
		    {
		        while(sql_query.next())
		        {
		            stAc[i].switch_on = sql_query.value(2).toInt();
					stAc[i].fan_speed = sql_query.value(5).toInt();
					stAc[i].mode = sql_query.value(4).toInt();;

					set_ac_on_status(i, stAc[i].switch_on, stAc[i].button_on);
					set_ap_mode_status(i);
					set_fan_speed_status(i);
					/*ENNO_LOG_ERR("device_id=%d, on=%d,mode=%d,fan=%d\r\n", 
						sql_query.value(0).toInt(),
						stAc[i].switch_on,
						stAc[i].target_heat_cool_mode,
						stAc[i].fan_speed);*/
		        }
		    }
		}
	}

    database.close();

    return ENNO_OK;
}
#if 1

enno_result HavcWidget::set_dev_update(char *buf_in, uint32 len_buf)
{

    enno_result ret;
	uint32 len;
	int flagCidValid = 0;
	CTL_DEVICE_CONF *pst_msg = (CTL_DEVICE_CONF *)buf_in;

	ENNO_CHECK_NULL_RET(buf_in);
   // qDebug()<<"device_num = "<<device_num<<"cid=["<<pst_msg->cid<<"],value=["<<pst_msg->value<<"]"<<"devid="<<pst_msg->device_id<<endl;
	for (int i = 0; i < device_num; i++)
	{
        if (stAc[i].device_id == pst_msg->device_id)
        {
            if (!stAc[i].vaild)
            {
                continue;
            }
			flagCidValid = 1;
			
			//qDebug()<<"set_dev_update="<<stAc[i].device_id<<"stAc["<<i<<"]"<<"cid="<<pst_msg->cid<<",value=["<<pst_msg->value<<"]"<<endl;
            switch(pst_msg->cid)
            {
            case CHAR_AC_ACTIVE:
            case CHAR_AP_ACTIVE:
				//qDebug()<<"switch_on="<<stAc[i].switch_on<<"ac_floor_mode="<<stAc[i].ac_floor_mode<<endl;
				//if (stAc[i].switch_on == pst_msg->value)
				//{
				//    return ENNO_OK;
				//}
				stAc[i].switch_on = pst_msg->value;
                set_ac_on_status(i, stAc[i].switch_on, stAc[i].button_on);
				if(CHAR_AC_ACTIVE == pst_msg->cid)
				{
				    if(stAc[i].switch_on)
					{
					   if(stAc[i].dev_type == DEV_RS485_HEATER_COOLER)
					   {
					       g_openBitMap |= BIT_AC;
					   }
					   else
					   {
	                       g_openBitMap |= BIT_FLOOR;  
					   }				   
					}
					else
					{
					       if(stAc[i].ac_floor_mode == 0 || stAc[i].ac_floor_mode == 1)
						   {
						       if(g_ptr_main_widget&&g_ptr_main_widget->isAllClose(DEV_RS485_HEATER_COOLER))
						       {
						            g_openBitMap &= (~BIT_AC);
						       }
						   }
						   else if(stAc[i].ac_floor_mode == 2)
						   {
						       if(g_ptr_main_widget&&g_ptr_main_widget->isAllClose(DEV_FLOOR_HEAT))
						       {
						            g_openBitMap &= (~BIT_FLOOR);  
						       }	                       
						   }
					}
					//qDebug()<<pst_msg->cid<<","<<stAc[i].switch_on<<","<<g_openBitMap<<endl;
				}
				break;

			case CHAR_AP_CYCLE_MODE:
				stAc[i].mode = pst_msg->value;
				if (stAc[i].dev_type == DEV_RS485_AIR_PURIFIER)
				{
					set_ap_mode_status(i);
				}
				break;

			case CHAR_AC_TARGET_TEMPERATURE:
				if (pst_msg->value < 16 || pst_msg->value > 32)
				{
				    break;
				}
				stAc[i].target_temperature = pst_msg->value;
				//ENNO_LOG_ERR("target_temperature=%d\n", pst_msg->value);
				if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER || stAc[i].dev_type == DEV_FLOOR_HEAT)
				{
					ac_temp_set_status(i);
				}
				break;

			case CHAR_AC_COOLING_TH_TEMP:
				if (pst_msg->value < 16 || pst_msg->value > 32)
				{
				    break;
				}
				stAc[i].target_temperature= pst_msg->value;
				//ENNO_LOG_ERR("target_temperature=%d\n", pst_msg->value);
				if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER || stAc[i].dev_type == DEV_FLOOR_HEAT)
				{
					ac_temp_set_status(i);
				}
				break;

			case CHAR_AC_HEATING_TH_TEMP:
				if (pst_msg->value < 16 || pst_msg->value > 32)
				{
				    break;
				}
				stAc[i].target_heat_cool_mode != AC_MODE_AUTO;
				{
			 	    stAc[i].target_temperature= pst_msg->value;
					//ENNO_LOG_ERR("target_temperature=%d\n", pst_msg->value);
					if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER || stAc[i].dev_type == DEV_FLOOR_HEAT)
					{
					    ac_temp_set_status(i);
					}
				}
				break;

			case CHAR_AC_TARGET_HEATER_COOLER_STATE:
				stAc[i].target_heat_cool_mode = pst_msg->value;
				stAc[i].mode = pst_msg->value;
				if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER )
				{
				    set_ac_mode_status(i);
				}
				break;
				
			case CHAR_AC_TARGET_HEATING_COOLING_STATE:
				stAc[i].target_heat_cool_mode = pst_msg->value;
				stAc[i].mode = pst_msg->value;
				if (stAc[i].dev_type == DEV_RS485_HEATER_COOLER)
				{
					set_ac_mode_status(i);
				}
				break;

            case CHAR_AC_ROTATION_SPEED:
				if (stAc[i].dev_type == DEV_FLOOR_HEAT)
				{ 
				    stAc[i].floor_ctrl_mode = pst_msg->value;
				    set_floor_auto_man_switch(i);
				    break;
				}
		             
            	stAc[i].fan_speed = pst_msg->value;
            	set_fan_speed_status(i);
				break;
				
            case CHAR_AP_ROTATION_SPEED:
				if (stAc[i].dev_type == DEV_RS485_AIR_PURIFIER)
				{ 
	            	stAc[i].fan_speed = pst_msg->value;
	            	set_fan_speed_status(i);
				}
				break;

			case CHAR_AP_PM25_DENSITY:
				stAc[i].PM25 = pst_msg->value;
				ap_pm25_set_status(i);
				break;

			case CHAR_AC_FLOOR_HEAT_AUTO_MAN_CTRL:
				stAc[i].floor_ctrl_mode = pst_msg->value;
				set_floor_auto_man_switch(i);
				break;
				
			default:
				//qDebug()<<"unknow cid="<<pst_msg->cid;
				flagCidValid = 0;
				break;
            }
			if( flagCidValid  
			&&  g_statusXmlHandle
		    &&  !g_statusXmlHandle->getReadOut())
		       {
		           g_statusXmlHandle->updateDevInfo(DEV_TYPE_HAVC,stAc[i].device_id,pst_msg->cid,pst_msg->value);
		       }
            return ENNO_OK;
        }
    }

    return ENNO_OK;
}
#endif	

void HavcWidget::turnIcon(QString current_icon)
{
	bool ok;  
	int current_index = current_icon.toInt(&ok, 10);
	QStringList string_list;
	QStringList string_list2;
	
	string_list<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop"
		<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext";

	string_list2<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext"
		<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop";
	//printf("turnIcon, current_icon=%d\n", current_index);
	for(int i=0; i<button_list.count(); i++)
	{
		QToolButton *tool_button = button_list.at(i);
		if(current_index == i)
		{
		    //printf("tool_button->isChecked()=%d\n", button_state[i]);
			button_state[i] = !button_state[i];
		    if (button_state[i])
		    {
		        //tool_button->setChecked(false);
			    QPixmap pixmap(string_list2.at(i));
			    tool_button->setIcon(pixmap);
			    tool_button->setIconSize(pixmap.size());
			    tool_button->update();
		    }
			else 
			{
			    //tool_button->setChecked(true);
			    QPixmap pixmap(string_list.at(i));
			    tool_button->setIcon(pixmap);
			    tool_button->setIconSize(pixmap.size());
			    tool_button->update();
			}
		}
		else 
		{
		    button_state[i] = 0;
		    QPixmap pixmap(string_list.at(i));
			tool_button->setIcon(pixmap);
			tool_button->setIconSize(pixmap.size());
			tool_button->update();
		}
	}

	//emit turnPage(current_index);
}

#if 0
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
#endif
/*void ContentWidget::paintEvent(QPaintEvent *e)
{
    printf("paintEvent\n");
}
*/
void HavcWidget::set_widget(Direction dir)
{
	int i = 0;

	if (LeftToRight == dir && 0 == current_uid)
	{
	    next_uid = -1;
		return;
	}
	else if (RightToLeft == dir && (device_num - 1) == current_uid)
	{
	    next_uid = device_num;
		return;
	}

    if (RightToLeft == dir)
    {
		next_uid = current_uid + 1;
    }
	else
	{
	    next_uid = current_uid - 1;
	}

	//if (stAc[next_uid].switch_on)
	//{
		//next_uid = i;
		//pLabel_ac_id[i]->setText(tr("AC %1") .arg(next_uid+1));

	if (stAc[next_uid].dev_type == DEV_RS485_HEATER_COOLER)
	{
		QString temp;
		temp.sprintf("%d%c", stAc[next_uid].target_temperature, 0xB0);
		pLabel_ac[next_uid]->setText(temp);
		//printf("next[%d]=wid_get[%d]\n", next_uid, i);
	}


}
void HavcWidget::mousePressEvent(QMouseEvent *event)
{
    if (!i_dev_cnt)
    {
        return;
    }

    //printf("HavcWidget::mousePressEvent\n");
	if(event->button() == Qt::LeftButton) 
	{
	    left_mouse_press = 1;
		m_mouseSrcPos = event->pos();
		m_mouseDstPos = m_mouseSrcPos;

        //qDebug() << "HavcWidget::mousePressEvent,  "<<"x="<< m_mouseSrcPos.x()<<",y="<<m_mouseSrcPos.y();
		//qDebug()<<"dev_type is "<<stAc[current_uid].dev_type;
		if(stAc[current_uid].dev_type == DEV_RS485_HEATER_COOLER)
		{
		   QRect areaBtnDown;
		   QRect areaBtnUp;

		   QRect areaBtnDownSrc;
		   QRect areaBtnUpSrc;

		   //qDebug() << "HavcWidget::mousePressEvent,  "<<"x="<< m_mouseSrcPos.x()<<",y="<<m_mouseSrcPos.y();

		   
		   if((stAc[current_uid].ac_floor_mode == 0 ||  stAc[current_uid].ac_floor_mode == 1))
		   {
		       areaBtnDown.setRect(0, 110, 85, 95);//0, 120, 75, 75
			   areaBtnUp.setRect(235, 110, 85, 95);//245, 120, 75, 75

			   areaBtnDownSrc.setRect(0, 120, 75, 75);
			   areaBtnUpSrc.setRect(245, 120, 75, 75);
			   
		   }
		   else if(stAc[current_uid].ac_floor_mode == 2)
		   {
		       areaBtnDown.setRect(0, 120, 85, 95);//0, 130, 75, 75
			   areaBtnUp.setRect(235, 120, 85, 95);//245, 130, 75, 75

			   areaBtnDownSrc.setRect(0, 130, 75, 75);
			   areaBtnUpSrc.setRect(245, 130, 75, 75);
		   }
		   
		   if(areaBtnDown.contains(m_mouseSrcPos)==true && areaBtnDownSrc.contains(m_mouseSrcPos)==false)
		   {
		       ac_temp_down();
		   }
		   else if(areaBtnUp.contains(m_mouseSrcPos)==true && areaBtnUpSrc.contains(m_mouseSrcPos)==false)
		   {
		       ac_temp_up();
		   }
		}
		//printf("src(%d,%d), dst(%d,%d)\n", 
		//	m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y());
	}
	/*else if(event->button() == Qt::RightButton)
	{
	    left_mouse_press = 0;
	    prev_Widget = current_Widget;
		current_Widget = wid_get[2];
		light_on_paint();
		//current_Widget->raise();
		content->hide();
		right_mouse_press = 1;
	    printf("light button press\n");
		update();
	}*/
	//printf("wid_get=%p\n", wid_get);
	//wid_get->raise();
	
}
void HavcWidget::havc_move_end()
{
	//	printf("current_uid=%d\r\n",current_uid);
		wid_get[0]->move(startx-current_uid*320, starty);
}

#define WIDGET_MOVE_TIME  150
#define WIDGET_MOVE_OFFSET 100
void HavcWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //printf("HavcWidget::mouseReleaseEvent\n");
#if 0
	if (current_Widget == wid_get2 && right_mouse_press)
	{
	    wid_get2->move(startx, starty);
	    wid_get->move(startx, starty);
        wid_get1->move(startx, starty);
		wid_get2->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;
		right_mouse_press = 0;
		light_brightness = 0;
		
		content->show();
	}
#endif
/*	if (current_uid == next_uid || left_mouse_press != 1 || (next_uid  == -1))
	{
	    //printf("current_index=%d,next_index=%d\n", current_uid, next_uid);
	    return;
	}*/

    if (!i_dev_cnt)
    {
        return;
    }

	if (current_uid == next_uid || left_mouse_press != 1 )
	{
	    printf("return for current_uid == next_uid || left_mouse_press != 1\n");
	    QTimer::singleShot(WIDGET_MOVE_TIME+500, this, SLOT(havc_move_end()));
	    return;
	}


	left_mouse_press = 0;
	m_mouseDstPos = event->pos();
	int offset = 0;

	if(next_uid== -1)
	{
		if(m_mouseDstPos.x() >  m_mouseSrcPos.x() )//->right
		{
			offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
			if( offset > WIDGET_MOVE_OFFSET+50)
			{
				offset = WIDGET_MOVE_OFFSET+50;
			}
			QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_l->setDuration(WIDGET_MOVE_TIME);
			anim_l->setKeyValueAt(0, QRect(startx + offset-current_uid*320, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-current_uid*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);

			group_move_widget->start();
		}
		 QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(havc_move_end()));
		return;

	}

	if(next_uid== device_num)
	{
		if(m_mouseDstPos.x() <  m_mouseSrcPos.x())//->left
		{
			offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
			if( offset > WIDGET_MOVE_OFFSET+50)
			{
				offset = WIDGET_MOVE_OFFSET+50;
			}
			QPropertyAnimation *anim_r = new QPropertyAnimation(wid_get[0], "geometry");
			group_move_widget->clear();

			anim_r->setDuration(WIDGET_MOVE_TIME);
			anim_r->setKeyValueAt(0, QRect(startx - offset-current_uid*320, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx-current_uid*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);

			group_move_widget->start();

		}
		 QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(havc_move_end()));
		return;

	}

	int move_time=0;
	 if (m_mouseDstPos.x() <  m_mouseSrcPos.x() && m_mouseSrcPos.x() - m_mouseDstPos.x() > 100) //-> left
	 {
		 	 offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
			group_move_widget->clear();

			anim_l->setDuration(move_time);
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_uid*320, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-320-current_uid*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);

			group_move_widget->start();
			current_uid = next_uid;
			next_uid = -1;
	 }
	 else if(m_mouseDstPos.x() >  m_mouseSrcPos.x() && m_mouseDstPos.x() - m_mouseSrcPos.x() > 100)//-> right
	 {
	 	 offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
	 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
		QPropertyAnimation *anim_r= new QPropertyAnimation(wid_get[0], "geometry");
		group_move_widget->clear();

		anim_r->setDuration(move_time);
		anim_r->setKeyValueAt(0, QRect(startx + offset-current_uid*320, starty, 320, 320));
		anim_r->setKeyValueAt(1, QRect(startx+320-current_uid*320, starty, 320, 320));

		group_move_widget->addAnimation(anim_r);

		group_move_widget->start();
		current_uid = next_uid;
		next_uid = -1;
	 }
	 else if(m_mouseDstPos.x() <  m_mouseSrcPos.x() && m_mouseSrcPos.x() - m_mouseDstPos.x() <= 100)//-> left but return
	 {
	 	 offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
	 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
		QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
		group_move_widget->clear();

		anim_l->setDuration(move_time);
		anim_l->setKeyValueAt(0, QRect(startx - offset-current_uid*320, starty, 320, 320));
		anim_l->setKeyValueAt(1, QRect(startx-current_uid*320, starty, 320, 320));

		group_move_widget->addAnimation(anim_l);
		group_move_widget->start();

	 }
	 else if(m_mouseDstPos.x() >  m_mouseSrcPos.x() && m_mouseDstPos.x() - m_mouseSrcPos.x() <= 100)//-> right but return
	 {
	 	 offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
	 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
		QPropertyAnimation *anim_r= new QPropertyAnimation(wid_get[0], "geometry");
		group_move_widget->clear();

		anim_r->setDuration(move_time);
		anim_r->setKeyValueAt(0, QRect(startx + offset-current_uid*320, starty, 320, 320));
		anim_r->setKeyValueAt(1, QRect(startx-current_uid*320, starty, 320, 320));

		group_move_widget->addAnimation(anim_r);

		group_move_widget->start();
	 }
	 update();



	 QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(havc_move_end()));
	 //


}

void HavcWidget::mouseMoveEvent(QMouseEvent *event)
{
    //printf("HavcWidget::mouseMoveEvent,\n");
    if (!i_dev_cnt)
    {
        return;
    }
	static int cnt = 0;
	if(cnt++%2 == 0)
	{
	    return;
	}
#if 0
    if (right_mouse_press)
    {
        if (light_brightness < 100)
			light_brightness++;
		light_on_paint();
		update();
        return;
    }
#endif
	if (left_mouse_press != 1)
	{
	    return;
	}
	m_mouseDstPos = event->pos();
    //printf("src(%d,%d), dst(%d,%d), move(%d,%d)\n", 
	//       m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y(),
	//       m_mouseDstPos.x() - m_mouseSrcPos.x(), m_mouseDstPos.y() - m_mouseSrcPos.y()); 

	float speed=0;
	int move_offset = 0;

	if (m_mouseDstPos.x() >  m_mouseSrcPos.x())//to right
	{
		set_widget(LeftToRight);

		if (-1 == next_uid)
		{
			if(m_mouseDstPos.x() - m_mouseSrcPos.x() <=WIDGET_MOVE_OFFSET)
			{
			    wid_get[0]->move(startx + current_uid*320 + m_mouseDstPos.x() - m_mouseSrcPos.x(), starty);
			}
			else if((m_mouseDstPos.x() - m_mouseSrcPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseDstPos.x() - m_mouseSrcPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseDstPos.x() - m_mouseSrcPos.x()))/(m_mouseDstPos.x() - m_mouseSrcPos.x());
				move_offset = 200-speed*(m_mouseDstPos.x() - m_mouseSrcPos.x());
			    wid_get[0]->move(startx + move_offset-current_uid*320, starty);
			}
		}
		else
		{
			wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_uid*320, starty);
		}
	}
	else if (m_mouseDstPos.x() <  m_mouseSrcPos.x()) //to left
	{
		set_widget(RightToLeft);
		if (next_uid == device_num)
		{
			if(m_mouseSrcPos.x()-m_mouseDstPos.x() <=WIDGET_MOVE_OFFSET)
			{
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x()- current_uid*320 , starty);
			}
			else if((m_mouseSrcPos.x()-m_mouseDstPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseSrcPos.x()-m_mouseDstPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseSrcPos.x()-m_mouseDstPos.x()))/(m_mouseSrcPos.x()-m_mouseDstPos.x());
				move_offset = 200-speed*(m_mouseSrcPos.x()-m_mouseDstPos.x());
				wid_get[0]->move(startx - move_offset-current_uid*320, starty);
			}
		}
		else
		{
			wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_uid*320, starty);
		}
	}
}

void HavcWidget::light_on_paint()
{
	int i;
	QStringList string_list2;
    string_list2<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext"
		<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop";

	//printf("havc light_on_paint\n");
	for(i=0; i<button_list.count(); i++)
	{
		QToolButton *tool_button = button_list.at(i);
		if(button_state[i])
		{
		    break;
		}
	}
	if (i == button_list.count())
	{
	   i = 0;
	}
#if 1	
	//QImage image = QPixmap(string_list2.at(i)).toImage();
	QImage image;
	//if (light_brightness < 30)
	//{
	    image = QPixmap("contentWidget/blue.png").toImage();
	//}

#if 0
	QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
//模糊值，值越大越模糊
    effect->setBlurRadius(5);
    b_label->setGraphicsEffect(effect);
	b_label->setPixmap(QPixmap("contentWidget/blue.png"));
#endif

	QPainter painter(&image);
	//设置画刷模式
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	//改变画笔和字体
	QPen pen = painter.pen();
	pen.setColor(Qt::white);
	QFont font = painter.font();
	font.setBold(true);//加粗
	font.setPixelSize(45);//改变字体大小
	painter.setPen(pen);
	painter.setFont(font);
	//将文字绘制在图片中心位置
	QString str;
	str.sprintf("%d%", light_brightness);
	painter.drawText(image.rect(), Qt::AlignCenter, str);

	int n = 100;
//保存图片
    image.save("text.jpg","JPG",n);
#endif

}

void HavcWidget::paintEvent(QPaintEvent *e)
{
    //printf("HavcWidget::paintEvent, current_uid=%d\n", current_uid);

   // return;
   
	if( isAnimation )
	{
		QPainter paint(this);
		switch( direction )
		{
		case LeftToRight:
			paintPrevious(paint, current_image);
			paintNext(paint, switch_new_image);
			break;
			
		case RightToLeft:
//			  paintPrevious(paint, nextIndex);
//			  paintNext(paint, currentIndex());
			paintPrevious(paint, switch_new_image);
			paintNext(paint, current_image);
			break;
		}
		
	}

    QPainter painter(this);             // 创建QPainter一个对象
    QPen pen;
	int start_pos = 0;
	int current_point_x = 0;

	if (device_num <= 1)
	{
	    return;
	}

    if (device_num <= 8)
    {
        start_pos = 160 - (device_num/2)*30;
    }
    else
    {
    	//printf("TODO:\r\n");
    	return;
    }

	pen.setColor(Qt::gray);           // 设置画笔为黄色
	pen.setWidth(1);
    painter.setPen(pen);                // 设置画笔

   // QPointF pointf[20];
	QPoint pointC;
	current_point_x = start_pos+current_uid*30.0;
    for (int i=0; i<device_num; ++i)
    {
    	if (i ==  current_uid)
		{
			painter.setPen(QPen(Qt::white, 6));//设置画笔形式
			painter.setBrush(Qt::white);
			painter.drawEllipse(current_point_x-17, 309, 6, 6);//画圆
			painter.drawRect(QRect(current_point_x-12, 309, 20, 6));
			painter.drawEllipse(current_point_x+5, 309, 6, 6);//画圆
		}
    	else if(i<current_uid)
    	{
			painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
			painter.setBrush(Qt::gray);
    		if(i==current_uid-1)
    		{
    			painter.drawEllipse(current_point_x-44, 309, 6, 6);//画圆
    		}
    		else
    		{
    			painter.drawEllipse(current_point_x-(44+32*(current_uid-i-1)), 309, 6, 6);//画圆
    		}
    	}
    	else if(i>current_uid)
		{
			painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
			painter.setBrush(Qt::gray);
			if(i==current_uid+1)
			{
				painter.drawEllipse(current_point_x+32, 309, 6, 6);//画圆
			}
			else
			{
				painter.drawEllipse(current_point_x+(32+32*(i-current_uid-1)), 309, 6, 6);//画圆
			}
		}
#if 0
        if (i !=  current_uid)
    	{
	       // pointf[i].setX(start_pos+i*30.0);
	       // pointf[i].setY(305);
			painter.setPen(QPen(Qt::gray, 5));//设置画笔形式
			painter.setBrush(Qt::gray); 
			painter.drawEllipse(start_pos+i*30.0, 309, 5, 5);//画圆*/
    	}
		else
		{
		    pointC.setX(start_pos+i*30.0);
	        pointC.setY(305);
			painter.setBrush(Qt::white); 
			painter.setPen(QPen(Qt::white, 6));//设置画笔形式
			painter.drawEllipse(start_pos+i*30.0, 309, 6, 6);//画圆*/
		}
#endif
    }
 /*   painter.drawPoints(pointf, device_num);

	pen.setColor(Qt::white);           // 设置画笔为黄色
	painter.setPen(pen);                // 设置画笔
	painter.drawPoint(pointC);
*/
//	tool_button->setIcon(pixmap);
//	tool_button->setIconSize(pixmap.size());
					
	//QRectF rectangle(0.0, 0.0, 130.0, 130.0);
	//	painter.setPen(QPen(Qt::blue,10));//设置画笔形式
		

    
	return;
#if 0	
	if (right_mouse_press)
	{
#if 0
	    QPainter paint(this);
	    QPixmap image1("text.jpg");	
	    //QPixmap image1("contentWidget/blue.png");
	    paint.drawPixmap(110,110,100,100,image1);
#endif
#if 1
		QPainter paint(this);
		QImage image = QPixmap("contentWidget/blue.png").toImage();
		QPainter painter(&image);	
        //设置画刷模式
		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
		//改变画笔和字体
		QPen pen = painter.pen();
		pen.setColor(Qt::white);
		QFont font = painter.font();
		font.setBold(false);//加粗
		font.setPixelSize(40);//改变字体大小
		painter.setPen(pen);
		painter.setFont(font);	

		QRect target(110.0, 110.0, 100.0, 100.0); //建立目标矩形，该区域是显示图像的目的地
		QRect source(0.0, 0.0, 100.0, 100.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域

		//将文字绘制在图片中心位置
		QString str;
		str.sprintf("%d%", light_brightness);
		painter.drawText(image.rect(), Qt::AlignCenter, str);
		paint.drawImage(target,image,source);
		//paint.drawPixmap(110,110,100,100,image1);
#endif
#if 0
			QPainter paint(this);
			QImage image;
			QImage *new_image;
	
			if (light_brightness==0)
			{
				image = QPixmap("contentWidget/blue.png").toImage();
				//new_image = blur(&image);
				
				new_image = &image;
			}
			else
			{
				image = QPixmap("text1.jpg").toImage();
				//new_image = blur(&image);
				new_image = &image;
			}
			QPainter painter(new_image);
			
			//设置画刷模式
			painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
			//改变画笔和字体
			QPen pen = painter.pen();
			pen.setColor(Qt::white);
			QFont font = painter.font();
			font.setBold(false);//加粗
			font.setPixelSize(40);//改变字体大小
			painter.setPen(pen);
			painter.setFont(font);
	
			
			//将文字绘制在图片中心位置
			QString str;
			str.sprintf("%d%", light_brightness);
			
			int n = 100;
			new_image->save("text1.jpg","JPG",n);
			painter.drawText(new_image->rect(), Qt::AlignCenter, str);
			
			//保存图片
			new_image->save("text.jpg","JPG",n);
			QPixmap image1("text.jpg"); 
			
			paint.drawPixmap(110,110,100,100,image1);
			//paint.drawPixmap(110,110,100,100,image1);
#endif

	}
#endif
}
void HavcWidget::switch_to_page(int new_page, QImage *new_image)
{
    if (new_page == 0)
    {
	    switch_new_image = new_image;
        forward();
    }
	else if (new_page == 1)
    {
	    switch_new_image = new_image;
        next();
    }
}

void HavcWidget::snap_shot(QImage **w_image)
{
	QWidget *w = content;
	QPixmap pixmap(w->size());

	w->render(&pixmap);
	QImage img = pixmap.toImage();
	*w_image = new QImage(img);

	//w_image->save("w_image.jpg","JPG",100);
}

void HavcWidget::setDuration(int d)
{
	duration = d;
}

void HavcWidget::paintPrevious(QPainter &paint, QImage *img)
{
#if 1
   // qDebug()<<"QA paintPrevious";
    //QWidget *w = widget(currentIndex);
    //QPixmap pixmap(w->size());
    //w->render(&pixmap);
    //QRect r = w->geometry();
    double value = currentValue.toDouble();
    QRectF r1(0.0, 0, 320, value);
    QRectF r2(0, 320-value, 320, value);
    //paint.drawPixmap(r1, pixmap, r2);
	
	paint.drawImage(r2, *img, r1);
#endif
}

void HavcWidget::paintNext(QPainter &paint, QImage *img)
{
   // qDebug()<<"QA paintNext";
#if 1
    /*QWidget *nextWidget = widget(nextIndex);
    QRect r = geometry();

    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);*/
    double value = currentValue.toDouble();
    QRectF r1(0, value, 320, 320-value);
    QRectF r2(0.0, 0.0, 320, 320-value);
    //paint.drawPixmap(r1, nextPixmap, r2);
	paint.drawImage(r2, *img, r1);
#endif
}

void HavcWidget::valueChanged_slot(QVariant value)
{
    //qDebug()<<"QA valueChanged_slot";

    currentValue = value;
    update();
}

void HavcWidget::animationFinished()
{
    //qDebug()<<"QA animationFinished";
    isAnimation = false;
   // widget(currentIndex())->show();
   // setCurrentIndex(nextIndex);
    content->show();
    delete current_image;
    delete switch_new_image;
    
}

void HavcWidget::showContent()
{
    content->show();  
}

void HavcWidget::next()
{
    //qDebug()<<"QA next";

    if( isAnimation )
    {
        return;
    }
	snap_shot(&current_image);
    isAnimation = true;
    direction = LeftToRight;
    content->hide();
    //QRect g = geometry();
    // int x = g.x();
    int width = 320;
    animation->setStartValue(width);
    animation->setEndValue(0);
    animation->setDuration(duration);
    animation->start();
}

void HavcWidget::forward()
{
    //qDebug()<<"QA forward";

    if( isAnimation )
    {
        return;
    }
	snap_shot(&current_image);

	//current_image->save("c2_image.jpg","JPG",100);
	//switch_new_image->save("s2_image.jpg","JPG",100);
    isAnimation = true;
    direction = RightToLeft;
    content->hide();
   // QRect g = geometry();
    //int x = g.x();
    int width = 320;
    animation->setStartValue(0);
    animation->setEndValue(320);
    animation->setDuration(duration);
    animation->start();
}

int HavcWidget::isAllAcClose(void)
{
    int ret = 1;
    for(int i=0;i<ac_num;i++)
    {
        if(stAc[i].switch_on)
        {
            ret = 0;
			break;
        }
    }

	return ret;
}

int HavcWidget::isAllFloorClose(void)
{
    int startI = ac_num + purify_num;
	int endI = ac_num + purify_num + floor_num;
	int ret = 1;
    for(int i=startI;i<endI;i++)
    {
        if(stAc[i].switch_on)
        {
            ret = 0;
			break;
        }
    }

	return ret;
}



