#include "roomlight.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include "common.h"
#include "wthread.h"

#define LIGHT_IMG_SIZE 100

extern int g_current_dim;

void RoomLightWidget::translateLanguage()
{

}

RoomLightWidget::RoomLightWidget(int room_id, TreeModel *model, QWidget *parent)
{
	QPixmap pixmap;
	int i;
	int j;

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	i_light_cnt = 0;
	i_light_num = 0;
	i_room_id = room_id;
#if 1
	QModelIndex title;
	QModelIndex room;
	QModelIndex light;
	
	//qDebug()<<"CurtainWidget widget"<<endl;

	room = model->index(room_id, 0, title);
	//qDebug() << "model->columnCount(room)="<<model->columnCount(room);
	for (i = 0; i < model->columnCount(room); i++)
	{
		QModelIndex room1 = model->index(0, i, title);
		QVariant columndata1 = model->data(room1, Qt::DisplayRole);
		//qDebug()<<"index["<<room1.column()<<"]room["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
	}

	for (j = 0; j < model->rowCount(room); j++)
	{
	    QModelIndex light1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(light1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
        int i_mode = str_mode.toInt(&ok,10);
		//qDebug()<<"curtain["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl;	
		if (1 == ok && (DEV_DALI_LIGHT == i_mode || DEV_DMX_LIGHT == i_mode 
			|| DEV_KNX_LIGHT_SWITCH == i_mode || DEV_DALI_GROUP == i_mode
			|| DEV_DALI_BOARDCAST == i_mode))
		{	
			//qDebug()<<"room child["<<j<<"] is a DEV_LIGHT"<<endl;
			/*if (DEV_DMX_LIGHT == i_mode)
			{
			    ast_dev[j].device_type = DEV_DMX_LIGHT;
			}
			else if (DEV_KNX_LIGHT_SWITCH == i_mode)
			{
			    ast_dev[j].device_type = DEV_KNX_LIGHT_SWITCH;
			}
			else
			{
			    ast_dev[j].device_type = DEV_DALI_LIGHT;
			}*/
			ast_dev[j].device_type = i_mode;
			i_light_num++;
		}
		else
		{ 
		    continue;
		}
		
		//qDebug()<<"i_light_num=["<<i_light_num<<"]";
		
		QList<QString> light_value;
		light = model->index(j, 0, room);
		for (i = 0; i < model->columnCount(light); i++)
		{
			QModelIndex light1 = model->index(j, i, room);
			columndata1 = model->data(light1, Qt::DisplayRole);
			//qDebug()<<"index=["<<light1.column();
			//qDebug()<<"light["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			light_value<<columndata1.value<QString>();
		}
		ast_dev[i_light_cnt].m_Id = light_value[0];
		ast_dev[i_light_cnt].m_name = light_value[1];
		ast_dev[i_light_cnt].m_mode = light_value[2];
		ast_dev[i_light_cnt].value = light_value[3];
		ast_dev[i_light_cnt].on = light_value[4].toInt(&ok,10);
		ast_dev[i_light_cnt].device_id =  (ast_dev[i_light_cnt].m_Id).toInt(&ok,10);
		ast_dev[i_light_cnt].dim_value = light_value[3].toInt(&ok,10);
		ast_dev[i_light_cnt].device_type = light_value[2].toInt(&ok,10);
       // qDebug()<<"room["<<"room_id"<<"],light["<<i_light_cnt<<"]="
		//	    <<ast_dev[i_light_cnt].device_id<<",type=["<<ast_dev[i_light_cnt].device_type<<"]";
        
		i_light_cnt++;
	}
	#endif
	//right_center_function_widget = new QWidget(this);
	#if 0
	dis_label = new QLabel(this);
	
	dis_label->setText("light on");
	dis_label->setStyleSheet("font-size:30px;letter-spacing:6px;background-color: black;"\
	                          "border-style:none;color:white;"\
	                          "font-weight:normal;text-align:left;");
	dis_label->setGeometry(QRect(90, 80+LIGHT_IMG_SIZE, 200, 50));
	#endif
	//right_center_function_widget->raise();
	//this->show();
	#if 0
	img_label_back = new QLabel(this);
	QImage *img = new QImage;
	img->load("roomlight/light_on");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);
	
    pixmap.load("roomlight/light_on");
	img_label_back->setPixmap(QPixmap::fromImage(*img));
	img_label_back->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label_back->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
	img_label_back->hide();
	#endif
	
	img_label = new QLabel(this);
	label_lighton = new QLabel(this);
	label_lightoff = new QLabel(this);
	
    movie_on = new QMovie("roomlight/lighton_gif");
	label_lighton->setMovie(movie_on);
	movie_on->start();
	movie_on->stop();
	label_lighton->hide();

	movie_off = new QMovie("roomlight/lightoff_gif");
	label_lightoff->setMovie(movie_off);
	movie_off->start();
	movie_off->stop();
	label_lightoff->hide();

	connect(movie_on, SIGNAL(frameChanged(int)), this, SLOT(lighton_close())); 
	connect(movie_off, SIGNAL(frameChanged(int)), this, SLOT(lightoff_close())); 
	
}

