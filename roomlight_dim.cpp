#include "roomlight.h"
#include "common.h"
#include "enno_encode.h"
#include <sys/time.h>

//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include "common.h"
#include "wthread.h"
#include <QMutex>

#define LIGHT_IMG_SIZE 100

int g_current_dim;

QMutex mutex;
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);
void RoomLightDimWidget::translateLanguage()
{

}

RoomLightDimWidget::RoomLightDimWidget(int room_id, TreeModel *model, QWidget *parent)
{
	QPixmap pixmap;
	int i;
	int j;
	
	//sem_dim.release(1);
	dim_sem = 0;

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);

	i_light_cnt = 0;
	i_light_num = 0;

	r_index = 0;
	w_index = 0;
	
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
		ast_dev[i_light_cnt].dim_value = 0;//light_value[3].toInt(&ok,10);
		ast_dev[i_light_cnt].device_type = light_value[2].toInt(&ok,10);
      //  qDebug()<<"room["<<"room_id"<<"],light["<<i_light_cnt<<"]="
		//	    <<ast_dev[i_light_cnt].device_id<<",type=["<<ast_dev[i_light_cnt].device_type<<"]"
		//	    <<"dim_v="<<ast_dev[i_light_cnt].dim_value;
        
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
//	main_wid = new QWidget(this);
//	main_wid->setFixedSize(OLED_PIX_X, OLED_PIX_Y);

	dim_text = new QLabel(this);
	dim_text->setGeometry(QRect(90, 80, 122, 75));
	dim_text->setText("0");
	dim_text->setStyleSheet("QWidget{font-size:75px;color: white;background:transparent;background-color:transparent;"\
			"border-top-left-radius:20px;border-bottom-left-radius:20px;}");

	dim_percent = new QLabel(this);
	dim_percent->setGeometry(QRect(170, 100, 30, 55));
	dim_percent->setText("%");
	dim_percent->setStyleSheet("QWidget{font-size:30px;color: white;background:transparent;background-color:transparent;"\
			"border-top-left-radius:20px;border-bottom-left-radius:20px;}");

	//dim_bar = new QProgressBar(main_wid);
	//dim_bar->setGeometry(QRect(0, 200, OLED_PIX_X, 60));
	//dim_bar->setRange(0, 100);
	//dim_bar->setValue(0);
   /* int L_len = 0;
	int R_len = 0;
	int C_x =0;
	int value = 0;
	value = 0;//ast_dev[current_index].dim_value;
	main_wid = new QWidget(this);
	main_wid->setFixedSize(OLED_PIX_X, OLED_PIX_Y);

	ProgressBar_L = new QLabel(main_wid);
	ProgressBar_L->setGeometry(QRect(0, 60, L_len, 44));
	ProgressBar_L->setStyleSheet("QWidget{background-color:rgb(255, 191, 37);"\
			"border-top-left-radius:20px;border-bottom-left-radius:20px;}");

	ProgressBar_R = new QLabel(main_wid);
	ProgressBar_R->setGeometry(QRect(L_len, 60, R_len, 44));
	ProgressBar_R->setStyleSheet("QWidget{background-color:rgb(150, 150, 150);"\
			"border-top-right-radius:20px;border-bottom-right-radius:20px;}");

	ProgressBar_C = new QLabel(main_wid);
	ProgressBar_C->setGeometry(QRect(C_x, 57, 50, 50));
	ProgressBar_C->setStyleSheet("QWidget{background-color:white;"\
			"border-radius:23px;}");
	ProgressBar_C->raise();*/
	timer_dim_out = new QTimer();
    timer_dim_out->setInterval(TIMER_DIM_OUT_MS);  //30s 
    last_value = 0;
    timer_dim_out->stop();
	connect(timer_dim_out, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    
	timer_dim_flash = new QTimer();
    timer_dim_flash->setInterval(500);  //30s 
    last_value = 0;
    timer_dim_flash->stop();
	connect(timer_dim_flash, SIGNAL(timeout()), this, SLOT(onTimerDimOut()));

    QSettings *configIni = new QSettings("/home/user/ui/qwid/config.ini", QSettings::IniFormat);

	slider_speed_time = configIni->value("roomlight/slider_speed").toInt();

    ENNO_LOG_ERR("slider_speed_time=%d\n", slider_speed_time);
	if (slider_speed_time <= 0)
	{
	    slider_speed_time = 300;
	}

	delete configIni;
	//main_wid->show();
}
void RoomLightDimWidget::onTimerOut()
{
    qDebug()<<"RoomLightDimWidget onTimerOut";
	emit closeRoomlightDim();
    timer_dim_out->stop();
	timer_dim_flash->stop();
}
void RoomLightDimWidget::onTimerDimOut()
{
    int step = 0;
	int sum;
	int sum_cnt = 0;
	int ul_cnt;
	int i;
	int aul_dim[6] = {10, 10, 10, 10, 10, 10};
	
	
    ENNO_LOG_ERR("onTimerDimOut time=%lld,r_index=%d, w_index=%d, \n", GetTickCount(), r_index, w_index);
    if (r_index != w_index)
    {
        step = array_step_buf[r_index];
		r_index++;
		if (r_index >= MAX_STEP_INDEX)
		{
		    r_index = 0;
		}
    }
	else
	{
	    ENNO_LOG_ERR("timer_stop, r_index=%d, w_index=%d\n", r_index, w_index);
		timer_dim_flash->stop();
	    return;
	}
	
	sum = 0;
		for (i = 0; i < i_light_num; i++)
		{
			
			if (ast_dev[i].device_type != DEV_DMX_LIGHT && ast_dev[i].device_type != DEV_KNX_LIGHT_SWITCH)
			{
				sum += ast_dev[i].dim_value;
				sum_cnt++;
				
			    ENNO_LOG_ERR("i=%d, value=%d\n", ast_dev[i].dim_value, sum);
			}
		}
		average = ((sum/sum_cnt)/10)*10;
		
		qDebug()<<"1sum="<<sum<<", cnt="<<sum_cnt<<", average="<<average;
		if (average >= 100)
		{
			average = 100;
		}
		else if (average <= 0)
		{
			average = 0;
		}
	
		qDebug()<<"2sum="<<sum<<",i_light_num="<<i_light_num<<",average="<<average;
	
		if (!step)//顺时针
		{
			if (average < 100)
			{
				average += aul_dim[ul_cnt];
				if (average >= 100)
				{
					average = 100;
				}
			}
			else
			{
				ENNO_LOG_ERR("average = 100, return\n");
				return;
			}
		}
		else
		{
			if (average > 0)
			{
				average -= aul_dim[ul_cnt];
				if (average >= 100 || average <= 0)
				{
					average = 0;
				}
			}
			else
			{		
				ENNO_LOG_ERR("average = 0, return\n");
				return;
			}
		}
	
	//	qDebug()<<"3end average="<<average;
		//dim_text->setText(tr("%1") .arg(average));
			
		for (i = 0; i < i_light_num; i++)
		{
			if (g_thread != NULL)
			{
				ast_dev[i].on = 1;
				if (ast_dev[i].device_type == DEV_DMX_LIGHT)
				{
					//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
					//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_RGB, 0xffffff);
				}
				else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
				{
					//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
				}
				else
				{
					ast_dev[i].dim_value = average;
	
					if (average >= 10 && average <= 100)
					{
						i_last_val = average;
					}
					qDebug()<<"i_last_val="<<i_last_val<<"i="<<i<<",light dim device_id="<<ast_dev[i].device_id<<", value="<<ast_dev[i].dim_value;
					   // dim_bar->setValue(ast_dev[i].dim_value);
					
					//dim_text->setText(tr("%1") .arg(ast_dev[i].dim_value));
					//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, average); 
					//mutex.lock();
					//sem_dim.acquire(1); 
					//while(dim_sem != 0)
					//{
					//	  sleep(1);
					//	qDebug()<<"dim_sem="<<dim_sem;
					//}
					//dim_sem = 1;	
					//qDebug()<<"set dim_sem="<<dim_sem;
									//qDebug()<<"usleep 250 start";
					//usleep(250000);
					//qDebug()<<"usleep 250 end";
				}
			}
			else
			{
				qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
			}
		}
	
		//mutex.lock();
		ENNO_LOG_ERR("start singleshot, r_index=%d, val=%d\n", r_index, average);
	#if 1
	   // QTimer::singleShot(50, this, SLOT(single_dim_paint1()));
		QTimer::singleShot(150, this, SLOT(single_dim_paint2()));
		//QTimer::singleShot(150, this, SLOT(single_dim_paint3()));
		QTimer::singleShot(300, this, SLOT(single_dim_paint4()));
		//QTimer::singleShot(50, this, SLOT(single_dim_paint1()));
		
		QTimer::singleShot(450, this, SLOT(single_dim_paint0()));
	#endif
		QTimer::singleShot(460, this, SLOT(single_send_light_dim()));
	
	//emit closeRoomlightDim();
    //timer_dim_out->stop();
#if 0
    if (dim_per <= 5)
    {
		if (last_value > average)
	    {
	        current_value = last_value - dim_per*(last_value - average)/5;
	    }
		else if (last_value < average)
		{
	        current_value = last_value + dim_per*(average - last_value)/5;
		}
		ENNO_LOG_ERR("dim_per=%d, current_value=%d\n", dim_per, current_value);

        if (dim_per == 5)
        {
			dim_text->setText(tr("%1") .arg(average));
			if (average == 0)
			{
				//dim_text->move(130, 105);
				//dim_percent->move(170, 125);
				dim_text->move(130, 105);
				dim_percent->move(170, 125);
			}
			else if (average == 100)
			{
				//dim_text->move(103, 105);
				//dim_percent->move(225, 125);
				dim_text->move(90, 105);
				dim_percent->move(212, 125);
			}
			else if (average > 0 && average < 100)
			{
				//dim_text->move(100, 105);
				//dim_percent->move(185, 125);
				dim_text->move(110, 105);
				dim_percent->move(190, 125);
			}
			
			last_value = average;
        }
    }
	
	dim_per++;
    repaint();

	if (dim_per > 5)
	{
	    timer_dim_flash->stop();
	}
	#endif
}

