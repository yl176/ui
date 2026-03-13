#include "standby_widget.h"
#include "common.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include "status_xml.h"

char g_szTmp[50] = {0};  
static int h_r=159,h_g=217,h_b=65;
static int t_r=159,t_g=217,t_b=65;
bool g_humi_update_flag = false;
bool g_tvoc_update_flag = false;

//#define OPEN_DEV_SCROLL_H 1
void StandbyUpWidget::translateLanguage()
{
    update_sensor_value();
    //get_sensor_value((char *)&msg_bak, 20);
}

void StandbyUpWidget::update_sensor_value()
{
    QSettings *configIni = new QSettings("/home/user/ui/qwid/indoor_status.ini", QSettings::IniFormat); 
    InDoorValues stBmeValue;

	stBmeValue.hum = configIni->value("indoor/humidity").toInt();
	stBmeValue.temp = configIni->value("indoor/temp").toInt();
	stBmeValue.voc = configIni->value("indoor/voc").toInt();

    ENNO_LOG_ERR("temperature=%d\n", stBmeValue.temp);
    show_temp_value(stBmeValue.temp/100);

	humidity = stBmeValue.hum;
	g_humi_update_flag = true;

	gas = stBmeValue.voc;
	g_tvoc_update_flag = true;
}

void StandbyUpWidget::gettime()
{
	time_t tNow =time(NULL);  
	time_t tEnd = tNow + 1800;  
	//注意下面两行的区别  
	struct tm* ptm = localtime(&tNow);  

	strftime(g_szTmp, 50, "%H:%M", ptm);  
	//qDebug()<<"timenow="<<g_szTmp;
	label_time_hour->setText(g_szTmp);
}

void StandbyUpWidget::onTimerUpdate()
{
	//printf("timer update\n");
    gettime();
}

#define MAIN_WINDOW_W 170
#define MAIN_WINDOW_H 200
#define MAIN_WINDOW_X 60
#define MAIN_WINDOW_Y 50
#define MAIN_ADD_H 30
#define MAIN_FONT_H 150
#define MAIN_FONT_W 85

#define M_D_ABC_x 0
#define M_B_ABC_x MAIN_FONT_W
#define M_A_y -110
#define M_B_y MAIN_ADD_H
#define M_C_y MAIN_ADD_H+MAIN_FONT_H+10
#define M_D_x M_D_ABC_x+MAIN_WINDOW_W
#define M_D_y MAIN_ADD_H
#define M_E_x M_D_ABC_x-MAIN_FONT_W
#define M_E_y MAIN_ADD_H
#define M_FONT_MOVE_TIME 300

static bool g_mb_flg =  true;
static bool g_mdb_flg = true;
static bool g_mmove_end_flg = true;