//static bool first_operation_flag = false;
void RoomLightWidget::lighton_close()
{
#if 0
	QImage *img = new QImage;
	img->load("roomlight/light_on");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);

    img_label->close();
	img_label->setPixmap(QPixmap::fromImage(*img));
	img_label->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
	qDebug()<<"room="<<g_current_room<<", light_num="<<i_light_num;
#endif
   // delete img_label;
//    qDebug()<<"on["<<movie_on->currentFrameNumber()<<","<<movie_on->frameCount()<<"]";
    if (movie_on->currentFrameNumber() == movie_on->frameCount() - 1) {
        movie_on->stop();
    }
    else
    {
        return;
    }

  // QPixmap pixmap;
  // qDebug()<<"on close end frame="<<movie_on->currentFrameNumber()<<"total="<<movie_on->frameCount();
   movie_on->stop();
   label_lighton->hide();

    //ctrl
    int i;

    #if 0
	if (i_light_num > NUM_OF_LIGHT_BEFORE_MOIVE)
	{
	    for (i = NUM_OF_LIGHT_BEFORE_MOIVE; i < i_light_num; i++)
		{
		    if (g_thread != NULL)
			{
			    ast_dev[i].on = 1;
				if (ast_dev[i].device_type == DEV_DMX_LIGHT)
				{
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_UI_ON_OFF, 1);
				}
				else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
				{
				    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
				}
				else
				{
				    #if 0 //touch button set to current value
				    if (!ast_dev[i].dim_value)
				   	{
						ast_dev[i].dim_value = (g_current_dim == 0) ? 100 : g_current_dim;
				    }
	                #endif

					//touch button set to 100
					ast_dev[i].dim_value = 100;
					
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on);
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, ast_dev[i].dim_value); 
				}
			}
			else
			{
			    qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
			}
		}
	}
	#endif
  // img_label->clear();
   
  // printf("movie_off->currentFrameNumber()=%d\r\n",movie_off->currentFrameNumber());
   //if(movie_off->currentFrameNumber()==movie_off->frameCount() - 1)
   if(QMovie::NotRunning == movie_off->state())
	   emit closeRoomlight();
   #if 0
   QImage *img = new QImage;
   img->load("roomlight/light_on");
   *img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);
   
   pixmap.load("roomlight/light_on");
   img_label_back->setPixmap(QPixmap::fromImage(*img));

   img_label_back->raise();
   img_label_back->show();
   #endif
}
int RoomLightWidget::lightNum()
{
    return i_light_num;
}
void RoomLightWidget::set_light_status(int val)
{
	QPixmap pixmap;
	int i;
    int light_status = (val==0) ? 0 : 1;

    qDebug()<<"set light status = "<<val;
	if (!g_room_num)
	{
	    qDebug()<<"there is no room, lighton fail";
		return;
	}
	
	/*for (i = 0; i < i_light_num; i++)
	{
	    ast_dev[i].on = light_status;
	}*/

	setLightStatus(light_status);
}