/*int RoomLightDimWidget::lightNum()
{
    return i_light_num;
}
*/
enno_result RoomLightDimWidget::set_light_dim_value(uint32 val)
{
    int i;
	int set_val;

    if (val > 100 || val < 0)
    {
        set_val = i_last_val;
		qDebug()<<"set_light_dim_value error="<<val;
    }
	else
	{
	    set_val = val;
	}
	
    qDebug()<<"set_light_dim_value val="<<val<<",setval="<<set_val;

    for (i = 0; i < i_light_num; i++)
	{
		if (ast_dev[i].device_type != DEV_DMX_LIGHT && ast_dev[i].device_type != DEV_KNX_LIGHT_SWITCH)
		{
		    ast_dev[i].dim_value = set_val;
		}
	}

	average = set_val;
	current_value = average;
	last_value = current_value;

	QTimer::singleShot(1, this, SLOT(single_dim_paint0()));
	//qDebug()<<"repaint";
	//repaint();

	return ENNO_OK;
}

void RoomLightDimWidget::single_send_light_dim()
{
    int i;

	//qDebug()<<"single_send_light_dim in";
	//mutex.unlock();
	//dim_sem = 0;
	//qDebug()<<"clear dim_sem="<<dim_sem;

	if (last_value == average)
	{
	    if (last_value != 0)
	    {
		    return;
		}
	}

//	qDebug()<<"send value average="<<average;
	for (i = 0; i < i_light_num; i++)
	{
		if (ast_dev[i].device_type == DEV_DMX_LIGHT)
		{
			g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
			g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_BRIGHTNESS, average);
		}
		else
		{		
	        g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, average);
		}
	}

	last_value = average;
}