void StandbyUpWidget::show_temp_value(int temp_val)
{
	static int temp_old = 0;
	int temp_tmp = 0;
	QString temp;

	temp_tmp = abs(temp_val);
	//qDebug()<<"show_temp_value="<<temp_tmp;

	if (temp_tmp < 0 || temp_tmp > 50)
	{
	    ENNO_LOG_ERR("error temperature = %d, temp_val=%d\r\n", temp_tmp, temp_val);
	    return;
	}

	ENNO_LOG_ERR("temperature=%d\n", temp_val);

	static int decade_bit = 0;
	QPropertyAnimation *anim_bb = new QPropertyAnimation(label_temp_bits, "geometry");
	QPropertyAnimation *anim_bd = new QPropertyAnimation(label_temp_bits_brother, "geometry");
	QPropertyAnimation *anim_db = new QPropertyAnimation(label_temp_decade, "geometry");
	QPropertyAnimation *anim_de = new QPropertyAnimation(label_temp_decade_brother, "geometry");
	
	//group_temp->stop();
	group_temp->clear();

	if(temp_tmp>temp_old)//UP
	{
		if(g_mb_flg)
		{
			anim_bb->setDuration(M_FONT_MOVE_TIME);
			anim_bb->setKeyValueAt(0, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.8, QRect(M_B_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.9, QRect(M_D_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(1, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));

			anim_bd->setDuration(M_FONT_MOVE_TIME);
			anim_bd->setKeyValueAt(0, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.1, QRect(M_D_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.2, QRect(M_B_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(1, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			g_mb_flg = false;
			temp.sprintf("%d", temp_tmp%10);
			label_temp_bits_brother->setText(temp);
		}
		else
		{
			anim_bd->setDuration(M_FONT_MOVE_TIME);
			anim_bd->setKeyValueAt(0, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.8, QRect(M_B_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.9, QRect(M_D_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(1, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));

			anim_bb->setDuration(M_FONT_MOVE_TIME);
			anim_bb->setKeyValueAt(0, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.1, QRect(M_D_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.2, QRect(M_B_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(1, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			g_mb_flg = true;
			temp.sprintf("%d", temp_tmp%10);
			label_temp_bits->setText(temp);
		}

		if(decade_bit<temp_tmp/10)
		{
			if(g_mdb_flg)
			{
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(0, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.8, QRect(M_D_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.9, QRect(M_E_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));

				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(0, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.1, QRect(M_E_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.2, QRect(M_D_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				g_mdb_flg = false;
				temp.sprintf("%d", temp_tmp/10);
				label_temp_decade_brother->setText(temp);
			}
			else
			{
				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(0, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.8, QRect(M_D_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.9, QRect(M_E_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));

				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(0, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.1, QRect(M_E_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.2, QRect(M_D_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				g_mdb_flg = true;
				temp.sprintf("%d", temp_tmp/10);
				label_temp_decade->setText(temp);
			}
			if(temp_val<=-10)
			{
				//label_temp_negative_flg->setText("-");
			}
			else
			{
				label_temp_negative_flg->setText(" ");
			}
		}
		else
		{
			 if(temp_tmp/10 == 0)
					{
						if(temp_val>=0)
						{
							if(g_mdb_flg)
							{
								label_temp_decade->setText(" ");
							}
							else
							{
								label_temp_decade_brother->setText(" ");
							}
						}
						else
						{
							if(g_mdb_flg)
							{
								label_temp_decade->setText("-");
							}
							else
							{
								label_temp_decade_brother->setText("-");
							}
						}
					}
			if(g_mdb_flg)
			{
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
			}
			else
			{
				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
			}
;
		}
		group_temp->addAnimation(anim_db);
		group_temp->addAnimation(anim_de);
		group_temp->addAnimation(anim_bb);
		group_temp->addAnimation(anim_bd);
		group_temp->start();
		g_mmove_end_flg = false;
	}
	else if(temp_tmp<temp_old)//DOWN
	{
		if(g_mb_flg)
		{
			anim_bb->setDuration(M_FONT_MOVE_TIME);
			anim_bb->setKeyValueAt(0, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.8, QRect(M_B_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.9, QRect(M_D_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(1, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));

			anim_bd->setDuration(M_FONT_MOVE_TIME);
			anim_bd->setKeyValueAt(0, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.1, QRect(M_D_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.2, QRect(M_B_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(1, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			g_mb_flg = false;
			temp.sprintf("%d", temp_tmp%10);
			label_temp_bits_brother->setText(temp);
		}
		else
		{
			anim_bd->setDuration(M_FONT_MOVE_TIME);
			anim_bd->setKeyValueAt(0, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.8, QRect(M_B_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(0.9, QRect(M_D_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bd->setKeyValueAt(1, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));

			anim_bb->setDuration(M_FONT_MOVE_TIME);
			anim_bb->setKeyValueAt(0, QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.1, QRect(M_D_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(0.2, QRect(M_B_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
			anim_bb->setKeyValueAt(1, QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
			g_mb_flg = true;
			temp.sprintf("%d", temp_tmp%10);
			label_temp_bits->setText(temp);
		}
		if(decade_bit>temp_tmp/10)
		{
			if(g_mdb_flg)
			{
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(0, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.8, QRect(M_D_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(0.9, QRect(M_E_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));

				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(0, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.1, QRect(M_E_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(0.2, QRect(M_D_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				g_mdb_flg = false;
				if(temp_val==9 )
				{
					label_temp_decade_brother->setText(" ");
				}
				else if(temp_val==-9 )
				{
					label_temp_decade_brother->setText("-");
				}
				else
				{
					temp.sprintf("%d", temp_tmp/10);
					label_temp_decade_brother->setText(temp);
				}

				if(temp_val<=-10)
				{
					//label_temp_negative_flg->setText("-");
				}
				else
				{
					label_temp_negative_flg->setText(" ");
				}
			}
			else
			{
					anim_de->setDuration(M_FONT_MOVE_TIME);
					anim_de->setKeyValueAt(0, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_de->setKeyValueAt(0.8, QRect(M_D_ABC_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_de->setKeyValueAt(0.9, QRect(M_E_x, M_C_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_de->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));

					anim_db->setDuration(M_FONT_MOVE_TIME);
					anim_db->setKeyValueAt(0, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_db->setKeyValueAt(0.1, QRect(M_E_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_db->setKeyValueAt(0.2, QRect(M_D_ABC_x, M_A_y, MAIN_FONT_W, MAIN_FONT_H));
					anim_db->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
					g_mdb_flg = true;
					if(temp_val==9)
					{
						label_temp_decade->setText(" ");
					}
					else if(temp_val==-9)
					{
						label_temp_decade->setText("-");
					}
					else
					{
						temp.sprintf("%d", temp_tmp/10);
						label_temp_decade->setText(temp);
					}
					if(temp_val<=-10)
					{
						//label_temp_negative_flg->setText("-");
					}
					else
					{
						label_temp_negative_flg->setText(" ");
					}
			}
		}
		else
		{
			if(temp_tmp/10 ==0)
					{
						if(temp_val>=0)
						{
							if(g_mdb_flg)
							{
								label_temp_decade->setText(" ");
							}
							else
							{
								label_temp_decade_brother->setText(" ");
							}
						}
						else
						{
							if(g_mdb_flg)
							{
								label_temp_decade->setText("-");
							}
							else
							{
								label_temp_decade_brother->setText("-");
							}
							label_temp_negative_flg->setText(" ");
						}
					}
			if(g_mdb_flg)
			{
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
			}
			else
			{
				anim_de->setDuration(M_FONT_MOVE_TIME);
				anim_de->setKeyValueAt(1, QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
				anim_db->setDuration(M_FONT_MOVE_TIME);
				anim_db->setKeyValueAt(1, QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
			}


		}
		group_temp->addAnimation(anim_db);
		group_temp->addAnimation(anim_de);
		group_temp->addAnimation(anim_bb);
		group_temp->addAnimation(anim_bd);
		//group_temp->start(QAbstractAnimation::DeleteWhenStopped);
		group_temp->start();
		g_mmove_end_flg = false;

	}

	decade_bit = temp_tmp/10;
	temp_old = temp_tmp;
}

void StandbyUpWidget::indoor_data_save_ini(int c_id, unsigned int val)
{
    QSettings *configIni = new QSettings("/home/user/ui/qwid/indoor_status.ini", QSettings::IniFormat);


	switch(c_id)
    {
	case CHAR_P2_TEMP_SENSOR:
		configIni->setValue("indoor/temp", val);
		break;

	case CHAR_P2_HUMIDITY_SENSOR:
		configIni->setValue("indoor/humidity", val);
		break;

	case CHAR_P2_VOC_SENSOR:
		configIni->setValue("indoor/voc", val);
		break;

	default:
		ENNO_LOG_ERR("error cid=%d, not find\n", c_id);
		break;
	}
	
    delete  configIni;
}

void StandbyUpWidget::get_sensor_value(char *value, uint32 len_buf)
{
	CTL_MSG_CONF *pmsg_conf;
	QString temp1;
	QString str;
    int flagCidValid = 1;

    //gettime();
    if (NULL == value)
    {
        qDebug()<<"get_sensor_value error NULL";
		return;
    }

  // static int temp =-6,flg=0;
	pmsg_conf = (CTL_MSG_CONF *)value;

	msg_bak.c_id = pmsg_conf->c_id;
	msg_bak.value = pmsg_conf->value;
	//qDebug()<<"pmsg_conf->c_id="<<pmsg_conf->c_id<<",value="<<pmsg_conf->value;
	ENNO_LOG_ERR("c_id=%d, value=%d\n", pmsg_conf->c_id, pmsg_conf->value);
	static int test_flg= 0;

    switch(pmsg_conf->c_id)
    {
    case CHAR_P2_TEMP_SENSOR:
		if (pmsg_conf->value/100 >= 0)
		{
			show_temp_value(pmsg_conf->value/100);

		/*	if(flg ==0)
			{
				temp++;
				if(temp==-5)
				{
					flg = 1;

				}
			}
			else if(flg ==1)
			{
				temp--;
				if(temp==-11)
				{
					flg = 0;

				}
			}*/


		    //temp1.sprintf("%d%c", pmsg_conf->value/100, 0xB0);
			//label_temp->setGeometry(QRect(60, 70, 170, 160));
		}
		else
		{
		   ENNO_LOG_ERR("error temperature=%d, don't disp\n", pmsg_conf->value/100);
//			show_temp_value(pmsg_conf->value/100);
		    //label_temp->setGeometry(QRect(100, 70, 130, 160));
		}
		//temp1.sprintf("%d", pmsg_conf->value/100);
		//label_temp->setText(temp1);
		break;

	case CHAR_P2_HUMIDITY_SENSOR:
	//	qDebug()<<"humidity="<<pmsg_conf->value;
		humidity = pmsg_conf->value;
		g_humi_update_flag = true;
		/*if (g_language_zh)
		{
		    label_gas->setStyleSheet("font-size:40px;color:rgb(255,255,255);");
		}
		else
		{
		    label_gas->setStyleSheet("font-size:34px;color:rgb(255,255,255);");
		}*/

	/*	test_flg++;
		if(test_flg==1)
		{
			humidity = 20;
		}
		else if(test_flg==2)
		{
			humidity = 50;
		}
		else if(test_flg==3)
		{
			humidity = 70;
			test_flg =0;
		}*/


		break;

	case CHAR_P2_VOC_SENSOR:
		gas = pmsg_conf->value;
		g_tvoc_update_flag = true;
	break;

	default:
		flagCidValid = 0;
		break;
    }
#if 0
	//QString str1 = QString::number(gas/1000000, 'f', 2);
	QString str1 = QString("voc:%1 ").arg(gas, 0, 'g', 3);
	//str2.sprintf(".H:%d", gas/1000000, humidity);
	QString str2 = QString("hum:%1").arg(humidity, 0, 'g', 2);
	label_gas->setText(str1+str2);
#endif
	//label_gas->setText(tr("voc:%1 .H:%2") .arg(gas/1000000) .arg(humidity));
	//timer_update->start();
	if (flagCidValid
		&&g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateInDoor(msg_bak.c_id, msg_bak.value);
	}
	
	indoor_data_save_ini(msg_bak.c_id, msg_bak.value);
}



void StandbyUpWidget::onTimerCheck()
{
	int current_time_humi  = 0;
	int current_time_tvoc  = 0;
	QPixmap pixmap_tvoc;
	QPixmap pixmap_humi;

	//printf("onTimerCheck\r\n");

	if (access(AQI_HIDE_FILE, F_OK) == 0)
    {
		aqi_hide_flag = 1;
    }
    else
   	{
	    aqi_hide_flag = 0;
    }

	

	if(g_tvoc_update_flag)
	{
		//current_time_humi = anim_humi->currentLoopTime();
		//if(current_time_humi>7800 & current_time_humi<14000)
		{
		     //printf("ui gas=%lf\n", gas);
			 if(gas > VOC_BAD_VALUE)
				 {
					 	label_font_tvoc->setText(tr("Air Polluted"));
						pixmap_tvoc.load("standby/look/tvoc.png");
						t_r=255;
						t_g=54;
						t_b=106;
						label_font_tvoc->setStyleSheet("font-size:37px;color:rgb(255,255,255);" \
									  "text-align:left;font-weight:440;background:transparent;");
				 }
			     else if (gas <= VOC_BAD_VALUE && gas > VOC_GOOD_VALUE)
			     {
			            label_font_tvoc->setText(tr("Air Norm"));
						pixmap_tvoc.load("standby/look/tvoc.png");
						t_r=255;
						t_g=191;
						t_b=37;
						label_font_tvoc->setStyleSheet("font-size:37px;color:rgb(255,255,255);" \
									  "text-align:left;font-weight:440;background:transparent;");
			     }
				 else
				 {
					 label_font_tvoc->setText(tr("Air Clean"));
					 pixmap_tvoc.load("standby/look/tvoc.png");
						t_r=159;
						t_g=217;
						t_b=65;
					label_font_tvoc->setStyleSheet("font-size:37px;color:rgb(255,255,255);" \
									  "text-align:left;font-weight:440;background:transparent;");
				 }
				
				 label_icon_tvoc->setPixmap(pixmap_tvoc);
				 update_color();
				 g_tvoc_update_flag = false;
		}
	}

	if(g_humi_update_flag)
	{
		//current_time_tvoc = anim_tvoc->currentLoopTime();
		//if(current_time_tvoc>600 & current_time_tvoc<7300)
		{
		
		     //printf("ui humidity=%d\n", humidity);
			 if (humidity < 30)
			 {
				 label_font_humi->setText(tr("Dry"));
				 pixmap_humi.load("standby/look/dry.png");
				 h_r=255;
				 h_g=54;
				 h_b=106;
			 }
			 else if (humidity >= 30 && humidity <=60)
			 {
				 label_font_humi->setText(tr("Normal"));
				 pixmap_humi.load("standby/look/normal.png");
				 h_r=159;
				 h_g=217;
				 h_b=65;
			 }
			 else if (humidity > 60)
			 {
				 label_font_humi->setText(tr("Humid"));
				 pixmap_humi.load("standby/look/wet.png");
				 h_r=90;
				 h_g=224;
				 h_b=255;
			 }

			 if(aqi_hide_flag)
			 {

			  	label_font_humi->setText(tr(" "));
				pixmap_humi.load("standby/look/normal.png");
				h_r=159;
				h_g=217;
				h_b=65;

			 }

			 
			label_font_humi->setStyleSheet("font-size:37px;color:rgb(255,255,255);" \
							  "text-align:left;font-weight:440;background:transparent;");

			 label_icon_humi->setPixmap(pixmap_humi);
			 update_color();
			 g_humi_update_flag = false;
		}
	}
}

void StandbyUpWidget::update_color()
{
//#ifndef NEW_LAKE
	group1->pause();
//#endif
	//anim_color = new QPropertyAnimation(effect_color,"color");
	anim_color->setDuration(8000);
	anim_color->setKeyValueAt(0, QColor(h_r,h_g,h_b));

	if (!aqi_hide_flag)
	{
		anim_color->setKeyValueAt(0.075, QColor(t_r,t_g,t_b));
		anim_color->setKeyValueAt(0.925, QColor(t_r,t_g,t_b));
	}
	anim_color->setKeyValueAt(1, QColor(h_r,h_g,h_b));
//#ifndef NEW_LAKE
	group1->resume();
//#endif
}




StandbyUpWidget::StandbyUpWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	label_time_hour = new QLabel(this);
	label_time_hour->setText("09:");
	label_time_hour->setStyleSheet("font-size:36px;color:rgb(255,255,255);" \
			  "font-weight:bold;text-align:left;"\
			  "background-color:rgb(0,0,0);");
	label_time_hour->setGeometry(QRect(220, 0, 100, 40));


	/***************************Edit by iovine***************************/

	temp_font_widget=new QWidget(this);
	temp_font_widget->setFixedSize(MAIN_WINDOW_W,MAIN_WINDOW_H);
	temp_font_widget->setStyleSheet("QWidget{background-color:rgb(0, 0, 0);}");
	temp_font_widget->move(MAIN_WINDOW_X, MAIN_WINDOW_Y);

	label_temp_decade = new QLabel(temp_font_widget);
	label_temp_decade->setStyleSheet("font-size:150px;color:white;font-weight:normal");
	label_temp_decade->setGeometry(QRect(M_D_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
	QString temp;
    temp.sprintf("%d", 2);
    label_temp_decade->setText(temp);

    label_temp_decade_brother = new QLabel(temp_font_widget);
    label_temp_decade_brother->setStyleSheet("font-size:150px;color:white;font-weight:normal");
    label_temp_decade_brother->setGeometry(QRect(M_E_x, M_E_y, MAIN_FONT_W, MAIN_FONT_H));
    temp.sprintf("%d", 2);
    label_temp_decade_brother->setText(temp);

    label_temp_bits = new QLabel(temp_font_widget);
    label_temp_bits->setStyleSheet("font-size:150px;color:white;font-weight:normal");
    label_temp_bits->setGeometry(QRect(M_B_ABC_x, M_B_y, MAIN_FONT_W, MAIN_FONT_H));
    temp.sprintf("%d", 0);
    label_temp_bits->setText(temp);

    label_temp_bits_brother = new QLabel(temp_font_widget);
    label_temp_bits_brother->setStyleSheet("font-size:150px;color:white;font-weight:normal");
    label_temp_bits_brother->setGeometry(QRect(M_D_x, M_D_y, MAIN_FONT_W, MAIN_FONT_H));
    temp.sprintf("%d", 0);
    label_temp_bits_brother->setText(temp);

	////////////////////////////////////////////////////////////////////

	QLabel *label_temp_unit = new QLabel(this);
	label_temp_unit->setStyleSheet("font-size:120px;color:rgb(255,255,255);" \
				"text-align:left;"\
				"background-color:rgb(0,0,0);");
	QString unit;
    unit.sprintf("%c", 0xB0);
	label_temp_unit->setText(unit);
	label_temp_unit->setGeometry(QRect(230, 60, 50, 160));

	label_temp_negative_flg = new QLabel(this);
	label_temp_negative_flg->setStyleSheet("font-size:120px;color:rgb(255,255,255);" \
				"text-align:left;"\
				"background-color:rgb(0,0,0);");
    label_temp_negative_flg->setText(" ");
    label_temp_negative_flg->setGeometry(QRect(0, 70, 50, 160));
	/***************************Edit by iovine***************************/




	if (access(AQI_HIDE_FILE, F_OK) == 0)
    {
		aqi_hide_flag = 1;
    }
    else
   	{
	    aqi_hide_flag = 0;
    }


	status_widget = new QLabel(this);
    status_widget->setFixedSize(320,70);
	status_widget->setStyleSheet("QWidget{border-radius:30px;}");
		//"border-top-left-radius:20px;border-top-right-radius:20px;}");
	status_widget->move(0, 250);

    label_icon_tvoc = new QLabel(status_widget);
	QPixmap pixmap_tvoc("standby/look/tvoc.png");
	label_icon_tvoc->setPixmap(pixmap_tvoc);
	label_icon_tvoc->setStyleSheet("background:transparent;");
	label_icon_tvoc->raise();

	label_font_tvoc = new QLabel(status_widget);
	label_font_tvoc->setText(tr("Air Clean"));
	label_font_tvoc->setStyleSheet("font-size:40px;color:rgb(255,255,255);" \
				  "text-align:left;font-weight:440;background:transparent;");
	label_font_tvoc->raise();
	this->gettime();

	if (aqi_hide_flag)
	{
		label_font_tvoc->setVisible(0);
	    label_icon_tvoc->setVisible(0);
	}

	timer_update = new QTimer();
    timer_update->setInterval(TIMER_UPDATE_MS);
    //timer_update->stop();
	
	connect(timer_update, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
	
	timer_update->start();

	timer_check = new QTimer();
	timer_check->setInterval(TIMER_CHECK_MS);
	connect(timer_check, SIGNAL(timeout()), this, SLOT(onTimerCheck()));

	timer_check->start();

    label_icon_humi = new QLabel(status_widget);
	QPixmap pixmap_humi("standby/look/normal.png");
	label_icon_humi->setPixmap(pixmap_humi);
	label_icon_humi->setStyleSheet("background:transparent;");
	label_icon_humi->raise();

	label_font_humi = new QLabel(status_widget);
	label_font_humi->setText(tr("normal"));
	label_font_humi->setStyleSheet("font-size:40px;color:rgb(255,255,255);" \
				  "text-align:left;font-weight:440;background:transparent;");
	
	if (aqi_hide_flag)
	{
		label_icon_humi->setGeometry(QRect(15, 0, 70, 70));
	    label_font_humi->setGeometry(QRect(110, 14, 165, 44));
	}
	
	label_font_humi->raise();

	effect_color = new QGraphicsColorizeEffect(this);
	status_widget->setGraphicsEffect(effect_color);


	anim_tvoc = new QPropertyAnimation(label_icon_tvoc, "geometry");// Set up anim1
	QPropertyAnimation *anim2 = new QPropertyAnimation(label_font_tvoc, "geometry");// Set up anim2

	
    anim_humi = new QPropertyAnimation(label_icon_humi, "geometry");
    QPropertyAnimation *anim4 = new QPropertyAnimation(label_font_humi, "geometry");
    anim_color = new QPropertyAnimation(effect_color,"color");

    group1 = new QParallelAnimationGroup;
	
	if (!aqi_hide_flag)
	{

	    anim_tvoc->setDuration(8000);
	    anim_tvoc->setKeyValueAt(0, QRect(15, 0+70, 70, 70));
	    anim_tvoc->setKeyValueAt(0.075, QRect(15, 0, 70, 70));//0.1
	    anim_tvoc->setKeyValueAt(0.925, QRect(15, 0, 70, 70));//0.9
	    anim_tvoc->setKeyValueAt(1, QRect(15, 0-70, 70, 70));
		anim2->setDuration(8000);
		anim2->setKeyValueAt(0, QRect(110, 0+70, 165, 44));
	    anim2->setKeyValueAt(0.075, QRect(110, 14, 165, 44));
	    anim2->setKeyValueAt(0.925, QRect(110, 14, 165, 44));
	    anim2->setKeyValueAt(1, QRect(110, 0-70, 165, 44));



	    anim_humi->setDuration(14000);
	    anim_humi->setKeyValueAt(0, QRect(15, 0, 70, 70));
	    anim_humi->setKeyValueAt(0.0428, QRect(15, 0-70, 70, 70));//0.057
	    anim_humi->setKeyValueAt(0.1, QRect(350, 0-70, 70, 70));
	    anim_humi->setKeyValueAt(0.2, QRect(350, 0+70, 70, 70));
	    anim_humi->setKeyValueAt(0.5282, QRect(15, 0+70, 70, 70));//0.514
	    anim_humi->setKeyValueAt(0.571, QRect(15, 0, 70, 70));//0.571
	    anim_humi->setKeyValueAt(1, QRect(15, 0, 70, 70));

		anim4->setDuration(14000);
		anim4->setKeyValueAt(0, QRect(110, 14, 165, 44));
		anim4->setKeyValueAt(0.0428, QRect(110, 14-70, 165, 44));
		anim4->setKeyValueAt(0.1, QRect(350, 14-70, 165, 44));
		anim4->setKeyValueAt(0.2, QRect(350, 14+70, 165, 44));
		anim4->setKeyValueAt(0.5282, QRect(110, 14+70, 165, 44));
		anim4->setKeyValueAt(0.571, QRect(110, 14, 165, 44));
		anim4->setKeyValueAt(1, QRect(110, 14, 165, 44));

		anim_color->setDuration(8000);
		anim_color->setKeyValueAt(0, QColor(h_r,h_g,h_b));
		anim_color->setKeyValueAt(0.075, QColor(t_r,t_g,t_b));
		anim_color->setKeyValueAt(0.925, QColor(t_r,t_g,t_b));
		anim_color->setKeyValueAt(1, QColor(h_r,h_g,h_b));
		


		group1->addAnimation(anim_tvoc);
		group1->addAnimation(anim2);
	


		

		group1->addAnimation(anim_humi);
		group1->addAnimation(anim4);
		group1->addAnimation(anim_color);
	}
	else
	{
	    //xiaoshi
		//group1->start();
	    group1->addAnimation(anim_color);
	}
	group1->start();  
	group1->setLoopCount(-1);
	
	group_temp = new QParallelAnimationGroup;

	this->translateLanguage();	
}

void StandbyWidget::translateLanguage()
{
    char aul_img[100];
	QPixmap pixmap_weather;
	QStringList string_list;
	string_list<<tr("Snow")<<tr("Rain")
		<<tr("Lightn.")<<tr("Sunny")
		<<tr("Fog")<<tr("Cloud")
		<<tr("Rain&S.");
		
	label_open_font->setText(tr("Devices On"));
	if (access(PAIR_OK_FILE, F_OK) == 0)
	{//pair ok
	    label_background->setText(tr("  All devices are off"));
	}
	else
	{
	    label_background->setText(tr("  Please add devices"));
	}
	
	label_weather_font->setText(tr("cloudy"));
	if (weather_id <= 7 && weather_id>= 1)
	    label_weather_font->setText(string_list.at(weather_id-1));
}

QString StandbyWidget::get_aqi_style(int valAqi)
{
    QString strColor;
	QString strFont = "font-size:36px;";
	QString strStyle;
	//qDebug()<<"TitleWidget::paintEvent";
	if (valAqi < 50)//0~50 良好//
	{
	    strColor = "color:rgb(98,219,64);";
	}
	else if (valAqi < 100)//51~100 中等 //
	{
		strColor = "color:rgb(252,238,33);";
	}
	else if (valAqi < 150)//101~150 对敏感人群有害 //
	{
		strColor = "color:rgb(247,147,30);";
	}
	else if (valAqi < 200)//151~200 不健康 //
	{
		strColor = "color:rgb(255,61,16);";
	}
	else if (valAqi < 300)//201~300 极不健康 //
	{
		strColor = "color:rgb(175,50,186);";
	}
	else if (valAqi)//301~500 有毒害 //
	{
		strColor = "color:rgb(149,12,50);";
	}
    strStyle = strFont + strColor + 
		       QString("text-align:right;font-weight:440;background-color:rgb(0,0,0);");
	return strStyle;
}

void StandbyWidget::outdoor_data_save_ini(int c_id, unsigned int val)
{
    QSettings *configIni = new QSettings("/home/user/ui/qwid/outdoor_status.ini", QSettings::IniFormat);


	switch(c_id)
    {
	case CHAR_P2_OUTDOOR_TEMP:
		configIni->setValue("outdoor/temp", val);
		break;

	case CHAR_P2_OUTDOOR_AQI:
		configIni->setValue("outdoor/aqi", val);
		break;

	case CHAR_P2_OUTDOOR_WEATHER:
		configIni->setValue("outdoor/weather", val);
		break;

	default:
		ENNO_LOG_ERR("error cid=%d, not find\n", c_id);
		break;
	}
	
    delete  configIni;
}


void StandbyWidget::outdoor_data_update_from_ini()
{
	QSettings *configIni = new QSettings("/home/user/ui/qwid/outdoor_status.ini", QSettings::IniFormat);
	uint32 data;
	QString qstr_aqi;
	QString temp;
	QString str;
	char aul_img[100];
	QPixmap pixmap_weather;
	QString img;
	QStringList string_list;
	string_list<<tr("Snow")<<tr("Rain")
		<<tr("Lightn.")<<tr("Sunny")
		<<tr("Fog")<<tr("Cloud")
		<<tr("Rain&S.");
	OutDoorValues stOutDoorValues;

	stOutDoorValues.temperature = configIni->value("outdoor/temp").toInt();
    stOutDoorValues.aqi = configIni->value("outdoor/aqi").toInt();
    stOutDoorValues.weather = configIni->value("outdoor/weather").toInt();

	if (stOutDoorValues.temperature <= MAX_OUTDOOR_TEMP 
		&& stOutDoorValues.temperature >= MIN_OUTDOOR_TEMP)
	{
	    temp.sprintf("%d%c", stOutDoorValues.temperature, 0xB0);
	}
	else
	{
		ENNO_LOG_ERR("[ini]get outdoor temp error:%d\r\n", stOutDoorValues.temperature);
	}
	label_temp_font->setText(temp);

	data = stOutDoorValues.aqi;
    if (data <= MAX_OUTDOOR_AQI 
		&& data >= MIN_OUTDOOR_AQI)
   	{
		qstr_aqi.sprintf("%d", data);
		label_aqi_data->setText(qstr_aqi);
		g_aqi_value = data;
		if (data < 50)
		{
		    str = "font-size:36px;color:rgb(98,219,64);";
		}
		else if (data < 100)
		{
		    str = "font-size:36px;color:rgb(252,238,33);";
		}
		else if (data < 150)
		{
		    str = "font-size:36px;color:rgb(247,147,30);";
		}
		else if (data < 200)
		{
		    str = "font-size:36px;color:rgb(255,61,16);";
		}
		else if (data < 300)
		{
		    str = "font-size:36px;color:rgb(175,50,186);";
		}
		else if (data < 2000)
		{
		    str = "font-size:36px;color:rgb(149,12,50);";
		}
		else
		{
		    ENNO_LOG_ERR("[ini]get outdoor aqi value error: %d\r\n", data);		
			system("cp /home/user/ui/qwid/status.ini /home/user/conf/");
			usleep(20000);
			system("sync");
		}
		
		label_aqi_font->setStyleSheet(str);
		label_aqi_data->setStyleSheet(str);
    }
	else
	{
	    ENNO_LOG_ERR("[ini]get outdoor aqi error:%d\r\n", data);
		system("cp /home/user/ui/qwid/status.ini /home/user/conf/");
		usleep(20000);
		system("sync");
	    //label_aqi_data->setText("***");
	}

    data = stOutDoorValues.weather;
    if (data <= 7 && data >= 1)
	{
		sprintf(aul_img, "standby/weather/weather%d", data);
		pixmap_weather.load(aul_img);//tr("standby/weather/weather%1") .arg(data));
	    label_weather->setPixmap(pixmap_weather);
		weather_id = data;
		label_weather_font->setText(string_list.at(data-1));
	}
	else
	{
	    data = 6;
	    sprintf(aul_img, "standby/weather/weather%d", data);
		pixmap_weather.load(aul_img);//tr("standby/weather/weather%1") .arg(data));
	    label_weather->setPixmap(pixmap_weather);
		weather_id = data;
		label_weather_font->setText(string_list.at(data-1));
	}
	
    delete  configIni;
}

void StandbyWidget::outdoor_info_data_set()
{
	QString temp;
	QString qstr_aqi;
	QString str;
	QPixmap pixmap_weather;
	QString img;
	uint32 data;
	int ret = 0;
	char aul_img[100];
	QStringList string_list;
	string_list<<tr("Snow")<<tr("Rain")
		<<tr("Lightn.")<<tr("Sunny")
		<<tr("Fog")<<tr("Cloud")
		<<tr("Rain&S.");

	outdoor_data_update_from_ini();
	
#if 0
	char outDoorBitMap;
	OutDoorValues stOutDoorValues;
	g_statusXmlHandle->getOutDoorValues(&outDoorBitMap,&stOutDoorValues);

	if ( outDoorBitMap > 0 )
	{
		ENNO_LOG_ERR("get xml temp=%d\r\n", stOutDoorValues.temperature);
	    if (stOutDoorValues.temperature != 0 && stOutDoorValues.temperature != 212)
	    { 
		    if(outDoorBitMap&BIT_TEMP)
		    {
				if (stOutDoorValues.temperature <= MAX_OUTDOOR_TEMP 
					&& stOutDoorValues.temperature >= MIN_OUTDOOR_TEMP)
				{
				    temp.sprintf("%d%c", stOutDoorValues.temperature, 0xB0);
				}
				else
				{
				    //temp.sprintf("**%c",0xB0);
					ENNO_LOG_ERR("get outdoor temp error:%d\r\n", stOutDoorValues.temperature);
					system("cp /home/user/ui/qwid/status.xml /home/user/conf/");
					ret = -1;
				}
				label_temp_font->setText(temp);
		    }

			if(outDoorBitMap&BIT_AQI)
		    {
                data = stOutDoorValues.aqi;
		        if (stOutDoorValues.aqi <= MAX_OUTDOOR_AQI 
					&& stOutDoorValues.aqi >= MIN_OUTDOOR_AQI)
		       	{
					qstr_aqi.sprintf("%d", stOutDoorValues.aqi);
					label_aqi_data->setText(qstr_aqi);
					g_aqi_value = data;
					if (data < 50)
					{
					    str = "font-size:36px;color:rgb(98,219,64);";
					}
					else if (data < 100)
					{
					    str = "font-size:36px;color:rgb(252,238,33);";
					}
					else if (data < 150)
					{
					    str = "font-size:36px;color:rgb(247,147,30);";
					}
					else if (data < 200)
					{
					    str = "font-size:36px;color:rgb(255,61,16);";
					}
					else if (data < 300)
					{
					    str = "font-size:36px;color:rgb(175,50,186);";
					}
					else if (data < 2000)
					{
					    str = "font-size:36px;color:rgb(149,12,50);";
					}
					else
					{
					    ENNO_LOG_ERR("get outdoor aqi value error: %d\r\n", data);		
						system("cp /home/user/ui/qwid/status.xml /home/user/conf/");
						ret = -2;
					}
					
					label_aqi_font->setStyleSheet(str);
					label_aqi_data->setStyleSheet(str);
		        }
				else
				{
				    ENNO_LOG_ERR("get outdoor aqi error:%d\r\n", stOutDoorValues.aqi);
					system("cp /home/user/ui/qwid/status.xml /home/user/conf/");
				    //label_aqi_data->setText("***");
				}
		    }

			if(outDoorBitMap&BIT_WEATHER)
		    {
		        data = stOutDoorValues.weather;
		        if (data <= 7 && data >= 1)
				{
					sprintf(aul_img, "standby/weather/weather%d", data);
					pixmap_weather.load(aul_img);//tr("standby/weather/weather%1") .arg(data));
				    label_weather->setPixmap(pixmap_weather);
					weather_id = data;
					label_weather_font->setText(string_list.at(data-1));
				}
				else
				{
				    data = 6;
				    sprintf(aul_img, "standby/weather/weather%d", data);
					pixmap_weather.load(aul_img);//tr("standby/weather/weather%1") .arg(data));
				    label_weather->setPixmap(pixmap_weather);
					weather_id = data;
					label_weather_font->setText(string_list.at(data-1));
					
					ret = -3;
				}
		    }		
	    }
		else
		{
		    ENNO_LOG_ERR("1090 get outdoor xml error\r\n");
	        system("cp /home/user/ui/qwid/status.xml /home/user/conf/");
			system("rm /home/user/ui/qwid/status.xml");
			outdoor_data_update_from_ini();
		}

		if (ret)
		{
		    ENNO_LOG_ERR("outdoor ret=%d\r\n", ret);
			outdoor_data_update_from_ini();
		}
	}
	else
	{
	    ENNO_LOG_ERR("1098 get outdoor xml error\r\n");
	    system("cp /home/user/ui/qwid/status.xml /home/user/conf/");
		outdoor_data_update_from_ini();
	}
#endif
	
}

StandbyWidget::StandbyWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);
	
	//right_center_function_widget = new QWidget(this);
	//function_label = new QLabel();
	//right_center_function_widget->raise();
	//this->show();
	/*QLabel *label =new QLabel(this);  
    QMovie *movie = new QMovie("standby.png");  
  
    label->setMovie(movie);  
	label->setGeometry(0,0,320,320);
    movie->start(); 
    this->show();*/
    label_open_font = new QLabel(this);
	label_open_font->setText(tr(" device opened"));
	label_open_font->setStyleSheet("font-size:30px;color:rgb(255,255,255);" \
				"text-align:left;font-weight:440;"\
				"background-color:rgb(0,0,0);");
	label_open_font->setGeometry(QRect(0, 145, OLED_PIX_X, 50));

    open_widget = new QWidget(this);
    open_widget->setFixedSize(320,115);
	open_widget->setStyleSheet("QWidget{background-color:rgb(77,77,77);"\
		"border-radius:10px;}");
		//"border-top-left-radius:20px;border-top-right-radius:20px;}");
	open_widget->move(0, 205);

	open_widget_list1 = new QWidget(open_widget);
	open_widget_list2 = new QWidget(open_widget);
	open_widget_list1->setFixedSize(800,100);
	open_widget_list2->setFixedSize(800,100);
	open_widget_list1->setStyleSheet("QWidget{background-color:gray;");	
	open_widget_list2->setStyleSheet("QWidget{background-color:gray;");
	open_widget_list1->move(320,115);
	open_widget_list2->move(320,115);
	
	label_background = new QLabel(open_widget);
	if (access(PAIR_OK_FILE, F_OK) == 0)
	{//pair ok
	    label_background->setText(tr("  all device closed"));
	}
	else
	{
	    label_background->setText(tr("  please add devices"));
	}

    if (access(AQI_HIDE_FILE, F_OK) == 0)
    {
		aqi_hide_flag = 1;
    }
    else
   	{
	    aqi_hide_flag = 0;
    }



    
	label_background->setStyleSheet("font-size:30px;color:rgb(255,255,255);" \
				"text-align:left;font-weight:440;"\
				"background-color:rgb(77,77,77);");
	label_background->setGeometry(QRect(20, 40, 300, 50));

	for (int i = 0; i < MAX_DEV_NUM_SHOW; i++)
	{
	    label_open[i] = new QLabel(open_widget_list1);
		label_open[i]->move(330, 115);

		label_open2[i] = new QLabel(open_widget_list2);
		label_open2[i]->move(330, 115);
	}

	label_weather = new QLabel(this);
	label_weather->setGeometry(QRect(0, 0, 162, 132));

    //返回天气对应的code，1:雪,2:雨,3:风暴,4:晴,5:雾,6:多云,7:雨夹雪
	QPixmap pixmap_weather("standby/weather/weather6");
	label_weather->setPixmap(pixmap_weather);

	label_weather_font = new QLabel(this);
	if (!aqi_hide_flag)
	{
		label_weather_font->setGeometry(QRect(150, 25, 110, 50));
	}
	else
	{
		label_weather_font->setGeometry(QRect(175, 25, 110, 50));
	}
	
	label_weather_font->setText(tr("cloudy"));//cloudy
	label_weather_font->setStyleSheet("font-size:36px;color:rgb(255,255,255);" \
				"text-align:right;font-weight:440;"\
				"background-color:rgb(0,0,0);");

	if (!aqi_hide_flag)
	{
		label_weather_font->setAlignment(Qt::AlignRight | Qt::AlignBottom);
	}

	label_temp_font = new QLabel(this);
	label_temp_font->setStyleSheet("font-size:36px;color:rgb(255,255,255);" \
				"text-align:left;font-weight:440;"\
				"background-color:rgb(0,0,0);");
	
	if (!aqi_hide_flag)
	{
	    label_temp_font->setGeometry(QRect(265, 25, 55, 50));
	}
	else
	{
	    label_temp_font->setGeometry(QRect(175, 85, 55, 50));  //x same, y move 
	}
	
	QString temp;
	int temperature = 23;
    temp.sprintf("%d%c", temperature, 0xB0);
	label_temp_font->setText(temp);
	label_temp_font->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    int ul_aqi = 70;
	QString aqiStyle = get_aqi_style(ul_aqi);

    label_aqi_font = new QLabel(this);
	label_aqi_font->setText("AQI");
	label_aqi_font->setStyleSheet(aqiStyle);
	label_aqi_font->setGeometry(QRect(175, 85, 75, 50));
	label_aqi_font->setAlignment(Qt::AlignRight | Qt::AlignBottom);

	label_aqi_data = new QLabel(this);
	label_aqi_data->setStyleSheet(aqiStyle);
	label_aqi_data->setGeometry(QRect(260, 85, 60, 50));
	QString qstr_aqi;	
    qstr_aqi.sprintf("%d", ul_aqi);
	label_aqi_data->setText(qstr_aqi);
	label_aqi_data->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

	if (aqi_hide_flag)
	{
	    label_aqi_font->setVisible(0);
		label_aqi_data->setVisible(0);
	}

	outdoor_info_data_set();

	group1 = new QParallelAnimationGroup;

	this->translateLanguage();
}

/*void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}*/
void StandbyWidget::changeLanguage()
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
void StandbyWidget::get_outdoor_value(char *value, uint32 len_buf)
{
	QString temp;
	QString qstr_aqi;
	QString str;
	QPixmap pixmap_weather;
	QString img;
	QStringList string_list;
	string_list<<tr("Snow")<<tr("Rain")
		<<tr("Lightn.")<<tr("Sunny")
		<<tr("Fog")<<tr("Cloud")
		<<tr("Rain&S.");
	char aul_img[100];
	uint32 c_id;
	uint32 data;
	uint32 len = len_buf;
	uint32 len_use;
	enno_result ret;
	char *buf = value;

	int flagCidValid = 1;

    if (NULL == value)
    {
        qDebug()<<"get_sensor_value error NULL";
		return;
    }
    ret = enno_decode_uint(buf, &c_id, &len_use, len);
	ENNO_CHECK_NORTN_RET(ret);
    
	len -= len_use;
	buf += len_use;
	ret = enno_decode_uint(buf, &data, &len_use, len);
	ENNO_CHECK_NORTN_RET(ret);
		
    //qDebug()<<"c_id="<<c_id<<", value="<<data;
	ENNO_LOG_ERR("get outdoor c_id=%d, value=%d\n", c_id, data);
	
    switch(c_id)
    {
	case CHAR_P2_OUTDOOR_TEMP:
	    temp.sprintf("%d%c", data, 0xB0);
		if (data < 60 && data >= 0)
		{
			label_temp_font->setText(temp);
		}
		else
		{
		    ENNO_LOG_ERR("get outdoor temp value error: %d\r\n", data);
			outdoor_info_data_set();
			flagCidValid = 0;
		}
		break;

	case CHAR_P2_OUTDOOR_AQI:
	    qstr_aqi.sprintf("%d", data);
		label_aqi_data->setText(qstr_aqi);
		g_aqi_value = data;
		if (data < 50)
		{
		    str = "font-size:36px;color:rgb(98,219,64);";
		}
		else if (data < 100)
		{
		    str = "font-size:36px;color:rgb(252,238,33);";
		}
		else if (data < 150)
		{
		    str = "font-size:36px;color:rgb(247,147,30);";
		}
		else if (data < 200)
		{
		    str = "font-size:36px;color:rgb(255,61,16);";
		}
		else if (data < 300)
		{
		    str = "font-size:36px;color:rgb(175,50,186);";
		}
		else if (data < 2000)
		{
		    str = "font-size:36px;color:rgb(149,12,50);";
		}
		else
		{
		    ENNO_LOG_ERR("get outdoor aqi value error: %d\r\n", data);
			outdoor_info_data_set();
			flagCidValid = 0;
			return;
		}
		
		label_aqi_font->setStyleSheet(str);
		label_aqi_data->setStyleSheet(str);
		break;

	case CHAR_P2_OUTDOOR_WEATHER:
		//img = ;
		if (data <= 7 && data >= 1)
		{
			sprintf(aul_img, "standby/weather/weather%d", data);
			pixmap_weather.load(aul_img);//tr("standby/weather/weather%1") .arg(data));
		    label_weather->setPixmap(pixmap_weather);
			weather_id = data;
			label_weather_font->setText(string_list.at(data-1));
			outdoor_info_data_set();
		}
		else
		{
		    flagCidValid = 0;
		}
		break;

	default:
		flagCidValid = 0;
		break;
    }
	
	/*if(flagCidValid
		&&g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateOutDoor(c_id,data);
		
	}*/
	outdoor_data_save_ini(c_id, data);
}
extern unsigned int g_openBitMap;
void StandbyWidget::get_dev_open_notify(char *value, uint32 len_buf)
{
    uint32 len;
	uint32 bitmap;
	static uint32 last_bitmap = 0xff;
	int i;
	int dev_cnt = 0;
	static int dev_num;
    enno_result ret;
	int dnum;
	double secA;
	double secB;	
	double secC;
	double secD;
	double mov_time;
	double sta_time;
	double total_time;
	char aul_img[100];
	QPixmap pixmap[MAX_DEV_NUM_SHOW];
	
    if (!value)
    {
        qDebug()<<"get_dev_open_notify value NULL";
    }

	ret = enno_decode_uint(value, &bitmap, &len, len_buf);
	if (ret)
	{
		qDebug()<<"qterr enno_decode_uint, get_firmware_update_per";
	}

    if ( (last_bitmap == bitmap)
		&&(!CTL_IS_BIT_SET(bitmap, 3))
	   )
    {
        return;
    }
	else
	{
	    last_bitmap = bitmap;
	}
	
    //qDebug()<<"get_dev_open_notify, bitmap="<<bitmap<<"g_openBitMap="<<g_openBitMap<<endl;

	if(    g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateOpenDev(bitmap);
	}

	if ((bitmap&BITMAP_MASK) || (CTL_IS_BIT_SET(bitmap, 25)))
	{
        label_background->move(320, 115);
		for (int i = 0; i < MAX_DEV_NUM_SHOW; i++)
		{
			label_open[i]->move(330, 115);
		}
		if (dev_num > 3)
		{
		    //group1->setLoopCount(0);
			group1->stop();
			group1->clear();
		}
		for (int i = 0; i < MAX_DEV_NUM_SHOW; i++)
		{
			label_open[i]->move(330, 115);
			label_open2[i]->move(330, 115);
		}

		if(CTL_IS_BIT_SET(bitmap, 3) || CTL_IS_BIT_SET(bitmap, 25))
		{
		    if(g_openBitMap!=0)
		    {
		        if(g_openBitMap&BIT_AC)
			    {
			        bitmap |= (1<<(AIR_CONDITION_SHOW-DEV_DALI_LIGHT));
			    }
				else
				{
				    bitmap &= (~(1<<(AIR_CONDITION_SHOW-DEV_DALI_LIGHT))); 
				}

				if(g_openBitMap&BIT_FLOOR)
				{
				    bitmap |= (1<<(DEV_RS485_HEATER_COOLER-DEV_DALI_LIGHT));
				}
				else
				{
				    bitmap &= (~(1<<(DEV_RS485_HEATER_COOLER-DEV_DALI_LIGHT)));
				}
		    }		    
		    
		}
		
		for (i = 0; i < MAX_DEV_NUM_SHOW; i++)
		{
		    if(CTL_IS_BIT_SET(bitmap, i))
		    {
		    	QImage *img = new QImage;

				sprintf(aul_img, "standby/devopen/open_%d.png", i);
				
				img->load(aul_img);
				*img = img->scaled(60,60,Qt::IgnoreAspectRatio);
		        pixmap[dev_cnt].load(aul_img);
				label_open[dev_cnt]->setPixmap(QPixmap::fromImage(*img));
				delete img;
				label_open[dev_cnt]->setFixedSize(60,60);
				#ifdef OPEN_DEV_SCROLL_H
				label_open[dev_cnt]->setGeometry(QRect(20+dev_cnt*105, 25, 60, 60));
				#else
				label_open[dev_cnt]->setGeometry(QRect(20+(dev_cnt%3)*105, 25 + (dev_cnt/3)*105, 60, 60));
				#endif
				dev_cnt++;
		    }
		}
		dev_num = dev_cnt;
		open_widget_list1->move(0,7);
		open_widget_list1->raise();

		if (dev_num > 3)
		{
#ifdef OPEN_DEV_SCROLL_H
		    open_widget_list1->setFixedSize(100*dev_num,100);
		    open_widget_list2->setFixedSize(100*dev_num,100);
			open_widget_list1->raise();
			dev_cnt = 0;
		    for (i = 0; i < MAX_DEV_NUM_SHOW; i++)
			{   
			    if(CTL_IS_BIT_SET(bitmap, i))
			    {
					QImage *img = new QImage;
					sprintf(aul_img, "standby/devopen/open_%d.png", i);
					img->load(aul_img);
					*img = img->scaled(60,60,Qt::IgnoreAspectRatio);
			        pixmap[dev_cnt].load(aul_img);
					label_open2[dev_cnt]->setPixmap(QPixmap::fromImage(*img));
					delete img;
					label_open2[dev_cnt]->setFixedSize(60,60);
					label_open2[dev_cnt]->setGeometry(QRect(20+dev_cnt*105, 25, 60, 60));
					dev_cnt++;
			    }
			}			
			open_widget_list2->raise();
			open_widget_list1->move(320, 7);
			open_widget_list2->move(320, 7);
#endif
#if 0
			QPropertyAnimation *anim1 = new QPropertyAnimation(open_widget_list1, "geometry");// Set up anim1  
			dnum = (double)dev_num;
			secA = (dnum-3.0)/(2*dnum);
			secB = dnum/(2*dnum);
			secC = (dnum-3+dnum)/(2*dnum);
			secD = (2*dnum)/(2*dnum);
			anim1->setDuration(dev_num*4000);
			anim1->setKeyValueAt(0, QRect(0, 7, 100, 100));
			anim1->setKeyValueAt(secA, QRect(300-dev_num*100, 7, 100, 100));
			anim1->setKeyValueAt(secB, QRect(0-dev_num*100, 7, 100, 100));
			anim1->setKeyValueAt(secB+0.01, QRect(0-dev_num*100, 7+300, 100, 100));
			anim1->setKeyValueAt(secB+0.02, QRect(320,  7+300, 100, 100));
			
			anim1->setKeyValueAt(secB+0.03, QRect(320,  7, 100, 100));
			anim1->setKeyValueAt(secC, QRect(320,  7, 100, 100));
			anim1->setKeyValueAt(secD, QRect(0,  7, 100, 100));
			anim1->setKeyValueAt(1, QRect(0,  7, 100, 100));

			//dnum = (double)dev_num;
			//secA = (dnum-3.0)/(2*dnum);
			//secB = dnum/(2*dnum);
			//secC = (dnum-3+dnum)/(2*dnum);
			//secD = (dnum+3.0)/(2*dnum);
			//qDebug()<<"secA="<<secA<<"secB="<<secB<<"secC="<<secC<<"secC1="<<secC+0.01<<"secC2="<<secC+0.02;

			QPropertyAnimation *anim2 = new QPropertyAnimation(open_widget_list2, "geometry");// Set up anim1  
			anim2->setDuration(dev_num*4000);
			anim2->setKeyValueAt(0, QRect(320, 7, 100, 100));
			anim2->setKeyValueAt(secA, QRect(320, 7, 100, 100));
			anim2->setKeyValueAt(secB, QRect(0, 7, 100, 100));
			anim2->setKeyValueAt(secC, QRect(300-dev_num*100, 7, 100, 100));
			anim2->setKeyValueAt(secD-0.04, QRect(0-dev_num*100, 7, 100, 100));
			anim2->setKeyValueAt(secD-0.03, QRect(0-dev_num*100, 7+300, 100, 100));
 		
			anim2->setKeyValueAt(secD-0.02, QRect(320, 7+300, 100, 100));
			anim2->setKeyValueAt(secD-0.01, QRect(320, 7, 100, 100));
			anim2->setKeyValueAt(1, QRect(320, 7, 100, 100));
#endif

#ifdef OPEN_DEV_SCROLL_H
			QPropertyAnimation *anim1 = new QPropertyAnimation(open_widget_list1, "geometry");
			mov_time = 2;   //1sec
			sta_time = 5;   //5sec
			dnum = (dev_num/3) + 1;
			total_time = 2*dnum*(mov_time+sta_time);

			anim1->setDuration(total_time*1000);
			anim1->setKeyValueAt(0, QRect(0, 7, 60, 60));
			anim1->setKeyValueAt(sta_time/total_time, QRect(0, 7, 60, 60));
			
			anim1->setKeyValueAt((sta_time+mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));
		
			anim1->setKeyValueAt((2*sta_time+mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));

            if (dnum == 3)
            {
				anim1->setKeyValueAt((2*sta_time+2*mov_time)/total_time, QRect(-640, 7, 60, 60));
		
				anim1->setKeyValueAt((3*sta_time+2*mov_time)/total_time, QRect(-640, 7, 60, 60));

				anim1->setKeyValueAt((3*sta_time+3*mov_time)/total_time, QRect(-960, 7, 60, 60));
				anim1->setKeyValueAt((3*sta_time+3*mov_time+1)/total_time, QRect(-960, 7+300, 60, 60));
				anim1->setKeyValueAt((3*sta_time+3*mov_time+2)/total_time, QRect(320, 7+300, 60, 60));
				anim1->setKeyValueAt((6*sta_time+5*mov_time)/total_time, QRect(320,	7, 60, 60));
				anim1->setKeyValueAt(1, QRect(0, 7, 60, 60));
			}
			else 
			{
			    anim1->setKeyValueAt((2*sta_time+2*mov_time)/total_time, QRect(-640, 7, 60, 60));
		
			    anim1->setKeyValueAt((2*sta_time+2*mov_time+1)/total_time, QRect(-640, 7+300, 60, 60));
				anim1->setKeyValueAt((2*sta_time+2*mov_time+2)/total_time, QRect(320, 7+300, 60, 60));
				anim1->setKeyValueAt((4*sta_time+3*mov_time)/total_time, QRect(320,	7, 60, 60));
				anim1->setKeyValueAt(1, QRect(0, 7, 60, 60));	
			}

			QPropertyAnimation *anim2 = new QPropertyAnimation(open_widget_list2, "geometry");// Set up anim1  
			anim2->setDuration(total_time*1000);
			anim2->setKeyValueAt(0, QRect(320, 7, 60, 60));
			if (dnum == 3)
			{
			    anim2->setKeyValueAt((3*sta_time+2*mov_time)/total_time, QRect(320, 7, 60, 60));
				
				anim2->setKeyValueAt((3*sta_time+3*mov_time)/total_time, QRect(0, 7, 60, 60));
				anim2->setKeyValueAt((4*sta_time+3*mov_time)/total_time, QRect(0, 7, 60, 60));

				anim2->setKeyValueAt((4*sta_time+4*mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));
				anim2->setKeyValueAt((5*sta_time+4*mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));

				anim2->setKeyValueAt((5*sta_time+5*mov_time)/total_time, QRect(-640, 7, 60, 60));
				anim2->setKeyValueAt((6*sta_time+5*mov_time)/total_time, QRect(-640, 7, 60, 60));

				anim2->setKeyValueAt((6*sta_time+6*mov_time-0.3)/total_time, QRect(-960, 7, 60, 60));
				anim2->setKeyValueAt((6*sta_time+6*mov_time-0.2)/total_time, QRect(-960, 7+300, 60, 60));
				anim2->setKeyValueAt((6*sta_time+6*mov_time-0.1)/total_time, QRect(320, 7+300, 60, 60));
				anim2->setKeyValueAt(1/total_time, QRect(320, 7, 60, 60));

			}
			else
			{
			    anim2->setKeyValueAt((2*sta_time+1*mov_time)/total_time, QRect(320, 7, 60, 60));
				
				anim2->setKeyValueAt((2*sta_time+2*mov_time)/total_time, QRect(0, 7, 60, 60));
				anim2->setKeyValueAt((3*sta_time+2*mov_time)/total_time, QRect(0, 7, 60, 60));

				anim2->setKeyValueAt((3*sta_time+3*mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));
				anim2->setKeyValueAt((4*sta_time+3*mov_time)/total_time, QRect(-OLED_PIX_X, 7, 60, 60));
				
				anim2->setKeyValueAt((4*sta_time+4*mov_time-0.3)/total_time, QRect(-640, 7, 60, 60));
				anim2->setKeyValueAt((4*sta_time+4*mov_time-0.2)/total_time, QRect(-640, 7+300, 60, 60));
				anim2->setKeyValueAt((4*sta_time+4*mov_time-0.1)/total_time, QRect(320, 7+300, 60, 60));
				anim2->setKeyValueAt(1/total_time, QRect(320, 7, 60, 60));

			}
		
			//qDebug()<<"secA="<<secA<<"secB="<<secB<<"secC="<<secC<<"secC1="<<secC+0.01<<"secC2="<<secC+0.02;
			group1->addAnimation(anim1);
			group1->addAnimation(anim2);
          //  open_widget_list1->move(320,0);
					    //open_widget_list2->move(20,7);
#endif	

#ifndef OPEN_DEV_SCROLL_H
        for (i = 0; i < dev_num; i++)
    	{
            anim[i] = new QPropertyAnimation(label_open[i], "geometry");
        }
        mov_time = 1;
        sta_time = 4;
		total_time = 2*sta_time+2*mov_time + 0.3;
		for (i = 0; i < 3; i++)
		{
			anim[i]->setDuration(total_time*1000);	
	        anim[i]->setKeyValueAt(0, QRect(20+105*i, 25, 60, 60));
	        anim[i]->setKeyValueAt(sta_time/total_time, QRect(20+105*i, 25, 60, 60));
			
	        anim[i]->setKeyValueAt((sta_time+mov_time)/total_time, QRect(20+105*i, 25-105, 60, 60));
	        anim[i]->setKeyValueAt((sta_time+mov_time+0.01)/total_time, QRect(320+20+105*i, 25-105, 60, 60));
	        anim[i]->setKeyValueAt((sta_time+mov_time+0.02)/total_time, QRect(320+20+105*i, 25+105, 60, 60));
	        anim[i]->setKeyValueAt((2*sta_time+mov_time+0.03)/total_time, QRect(20+105*i, 25+105, 60, 60));
	        anim[i]->setKeyValueAt(1, QRect(20+105*i, 25, 60, 60));
            group1->addAnimation(anim[i]);
		}
		for (i = 3; i < dev_num && i < 6; i++)
		{
			anim[i]->setDuration(total_time*1000);	
	        anim[i]->setKeyValueAt(0, QRect(20+105*(i%3), 25+105, 60, 60));
			anim[i]->setKeyValueAt(sta_time/total_time, QRect(20+105*(i%3), 25+105, 60, 60));
			
	        anim[i]->setKeyValueAt((sta_time+mov_time)/total_time, QRect(20+105*(i%3), 25, 60, 60));
	        anim[i]->setKeyValueAt((2*sta_time+mov_time)/total_time, QRect(20+105*(i%3), 25, 60, 60));
			
	        anim[i]->setKeyValueAt((2*sta_time+2*mov_time)/total_time, QRect(20+105*(i%3), 25-105, 60, 60));
	        anim[i]->setKeyValueAt((2*sta_time+ 2*mov_time+0.01)/total_time, QRect(320+20+105*(i%3), 25-105, 60, 60));
	        anim[i]->setKeyValueAt((2*sta_time+ 2*mov_time+0.02)/total_time, QRect(320+20+105*(i%3), 25+105, 60, 60));
	        anim[i]->setKeyValueAt(1, QRect(20+105*(i%3), 25+105, 60, 60));
            group1->addAnimation(anim[i]);
		}

#endif
			//group1->start();			
			group1->setLoopCount(-1);
		    group1->start();	
		}
		/*else if(dev_num)
		{
		    open_widget_list1->move(0,7);
			open_widget_list1->raise();
		}*/
	}
	else
	{
	    label_background->move(0, 40);
		open_widget_list1->move(320, 7);
		open_widget_list2->move(320,7);
	}
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

