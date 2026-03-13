#include "curtain_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include "status_xml.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "common.h"

extern QElapsedTimer g_click_timer;  
extern int g_no_shake;
extern int g_current_index;
int shade_dim_press;
//#define SCENE_BUTTON_X 110
//#define SCENE_BUTTON_Y 110

//#define SCENE_DO_TIME 1500
extern int g_do_scene_flag;
void CurtainWidget::translateLanguage()
{
	//more_button->setText(tr("language"));

	if (i_curtain_num == 0 && content)
    {
        QLabel *no_curtain = content->findChild<QLabel*>("noshade");
		if(no_curtain)
		{
		   no_curtain->setText(tr("There's no shade here.\n")); 
		}
    }
}
void CurtainWidget::onTimerOut()
{
    //qDebug()<<"onTimerOut in";
    if (!right_mouse_press)
    {
		shade_dim_press = 0;
		timer_shade_out->stop();
        return;
    }
	
    if (current_Widget == wid_get2)
	{	    
		/*shade_dim_press = 0;
	    wid_get2->move(startx, starty);
	   // wid_get->move(startx, starty);
       // wid_get1->move(startx, starty);
		wid_get2->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;
		
	    QImage *img = new QImage;
	    img->load(button_select->m_img);
		//*img = img->scaled(115,115,Qt::KeepAspectRatio);
	    button_select->setIcon(QPixmap::fromImage(*img));
	    button_select->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
	    button_select->update();

		content->show();
		right_mouse_press = 0;
		g_no_shake = 0;*/
		wid_get2->move(startx, starty);
	    // wid_get->move(startx, starty);
		// wid_get1->move(startx, starty);
		wid_get2->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;

		content->show();
		g_no_shake = 0;
		shade_dim_press = 0;
		right_mouse_press = 2;
		
		lightdim_to_shade_start();
		return;
	}
}
#define DB_CMD_LEN 256
#define OS_ENNO_DB_FILE "/home/user/conf/enno_db"