enno_result RoomLightDimWidget::light_dim(char *buf_in, uint32 len_buf)
{
    char *buf;
	static uint32 last_step = 2;
	uint32 step;
	//int aul_dim[6] = {10, 5, 3, -3, -5, -10}; //调速模式
	//int aul_dim[6] = {1, 1, 1, 1, 1, 1};
	int aul_dim[6] = {10, 10, 10, 10, 10, 10};
	//int aul_dim[6] = {1, 2, 2, 2, 3, 3};
	static int ul_cnt = 0;
	char tmpbuf[32];
	uint32 device_id;
	uint32 len_use;
	uint32 len;
	uint32 last_dim;
	uint32 dimmer;
	uint32 sum = 0;
	uint32 sum_cnt = 0;
	uint32 cnt = 0;
	uint32 aulData[2];
	static int val_cnt[2]={0,0};
    int64_t current_time = 0;
	static int64_t last_time = 0;
	int64_t dim_time = 0;
	static int64_t last_dim_time = 0;
	uint32 i;
	uint32 j;
	enno_result ret;

    ENNO_LOG_ERR("light_dim, time=%lld\n", GetTickCount());
	timer_dim_out->start();
	
    ret = enno_decode_uint((void *)buf_in, (uint32*)&step, &len, len_buf);
    ENNO_CHECK_RET(ret);

	if (i_light_num <= 0)
	{
	    qDebug()<<"i_light_num<=0 dim return";

		return ENNO_OK;
	}

	//ENNO_LOG_ERR("step=%d\n", step);
	current_time = GetTickCount();
	if (current_time - last_time >= 1000)
	{
	    ul_cnt = 0;
		//printf("recnt, cu=%lld, la=%lld, v=%lld\n", current_time, last_time, current_time-last_time);
		val_cnt[0] = 0;
		val_cnt[1] = 0;
	}
	else
	{
	    if (ul_cnt <= 4)
	   	{
	        ul_cnt++;
	    }
	//	printf("ul_cnt=%d,aul=%d\n", ul_cnt, aul_dim[ul_cnt]);
		if (!step)
		{
		    val_cnt[1]++;
		}
		else
		{
		    val_cnt[0]++;
		}
	}
	last_time = current_time;

    // 就算加权值。
	if (val_cnt[0] > val_cnt[1])
	{
	    if (step == 0 && last_step != 0) 
	    {
	        last_step = step;
	        ul_cnt = 0;
	        printf("step1, cnt0=%d,cnt1=%d\n", val_cnt[0], val_cnt[1]);
	        return ENNO_OK;
	    }
		else if (step == 0 && last_step == 0)
		{
		    val_cnt[0] = 0;
		    return ENNO_OK;
		}
	}
	else
	{
	    if (step == 1 && last_step != 1)
	    {
	        last_step = step;
	        ul_cnt = 0;
	        printf("step0, cnt0=%d,cnt1=%d\n", val_cnt[0], val_cnt[1]);
	        return ENNO_OK;
	    }
		else if (step == 1 && last_step == 1)
		{
		    val_cnt[1] = 0;
		    return ENNO_OK;
		}
	}
	if (!step)//顺时针
	{
		if (average >= 100)
		{
			ENNO_LOG_ERR("average = 100, return\n");
			return ENNO_OK;
		}
	}
	else
	{
		if (average == 0)
		{	
			ENNO_LOG_ERR("average = 0, return\n");
			return ENNO_OK;
		}
	}
	
	last_step = step;
	#if 0
	if(!timer_dim_flash->isActive())
	{
		timer_dim_flash->start();
		ENNO_LOG_ERR("timer_dim_flash->start\n");
	}

	
	array_step_buf[w_index] = step;
    w_index++;
	if (w_index >= MAX_STEP_INDEX)
	{
	    w_index = 0;
	}

	ENNO_LOG_ERR("w_index=%d, step=%d\n", w_index, array_step_buf[w_index]);

	return ENNO_OK;
	#endif
#if 0	
	dim_time = GetTickCount();
	ENNO_LOG_ERR("dim_time=%lld, last_time=%lld, ms=%lld\n", 
		dim_time, last_dim_time, dim_time - last_dim_time);
	if (dim_time - last_dim_time <= slider_speed_time)
	{
	    //ENNO_LOG_ERR("sleep %lld ms\r\n", 500 - (dim_time - last_dim_time));
	    //usleep((500 - (dim_time - last_dim_time))*1000);
	    //ENNO_LOG_ERR("sleep ms end");
	    ENNO_LOG_ERR("sleep %lld ms\r\n", dim_time - last_dim_time);
	    return ENNO_OK;
	}
	last_dim_time = GetTickCount();
#endif
    sum = 0;
	for (i = 0; i < i_light_num; i++)
	{
	    
		if (ast_dev[i].device_type != DEV_DMX_LIGHT && ast_dev[i].device_type != DEV_KNX_LIGHT_SWITCH)
		{
		    sum += ast_dev[i].dim_value;
			sum_cnt++;
			
		   // ENNO_LOG_ERR("i=%d, value=%d\n", ast_dev[i].dim_value, sum);
		}
	}
	average = ((sum/sum_cnt)/10)*10;
	
	qDebug()<<"1sum="<<sum<<", cnt="<<sum_cnt<<", average="<<average;
	if (average >= 100)
	{
	    average = 100;
	}
	else if (average <= 0)
	{
	    average = 0;
	}

	//qDebug()<<"2sum="<<sum<<",i_light_num="<<i_light_num<<",average="<<average;

	if (!step)//顺时针
	{
		if (average < 100)
		{
			average += aul_dim[ul_cnt];
			if (average >= 100)
			{
				average = 100;
			}
		}
		else
		{
		    ENNO_LOG_ERR("average = 100, return\n");
		    return ENNO_OK;
		}
	}
	else
	{
		if (average > 0)
		{
			average -= aul_dim[ul_cnt];
			if (average >= 100 || average <= 0)
			{
				average = 0;
			}
		}
		else
		{		
		    ENNO_LOG_ERR("average = 0, return\n");
		    return ENNO_OK;
		}
	}

//	qDebug()<<"3end average="<<average;
	//dim_text->setText(tr("%1") .arg(average));
		
	for (i = 0; i < i_light_num; i++)
	{
		if (g_thread != NULL)
		{
			ast_dev[i].on = 1;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_RGB, 0xffffff);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
			    ast_dev[i].dim_value = average;

				if (average >= 10 && average <= 100)
				{
					i_last_val = average;
				}
			//	qDebug()<<"i_last_val="<<i_last_val<<"i="<<i<<",light dim device_id="<<ast_dev[i].device_id<<", value="<<ast_dev[i].dim_value;
				//dim_bar->setValue(ast_dev[i].dim_value);
				
				//dim_text->setText(tr("%1") .arg(ast_dev[i].dim_value));
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, average); 
				//mutex.lock();
				//sem_dim.acquire(1); 
				//while(dim_sem != 0)
				//{
				//    sleep(1);
				//	qDebug()<<"dim_sem="<<dim_sem;
				//}
				//dim_sem = 1;	
				//qDebug()<<"set dim_sem="<<dim_sem;
								//qDebug()<<"usleep 250 start";
				//usleep(250000);
				//qDebug()<<"usleep 250 end";
			}
		}
		else
		{
			qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
		}
	}

	//mutex.lock();
	//ENNO_LOG_ERR("start singleshot\n");
	#if 1
   // QTimer::singleShot(50, this, SLOT(single_dim_paint1()));
	QTimer::singleShot(slider_speed_time/3, this, SLOT(single_dim_paint2()));
	//QTimer::singleShot(150, this, SLOT(single_dim_paint3()));
	QTimer::singleShot(slider_speed_time*2/3, this, SLOT(single_dim_paint4()));
	//QTimer::singleShot(50, this, SLOT(single_dim_paint1()));
	
	QTimer::singleShot(slider_speed_time, this, SLOT(single_dim_paint0()));
	#endif
	QTimer::singleShot(slider_speed_time, this, SLOT(single_send_light_dim()));

	