void RoomLightWidget::set_light_value(int val)
{
	QPixmap pixmap;
	int i;
    int light_value = val;

	if (val >= 0 && val <= 100)
	{
	    light_value = val;
	}
	else
	{
        qDebug()<<"set light val error = "<<val;
	    return;
	}

    qDebug()<<"set light val = "<<val;
	if (!g_room_num)
	{
	    qDebug()<<"there is no room, lighton fail";
		//return;
	}

    if (!g_room_num)
    {
        return;
    }
	for (i = 0; i < i_light_num; i++)
	{
	    ast_dev[i].dim_value = light_value;
		//qDebug()<<"val="<<ast_dev[i].dim_value;
	}
}

int RoomLightWidget::get_metal_scene_set()
{
    int set = 0;
    QSettings *configIni = new QSettings("/home/user/conf/ctl_metal.ini", QSettings::IniFormat);

	set = configIni->value("metal_scene/scene_set").toInt();

    delete  configIni;

	//ENNO_LOG_ERR("set=%d\r\n", set);

	return set;
}

int RoomLightWidget::get_metal_scene_status()
{
    int status = 0;
    QSettings *configIni = new QSettings("/home/user/conf/ctl_metal.ini", QSettings::IniFormat);

	status = configIni->value("metal_scene/scene_status").toInt();

    delete  configIni;
	
	//ENNO_LOG_ERR("status=%d\r\n", status);

	return status;
}

void RoomLightWidget::lighton_ctrl()
{
    int i;
	
    for (i = 0; i < i_light_num; i++)
	{
	    if (g_thread != NULL)
		{
		    ast_dev[i].on = 1;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_UI_ON_OFF, 1);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
			    #if 0 //touch button set to current value
			    if (!ast_dev[i].dim_value)
			   	{
					ast_dev[i].dim_value = (g_current_dim == 0) ? 100 : g_current_dim;
			    }
                #endif

				//touch button set to 100
				ast_dev[i].dim_value = 100;
				
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, ast_dev[i].dim_value); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
		}
	}
}
void RoomLightWidget::lighton_press()
{
	QPixmap pixmap;
	int i;
	int light_n;

	if (!g_room_num)
	{
	    qDebug()<<"there is no room, lighton fail";
		//return;
	}
#if 0
	pixmap.load("roomlight/light_on");
	img_label_back->setPixmap(QPixmap::fromImage(*img));
	img_label_back->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label_back->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
#endif
	//img_label->setFixedSize(OLED_PIX_X, OLED_PIX_Y);
	//img_label->setGeometry(QRect(0, 0, OLED_PIX_X, OLED_PIX_Y));
//	QTimer::singleShot(2000, this, SLOT(lighton_close()));
#if 0
    QMovie *movie = new QMovie("roomlight/lighton_gif");
    img_label->raise();
	img_label->show();
    img_label->setMovie(movie);
#endif
    label_lighton->raise();
	label_lighton->show();
	movie_on->setSpeed(150);
	movie_on->jumpToFrame(0);
	qDebug()<<"on current farme="<<movie_on->currentFrameNumber();
	
	movie_on->start();
	//update();
	//qDebug()<<"room="<<g_current_room<<", light_num="<<i_light_num;

    if (!g_room_num)
    {
        return;
    }
	
	ENNO_LOG_ERR("light on end\n");
}