CurtainWidget::CurtainWidget(int room_id, TreeModel *model, QWidget *parent)
{
	QSqlDatabase database;
	char *sql1 = "select * from ShadeInfo WHERE DevID=";
	char sql[DB_CMD_LEN];
	
	database = QSqlDatabase::addDatabase("QSQLITE");
	database.setDatabaseName(OS_ENNO_DB_FILE);
	if (!database.open())
	{
		qDebug() << "Error: Failed to connect database." << database.lastError();
	}
	else
	{
		qDebug() << "Succeed to connect database." ;
	}
		
	QSqlQuery sql_query;

	QList<QVariant> columnData;
	int i;
	int j;
	i_curtain_num = 0;
	i_curtain_cnt = 0;
	right_mouse_press = 0;
	
	shade_dim_press = 0;
	isAnimation = false;
    duration = 200;
    animation = new QPropertyAnimation(this, QByteArray());
	group_shade_anima = new QParallelAnimationGroup;
	group_light_anima = new QParallelAnimationGroup;
	group_move_widget = new QParallelAnimationGroup;

    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(640,320);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	content = new QWidget(this);
	content->setFixedSize(320,306);
	content->raise();

	timer_shade_out = new QTimer();
    timer_shade_out->setInterval(TIMER_LIGHT_OUT_MS);  //30s 

    connect(timer_shade_out, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    //qDebug()<<"CurtainWidget in"<<endl;
	QStringList string_list;
	string_list<<"curtainWidget/curtain_off"<<"curtainWidget/curtain_on"
		<<"curtainWidget/curtain_dim";

	QStringList string_list_window;
	string_list_window<<"curtainWidget/window_off.png"<<"curtainWidget/window_on.png";
		//<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add";

	///grid_layout = new QGridLayout();
	///grid_layout1 = new QGridLayout();
	//grid_layout2 = new QGridLayout();
	
	//wid_get = new QWidget(content);
	//wid_get1 = new QWidget(content);
	wid_get2 = new QWidget(this);

	QModelIndex title;
	QModelIndex room;
	QModelIndex curtain;
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
	    QModelIndex curtain1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(curtain1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
        int i_mode = str_mode.toInt(&ok,10);
		//qDebug()<<"curtain["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl;	
		if (1 == ok && 
			(   DEV_RS485_SHADES == i_mode 
			 || DEV_KNX_WINDOW == i_mode )
		    )
		{	
			i_curtain_num++;
		}
		else
		{ 
		    continue;
		}
		
		QList<QString> curtain_value;
		curtain = model->index(j, 0, room);
		for (i = 0; i < model->columnCount(curtain); i++)
		{
			QModelIndex curtain1 = model->index(j, i, room);
			columndata1 = model->data(curtain1, Qt::DisplayRole);
			//qDebug()<<"index=["<<curtain1.column();
			//qDebug()<<"curtain["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			curtain_value<<columndata1.value<QString>();
		}

		//qDebug()<<"i_curtain_cnt="<<i_curtain_cnt<<", init";
		ast_dev[i_curtain_cnt].m_Id = curtain_value[0];
		ast_dev[i_curtain_cnt].m_name = curtain_value[1];
		ast_dev[i_curtain_cnt].m_mode = curtain_value[2];
		ast_dev[i_curtain_cnt].value = curtain_value[3];
		ast_dev[i_curtain_cnt].on = curtain_value[4].toInt(&ok,10);
		ast_dev[i_curtain_cnt].target_pos = curtain_value[3].toInt(&ok,10);
		ast_dev[i_curtain_cnt].device_type = i_mode;

		if (ast_dev[i_curtain_cnt].target_pos > 100)
		{
		    ast_dev[i_curtain_cnt].target_pos = 100;
		}
		else if (ast_dev[i_curtain_cnt].target_pos < 0)
		{
		    ast_dev[i_curtain_cnt].target_pos = 0;
		}
		
		ast_dev[i_curtain_cnt].current_pos = ast_dev[i_curtain_cnt].target_pos;
		ast_dev[i_curtain_cnt].device_id =  (ast_dev[i_curtain_cnt].m_Id).toInt(&ok,10);

		snprintf(sql, DB_CMD_LEN, "%s%d", sql1, ast_dev[i_curtain_cnt].device_id);
			
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
	            ast_dev[i_curtain_cnt].factory_model = sql_query.value(2).toInt();
	            ast_dev[i_curtain_cnt].addr = sql_query.value(3).toInt();
				ENNO_LOG_ERR("i=%d, model=%d, addr=%d\n", 
					i_curtain_cnt, 
					ast_dev[i_curtain_cnt].factory_model, 
					ast_dev[i_curtain_cnt].addr);
        	}
	    }

		i_curtain_cnt++;
		//qDebug()<<"i_curtain_cnt="<<i_curtain_cnt<<", init end";
	}
	
    database.close();
	
	QSignalMapper *signal_mapper = new QSignalMapper(this);

    qDebug()<<"i_curtain_num="<<i_curtain_num;
    if (!i_curtain_num)
    {
        QLabel *no_curtain = new QLabel(content);
		no_curtain->setText(tr("There's no shade here.\n"));
		//no_curtain->setStyleSheet("font-size:30px;color: white;border-width:2px;border-style:solid;border-color:black;font-weight:bold;text-align:left;");
		//no_curtain->setGeometry(QRect(0, 0, 320, 120));
		no_curtain->setObjectName("noshade");
	wid_get[0]->setFixedSize(OLED_PIX_X*(page_num+2),306);
		label_scene_onoff[i]->setGeometry(QRect(OLED_PIX_X*(i+1), 150, OLED_PIX_X, 60));
		label_scene_name[i]->setGeometry(QRect(OLED_PIX_X*(i+1), 90, OLED_PIX_X, 60));
        return;
    }
    page_num = (i_curtain_num+3)/4;
    wid_get[0] = new sceneWidget(content);
	wid_get[0]->setFixedSize(320*(page_num+2),306);
	wid_get[0]->hide();
	for (i = 0; i < page_num; i++)
	{
		label_scene_onoff[i] = new QLabel(wid_get[0]);
		label_scene_onoff[i]->setGeometry(QRect(320*(i+1), 150, 320, 60));
		label_scene_onoff[i]->setStyleSheet("font-size:44px;font-weight:bold;color: white;text-align:left;background:transparent;");
		label_scene_onoff[i]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
		label_scene_onoff[i]->hide();

		label_scene_name[i] = new QLabel(wid_get[0]);
		label_scene_name[i]->setGeometry(QRect(320*(i+1), 90, 320, 60));
		label_scene_name[i]->setStyleSheet("font-size:44px;font-weight:bold;color: white;text-align:left;background:transparent;");
		label_scene_name[i]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
		label_scene_name[i]->hide();
	}
    //qDebug()<<"i_curtain_num="<<i_curtain_num<<endl;
	for (i=0; i<i_curtain_num; i++)
	{
		 widget_tmp = wid_get[0];
		SceneButton *tool_button = new SceneButton(widget_tmp);
		tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		int image_id = ast_dev[i].on;
		button_state[i] = ast_dev[i].on;
		if (image_id >= string_list.size())
		{
		    //qDebug()<<"error image_id = "<<image_id;
			continue;
		}
		//qDebug()<<"image_id = "<<image_id;
		QString strImg;
		if(ast_dev[i].device_type == DEV_KNX_WINDOW)
		{
		    strImg = string_list_window.at(image_id);
		}
		else if(ast_dev[i].device_type == DEV_RS485_SHADES)
		{
		    strImg = string_list.at(image_id);
		}
		QPixmap button_pixmap(strImg);
		bool b = ast_dev[i].m_name.contains(QRegExp("[\\x4e00-\\x9fa5]+")); 
		QString str_name;
		
		if (b)
		{
			if (ast_dev[i].m_name.size() <= MIN_CHINESE_NAME_SIZE)
			{
				str_name = QString("  ") + ast_dev[i].m_name;
			}
			else if (ast_dev[i].m_name.size() > MIN_CHINESE_NAME_SIZE
				&& ast_dev[i].m_name.size() <= MID_CHINESE_NAME_SIZE)
			{
				str_name = ast_dev[i].m_name;
			}
			else
			{
				str_name = ast_dev[i].m_name.left(MID_CHINESE_NAME_SIZE) + QString("..");
			}
		}
		else
		{
			if (ast_dev[i].m_name.size() <= 4)
			{
				str_name = QString("  ") + ast_dev[i].m_name;					
			}
			else if (ast_dev[i].m_name.size() <= 5)
			{
				str_name = ast_dev[i].m_name;
			}
			else
			{
				str_name = ast_dev[i].m_name.left(5) + QString(".");
			}
		}
		
		tool_button->setText(str_name);
		tool_button->setImg(strImg);
		tool_button->setIcon(button_pixmap);//QPixmap::fromImage(img)
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));//(button_pixmap.size());
		//tool_button->setFixedSize(SCENE_BUTTON_X, SCENE_BUTTON_Y+40);
		tool_button->setObjectName("functionButton");
		tool_button->stCurtain = &ast_dev[i];
		tool_button->setIndex(i);

		button_list.append(tool_button);
		connect(tool_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
		connect(tool_button, SIGNAL(pressIcon()), this, SLOT(PressShade()));
		
		signal_mapper->setMapping(tool_button, QString::number(i, 10));
		int buttom_offset_x =0;
		int buttom_offset_y =0;
        j = i%4;
		if(j%2==0)
		{
			buttom_offset_x = (j%2)*(320-SCENE_BUTTON_X)+15+(i/4+1)*320;
			buttom_offset_y = (j/2)*(310/2);
			tool_button->setGeometry(QRect(buttom_offset_x, buttom_offset_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
			tool_button->setLocalPos(buttom_offset_x,buttom_offset_y);
		}
		else if(j%2==1)
		{
			buttom_offset_x =(j%2)*(320-SCENE_BUTTON_X)-30 +(i/4+1)*320;
			buttom_offset_y = (j/2)*(310/2);
			tool_button->setGeometry(QRect(buttom_offset_x, buttom_offset_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
			tool_button->setLocalPos(buttom_offset_x,buttom_offset_y);
		}
		tool_button->page_id = i/4;
		tool_button->item_id = j;
		if (i_curtain_num == 1)
		{
		    tool_button->setGeometry(QRect(95+OLED_PIX_X, 95+2, 130, 145));//130
			tool_button->setLocalPos(95+OLED_PIX_X, 95+2);
		}


	}

	wid_get[0]->show();
	wid_get[0]->raise();
	startx = -OLED_PIX_X;
	starty = 0;
	wid_get[0]->move(startx, starty);

	wid_get2->setFixedSize(320,120);	

	label_curtain = new QLabel(wid_get2);
	label_curtain->setGeometry(QRect(142, 0, 320, 35));

	QPixmap pixmap_light("curtainWidget/curtain_0");
	label_curtain->setPixmap(pixmap_light);

	int L_len = 10;
	int R_len = 10;
	int C_x =10;
	int value = 0;

	if(value>=0 || value <=100)
	{
		L_len = 25+270*value/100;
		R_len = 320-L_len;
		C_x  = L_len-25;
	}
	
	ProgressBar_L = new QLabel(wid_get2);
	ProgressBar_L->setGeometry(QRect(0, 60, L_len, 44));
	ProgressBar_L->setStyleSheet("QWidget{background-color:rgb(123, 226, 80);"\
			"border-top-left-radius:20px;border-bottom-left-radius:20px;}");

	ProgressBar_R = new QLabel(wid_get2);
	ProgressBar_R->setGeometry(QRect(L_len, 60, R_len, 44));
	ProgressBar_R->setStyleSheet("QWidget{background-color:rgb(150, 150, 150);"\
			"border-top-right-radius:20px;border-bottom-right-radius:20px;}");

	ProgressBar_C = new QLabel(wid_get2);
	ProgressBar_C->setGeometry(QRect(C_x, 57, 50, 50));
	ProgressBar_C->setStyleSheet("QWidget{background-color:white;"\
			"border-radius:23px;}");
	ProgressBar_C->raise();

	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnIcon(QString)));

	wid_get2->hide();
	wid_get[0]->raise();
	current_Widget = wid_get[0];
	last_index = -1;
	current_page = 0;
	next_page = -1;

	this->translateLanguage();
}

CurtainWidget::~CurtainWidget()
{
    delete content;
	//delete grid_layout;
	//delete grid_layout1;
	//delete wid_get;
	//delete wid_get1;
	delete wid_get2;
	//delete bar;
	delete label_curtain;
	for(int i = 0; i < button_list.count(); i++)
	{
		SceneButton *tool_button = button_list.at(i);
	    delete tool_button;
	}
}

void CurtainWidget::setLightValue(int value)
{
    //qDebug()<<"bar->value()="<<bar->value();
  //  set_pos = bar->value();
	ast_dev[current_index].current_pos = set_pos;
	
	if (set_pos == 100)
	{
		QPixmap pixmap_light("curtainWidget/curtain_100");
		label_curtain->setPixmap(pixmap_light);
	}
	else
	{
	    QPixmap pixmap_light("curtainWidget/curtain_0");
		label_curtain->setPixmap(pixmap_light);
	}
    update();
	if (timer_shade_out->isActive())
	{
		timer_shade_out->start();
	}
}

enno_result CurtainWidget::set_dev_update(char *buf_in, uint32 len_buf)
{
    enno_result ret;
	uint32 len;
	int flagCidValid = 0;
	CTL_DEVICE_CONF *pst_msg = (CTL_DEVICE_CONF *)buf_in;
	QStringList string_list_c;
	QStringList string_list_w;
	
	string_list_c<<"curtainWidget/curtain_off"<<"curtainWidget/curtain_on"
		<<"curtainWidget/curtain_dim"<<"curtainWidget/night_off"
		<<"curtainWidget/dinner_off";
	
	string_list_w<<"curtainWidget/window_off"<<"curtainWidget/window_on";

    QStringList string_list;
	ENNO_CHECK_NULL_RET(buf_in);


    for (int i = 0; i < i_curtain_num; i++)
    {
		SceneButton *tool_button = button_list.at(i);
		//qDebug()<<"device_id="<<ast_dev[i].device_id<<"arm_dev_id="<<pst_msg->device_id;
		//qDebug()<<"cid=["<<pst_msg->cid<<"],value=["<<pst_msg->value<<"]";
		
        if (ast_dev[i].device_id == pst_msg->device_id)
        {
            if(ast_dev[i].device_type == DEV_KNX_WINDOW)
            {
                string_list = string_list_w;
            }
			else if(ast_dev[i].device_type == DEV_RS485_SHADES)
			{
			    string_list = string_list_c;
			}
            flagCidValid = 1;
            switch(pst_msg->cid)
            {
            case CHAR_SHADE_ON:
				//qDebug()<<"button_state[i]"<<button_state[i]<<", pstvalue="<<pst_msg->value;
//				ENNO_LOG_ERR("model=%d, addr=%d\n", ast_dev[i].factory_model, (ast_dev[i].addr>>24)&0xff);
				if (button_state[i] == pst_msg->value)
				{
				    if (ast_dev[i].factory_model == (RS485_SHADES_VELUX_KLE100 - RS485_SHADES_DOOYA_ALL)
							&& (((ast_dev[i].addr>>24)&0xff) == 0))//
				    {
				        if (button_state[i])
						{
							tool_button->setImg(string_list.at(0));
						}
						else
						{
							tool_button->setImg(string_list.at(1));
						}
				    }
					else
					{
					    ENNO_LOG_ERR("\n");
						if (button_state[i])
						{
							tool_button->setImg(string_list.at(1));
						}
						else
						{
							tool_button->setImg(string_list.at(0));
						}
					}
				    return ENNO_OK;
				}
				
				//qDebug()<<"2button_state[i]"<<button_state[i]<<", pstvalue="<<pst_msg->value;
//				ENNO_LOG_ERR("model=%d, addr=%d\n", ast_dev[i].factory_model, (ast_dev[i].addr>>24)&0xff);
				button_state[i] = pst_msg->value;
				if (ast_dev[i].factory_model == (RS485_SHADES_VELUX_KLE100 - RS485_SHADES_DOOYA_ALL)
							&& (((ast_dev[i].addr>>24)&0xff) == 0))//
				{
				
				    button_state[i] = !pst_msg->value;
				}
				
				if (button_state[i])
				{
					QPixmap pixmap(string_list.at(1));
					
				    //qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", tool_button setImg"<<string_list.at(1);  
					tool_button->setImg(string_list.at(1));
				    tool_button->setIcon(pixmap);
				    tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
				    tool_button->update();
				}
				else
				{
				    //qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
					QPixmap pixmap(string_list.at(0));
					
				    //qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", tool_button setImg"<<string_list.at(0);  
					tool_button->setImg(string_list.at(0));
				    tool_button->setIcon(pixmap);
				    tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
				    tool_button->update();
				}
				break;

			case CHAR_SHADE_TARGET_POSITION:
//				ENNO_LOG_ERR("model=%d, addr=%d\n", ast_dev[i].factory_model, (ast_dev[i].addr>>24)&0xff);
				if ((ast_dev[i].factory_model == (RS485_SHADES_VELUX_KLE100 - RS485_SHADES_DOOYA_ALL)) 
					&& (((ast_dev[i].addr>>24)&0xff) == 0))//
				{
					button_state[i] = (pst_msg->value==100) ? 0 : 1;
					
//				    ENNO_LOG_ERR("\n");
				}
				else
				{
				    button_state[i] = pst_msg->value ? 1 : 0;
					
//				    ENNO_LOG_ERR("\n");
				}
				
//				ENNO_LOG_ERR("button_state[i]=%d, pst_msg->value=%d,\n", button_state[i], pst_msg->value);
				ast_dev[i].target_pos = pst_msg->value;
				set_pos = ast_dev[i].target_pos;
				
			    if(set_pos>100)
			        set_pos = 100;
				else if(set_pos<0)
			        set_pos = 0;
				//qDebug()<<"set curtain pos="<<set_pos;
				set_progressbar_value(set_pos);
				ast_dev[i].current_pos = set_pos;
				update();
				
				if (set_pos == 100)
				{
					QPixmap pixmap_light("curtainWidget/curtain_100");
					label_curtain->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("curtainWidget/curtain_0");
					label_curtain->setPixmap(pixmap_light);
				}
				if (button_state[i])
				{
				    //qDebug()<<"set button on";
					QPixmap pixmap(string_list.at(1));
					
//				    qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", tool_button setImg"<<string_list.at(1);  
					tool_button->setImg(string_list.at(1));
				    tool_button->setIcon(pixmap);
				    tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
				    tool_button->update();
				}
				else
				{
					QPixmap pixmap(string_list.at(0));
					
//				    qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", tool_button setImg"<<string_list.at(0);  
					tool_button->setImg(string_list.at(0));
				    tool_button->setIcon(pixmap);
				    tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
				    tool_button->update();
				}
				break;

			case CHAR_SHADE_CURRENT_POSITION:
				ast_dev[i].current_pos = pst_msg->value;
				break;

			default:
				qDebug()<<"unknow cid="<<pst_msg->cid;
				flagCidValid = 0;
				break;
            }
			if( flagCidValid  
			&&  g_statusXmlHandle
		    &&  !g_statusXmlHandle->getReadOut())
		       {
		           g_statusXmlHandle->updateDevInfo(DEV_TYPE_CURTAIN,pst_msg->device_id,pst_msg->cid,pst_msg->value);
		       }
            return ENNO_OK;
        }
    }
	
    return ENNO_OK;
}
void CurtainWidget::shade_turn_onoff()
{
	label_scene_onoff[current_page]->hide();
	label_scene_name[current_page]->hide();
}
void CurtainWidget::shade_main_back()
{
	QStringList string_list;
	QStringList string_list_w;
	
	string_list<<"curtainWidget/curtain_off"<<"curtainWidget/curtain_on"
		<<"curtainWidget/curtain_dim"<<"curtainWidget/night_off"
		<<"curtainWidget/dinner_off";

	string_list_w<<"curtainWidget/window_off"<<"curtainWidget/window_on";
	
    //do_scene_flag = 0;
    timer_shade_out->stop();
  //  qDebug()<<"wait_for_status=0, stop"<<endl;
	wid_get2->hide();

  	for(int i=0; i<button_list.count(); i++)
	{
	    SceneButton *tool_button = button_list.at(i);
		QImage img;
		
		//qDebug()<<"button state="<<button_state[i];
		if (button_state[i])
		{
			if(ast_dev[i].device_type == DEV_KNX_WINDOW)
			{
			    img.load(string_list_w.at(1));
			}
			else if(ast_dev[i].device_type == DEV_RS485_SHADES)
			{
			    img.load(string_list.at(1));
			}
			//qDebug()<<"load string_list2";
		}
		else
		{
		
		//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";	
			img.load(string_list.at(0));
			if(ast_dev[i].device_type == DEV_KNX_WINDOW)
			{
			    img.load(string_list_w.at(0));
				///qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
			}
			else if(ast_dev[i].device_type == DEV_RS485_SHADES)
			{
			
			//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";	
			    img.load(string_list.at(0));
			}
			//qDebug()<<"load string_list";
		}
		//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
		
		//QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		
	//模糊值，值越大越模糊
	   // effect->setBlurRadius(0);
			
	    //tool_button->setGraphicsEffect(effect);
	    
		//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", setIcon";	
		tool_button->setIcon(QPixmap::fromImage(img));
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		//tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		
		tool_button->setGraphicsEffect(NULL);
		tool_button->setStyleSheet("color:rgb(255,255,255);");
		tool_button->raise();
//		g_blur_cnt = 1;
//		g_blur_dir = 1;
	}
	
	label_scene_onoff[current_page]->hide();
	label_scene_name[current_page]->hide();
    content->show();
	content->raise();
	g_do_scene_flag = 0;
   
	//delete new_image;
}

void CurtainWidget::turnIcon(QString current_icon)
{
	bool ok;  
	int delay_time = g_click_timer.elapsed();
	int current_index = current_icon.toInt(&ok, 10);
	QStringList string_list;
	QStringList string_list_w;
	QString str_name; 
	
	string_list<<"curtainWidget/curtain_off"<<"curtainWidget/curtain_on"
		<<"curtainWidget/curtain_dim"<<"curtainWidget/night_off"
		<<"curtainWidget/dinner_off";

	string_list_w<<"curtainWidget/window_off"<<"curtainWidget/window_on";
	
	//qDebug()<<"clicktime=["<<delay_time<<"]"<<endl;
	if (g_do_scene_flag)
	{
	    return;
	}
	
	if (shade_dim_press == 2)
	{
	    return;
	}
	else if(shade_dim_press)
	{
	    shade_dim_press = 0;
	}
	else
	{
	    return;
	}

	if (button_select->mouse_is_move)
    {
        return;
    }

	SceneButton *tool_button = button_list.at(current_index);
	if (ast_dev[current_index].factory_model == (RS485_SHADES_VELUX_KLE100 - RS485_SHADES_DOOYA_ALL)
							&& (((ast_dev[current_index].addr>>24)&0xff) == 0))
	{
	    button_state[current_index] = button_state[current_index];
	}
	else
	{
	    button_state[current_index] = !button_state[current_index];
	}
	//
	//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", button_state non";	
	button_select = tool_button;
	#if 1
    if (button_state[current_index])
    {
        //tool_button->setChecked(false);
	    // QPixmap pixmap(string_list2.at(i));
	   /* QPixmap pixmap(string_list.at(1));
		tool_button->setImg(string_list.at(1));
	    tool_button->setIcon(pixmap);
	    tool_button->setIconSize(pixmap.size());
	    tool_button->update();*/
		if (ast_dev[current_index].factory_model == (RS485_SHADES_VELUX_KLE100 - RS485_SHADES_DOOYA_ALL)
			&& (((ast_dev[current_index].addr>>24)&0xff) == 0))
		{
			ast_dev[current_index].target_pos = 100;
		}
		else
		{
		    if (0 == ast_dev[current_index].target_pos)
	        {
	            ast_dev[current_index].target_pos = 100;
	        }
		}
			
		target_pos = ast_dev[current_index].target_pos;
		tool_button->devCtrl(target_pos);
		ast_dev[current_index].current_pos = target_pos;
    }
	else //if(delay_time < 1000)
	{
	    //tool_button->setChecked(true);
	   /* QPixmap pixmap(string_list.at(0));
		tool_button->setImg(string_list.at(0));
	    tool_button->setIcon(pixmap);
	    tool_button->setIconSize(pixmap.size());
	    tool_button->update();*/
					
		target_pos = 0;
	    tool_button->devCtrl(target_pos);
		ast_dev[current_index].current_pos = target_pos;
	}
	#endif

	if (delay_time < 1000)
	{
	    for (int i=0; i<button_list.count(); i++)
	    	{
		if(current_index == i)
		{
		    button_select = tool_button;
			bool b = ast_dev[i].m_name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));  
			if (b)
			{
				if (ast_dev[i].m_name.size() <= 6)
				{
					str_name =  ast_dev[i].m_name;
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
				}
				/*else if (ast_dev[i].m_name.size() > MIN_CHINESE_NAME_SIZE 
					     && ast_dev[i].m_name.size() <= MID_CHINESE_NAME_SIZE)
				{
					str_name = ast_dev[i].m_name;
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
				}*/
				else
				{
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					str_name = ast_dev[i].m_name.left(6) + QString("...");
				}
			}
			else
			{
				if (ast_dev[i].m_name.size() <= 11)
				{
					str_name = ast_dev[i].m_name;
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
				}
				/*else if (ast_dev[i].m_name.size() <= 8)
				{
					str_name = ast_dev[i].m_name;
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
				}*/
				else
				{
					str_name = ast_dev[i].m_name.left(11) + QString("...");
					label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
				}
			}
			label_scene_name[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
			label_scene_name[current_page]->setText(str_name);
			if (button_state[i])
			{
				label_scene_onoff[current_page]->setStyleSheet("font-size:44px;font-weight:bold;color:rgb(0, 255, 0);text-align:left;background:transparent;");
				label_scene_onoff[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
				label_scene_onoff[current_page]->setText(tr(" Opened "));
			}
			else
			{
				label_scene_onoff[current_page]->setStyleSheet("font-size:44px;font-weight:bold;color:rgb(255,255, 255);text-align:left;background:transparent;");
				label_scene_onoff[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
				label_scene_onoff[current_page]->setText(tr(" Closed "));
			}
			/*if (button_state[i] == 1)
			{
				if (flag)
				{  
					continue;
				}
			}*/
			
			if (!button_state[i])
			{
				QImage img;
				if(ast_dev[i].device_type == DEV_KNX_WINDOW)
				{
				    img.load(string_list_w.at(0));
					//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
				}
				else if(ast_dev[i].device_type == DEV_RS485_SHADES)
				{
				
				//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";	
				    img.load(string_list.at(0));
				}
				
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
				tool_button->setIcon(QPixmap::fromImage(img));
				tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
				tool_button->update();

			}
			else 
			{
				//QPixmap pixmap(string_list.at(1));
		#if 1
				QImage img;
		        if(ast_dev[i].device_type == DEV_KNX_WINDOW)
				{
				    img.load(string_list_w.at(1));
				}
				else if(ast_dev[i].device_type == DEV_RS485_SHADES)
				{
				    img.load(string_list.at(1));
				}
				
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
				tool_button->setIcon(QPixmap::fromImage(img));
				tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
				tool_button->update();

		#endif
			}
		}
		
		//last_state[i] = button_state[i];
	    }
		//group_shade_anima = new QParallelAnimationGroup;
		group_shade_anima->clear();
	    for(int i=0; i<button_list.count(); i++)
		{
		    if (current_page != i/4)
		    {
		        continue;
		    }
		    SceneButton *tool_button = button_list.at(i);
			QImage img;
			//qDebug()<<"button="<<i<<",state="<<button_state[i]<<",load 1";
			if (button_state[i])
			{
			    //qDebug()<<"button="<<i<<",state="<<button_state[i]<<",load list2";
			    if(ast_dev[i].device_type == DEV_KNX_WINDOW)
				{
				    img.load(string_list_w.at(1));
				}
				else if(ast_dev[i].device_type == DEV_RS485_SHADES)
				{
				    img.load(string_list.at(1));
				}
			}
			else
			{
			    //qDebug()<<"button="<<i<<",state="<<button_state[i]<<",load list0";
			    if(ast_dev[i].device_type == DEV_KNX_WINDOW)
				{
				    //qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
				    img.load(string_list_w.at(0));
					//qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
				}
				else if(ast_dev[i].device_type == DEV_RS485_SHADES)
				{
				   // qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";  
				    img.load(string_list.at(0));
				}				
			}

		    //模糊值，值越大越模糊
		    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		    tool_button->setGraphicsEffect(effect);
			tool_button->setIcon(QPixmap::fromImage(img));
			tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
			tool_button->setStyleSheet("color:rgb(0,0,0);");
			

			QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"blurRadius");
			animation_bar->setDuration(SCENE_DO_TIME);
			animation_bar->setKeyValueAt(0, 0);
			animation_bar->setKeyValueAt(0.1, 25);
			animation_bar->setKeyValueAt(0.2, 40);
			animation_bar->setKeyValueAt(0.3, 50);
			animation_bar->setKeyValueAt(0.7, 50);
			animation_bar->setKeyValueAt(0.8, 40);
			animation_bar->setKeyValueAt(0.9, 25);
			animation_bar->setKeyValueAt(1, 0);
			animation_bar->setEasingCurve(QEasingCurve::Linear);
			
		    group_shade_anima->addAnimation(animation_bar);
		}
		
		g_do_scene_flag = 1;
		
		//Util::UI_motor_shake(1);
        QTimer::singleShot(SCENE_DO_TIME-10, this, SLOT(shade_turn_onoff()));
        QTimer::singleShot(SCENE_DO_TIME, this, SLOT(shade_main_back()));
		
		label_scene_onoff[current_page]->raise();
		label_scene_onoff[current_page]->show();
		label_scene_name[current_page]->raise();
		label_scene_name[current_page]->show();
#if 1
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
		QPropertyAnimation *animation_label = new QPropertyAnimation(effect,"opacity");
		label_scene_name[current_page]->setGraphicsEffect(effect);
		animation_label->setDuration(SCENE_DO_TIME);
		animation_label->setKeyValueAt(0, 0.2);
		//animation_label->setKeyValueAt(0.05, 0.5);
		animation_label->setKeyValueAt(0.3, 1);
		animation_label->setKeyValueAt(0.7, 1);
		animation_label->setKeyValueAt(0.95, 0.5);
		animation_label->setKeyValueAt(1, 0);
		animation_label->setEasingCurve(QEasingCurve::Linear);

		QGraphicsOpacityEffect *effect1 = new QGraphicsOpacityEffect(this);
		QPropertyAnimation *animation_label1 = new QPropertyAnimation(effect1,"opacity");
		label_scene_onoff[current_page]->setGraphicsEffect(effect1);
		animation_label1->setDuration(SCENE_DO_TIME);
		animation_label1->setKeyValueAt(0, 0.2);
		//animation_label1->setKeyValueAt(0.05, 0.5);
		animation_label1->setKeyValueAt(0.3, 1);
		animation_label1->setKeyValueAt(0.7, 1);
		animation_label1->setKeyValueAt(0.95, 0.5);
		animation_label1->setKeyValueAt(1, 0);
		animation_label1->setEasingCurve(QEasingCurve::Linear);
		
		group_shade_anima->addAnimation(animation_label);
		group_shade_anima->addAnimation(animation_label1);
#endif	    
		group_shade_anima->start();
#if 0
		QWidget *w = content;
		QPixmap pixmap(w->size());
	
		w->render(&pixmap);
		QImage img = pixmap.toImage();
		new_image = new QImage(img);
#endif
		//qDebug()<<"timer_exe start";
		//timer_exe->start();	
	    	
	}
	cur_button = string_list.at(2);
		
	if (delay_time > 1000)
	{
	    //wid_get2->raise();
		prev_Widget = current_Widget;
		current_Widget = wid_get2;
		//light_on_paint();
		//current_Widget->raise();
		content->hide();
		wid_get2->move(0, 200);
		cur_curtain = current_index;
		current_pos = ast_dev[cur_curtain].current_pos;
		set_pos = current_pos;
		//bar->setValue(current_pos);
		  if(current_pos>100)
			  current_pos =100;
		  else if(current_pos<0)
			  current_pos = 0;

		   set_progressbar_value(current_pos);

		if (current_pos == 100)
		{
			QPixmap pixmap_light("curtainWidget/curtain_100");
			label_curtain->setPixmap(pixmap_light);
		}
		else
		{
			QPixmap pixmap_light("curtainWidget/curtain_0");
			label_curtain->setPixmap(pixmap_light);
		}
		
		wid_get2->show();
		wid_get2->raise();
		right_mouse_press = 1;
	   // printf("curtain button press\n");
		update();
	}
	//emit turnPage(current_index);
}

/*void ContentWidget::paintEvent(QPaintEvent *e)
{
    printf("paintEvent\n");
}
*/

void CurtainWidget::switch_to_page(int new_page, QImage *new_image)
{
    if (new_page == 2)
    {
	    switch_new_image = new_image;
        forward();
    }
	else if (new_page == 0)
    {
	    switch_new_image = new_image;
        next();
    }
}

void CurtainWidget::snap_shot(QImage **w_image)
{
	QWidget *w = content;
	QPixmap pixmap(w->size());

	w->render(&pixmap);
	QImage img = pixmap.toImage();
	*w_image = new QImage(img);

	//w_image->save("w_image.jpg","JPG",100);
	if (current_Widget == wid_get2)
	{
		wid_get2->move(startx, starty);
		wid_get2->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;

		content->show();
		g_no_shake = 0;
		shade_dim_press = 0;
		right_mouse_press = 2;
		QTimer::singleShot(1, this, SLOT(lightdim_to_shade()));
	}
}

void CurtainWidget::lightdim_to_shade()
{

	int pos_x, pos_y;
	QStringList string_list_c;
	QStringList string_list_w;
	QString str_name;

	string_list_c<<"curtainWidget/curtain_off"<<"curtainWidget/curtain_on"
		<<"curtainWidget/curtain_dim"<<"curtainWidget/night_off"
		<<"curtainWidget/dinner_off";

    
	string_list_w<<"curtainWidget/window_off"<<"curtainWidget/window_on";

	QStringList string_list;

	if(ast_dev[current_index].device_type == DEV_KNX_WINDOW)
	{
	    string_list = string_list_w;
	}
	else if(ast_dev[current_index].device_type == DEV_RS485_SHADES)
	{
	    string_list = string_list_c;
	}

	QImage img;
	if (!button_state[current_index])
	{
		img.load(string_list.at(0));
		qDebug()<<"func="<<__func__<<", line="<<__LINE__<<", curtain off";   
	}
	else
	{
		img.load(string_list.at(1));
	}

	//*img = img->scaled(115,115,Qt::KeepAspectRatio);
	button_select->setIcon(QPixmap::fromImage(img));
	button_select->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
	
	button_select->setGraphicsEffect(NULL);
	button_select->getLocalPos(&pos_x, &pos_y);
	button_select->setGeometry(QRect(pos_x, pos_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	button_select->update();
	content->show();
	//light_flag = 0;
	g_no_shake = 0;
	shade_dim_press = 0;
	right_mouse_press = 0;

	update();
}

void CurtainWidget::lightdim_to_shade_start()
{
    int pos_x;
	int pos_y;
	right_mouse_press = 2;
	//group_light_anima = new QParallelAnimationGroup;
	
	group_light_anima->clear();
	QPropertyAnimation *anim = new QPropertyAnimation(button_list.at(current_index), "geometry");				
	anim->setDuration(200);
	button_list.at(current_index)->getLocalPos(&pos_x, &pos_y);
	
	//qDebug()<<"lightdim_to_scene_start,pos_x="<<pos_x<<",pos_y="<<pos_y;
	anim->setKeyValueAt(0, QRect(80+OLED_PIX_X*(current_page+1), 80, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	anim->setKeyValueAt(1, QRect(pos_x, pos_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	//anim->setKeyValueAt(1, QRect(last_x, last_y, SCENE_BUTTON_X, SCENE_BUTTON_Y));
	
	group_light_anima->addAnimation(anim);

	QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
	button_list.at(current_index)->setGraphicsEffect(effect);
	QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"blurRadius");

	animation_bar->setDuration(200);
	animation_bar->setKeyValueAt(0, 30);
	animation_bar->setKeyValueAt(1, 0);
	//animation_bar->setKeyValueAt(1, 0);
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	group_light_anima->addAnimation(animation_bar);
   //group_light_anima->start();
	group_light_anima->start();
	
	QTimer::singleShot(200, this, SLOT(lightdim_to_shade()));

}

void CurtainWidget::mousePressEvent(QMouseEvent *event)
{
	if (g_do_scene_flag)
	{
		return;
	}
	
	if (!i_curtain_num)
    {
	    return;
    }

   // printf("CurtainWidget::mousePressEvent,x=%d, y=%d\n", event->pos().x(), event->pos().y());
    #if 1
	if (right_mouse_press)
	{
	    if (event->pos().y() < 160)
	    {
	        if (current_Widget == wid_get2)
			{
			/*
			    wid_get2->move(startx, starty);
			   // wid_get->move(startx, starty);
		       // wid_get1->move(startx, starty);
				wid_get2->hide();
				prev_Widget->raise();
				current_Widget = prev_Widget;
				QImage *img = new QImage;
			    img->load(button_select->m_img);
				//*img = img->scaled(115,115,Qt::KeepAspectRatio);
			    button_select->setIcon(QPixmap::fromImage(*img));
			    button_select->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
			    button_select->update();
				content->show();
				//light_flag = 0;
				g_no_shake = 0;
				shade_dim_press = 0;
				right_mouse_press = 0;
*/
				wid_get2->move(startx, starty);
				// wid_get->move(startx, starty);
				// wid_get1->move(startx, starty);
				wid_get2->hide();
				prev_Widget->raise();
				current_Widget = prev_Widget;

				content->show();
				g_no_shake = 0;
				shade_dim_press = 0;
				right_mouse_press = 2;
				
				lightdim_to_shade_start();
				
				return;
			}
	    }
		else
		{
		    m_mouseSrcPos = event->pos();
			m_mouseDstPos = m_mouseSrcPos;
		   // printf("src(%d,%d), dst(%d,%d)\n", 
			//    m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y());
			   if(event->pos().y() >=255 &&  event->pos().y()<=310)
			   {
				   if(m_mouseSrcPos.x()<=25)
					   set_pos = 0;
				   else if(m_mouseSrcPos.x()>=295)
					   set_pos = 100;
				   else
				   {
					   set_pos = (m_mouseSrcPos.x()-25)*100/270;
				   }

				  if(set_pos>100)
					  set_pos =100;
				  else if(set_pos<0)
					  set_pos = 0;

				   set_progressbar_value(set_pos);
					if (set_pos == 100)
					{
						QPixmap pixmap_light("curtainWidget/curtain_100");
						label_curtain->setPixmap(pixmap_light);
					}
					else
					{
					    QPixmap pixmap_light("curtainWidget/curtain_0");
						label_curtain->setPixmap(pixmap_light);
					}
				   update();
			   }

		}
			
	    return;
	}
	#endif
	if(event->button() == Qt::LeftButton) 
	{
	    left_mouse_press = 1;
		m_mouseSrcPos = event->pos();
		m_mouseDstPos = m_mouseSrcPos;
		//printf("src(%d,%d), dst(%d,%d)\n", 
		//	m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y());

        //scene move disable, when i_scene_num beyond 4
	}
	/*else if(event->button() == Qt::RightButton)
	{
	    left_mouse_press = 0;
	    prev_Widget = current_Widget;
		current_Widget = wid_get2;
		//light_on_paint();
		//current_Widget->raise();
		content->hide();
		right_mouse_press = 1;
	    printf("light button press\n");
		update();
	}
	printf("wid_get=%p\n", wid_get);
	*/
	//wid_get->raise();
	
}

void CurtainWidget::curtaion_move_end()
{
	//	printf("current_uid=%d\r\n",current_uid);
		wid_get[0]->move(startx-current_page*OLED_PIX_X, starty);
}


#define WIDGET_MOVE_TIME  150
#define WIDGET_MOVE_OFFSET 100
void CurtainWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (g_do_scene_flag)
	{
		return;
	}
	if (!i_curtain_num)
    {
	    return;
    }

	if (current_Widget == wid_get2 && right_mouse_press)
	{  
#if 0
	    wid_get2->move(startx, starty);
	    wid_get->move(startx, starty);
        wid_get1->move(startx, starty);
		wid_get2->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;
		right_mouse_press = 0;
		light_brightness = 0;
		
		content->show();	
#endif
        if (set_pos > 100)
        {
            set_pos = 100;
        }
		else if (set_pos < 0)
		{
		    set_pos = 0;
		}
        current_pos = set_pos;
		//qDebug()<<"cur_curtain="<<cur_curtain;
        ast_dev[cur_curtain].current_pos = set_pos;
		ast_dev[cur_curtain].target_pos = set_pos;
		button_list.at(cur_curtain)->devCtrl(set_pos);
		
		timer_shade_out->start();
	}

	if (current_page == next_page || left_mouse_press != 1)
	{
		QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(curtaion_move_end()));
	    return;
	}
	left_mouse_press = 0;
	m_mouseDstPos = event->pos();
		
#ifdef SLIDER_VERTICAL
#else
	int offset = 0;
	int move_time=0;
    if (current_page < next_page)//->left
	{
    	if (next_page == page_num)
    	{
    		offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
			if( offset > WIDGET_MOVE_OFFSET+50)
			{
				offset = WIDGET_MOVE_OFFSET+50;
			}
			QPropertyAnimation *anim_r = new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_rb = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_r->setDuration(WIDGET_MOVE_TIME);
			anim_r->setKeyValueAt(0, QRect(startx - offset -current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx-current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);
			//group_move_widget->addAnimation(anim_rb);

			group_move_widget->start();
			QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(curtaion_move_end()));
    		return;
    	}
        //qDebug()<<"line="<<__LINE__;
	    if ((m_mouseDstPos.x() <  m_mouseSrcPos.x()) 
			&& (m_mouseSrcPos.x() - m_mouseDstPos.x() > 100))
	    {
		 	 offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_ln = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_l->setDuration(move_time);
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-OLED_PIX_X-current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);
			//group_move_widget->addAnimation(anim_ln);
			group_move_widget->start();
			current_page = next_page;
	    }
		else
		{
		 	 offset = m_mouseSrcPos.x() -m_mouseDstPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_ln = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_l->setDuration(move_time);
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);
			//group_move_widget->addAnimation(anim_ln);

			group_move_widget->start();
		}
	    QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(curtaion_move_end()));
	}
	else if (current_page > next_page)//->right
	{
    	if (next_page == -1)
    	{
    		offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
			if( offset > WIDGET_MOVE_OFFSET+50)
			{
				offset = WIDGET_MOVE_OFFSET+50;
			}
			QPropertyAnimation *anim_l = new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_lb = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_l->setDuration(WIDGET_MOVE_TIME);
			anim_l->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx - current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);
			//group_move_widget->addAnimation(anim_lb);

			group_move_widget->start();
			QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(curtaion_move_end()));
    		return;
    	}
	    if ((m_mouseDstPos.x() >  m_mouseSrcPos.x()) 
			&& (m_mouseDstPos.x() - m_mouseSrcPos.x() > 100))
	    {
		 	 offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_r= new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_rn = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_r->setDuration(move_time);
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx+OLED_PIX_X - current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);
			//group_move_widget->addAnimation(anim_rn);

			group_move_widget->start();
			current_page = next_page;
	    }
		else
		{
		 	 offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_r= new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_rn = new QPropertyAnimation(wid_get[next_page], "geometry");
			//group_move_widget = new QParallelAnimationGroup;
			group_move_widget->clear();

			anim_r->setDuration(move_time);
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx - current_page*OLED_PIX_X, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);
			//group_move_widget->addAnimation(anim_rn);

			group_move_widget->start();
		}
	    QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(curtaion_move_end()));
	}
	update();