#if 0
	for (i = 0; i < i_light_num; i++)
	{
		if (!step)//顺时针
	   	{
	        if (ast_dev[i].dim_value < 100)
	        {
	            ast_dev[i].dim_value += aul_dim[ul_cnt];
				if (ast_dev[i].dim_value >= 100)
				{
				    ast_dev[i].dim_value = 100;
				}
	        }
			else
			{
			    continue;
			}
	    }
		else
		{
		    if (ast_dev[i].dim_value > 0)
		    {
		        ast_dev[i].dim_value -= aul_dim[ul_cnt];
				if (ast_dev[i].dim_value >= 100 || ast_dev[i].dim_value <= 0)
				{
				    ast_dev[i].dim_value = 0;
				}
		    }
			else
			{
			    continue;
			}
		}

		if (g_thread != NULL)
		{
		    ast_dev[i].on = 1;
			if (ast_dev[i].device_type == DEV_DMX_LIGHT)
			{
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_ON, ast_dev[i].on);
				//g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_DMX_RGB, 0xffffff);
			}
			else if (ast_dev[i].device_type == DEV_KNX_LIGHT_SWITCH)
			{
			    //g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_SW_ON, ast_dev[i].on);
			}
			else
			{
			    qDebug()<<"light dim device_id="<<ast_dev[i].device_id<<", value="<<ast_dev[i].dim_value;
				//dim_bar->setValue(ast_dev[i].dim_value);
				
				dim_text->setText(tr("%1") .arg(ast_dev[i].dim_value));
				g_thread->msg_dev_ctrl(ast_dev[i].device_id, CHAR_LIGHT_BRIGHTNESS, ast_dev[i].dim_value); 
			}
		}
		else
		{
		    qDebug()<<"[qterror],lighton g_thread is NULL"<<endl;
		}
	}