void RoomLightWidget::lighton()
{
	QPixmap pixmap;
	int i;
	int light_n;
	//img_label = new QLabel(this);
/*	QImage *img = new QImage;
	img->load("roomlight/light_on");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);
*/
	//dis_label->setText("light on");
	if (!g_room_num)
	{
	    qDebug()<<"there is no room, lighton fail";
		//return;
	}
#if 0
	pixmap.load("roomlight/light_on");
	img_label_back->setPixmap(QPixmap::fromImage(*img));
	img_label_back->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label_back->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
#endif
	//img_label->setFixedSize(OLED_PIX_X, OLED_PIX_Y);
	//img_label->setGeometry(QRect(0, 0, OLED_PIX_X, OLED_PIX_Y));
//	QTimer::singleShot(2000, this, SLOT(lighton_close()));
#if 0
    QMovie *movie = new QMovie("roomlight/lighton_gif");
    img_label->raise();
	img_label->show();
    img_label->setMovie(movie);
#endif
    label_lighton->raise();
	label_lighton->show();
	movie_on->setSpeed(100);
	movie_on->jumpToFrame(0);
	qDebug()<<"on current farme="<<movie_on->currentFrameNumber();
	
	ENNO_LOG_ERR("light on start\n");
	movie_on->start();
	//update();
	//qDebug()<<"room="<<g_current_room<<", light_num="<<i_light_num;

	if (access(CTL_METAL_FILE, F_OK) == 0)
	{
	    if (get_metal_scene_set())
	    {
	        ENNO_LOG_ERR("msg_metal_scene_ctrl 1\r\n");
            g_thread->msg_metal_scene_ctrl(1,100);
		    return;
	    }
		else
		{
		    if (g_room_num)
			    g_thread->msg_set_metal_light_ctrl(g_current_room, 1);
		    ENNO_LOG_ERR("not set metal\r\n");
		}
	}
	else
	{
	    ENNO_LOG_ERR("access CTL_METAL_FILE fail\r\n");
	}

    if (!g_room_num)
    {
        return;
    }
	
    #if 0
	ENNO_LOG_ERR("light num=%d\n", i_light_num);

	light_n = (i_light_num > NUM_OF_LIGHT_BEFORE_MOIVE) ? NUM_OF_LIGHT_BEFORE_MOIVE : i_light_num;

    for (i = 0; i < light_n; i++)
	{
	    if (g_thread != NULL)
		{
		    ast_dev[i].on = 1;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_UI_ON_OFF, 1);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
			    #if 0 //touch button set to current value
			    if (!ast_dev[i].dim_value)
			   	{
					ast_dev[i].dim_value = (g_current_dim == 0) ? 100 : g_current_dim;
			    }
                #endif

				//touch button set to 100
				ast_dev[i].dim_value = 100;
				
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, ast_dev[i].dim_value); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
		}
	}
	#endif
	
//	QTimer::singleShot(1200, this, SLOT(lighton_ctrl()));
/*	for (i = 0; i < i_light_num; i++)
	{
	    if (g_thread != NULL)
		{
		    ast_dev[i].on = 1;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_UI_ON_OFF, 1);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
			    #if 0 //touch button set to current value
			    if (!ast_dev[i].dim_value)
			   	{
					ast_dev[i].dim_value = (g_current_dim == 0) ? 100 : g_current_dim;
			    }
                #endif

				//touch button set to 100
				ast_dev[i].dim_value = 100;
				
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on);
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, ast_dev[i].dim_value); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
		}
	}
*/
	ENNO_LOG_ERR("light on end\n");
}
void RoomLightWidget::lightoff_close()
{
#if 0
	QImage *img = new QImage;
	img->load("roomlight/light_off");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);
	QLabel *img_label = new QLabel(this);

    img_label->close();
	img_label->setPixmap(QPixmap::fromImage(*img));
	img_label->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
	qDebug()<<"room="<<g_current_room<<", light_num="<<i_light_num;	
#endif
   // delete img_label;
   