#endif
}
void CurtainWidget::shade_to_lightdim()
{
	content->hide();
	wid_get2->move(0, 200);
	wid_get2->show();
	wid_get2->raise();
	cur_curtain = current_index;
	current_pos = ast_dev[cur_curtain].current_pos;
	set_pos = current_pos;
	//bar->setValue(current_pos);
	set_progressbar_value(set_pos);
	if (set_pos == 100)
	{
		QPixmap pixmap_light("curtainWidget/curtain_100");
		label_curtain->setPixmap(pixmap_light);
	}
	else
	{
	    QPixmap pixmap_light("curtainWidget/curtain_0");
		label_curtain->setPixmap(pixmap_light);
	}
	right_mouse_press = 1;
	timer_shade_out->start();
	
	update();
}

void CurtainWidget::shade_pos_check()
{
    int last_x;
	int last_y;
 
	if (shade_dim_press == 1)
	{
	   if (!button_select->mouse_is_move)
	   {
		   shade_dim_press = 2;
	   }
	   else
	   {
		   shade_dim_press = 0;
		   return;
	   }
	}
	else
	{
	   return;
	}

	prev_Widget = current_Widget;
	current_Widget = wid_get2;
	
    //qDebug()<<"shade_pos_check shade_dim_press=1"<<endl;
	set_pos = ast_dev[current_index].current_pos;
	
	last_pos = set_pos;
	//bar->setValue(set_pos);
	  if(set_pos>100)
		  set_pos =100;
	  else if(set_pos<0)
		  set_pos = 0;

	   set_progressbar_value(set_pos);
	if (set_pos == 100)
	{
		QPixmap pixmap_light("curtainWidget/curtain_100");
		label_curtain->setPixmap(pixmap_light);
	}
	else
	{
	    QPixmap pixmap_light("curtainWidget/curtain_0");
		label_curtain->setPixmap(pixmap_light);
	}
	#if 0
	content->hide();
	wid_get2->move(0, 200);
	wid_get2->show();
	wid_get2->raise();
	cur_curtain = current_index;
	current_pos = ast_dev[cur_curtain].current_pos;
	set_pos = current_pos;
	bar->setValue(current_pos);
	right_mouse_press = 1;
	timer_shade_out->start();
	update();
	#endif
	//group_light_anima = new QParallelAnimationGroup;
	
	group_light_anima->clear();
	//qDebug()<<"scene_to_lightdim scene";
	QPropertyAnimation *anim = new QPropertyAnimation(button_list.at(current_index), "geometry");				
	anim->setDuration(200);
	last_x = button_list.at(current_index)->pos().x();
	last_y = button_list.at(current_index)->pos().y();
	anim->setKeyValueAt(0, QRect(last_x, last_y, SCENE_BUTTON_X, SCENE_BUTTON_Y));
	anim->setKeyValueAt(1, QRect(80+OLED_PIX_X*(current_page+1), 80, SCENE_BUTTON_X, SCENE_BUTTON_Y));
	//anim->setKeyValueAt(1, QRect(last_x, last_y, SCENE_BUTTON_X, SCENE_BUTTON_Y));
	
	group_light_anima->addAnimation(anim);

	QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
	button_list.at(current_index)->setGraphicsEffect(effect);
	QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"blurRadius");

	animation_bar->setDuration(200);
	animation_bar->setKeyValueAt(0, 0);
	animation_bar->setKeyValueAt(1, 30);
	//animation_bar->setKeyValueAt(1, 0);
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	group_light_anima->addAnimation(animation_bar);
   //group_light_anima->start();
	group_light_anima->start();

	