#endif

   // qDebug()<<"update dim";
    
	//update();

	return ENNO_OK;
}

#if 0
int RoomLightDimWidget::light_dim(char *buf_in, uint32 len_buf)
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
#endif


void RoomLightDimWidget::single_dim_paint0()
{
    //qDebug()<<"per="<<0;
 /*   if (last_value > average)
    {
        current_value = last_value - (last_value - average);
    }
	else if (last_value < average)
	{
        current_value = last_value + (average - last_value);
	}
*/
    //mutex.unlock();
	current_value = average;
	
	dim_text->setText(tr("%1") .arg(average));
	if (average == 0)
	{
		//dim_text->move(130, 105);
		//dim_percent->move(170, 125);
		dim_text->move(130, 105);
		dim_percent->move(170, 125);
	}
	else if (average == 100)
	{
		//dim_text->move(103, 105);
		//dim_percent->move(225, 125);
		dim_text->move(90, 105);
		dim_percent->move(212, 125);
	}
	else if (average > 0 && average < 100)
	{
		//dim_text->move(100, 105);
		//dim_percent->move(185, 125);
		dim_text->move(110, 105);
		dim_percent->move(190, 125);
	}

    //repaint();
    
//	if (paint_value != current_value)
//	{
	    update();
//	}
}
void RoomLightDimWidget::single_dim_paint1()
{
    
   // qDebug()<<"per="<<1;
    if (last_value > average)
    {
        current_value = last_value - (last_value - average)/5;
    }
	else if (last_value < average)
	{
        current_value = last_value + (average - last_value)/5;
	}

	if (paint_value != current_value)
	    update();

   // repaint();
}
void RoomLightDimWidget::single_dim_paint2()
{
    qDebug()<<"per="<<2<<"r_index="<<r_index<<"current_value="<<current_value<<",average="<<average;
    if (last_value > average)
    {
        current_value = last_value - 2*(last_value - average)/5;
    }
	else if (last_value < average)
	{
        current_value = last_value + 2*(average - last_value)/5;
	}
	
	if (paint_value != current_value)
	    update();

   // repaint();
}
void RoomLightDimWidget::single_dim_paint3()
{
   // qDebug()<<"per="<<3;
    if (last_value > average)
    {
        current_value = last_value - 3*(last_value - average)/5;
    }
	else if (last_value < average)
	{
        current_value = last_value + 3*(average - last_value)/5;
	}

	
	if (paint_value != current_value)
	    update();

   // repaint();
}
void RoomLightDimWidget::single_dim_paint4()
{
    qDebug()<<"per="<<4<<"r_index="<<r_index;
    if (last_value > average)
    {
        current_value = last_value - 4*(last_value - average)/5;
    }
	else if (last_value < average)
	{
        current_value = last_value + 4*(average - last_value)/5;
	}
	
	if (paint_value != current_value)
	    update();

    //repaint();
}