//    qDebug()<<"off["<<movie_off->currentFrameNumber()<<","<<movie_off->frameCount()<<"]";
    if (movie_off->currentFrameNumber() == movie_off->frameCount() - 1) {
        movie_off->stop();
    }
    else
    {
        return;
    }
	
	QPixmap pixmap;
	movie_off->stop();
    label_lightoff->hide();

    //light off ctrl
	int i;
    #if 0
    if (i_light_num > NUM_OF_LIGHT_BEFORE_MOIVE)
	{
		for (i = NUM_OF_LIGHT_BEFORE_MOIVE; i < i_light_num; i++)
		{
		    if (g_thread != NULL)
			{
			    ast_dev[i].on = 0;
				if (ast_dev[i].device_type == DEV_DMX_LIGHT)
				{
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				}
				else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
				{
				    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
				}
				else
				{
					g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on); 
				}
			}
			else
			{
			    qDebug()<<"[qterror],lightoff g_thread is NULL"<<endl;
			}
		}
	}
	#endif
	//qDebug()<<"off_close end frame="<<movie_on->currentFrameNumber()<<"total="<<movie_on->frameCount();
	//img_label->clear();
    //printf("movie_on->currentFrameNumber()=%d\r\n",movie_on->currentFrameNumber());
    //if(movie_on->currentFrameNumber()==movie_on->frameCount() - 1)
	if(QMovie::NotRunning == movie_on->state())
	emit closeRoomlight();

	#if 0
	QImage *img = new QImage;
	img->load("roomlight/light_off");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);

	pixmap.load("roomlight/light_off");
	img_label_back->setPixmap(QPixmap::fromImage(*img));

    img_label_back->raise();
    img_label_back->show();
	#endif

}
void RoomLightWidget::lightoff_ctrl()
{
    int i;

	for (i = 0; i < i_light_num; i++)
	{
	    if (g_thread != NULL)
		{
		    ast_dev[i].on = 0;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lightoff g_thread is NULL"<<endl;
		}
	}
}

void RoomLightWidget::lightoff()
{
	int i;
	int light_n;
	QPixmap pixmap;
    //img_label = new QLabel(this);

	/*QImage *img = new QImage;
	img->load("roomlight/light_off");
	*img = img->scaled(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE,Qt::IgnoreAspectRatio);
	*/
	//dis_label->setText("light off");
#if 0
	pixmap.load("roomlight/light_off");
	img_label_back->setPixmap(QPixmap::fromImage(*img));
	img_label_back->setFixedSize(LIGHT_IMG_SIZE,LIGHT_IMG_SIZE);
	img_label_back->setGeometry(QRect(90, 70, LIGHT_IMG_SIZE, LIGHT_IMG_SIZE));
	qDebug()<<"room="<<g_current_room<<", light_num="<<i_light_num;
#endif
	//img_label->setFixedSize(OLED_PIX_X, OLED_PIX_Y);
	//img_label->setGeometry(QRect(0, 0, OLED_PIX_X, OLED_PIX_Y));
//	QTimer::singleShot(2000, this, SLOT(lightoff_close()));

#if 0
	QMovie *movie = new QMovie("roomlight/lightoff_gif");
	
	img_label->raise();
	img_label->show();
	img_label->setMovie(movie);	
#endif
	if (!g_room_num)
	{
		qDebug()<<"there is no room, lighton fail";
		//return;
	}

    label_lightoff->raise();
	label_lightoff->show();
	movie_off->setSpeed(100);
	movie_off->jumpToFrame(0);
	movie_off->start();
	
	ENNO_LOG_ERR("light off start\n");

	qDebug()<<"off current farme="<<movie_off->currentFrameNumber();

	if (access(CTL_METAL_FILE, F_OK) == 0)
	{
	    if (get_metal_scene_set())
	    {
	        ENNO_LOG_ERR("msg_metal_scene_ctrl 0\r\n");
            g_thread->msg_metal_scene_ctrl(0,100);
		    return;
	    }
		else
		{
		    ENNO_LOG_ERR("not set metal\r\n");
			if (g_room_num)
		        g_thread->msg_set_metal_light_ctrl(g_current_room, 0);
		}
	}
	//update();
	if (!g_room_num)
	{
		return;
	}
	
	//QTimer::singleShot(1200, this, SLOT(lightoff_ctrl()));
    #if 0
	light_n = (i_light_num > NUM_OF_LIGHT_BEFORE_MOIVE) ? NUM_OF_LIGHT_BEFORE_MOIVE : i_light_num;
	for (i = 0; i < light_n; i++)
	{
	    if (g_thread != NULL)
		{
		    ast_dev[i].on = 0;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_ON, ast_dev[i].on); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lightoff g_thread is NULL"<<endl;
		}
	}
    #endif
	
	ENNO_LOG_ERR("light off end\n");
}