#ifdef MOTOR_ENABLE
	Util::UI_motor_shake(2);
#endif
	QTimer::singleShot(200, this, SLOT(shade_to_lightdim()));
	
	last_index = current_index;  
}

void CurtainWidget::PressShade()
{

	bool ok;  
	current_index = g_current_index;
	QStringList string_list1;

	string_list1<<"curtainWidget/curtain_dim"<<"curtainWidget/curtain_dim";
	
	//qDebug()<<"2PressShade, current_icon="<<current_index;
	
#ifdef MOTOR_ENABLE
	Util::UI_motor_shake(1);
#endif

    QTimer::singleShot( 5*100, this, SLOT(shade_pos_check()));
	//current_index = g_current_index;

	cur_button = string_list1.at(1);
	button_select = button_list.at(current_index);

}

void CurtainWidget::set_progressbar_value(int value)
{
	int L_len = 0;
	int R_len = 0;
	int C_x =0;

	if(value>=0 || value <=100)
	{
		L_len = 25+270*value/100;
		R_len = 320-L_len;
		C_x  = L_len-25;
		ProgressBar_L->setGeometry(QRect(0, 60, L_len, 44));

		ProgressBar_R->setGeometry(QRect(L_len, 60, R_len, 44));

		ProgressBar_C->setGeometry(QRect(C_x, 57, 50, 50));
		//update();
	}

}

void CurtainWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (g_do_scene_flag)
	{
		return;
	}

    if (!i_curtain_num)
    {
        return;
    }

    //printf("ContentWidget::mouseMoveEvent,");
    if (right_mouse_press)
    {
        g_no_shake = 1;
		
		m_mouseDstPos = event->pos();
	//	return;
		#if 1
		if (m_mouseDstPos.x() > m_mouseSrcPos.x())
		{
		    if (set_pos < 100)
		    {
			    set_pos = ast_dev[current_index].current_pos + (m_mouseDstPos.x() - m_mouseSrcPos.x())/3;
			    if(m_mouseSrcPos.x()>300)//add 10 just for edge problem
			    {
			    	set_pos+= 10;
			    }
				if (set_pos > 100)
				{
				    set_pos = 100;
				}
				//bar->setValue(set_pos);
				set_progressbar_value(set_pos);
				if (set_pos == 100)
				{
					QPixmap pixmap_light("curtainWidget/curtain_100");
					label_curtain->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("curtainWidget/curtain_0");
					label_curtain->setPixmap(pixmap_light);
				}
		    }
		}
		else
		{
		    if (set_pos > 0)
		    {
			    set_pos = ast_dev[current_index].current_pos - (m_mouseSrcPos.x() - m_mouseDstPos.x())/3;
				if (set_pos < 0)
				{
				    set_pos = 0;
				}
				//bar->setValue(set_pos);
				set_progressbar_value(set_pos);
				if (set_pos == 100)
				{
					QPixmap pixmap_light("curtainWidget/curtain_100");
					label_curtain->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("curtainWidget/curtain_0");
					label_curtain->setPixmap(pixmap_light);
				}
		    }
		}

		  if(set_pos>100)
			  set_pos =100;
		  else if(set_pos<0)
			  set_pos = 0;
		set_progressbar_value(set_pos);
		if (set_pos == 100)
		{
			QPixmap pixmap_light("curtainWidget/curtain_100");
			label_curtain->setPixmap(pixmap_light);
		}
		else
		{
		    QPixmap pixmap_light("curtainWidget/curtain_0");
			label_curtain->setPixmap(pixmap_light);
		}
		//bar->setValue(set_pos);
	    //last_dim = set_pos;	
#endif
		//light_on_paint();
			
		update();

		if (timer_shade_out->isActive())
		{
			timer_shade_out->stop();
		}
        return;
		
        #if 0
		m_mouseDstPos = event->pos();
		if (m_mouseDstPos.x() > m_mouseSrcPos.x())
		{
		   if (set_pos < 100)
		    {
			    set_pos = current_pos + (m_mouseDstPos.x() - m_mouseSrcPos.x())/3;
				if (set_pos > 100)
				{
				    set_pos = 100;
				}
				qDebug()<<"dst.x="<<m_mouseDstPos.x()<<", src.x="<<m_mouseSrcPos.x()<<endl;
				qDebug()<<"set_pos="<<set_pos;
				bar->setValue(set_pos);
				if (set_pos == 100)
				{
					QPixmap pixmap_light("curtainWidget/curtain_100");
					label_curtain->setPixmap(pixmap_light);
				}
				else
				{
					QPixmap pixmap_light("curtainWidget/curtain_0");
					label_curtain->setPixmap(pixmap_light);
				}
		    }
		}
		else
		{
		    if (set_pos > 0)
		    {
			    set_pos = current_pos - (m_mouseSrcPos.x() - m_mouseDstPos.x())/3;
				if (set_pos < 0)
				{
				    set_pos = 0;
				}
				qDebug()<<"dst.x="<<m_mouseDstPos.x()<<", src.x="<<m_mouseSrcPos.x()<<endl;
				qDebug()<<"set_pos="<<set_pos;
				bar->setValue(set_pos);
				if (set_pos == 100)
				{
					QPixmap pixmap_light("curtainWidget/curtain_100");
					label_curtain->setPixmap(pixmap_light);
				}
				else
				{
					QPixmap pixmap_light("curtainWidget/curtain_0");
					label_curtain->setPixmap(pixmap_light);
				}
		    }
		}

		//light_on_paint();
		update();
		if (timer_shade_out->isActive())
		{
			timer_shade_out->stop();
		}
        return;
		#endif
    }
	if ( left_mouse_press != 1)
	{
	    return;
	}
	m_mouseDstPos = event->pos();
    //printf("src(%d,%d), dst(%d,%d), move(%d,%d)\n", 
	//       m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y(),
	//       m_mouseDstPos.x() - m_mouseSrcPos.x(), m_mouseDstPos.y() - m_mouseSrcPos.y()); 