void RoomLightDimWidget::paintEvent(QPaintEvent *e)
{
    double d_val;
	float m_alpha;
	int i;
	int total_t = 20;
	int width = this->width();
    int height = this->height();
    side = 20;
	outRadis = 300;
	
    QPainter painter(this);

	QImage image1;

    if (current_value == 0)
    {
        image1 = QPixmap("icon/light_dim_off.png").toImage();	
    }
	else
	{
	    image1 = QPixmap("icon/light_dim_on.png").toImage();	
	}
	
	qDebug()<<"paint cv="<<current_value;	

	QRect target(116.0, 160.0, 101.0, 101.0); //建立目标矩形，该区域是显示图像的目的地
	QRect source(0.0, 0.0, 101.0, 101.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域

    m_alpha = current_value;
	paint_value = current_value;

	g_current_dim = current_value;
	
	m_alpha = m_alpha/100;
	m_alpha = m_alpha/2;
	m_alpha = 0.5 + m_alpha;

	painter.setOpacity(m_alpha);
	//qDebug()<<"opacity="<<m_alpha;

	paintSide(&painter);
	paintOutE(&painter);
	//paintValue(&painter);//绘制value对应的填充角度

	radiusWidth = 30;
	shadowWidth = 10;
	shadowColor = QColor(240, 220, 0);

	drawArc(&painter, 140);
		
	painter.drawImage(target,image1,source);

	if (i_light_num <= 0)
	{
	    return;
	}
		
	
 #if 0
	painter.setOpacity(1);
	//QPen pen=painter.pen();
	//pen.setColor(QColor(255,255,255,20));	
    QPen pen(Qt::white, 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	//painter.setPen(QPen(QColor(79,79,79,80)));
	pen.setColor(QColor(85,85,85));


	painter.setPen(pen);
	
	painter.drawEllipse(15, 15, 290, 290);

    QPen pen1(Qt::white, 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	//painter.setPen(QPen(QColor(79,79,79,80)));
	pen1.setColor(QColor(240, 200, 0));
	painter.setPen(pen1);
	
	QRectF rect(15, 15, 290, 290); //x,y,w,h  
    int spanAngle = 90 * 16; //值为，实际角度 * 16  
    d_val = average;
	qDebug()<<"d_val="<<d_val<<", dm="<<average;
    int startAngle = (d_val / 100.0) * 360.0;  
	int endAngle;

	qDebug()<<"startAngle="<<startAngle;

  
	if (startAngle < 90)
	{
	    startAngle = 90 - (d_val/10.0) * 36.0;
	    qDebug()<<"sAngle1="<<startAngle;
		endAngle =  16 * 36.0 * (d_val/10.0);

		if (endAngle == 0)
		{	    
			painter.drawArc(rect, startAngle * 16, 0);
		}
		else
		{		
			painter.drawArc(rect, startAngle * 16, endAngle);
		}
		qDebug()<<"val="<<d_val<<", start="<<startAngle<<",end="<<spanAngle/16;
	}
	else
	{
	    painter.drawArc(rect, 1 * 16, 89 * 16);
		
		qDebug()<<"1val="<<d_val<<", start="<<1<<",end="<<spanAngle/16;
		startAngle = 360 - ((d_val/10.0) * 36.0 - 90);
		qDebug()<<"sAngle2="<<startAngle;
		endAngle = (360-startAngle)*16;
		
	    //painter.drawArc(rect, startAngle*16, (360-startAngle)*16);
		painter.drawArc(rect, startAngle * 16, endAngle);
		qDebug()<<"val="<<d_val<<", start="<<startAngle<<",end="<<spanAngle/16;
		qDebug()<<"2val="<<d_val<<", start="<<startAngle<<",end="<<360;
	}
#endif      
    //三个参数：rect表示弧线所在的矩形，startAngle起始角度，spanAngle跨越角度  
    
	/*if (average == 0)
	{
		dim_text->move(130, 105);
		dim_percent->move(170, 125);
		//qDebug()<<"per move to 180, 140, dim="<<ast_dev[0].dim_value;
		//dim_percent->setGeometry(QRect(180, 140, 60, 60));
	}
	else if (average == 100)
	{
	    dim_text->move(103, 105);
		dim_percent->move(225, 125);
		//qDebug()<<"per move to 220, 140, dim="<<ast_dev[0].dim_value;
		//dim_percent->move(QRect(180, 140, 60, 60));
	}
	else if (average > 0 && average < 100)
	{
	    dim_text->move(100, 105);
		dim_percent->move(185, 125);
		//qDebug()<<"per move to 180, 140, dim="<<ast_dev[0].dim_value;
	}*/

}

void RoomLightDimWidget::changeLanguage()
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
void RoomLightDimWidget::paintSide(QPainter *painter) //out side
{
    int radis = outRadis-side;
    QRect rect(20, 20, 280, 280);
    painter->save();
    painter->setBrush(QBrush(QColor("#868686")));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(rect);
    painter->restore();
}

void RoomLightDimWidget::paintOutE(QPainter *painter) //in side
{
    int radis = outRadis-2*(side+30);
    QRect rect(50, 50, 220, 220);
    painter->save();
    painter->setBrush(QBrush(QColor("#000000")));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(rect);
    painter->restore();
}
void RoomLightDimWidget::drawArc(QPainter *painter, int radius)
{
    double d_val;
    int angle = ((double)current_value/100)*360;
    painter->save();
    painter->setPen(Qt::NoPen);

    int smallradius = radius - radiusWidth;
    int maxRaidus = radius + shadowWidth;
    int minRadius = smallradius - shadowWidth;

    //采用圆形渐变,形成光晕效果
    QRadialGradient radialGradient(QPointF(160, 160), maxRaidus);
    QColor color = shadowColor;
    QColor lightColor = shadowColor.lighter(100);

    color.setAlphaF(0);
    radialGradient.setColorAt(0, color);
    radialGradient.setColorAt(minRadius * 1.0 / maxRaidus, color);
    color.setAlphaF(0.5);
    radialGradient.setColorAt(smallradius * 1.0 / maxRaidus, color);

    radialGradient.setColorAt((smallradius + 1) * 1.0 / maxRaidus, lightColor);
    radialGradient.setColorAt((radius - 1) * 1.0 / maxRaidus, lightColor);
    radialGradient.setColorAt(radius * 1.0 / maxRaidus, color);
    color.setAlphaF(0);
    radialGradient.setColorAt(1, color);

    painter->setBrush(radialGradient);
	
    //painter->drawPie(-maxRaidus, -maxRaidus, maxRaidus * 2, maxRaidus * 2, 270 * 16, angle * 16);
	
	QRectF rect(15, 15, 290, 290); //x,y,w,h  
    int spanAngle = 90 * 16; //值为，实际角度 * 16  
    d_val = current_value;
    int startAngle = (current_value / 100.0) * 360.0;  
	int endAngle;

	//qDebug()<<"startAngle="<<startAngle;
  
	if (startAngle < 90)
	{
	    startAngle = 90 - (d_val/10.0) * 36.0;
	   // qDebug()<<"sAngle1="<<startAngle;
		//endAngle =  36.0 * (d_val/10.0);
		
		endAngle = 90 - startAngle;

		if (endAngle == 0)
		{	    
			painter->drawPie(rect, startAngle * 16, 0);
		}
		else
		{		
			painter->drawPie(rect, startAngle * 16, endAngle * 16);
		}
		//qDebug()<<"val="<<d_val<<", start="<<startAngle<<",end="<<spanAngle/16;
	}
	else
	{
	    painter->drawPie(rect, 1 * 16, 89 * 16);
		
		//qDebug()<<"1val="<<d_val<<", start="<<1<<",end="<<spanAngle/16;
		startAngle = 360 - ((d_val/10.0) * 36.0 - 90);
		//qDebug()<<"sAngle2="<<startAngle;
		endAngle = (361-startAngle)*16;
		
	    //painter.drawArc(rect, startAngle*16, (360-startAngle)*16);
		painter->drawPie(rect, startAngle * 16, endAngle);
		//qDebug()<<"val="<<d_val<<", start="<<startAngle<<",end="<<spanAngle/16;
		//qDebug()<<"2val="<<d_val<<", start="<<startAngle<<",end="<<360;
	}
    painter->restore();
}

void RoomLightDimWidget::paintValue(QPainter *painter)
{
    //int outRadis = 310;
	int inRadis = 280;
    int oRandis = outRadis-side-30;
   // qDebug()<<"value::"<<current_value;
    int rangle = ((double)current_value/100)*360;
	QPoint startPt(35, 35);
	
	QRectF rect(startPt.x(), startPt.y(), 250, 250);
	int arcR = rect.width()/2;
	
    //QRectF rect(-oRandis, -oRandis, oRandis << 1, oRandis << 1);
    QPainterPath path;
   // qDebug()<<"rangle:"<<rangle<<" start:"<<0-rangle;

	if (rangle == 0)
	{
	    return;
	}
	path.moveTo(startPt.x() + arcR, startPt.y() + arcR);
    path.arcTo(rect, 90-rangle, rangle);

    // QRectF(-120, -120, 240, 240)
    QPainterPath subPath;
    //subPath.addEllipse(rect.adjusted((oRandis-inRadis),(oRandis-inRadis),-(oRandis-inRadis),-(oRandis-inRadis)));
	subPath.addEllipse(rect.adjusted(1, 1, -1, -1));

	QPen pen(Qt::white, 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		//painter.setPen(QPen(QColor(79,79,79,80)));
	pen.setColor(QColor(240, 200, 0));

    // path为扇形 subPath为椭圆
    path -= subPath;
    painter->save();
    painter->setBrush(QColor(85, 85, 85, 30));
    painter->setPen(pen);
    painter->drawPath(path);
    painter->restore();
}