void RoomLightWidget::lightoff_press()
{
	int i;
	int light_n;
	QPixmap pixmap;

	if (!g_room_num)
	{
		qDebug()<<"there is no room, lighton fail";
		//return;
	}

    label_lightoff->raise();
	label_lightoff->show();
	movie_off->setSpeed(150);
	movie_off->jumpToFrame(0);
	movie_off->start();
	
	ENNO_LOG_ERR("light off start\n");

	qDebug()<<"off current farme="<<movie_off->currentFrameNumber();

	//update();
	if (!g_room_num)
	{
		return;
	}
	
	ENNO_LOG_ERR("light off end\n");
}

void RoomLightWidget::updateStatus(uint32 device_id, uint32 value)
{
    int ret;
    int i;
	
    for (i = 0; i < i_light_num; i++)
	{
		if(device_id == ast_dev[i].device_id)
		{
			if (value == 0)
			{
				ast_dev[i].on = 0; 
				//qDebug()<<"ast_dev["<<i<<"]=0";
				ret = 0;
			}
			else
			{
				ast_dev[i].on = 1; 
				//qDebug()<<"ast_dev["<<i<<"]=1";
				ret = 1;
			}
		}
	}
}

int RoomLightWidget::updateDeviceStatus(char *buf_in, uint32 len)
{
	int i = 0;
	int ret = -1;
	char *buf = (char *)buf_in;
	CTL_MSG_CONF msg_conf;
	uint32 device_id;
	uint32 c_id;
	uint32 c_value;
	uint32 len_out;
	int len_total = len;
	
	//qDebug()<<"device_id="<<pst_msg->device_id<<",iid="<<pst_msg->cid<<",value="<<pst_msg->value;
	while (len >= 12)
	{
		ret = enno_decode_uint(buf, &device_id, &len_out, len);
		ENNO_CHECK_RET(ret);
		buf += len_out;
		ENNO_CHECK_PARA_RET(len < len_out);
		len -= len_out;
	   
		ret = enno_decode_uint(buf, &c_id, &len_out, len);
		ENNO_CHECK_RET(ret);
		buf += len_out;
		ENNO_CHECK_PARA_RET(len < len_out);
		len -= len_out;

		ret = enno_decode_uint(buf, &c_value, &len_out, len);
		ENNO_CHECK_RET(ret);
		buf += len_out;
		ENNO_CHECK_PARA_RET(len < len_out);
		len -= len_out;

		//
	   // ENNO_LOG_ERR("id=%d, c_id=%d, val=%d\r\n", device_id, c_id, c_value);
        updateStatus(device_id, c_value);
	}

	return ret;

}
int RoomLightWidget::updateLightStatus(char *buf_in, uint32 len_buf)
{
    CTL_DEVICE_CONF *pst_msg = (CTL_DEVICE_CONF *)buf_in;
    int i = 0;
	int ret = -1;
	//qDebug()<<"device_id="<<pst_msg->device_id<<",iid="<<pst_msg->cid<<",value="<<pst_msg->value;
	for (i = 0; i < i_light_num; i++)
	{
	    if(pst_msg->device_id == ast_dev[i].device_id)
	    {
	        if(pst_msg->value == 0)
	        {
	            ast_dev[i].on = 0; 
				//qDebug()<<"ast_dev["<<i<<"]=0";
				ret = 0;
	        }
			else
			{
			    ast_dev[i].on = 1; 
				//qDebug()<<"ast_dev["<<i<<"]=1";
				ret = 1;
			}
	    }
	}
	//qDebug()<<"end";

	return ret;
	
}

int RoomLightWidget::getMetalButtonStatus(void)
{    
    int ret = 1;
	int i = 0;

    if (access(CTL_METAL_FILE, F_OK) == 0)
	{
	    if (get_metal_scene_set())
	    {
	        ret = get_metal_scene_status();

			return !ret;
	    }
 	}
	else
	{
	    ENNO_LOG_ERR("access CTL_METAL_FILE fail \r\n");
	}

	ret = !i_light_status;
	
	return ret;
}