#ifdef SLIDER_VERTICAL

#else

	float speed=0;
	int move_offset = 0;

	if (m_mouseDstPos.x() <  m_mouseSrcPos.x())
	{
	    //up
	    if (current_page + 1 != page_num)
	    {
	        next_page = current_page + 1;
	    }
		else
		{
			//return;
			next_page = page_num;
		}
	}
	else if (m_mouseDstPos.x() >  m_mouseSrcPos.x())
	{
	    if (current_page - 1 != -1)
	    {
	        next_page = current_page - 1;
	    }
		else
		{
		    next_page = -1;
		//	return;
		}
	}
	if (next_page == page_num  || next_page == -1)
	{
		if(m_mouseDstPos.x() <  m_mouseSrcPos.x())
		{
			if(m_mouseSrcPos.x()-m_mouseDstPos.x() <=WIDGET_MOVE_OFFSET)
			{
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x()- current_page*OLED_PIX_X , starty);
			}
			else if((m_mouseSrcPos.x()-m_mouseDstPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseSrcPos.x()-m_mouseDstPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseSrcPos.x()-m_mouseDstPos.x()))/(m_mouseSrcPos.x()-m_mouseDstPos.x());
				move_offset = 200-speed*(m_mouseSrcPos.x()-m_mouseDstPos.x());
				wid_get[0]->move(startx - move_offset-current_page*OLED_PIX_X, starty);
			}

		}
		else if(m_mouseDstPos.x() >  m_mouseSrcPos.x())
		{
			if(m_mouseDstPos.x() - m_mouseSrcPos.x() <=WIDGET_MOVE_OFFSET)
			{
			    wid_get[0]->move(startx + current_page*OLED_PIX_X + m_mouseDstPos.x() - m_mouseSrcPos.x(), starty);
			}
			else if((m_mouseDstPos.x() - m_mouseSrcPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseDstPos.x() - m_mouseSrcPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseDstPos.x() - m_mouseSrcPos.x()))/(m_mouseDstPos.x() - m_mouseSrcPos.x());
				move_offset = 200-speed*(m_mouseDstPos.x() - m_mouseSrcPos.x());
			    wid_get[0]->move(startx + move_offset-current_page*OLED_PIX_X, starty);
			}
		}

	}
	else
	{
		if ((current_page < next_page) && (m_mouseDstPos.x() <  m_mouseSrcPos.x()))//to left
		{
			wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*OLED_PIX_X, starty);
		}
		else if ((current_page > next_page) && (m_mouseDstPos.x() >  m_mouseSrcPos.x()))//to right
		{
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*OLED_PIX_X, starty);
		}
	}

#endif
}
void CurtainWidget::paintEvent(QPaintEvent *e)
{
    int i;
	if (!i_curtain_num)
    {
	    return;
    }
    //printf("CurtainWidget::paintEvent, right_mouse_press=%d\n", right_mouse_press);
	if (right_mouse_press == 1)
	{
		QImage image = QPixmap(cur_button).toImage();			
#if 1       
		image.save("text1.jpg", "JPG", 100);
		QImage image1 = QPixmap("text1.jpg").toImage();	

		QPainter paint(this);
        //qDebug()<<"cur_button=["<<cur_button<<"]"<<endl;
		QPainter painter(&image1);	
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
		//QRect target(0, 0, 261.0, 261.0);
		QRect source(0.0, 0.0, 261.0, 261.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域
		//QRect target(30.0, -30.0, 320.0, 320.0);
		//QRect source(0.0, 0.0, 320.0, 320.0);

		//将文字绘制在图片中心位置
		QString str;
		str.sprintf("%d%", set_pos);
		painter.drawText(image1.rect(), Qt::AlignCenter, str);
		//QRect imgrec(-90, 0, 400, 261);
		//painter.drawText(imgrec, Qt::AlignCenter, str);
		paint.drawImage(target,image1,source);
		//paint.drawPixmap(110,110,100,100,image1);
		//image.save("text.jpg", "JPG", 100);
#endif
	}
	else if (!right_mouse_press)
	{
		QPainter painter(this);             // 创建QPainter一个对象
	    QPen pen;
		int start_pos = 0;
		int current_point_x = 0;

		//page switch dot fresh
		if (page_num > 1)
		{
		    if (page_num <= 8)
		    {
		        start_pos = 160 - (page_num/2)*30;
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
		   current_point_x = start_pos+current_page*30.0;

		    for (int i=0; i<page_num; ++i)//current_page
		    {
		    	if (i ==  current_page)
				{
					painter.setPen(QPen(Qt::white, 6));//设置画笔形式
					painter.setBrush(Qt::white);
					painter.drawEllipse(current_point_x-17, 309, 6, 6);//画圆
					painter.drawRect(QRect(current_point_x-12, 309, 20, 6));
					painter.drawEllipse(current_point_x+5, 309, 6, 6);//画圆
				}
		    	else if (i<current_page)
		    	{
					painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
					painter.setBrush(Qt::gray);
		    		if (i==current_page-1)
		    		{
		    			painter.drawEllipse(current_point_x-44, 309, 6, 6);//画圆
		    		}
		    		else
		    		{
		    			painter.drawEllipse(current_point_x-(44+32*(current_page-i-1)), 309, 6, 6);//画圆
		    		}
		    	}
		    	else if (i>current_page)
				{
					painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
					painter.setBrush(Qt::gray);
					if (i==current_page+1)
					{
						painter.drawEllipse(current_point_x+32, 309, 6, 6);//画圆
					}
					else
					{
						painter.drawEllipse(current_point_x+(32+32*(i-current_page-1)), 309, 6, 6);//画圆
					}
				}
		    }
		}
	}
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
//            paintPrevious(paint, nextIndex);
//            paintNext(paint, currentIndex());
			paintPrevious(paint, switch_new_image);
			paintNext(paint, current_image);
            break;
        }
    }
   // printf("curtainWidget::paintEvent end\n");
}

/*void CurtainWidget::setDuration(int d)
{
    qDebug()<<"QA setDuration";

    duration = d;
}
*/
void CurtainWidget::setDuration(int d)
{
	duration = d;
}

void CurtainWidget::paintPrevious(QPainter &paint, QImage *img)
{
#if 1
    //qDebug()<<"QA paintPrevious";
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

void CurtainWidget::paintNext(QPainter &paint, QImage *img)
{
    //qDebug()<<"QA paintNext";
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

void CurtainWidget::valueChanged_slot(QVariant value)
{
    //qDebug()<<"QA valueChanged_slot";

    currentValue = value;
    update();
}

void CurtainWidget::animationFinished()
{
    //qDebug()<<"QA animationFinished";
    isAnimation = false;
   // widget(currentIndex())->show();
   // setCurrentIndex(nextIndex);
    delete current_image;
    delete switch_new_image;
    content->show();
}

void CurtainWidget::showContent()
{
    content->show();  
}


void CurtainWidget::next()
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

void CurtainWidget::forward()
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
    qDebug()<<"animation="<<animation;
    animation->setStartValue(0);
    animation->setEndValue(320);
    animation->setDuration(duration);
    animation->start();
}