int RoomLightWidget::isAllLightOff(void)
{
    int ret = 1;
	int i = 0;
	
	/*for (i = 0; i < i_light_num; i++)
	{
	    if(ast_dev[i].on == 1)
	    {
	        //qDebug()<<"ast_dev["<<i<<"] is on, dev_id="<<ast_dev[i].device_id;
	        ret = 0;
			break;
	    }
	}*/
	
	return !i_light_status;
}

void RoomLightWidget::setLightStatus(uint32 status)
{
    i_light_status = status;
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void RoomLightWidget::changeLanguage()
{
    language_flag ^= 1;
    if (language_flag)
    {
        translator->load(QString("qwid_zh_CN.qm"));
    }
	else
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);
	//labelText->setGeometry(QRect(0, 0, OLED_PIX_X, 100));

	perBar->setGeometry(QRect(0, 142, OLED_PIX_X, 18));
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

RoomLightTextWidget::RoomLightTextWidget(QWidget *parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);

	labelText = new QLabel(this);
	//labelText->setGeometry(QRect(0, 0, 320, 100));
	labelText->setStyleSheet("font-size:40px;font-weight:440;color:rgb(255,255, 255);text-align:center;background:transparent;");
	//labelText->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
	labelText->setGeometry(QRect(0, 0, 320, 60*2)); //ËÄ±¶ÐÐ¾à 
	labelText->setWordWrap(true); 
	labelText->setAlignment(Qt::AlignCenter); 

	labelText->setText(tr("light OFF"));

	perBar = new QProgressBar(this);
	perBar->setFormat(QString::fromLocal8Bit(""));
	perBar->setStyleSheet("background-color:rgb(109, 109, 109); color:rgb(90, 224, 255);border-radius:10px");
	perBar->setGeometry(QRect(0, 142, 320, 18));
	perBar->setRange(0, 100-1);
	perBar->setValue(0);
	perBar->setObjectName("lightPorgress");
	perBar->setVisible(0);

	timer_bar_out = new QTimer();
	connect(timer_bar_out, SIGNAL(timeout()), this, SLOT(onTimerBarOut()));
}

void RoomLightTextWidget::onTimerBarOut()
{
    QObject *object = QObject::sender();
    QTimer *timer2 = qobject_cast<QTimer *>(object);

    timer_cnt++;
	perBar->setValue(timer_cnt);

	if (timer_cnt >= 100)
	{
	    timer_bar_out->stop();
		perBar->setVisible(0);
	}
	
}

void RoomLightTextWidget::light_release()
{
    timer_bar_out->stop();
	perBar->setVisible(0);
	timer_cnt = 0;
}


void RoomLightTextWidget::lighton()
{
    labelText->setText(tr("Lights On"));
	if (g_room_num == 0)
	{
	    labelText->setText(tr("NO ROOM"));
	}
}

void RoomLightTextWidget::lighton_press()
{
    labelText->setText(tr("Long press to turn on the light"));
	
    timer_bar_out->start(TIMER_BAR_MS/100);
	perBar->setVisible(1);
	timer_cnt = 0;
	if (g_room_num == 0)
	{
	    labelText->setText(tr("NO ROOM"));
	}
}


void RoomLightTextWidget::nolight()
{
    labelText->setText(tr("No Light"));
}


void RoomLightTextWidget::lightoff()
{
    labelText->setText(tr("Lights Off"));
	if (g_room_num == 0)
	{
	   labelText->setText(tr("NO ROOM"));
	}
}

void RoomLightTextWidget::lightoff_press()
{
    labelText->setText(tr("Long press to turn off the light"));
	if (g_room_num == 0)
	{
	   labelText->setText(tr("NO ROOM"));
	}

	timer_bar_out->start(TIMER_BAR_MS/100);
	perBar->setVisible(1);
	timer_cnt = 0;
}

void RoomLightTextWidget::translateLanguage()
{
    
}


