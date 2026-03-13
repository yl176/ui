#include "content_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QApplication>
//#include "qanimationstackedwidget.h"
//#include "sliderpicture.h"
#include "treeitem.h"
#include "wthread.h"
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QPixmap>
#include <QBitmap>
#include "status_xml.h"

#pragma execution_character_set("utf-8")
#define SCENE_ANIMA_GROUP 1
static int g_blur_cnt = 1;
static int g_blur_dir = 1;
static int g_stop_flag = 0;

int light_dim_press;
int g_do_scene_flag = 0;
extern int shade_dim_press;
int g_current_index;
QElapsedTimer g_click_timer;  
uint32 g_sid_index[MAX_SCENE];
extern int g_no_shake;
#define IMG_POS_Y 155

//#define SCENE_BUTTON_X 105 //110
//#define SCENE_BUTTON_Y 105

#define SCENE_ZOOM_OUT 115

void SceneButton::enterEvent(QEvent *)
{
	mouse_over = true;
}

void SceneButton::leaveEvent(QEvent *)
{
	mouse_over = false;
}
void SceneButton::sceneCtrl(int on)
{
	extern WThread *g_thread;
	if (stScene == NULL)
	{
	    //qDebug()<<"tool button stScene is NULL"<<endl;
	}
	//qDebug()<<"sceneCtrl, scene_id="<<stScene->scene_id<<", cmd="<<on<<endl;
	if (g_thread != NULL)
	{
	     g_thread->msg_scene_ctrl(on, stScene->scene_id);
	}
	else
	{
	    //qDebug()<<"[qterror], g_thread is NULL"<<endl;
	}
}

void SceneButton::setImg(QString str_img)
{
    //qDebug()<<"set Img ="<<str_img;
    m_img = str_img;
}
void SceneButton::setIndex(int index)
{
    m_index = index;
}
void SceneButton::setLocalPos(int x, int y)
{
    m_pos_x = x;
	m_pos_y = y;
}
void SceneButton::getLocalPos(int *p_x, int *p_y)
{
    *p_x = m_pos_x;
	*p_y = m_pos_y;
}

void SceneButton::devCtrl(int setpos)
{
	extern WThread *g_thread;
	if (stCurtain == NULL)
	{
	    //qDebug()<<"tool button stCurtain is NULL"<<endl;
		return;
	}
	//qDebug()<<"stCurtain, curtain_id="<<stCurtain->device_id<<", target_pos="<<setpos<<endl;
	if (g_thread != NULL)
	{
	     if(stCurtain->device_type == DEV_KNX_WINDOW)
	     {
	         g_thread->msg_dev_ctrl(stCurtain->device_id, CHAR_SW_ON, setpos?1:0);
	     }
		 else
		 {
		     g_thread->msg_dev_ctrl(stCurtain->device_id, CHAR_SHADE_TARGET_POSITION, setpos);
		 }	     
	}
	else
	{
	   //qDebug()<<"[qterror], g_thread is NULL"<<endl;
	}
}

void SceneButton::mousePressEvent(QMouseEvent *event)
{
    if (g_do_scene_flag)
    {
        return;
    }
	
#ifdef MOTOR_ENABLE
	Util::UI_motor_shake(1);
#endif

	//qDebug()<<"1 SceneButton pressevent"<<endl;
	g_click_timer.restart();
#if 0
	QImage img;
    img.load(m_img);
	//*img = img->scaled(130,130,Qt::KeepAspectRatio);
    this->setIcon(QPixmap::fromImage(img));
    this->setIconSize(QSize(SCENE_ZOOM_OUT,SCENE_ZOOM_OUT));
    this->update();
#endif	
    light_dim_press = 1;
    shade_dim_press = 1;
	g_current_index = m_index;
	mouse_is_move = 0;
	button_press_pos = event->pos();

	//QTimer::singleShot( 5*100, the_parent, SLOT(light_dim_check()));
	emit pressIcon();

    QToolButton::mousePressEvent(event);
	QWidget::mousePressEvent(event);
}

void SceneButton::mouseMoveEvent(QMouseEvent *event)
{
    if (g_do_scene_flag)
    {
        return;
    }

	button_move_pos = event->pos();
    //printf("1、Scene mouseMove, src(%d,%d), dst(%d,%d), move(%d,%d)\n", 
	//       button_press_pos.x(), button_press_pos.y(), button_move_pos.x(), button_press_pos.y(),
	//       button_move_pos.x() - button_press_pos.x(), button_move_pos.y() - button_press_pos.y()); 
	if (button_move_pos.x() - button_press_pos.x() >= BUTTON_MOVE_THD
		|| button_press_pos.x() - button_move_pos.x() >= BUTTON_MOVE_THD)
	{
	    mouse_is_move = 1;
	}
	
    QWidget::mouseMoveEvent(event);
}

void SceneButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (g_do_scene_flag)
    {
        return;
    }

//    qDebug()<<"SceneButton::mouseReleaseEvent";
	//light_dim_press = 0;
	//shade_dim_press = 0;
	QImage img;
	//img->load(string_list2.at(ast_dev[i].image_id));
	img.load(m_img);
	
	//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
	this->setIcon(QPixmap::fromImage(img));
	this->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
	
				
	QToolButton::mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}


/*void SceneButton::mouseReleaseEvent(QMouseEvent *event)
{
	qDebug()<<"SceneButton releaseevent"<<endl;
	
}*/

SceneButton::SceneButton(QWidget *parent)
	:QToolButton(parent)
{
	mouse_over = false;
	mouse_press = false;
}

SceneButton::~SceneButton()
{

}
ContentWidget::ContentWidget(int room_id, TreeModel *model, QWidget *parent)
{
	QList<QVariant> columnData;
	int i;
	int j;
	i_scene_num = 0;
	i_scene_cnt = 0;
	light_flag = 0;
	static int s_scene_id = 0;
	wait_for_status = 0;
	do_scene_flag = 0;
	page_num = 0;
	//dim_value = 0;
	button_select = 0;
	light_dim_press = 0;
	//img_effect = new QGraphicsBlurEffect(this);
	group_light_anima = new QParallelAnimationGroup;
	group_scene_anima = new QParallelAnimationGroup;
	group_move_widget = new QParallelAnimationGroup;
	char*  ch;

	isAnimation = false;
    duration = 200;
    animation = new QPropertyAnimation(this, QByteArray());
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()), this, SLOT(animationFinished()));

	timer_exe = new QTimer();
	//timer_off->setInterval(60000);
    timer_exe->setInterval(TIMER_RATAT_MS);
    //timer_exe->setInterval(60000);
    timer_exe->stop();

	connect(timer_exe, SIGNAL(timeout()), this, SLOT(onTimerExe()));

    if (s_scene_id == 0)
    {
        memset(g_sid_index, 0xff, MAX_SCENE);
    }

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(640,320);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	content = new QWidget(this);
	content->setFixedSize(320,306);
	content->raise();

	QStringList string_list;
	QStringList string_list1;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
		

	string_list1<<"contentWidget/userdef_off_b"<<"contentWidget/morning_off_b"
		<<"contentWidget/enjoy_off_b"<<"contentWidget/night_off_b"
		<<"contentWidget/dinner_off_b"<<"contentWidget/read_off_b"
		<<"contentWidget/welcome_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/away_off_b"<<"contentWidget/metting_off_b"
		<<"contentWidget/relax_off_b"<<"contentWidget/bath_off_b"
		<<"contentWidget/party_off_b"<<"contentWidget/motion_off_b"
		<<"contentWidget/music_off_b"<<"contentWidget/muse_off_b"
	    <<"contentWidget/romantic_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/cooking_off_b"<<"contentWidget/save_off_b"
	    <<"contentWidget/purify_off_b";
		//<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add";

	//grid_layout = new QGridLayout();
	//grid_layout1 = new QGridLayout();
	//grid_layout2 = new QGridLayout();
	
	//wid_get = new sceneWidget(content);
	//wid_get1 = new sceneWidget(content);
	widget_light = new sceneWidget(this);
	wid_get_wait = new sceneWidget(this);

	wid_get_wait->setFixedSize(320,306);

	QModelIndex title;
	QModelIndex room;
	QModelIndex scene;
	
	//qDebug()<<"content widget"<<endl;

	room = model->index(room_id, 0, title);
	//qDebug() << "model->columnCount(room)="<<model->columnCount(room);
	for (i = 0; i < model->columnCount(room); i++)
	{
		QModelIndex room1 = model->index(0, i, title);
		QVariant columndata1 = model->data(room1, Qt::DisplayRole);
	//	qDebug()<<"index["<<room1.column()<<"]room["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
	}

	scene = model->index(0, 0, room);
	for (j = 0; j < model->rowCount(room); j++)
	{
	    QModelIndex scene1 = model->index(j, 2, room);
		QVariant columndata1 = model->data(scene1, Qt::DisplayRole);
		QString str_mode = columndata1.value<QString>();
		bool ok;  
        int i_mode = str_mode.toInt(&ok,10);
		//qDebug()<<"scene["<<j<<"]"<<",mode="<<columndata1.value<QString>()<<endl;	
		if (1 == ok && DEV_SCENE == i_mode)
		{	
		//	qDebug()<<"room child["<<j<<"] is a scene"<<endl;
			i_scene_num++;
		}
		else
		{ 
		    continue;
		}
		
		QList<QString> scene_value;
		for (i = 0; i < model->columnCount(scene); i++)
		{
			QModelIndex scene1 = model->index(j, i, room);
			columndata1 = model->data(scene1, Qt::DisplayRole);
		//	qDebug()<<"index=["<<scene1.column();
		//	qDebug()<<"scene["<<i<<"]"<<"="<<columndata1.value<QString>()<<endl;
			scene_value<<columndata1.value<QString>();
		}
		
		ast_dev[i_scene_cnt].m_Id = scene_value[0];
		ast_dev[i_scene_cnt].m_name = scene_value[1];
		ast_dev[i_scene_cnt].m_mode = scene_value[2];
		ast_dev[i_scene_cnt].value = scene_value[3];
		ast_dev[i_scene_cnt].speed = scene_value[4];
		ast_dev[i_scene_cnt].on = 0;

		if (ast_dev[i_scene_cnt].value.toInt(&ok,10) >= string_list.size())
		{
		    ENNO_LOG_ERR("image_id=%d, butsize=%d\n", 
				ast_dev[i_scene_cnt].value.toInt(&ok,10), string_list.size());
		    ast_dev[i_scene_cnt].image_id = 0;
		}
		else
		{
	    	ast_dev[i_scene_cnt].image_id = ast_dev[i_scene_cnt].value.toInt(&ok,10);
		}
		ast_dev[i_scene_cnt].scene_id = ast_dev[i_scene_cnt].m_Id.toInt(&ok,10);
		//g_sid_index[s_scene_id-1] = i_scene_cnt;
        #if 0
		char* ch;
		QByteArray ba = scene_value[1].toLocal8Bit(); // must
		ch = ba.data();
		//qDebug()<<"set_scene_name="<<ch;
        printf("set_scene_name name(%d)={", ba.size());
		for (int k = 0; k < ba.size(); k++)
		{
		    printf("0x%x ", ch[k]);
		}
		printf("}");
	    #endif		
		ast_dev[i_scene_cnt].dim_value = 0;
		i_scene_cnt++;
		if (i_scene_cnt >= MAX_SCENE)
		{
			ENNO_LOG_ERR("MAX_SCENE error\r\n");
		    return;
		}
	}

	signal_mapper = new QSignalMapper(this);
	//signal_mapper_light = new QSignalMapper(this);

    if (0 == i_scene_num)
    {
        QLabel *no_scene = new QLabel(content);
		no_scene->setText(tr("There's no scene here.\n"));
		//no_curtain->setStyleSheet("font-size:30px;color: white;border-width:2px;border-style:solid;border-color:black;font-weight:bold;text-align:left;");
		//no_curtain->setGeometry(QRect(0, 0, 320, 120));
		no_scene->setObjectName("noscene");
	wid_get[0]->setFixedSize(OLED_PIX_X*(page_num+2),306);
		label_scene_onoff[i]->setGeometry(QRect(OLED_PIX_X*(i+1), 150, OLED_PIX_X, 60));
		label_scene_name[i]->setGeometry(QRect(OLED_PIX_X*(i+1), 90, OLED_PIX_X, 60));
		label_scene_first_name[i]->setGeometry(QRect(OLED_PIX_X*(i+1), 30, OLED_PIX_X, 60));
    }

    page_num = (i_scene_num+3)/4;
    wid_get[0] = new sceneWidget(content);
	wid_get[0]->setFixedSize(320*(page_num+2),306);
	wid_get[0]->hide();
    for ( i = 0; i < page_num; i++)
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

		label_scene_first_name[i] = new QLabel(wid_get[0]);
		label_scene_first_name[i]->setGeometry(QRect(320*(i+1), 30, 320, 60));
		label_scene_first_name[i]->setStyleSheet("font-size:44px;font-weight:bold;color: white;text-align:left;background:transparent;");
		label_scene_first_name[i]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
		label_scene_first_name[i]->hide();
	}
    //qDebug()<<"i_scene_num="<<i_scene_num<<endl;
	for(i=0; i<i_scene_num; i++)
	{
	    widget_tmp = wid_get[0];
		SceneButton *tool_button = new SceneButton(widget_tmp);
		tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		int image_id = ast_dev[i].image_id ;

		if (image_id >= string_list.size())
		{
		   // qDebug()<<"error image_id = "<<image_id;
			//continue;
			image_id = 0;
		}
		QPixmap button_pixmap(string_list.at(image_id));
	    bool b = ast_dev[i].m_name.contains(QRegExp("[\\x4e00-\\x9fa5]+")); 
		QString str_name;
		
		if (b)
		{
			if (ast_dev[i].m_name.size() <= 2)
			{
				str_name = ast_dev[i].m_name;
			}
			else if (ast_dev[i].m_name.size() > 2 && ast_dev[i].m_name.size() <= 3)
			{
				str_name = ast_dev[i].m_name;
			}
			else
			{
				str_name = ast_dev[i].m_name.left(3) + QString("..");
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
		tool_button->setImg(string_list1.at(image_id));
		tool_button->setIcon(button_pixmap);
		
		tool_button->setIconSize(QSize(SCENE_BUTTON_X, SCENE_BUTTON_Y));
		//tool_button->setFixedSize(SCENE_BUTTON_X, SCENE_BUTTON_Y);

		tool_button->setObjectName("functionButton");
		tool_button->stScene = &ast_dev[i];
		tool_button->setIndex(i);
		button_state[i] = 0;
		
		button_list.append(tool_button);
		connect(tool_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
		
		signal_mapper->setMapping(tool_button, QString::number(i, 10));
		int buttom_offset_x =0;
		int buttom_offset_y =0;
		j = i%4;  //page icon id 0-3
		if(j%2==0) //
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
		if (i_scene_num == 1)
		{
		    tool_button->setGeometry(QRect(95+OLED_PIX_X, 95+2, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));//130
			tool_button->setLocalPos(95+OLED_PIX_X, 95+2);
		}

		connect(tool_button, SIGNAL(pressIcon()), this, SLOT(PressIcon()));

	}

	wid_get[0]->setButton(0);
	wid_get[0]->show();
	wid_get[0]->raise();
	startx = -OLED_PIX_X;
	starty = 0;

	wid_get[0]->move(startx, starty);

	widget_light->setFixedSize(320,120);

	timer_light_out = new QTimer();
    timer_light_out->setInterval(TIMER_LIGHT_OUT_MS);  //30s 

    connect(timer_light_out, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    label_light = new QLabel(widget_light);
	label_light->setGeometry(QRect(142, 0, 60, 30));

	QPixmap pixmap_light("contentWidget/light_0");
	label_light->setPixmap(pixmap_light);


/*	border-top-left-radius 设置左上角圆角;
	border-top-right-radius 设置右上角圆角;
	border-bottom-left-radius 设置左下角圆角;
	border-bottom-right-radius 设置右下角圆角;
	border-radius 设置四个角圆角;*/
	int L_len = 0;
	int R_len = 0;
	int C_x =0;
	int value = 0;
	value = 0;//ast_dev[current_index].dim_value;


	ProgressBar_L = new QLabel(widget_light);
	ProgressBar_L->setGeometry(QRect(0, 60, L_len, 44));
	ProgressBar_L->setStyleSheet("QWidget{background-color:rgb(255, 191, 37);"\
			"border-top-left-radius:20px;border-bottom-left-radius:20px;}");

	ProgressBar_R = new QLabel(widget_light);
	ProgressBar_R->setGeometry(QRect(L_len, 60, R_len, 44));
	ProgressBar_R->setStyleSheet("QWidget{background-color:rgb(150, 150, 150);"\
			"border-top-right-radius:20px;border-bottom-right-radius:20px;}");

	ProgressBar_C = new QLabel(widget_light);
	ProgressBar_C->setGeometry(QRect(C_x, 57, 50, 50));
	ProgressBar_C->setStyleSheet("QWidget{background-color:white;"\
			"border-radius:23px;}");
	ProgressBar_C->raise();

	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnIcon(QString)));

	wid_get_wait->hide();
	widget_light->hide();
	//wid_get2->move(0, 320);
	wid_get[0]->raise();
	//wid_get->raise();
	last_index = -1;
	//wid_get1->raise();
	//wid_get2->move(startx, starty); 
	current_Widget = wid_get[0];
	current_page = 0;
	next_page = -1;

	g_thread->msg_get_light_dimvalue(i_scene_num);
	this->translateLanguage();
	
	g_thread->msg_get_light_dimvalue(i_scene_num);
	content->show();

}
ContentWidget::~ContentWidget()
{
    delete timer_exe;
	delete content;
	//delete grid_layout;
	//delete grid_layout1;
	//delete wid_get;
	//delete wid_get1;
	delete widget_light;
	delete wid_get_wait;
	delete signal_mapper;
//	delete bar;
	delete label_light;
//	delete label_mask_L;
//	delete label_mask_R;
	for(int i = 0; i < button_list.count(); i++)
	{
		SceneButton *tool_button = button_list.at(i);
	    delete tool_button;
	}
}

void ContentWidget::translateLanguage()
{
	if (0 == i_scene_num && content)
    {
        QLabel * no_scene = content->findChild<QLabel*>("noscene");
		if(no_scene)
		{
		   no_scene->setText(tr("There's no scene here.\n")); 
		}		
    }
}
void ContentWidget::lightdim_to_scene_start()
{
    int pos_x;
	int pos_y;
	light_flag = 2;
	//group_light_anima = new QParallelAnimationGroup;
	group_light_anima->clear();
	
	QPropertyAnimation *anim = new QPropertyAnimation(button_list.at(current_index), "geometry");				
	anim->setDuration(200);
	button_list.at(current_index)->getLocalPos(&pos_x, &pos_y);
	
	qDebug()<<"lightdim_to_scene_start,pos_x="<<pos_x<<",pos_y="<<pos_y;
	anim->setKeyValueAt(0, QRect(80+OLED_PIX_X*(current_page+1), 80, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	anim->setKeyValueAt(1, QRect(pos_x, pos_y,SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
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
	
	QTimer::singleShot(200, this, SLOT(lightdim_to_scene()));

}
void ContentWidget::onTimerOut()
{
    if (!light_flag)
    {
		light_dim_press = 0;
		timer_light_out->stop();
        return;
    }
	
    if (current_Widget == widget_light)
	{	    
		light_dim_press = 0;
	    widget_light->move(startx, starty);
	   // wid_get->move(startx, starty);
       // wid_get1->move(startx, starty);
		widget_light->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;
		
	   /* QImage *img = new QImage;
	    img->load(button_select->m_img);
		//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
	    button_select->setIcon(QPixmap::fromImage(*img));
	    button_select->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
	    button_select->update();*/

		content->show();
		light_flag = 2;
		g_no_shake = 0;
        update();

		lightdim_to_scene_start();
		return;
	}
}
void ContentWidget::buttonClicked()
{
   // printf("button clicked!!!\n");
	QStringList string_list;
	string_list<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop"
		<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext"
		<<"contentWidget/first_add";
	QString map[2] = {"contentWidget/first_add", "contentWidget/recovery"};
	static int i = 0;
	QPixmap pixmap(map[i++%2]);
	scene_off();

	QObject *object = QObject::sender();
    QToolButton *tool_button = qobject_cast<QToolButton *>(object);

    //printf("button clicked!!!icon=%s\n", tool_button->icon());
	tool_button->setIcon(pixmap);
	tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));//
	
	tool_button->update();
}
void ContentWidget::setLightValue(int value)
{
   // qDebug()<<"bar->value()="<<bar->value();
    light_brightness = bar->value();
	last_dim = light_brightness;
	ast_dev[current_index].dim_value = last_dim;
	if (light_brightness == 100)
	{
		QPixmap pixmap_light("contentWidget/light_100");
		label_light->setPixmap(pixmap_light);
	}
	else
	{
		QPixmap pixmap_light("contentWidget/light_0");
		label_light->setPixmap(pixmap_light);
	}
		
    update();
	if (timer_light_out->isActive())
	{
		timer_light_out->start();
	}
}

void ContentWidget::scene_off()
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
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));//
		tool_button->update();
    }
}
#if 0
ContentWidget::ContentWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	this->setMaximumSize(320,640);
	this->setMinimumSize(0,0);
	this->setStyleSheet("background:red");

	
	QStringList string_list;
	string_list<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop"
		<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext";
		//<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add"<<"contentWidget/first_add";

    QSignalMapper *signal_mapper = new QSignalMapper(this);

	for(int i=0; i<string_list.size(); i++)
	{
		QToolButton *tool_button = new QToolButton(this);
		tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		QPixmap button_pixmap(string_list.at(i));
		tool_button->setIcon(button_pixmap);
		tool_button->setIconSize(button_pixmap.size());
		tool_button->setFixedSize(button_pixmap.width()+50, button_pixmap.height()+35);
		tool_button->setObjectName("functionButton");

		tool_button->setGeometry(QRect(70 + (i%2)*100, 70 + (i/2)*100, 50, 35));

		button_list.append(tool_button);
		connect(tool_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
		
		signal_mapper->setMapping(tool_button, QString::number(i, 10));
		//connect(tool_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	}
	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnIcon(QString)));
	this->show();
	this->translateLanguage();

}
#endif

void ContentWidget::changeLanguage()
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


void ContentWidget::light_dim_check()
{
   // printf("light_dim_check in, light_dim_press=%d\n", light_dim_press);
	//qDebug()<<"button_select.mouse_is_move="<<button_select->mouse_is_move;
    int last_x,last_y;

    return; //add by iovine
	
	if (g_click_timer.elapsed() < TIME_LIGHT_CLICK_MS)
	{
	    return;
	}
	
    if (light_dim_press == 1)
    {
        if (!button_select->mouse_is_move)
    	{
	        light_dim_press = 2;
        }
		else
		{
		    light_dim_press = 0;
			return;
		}
    }
	else
	{
	    return;
	}

	prev_Widget = current_Widget;
	current_Widget = widget_light;
	
    //qDebug()<<"set light_flag=1"<<endl;
	light_brightness = ast_dev[current_index].dim_value;
	last_dim = light_brightness;

	//bar->setValue(light_brightness);
	anim->setKeyValueAt(1, QRect(80+OLED_PIX_X*(current_page+1), 80, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
		  light_brightness = 100;
	  else if (light_brightness < 0)
		  light_brightness = 0;
	  
	set_progressbar_value(light_brightness);
	//qDebug()<<"set barval="<<bar->value();
	if (light_brightness == 100)
	{
		QPixmap pixmap_light("contentWidget/light_100");
		label_light->setPixmap(pixmap_light);
	}
	else
	{
	    QPixmap pixmap_light("contentWidget/light_0");
		label_light->setPixmap(pixmap_light);
	}
	//content->hide();
	group_light_anima->clear();
	//group_light_anima = new QParallelAnimationGroup;
	qDebug()<<"scene_to_lightdim scene";
	QPropertyAnimation *anim = new QPropertyAnimation(button_list.at(current_index), "geometry");				
	anim->setDuration(200);
	last_x = button_list.at(current_index)->pos().x();
	last_y = button_list.at(current_index)->pos().y();
	anim->setKeyValueAt(0, QRect(last_x, last_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	anim->setKeyValueAt(1, QRect(80+320*(current_page+1), 80, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));
	//anim->setKeyValueAt(1, QRect(last_x, last_y, SCENE_BUTTON_X, SCENE_BUTTON_Y));
	
	group_light_anima->addAnimation(anim);

	QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
	button_list.at(current_index)->setGraphicsEffect(effect);
	QPropertyAnimation *animation_bar = new QPropertyAnimation(effect, "blurRadius");

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
	QTimer::singleShot(200, this, SLOT(scene_to_lightdim()));
	//update();
	
	last_index = current_index;  
}
void ContentWidget::scene_to_lightdim()
{
	content->hide();

    widget_light->move(0, 200);
	widget_light->show();
	widget_light->raise();
	light_flag = 1;
	timer_light_out->start();
	update();
}

void ContentWidget::lightdim_to_scene()
{
    qDebug()<<"lightdim_to_scene";
	int pos_x, pos_y;
/*	widget_light->move(startx, starty);
	// wid_get->move(startx, starty);
	// wid_get1->move(startx, starty);
	widget_light->hide();
	prev_Widget->raise();
	current_Widget = prev_Widget;
*/
	content->show();

	QImage img;
	img.load(button_select->m_img);
	//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
	button_select->setIcon(QPixmap::fromImage(img));
	button_select->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
	
	button_select->setGraphicsEffect(NULL);
	//button_select->update();
	button_select->getLocalPos(&pos_x, &pos_y);
	button_select->setGeometry(QRect(pos_x, pos_y, SCENE_BUTTON_X+30, SCENE_BUTTON_Y+45));

	light_flag = 0;
	g_no_shake = 0;

	update();
}

void ContentWidget::PressIcon()
{
	bool ok;  
	current_index = g_current_index;
	QStringList string_list;
	QStringList string_list1;
	QStringList string_list2;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
		;

	string_list1<<"contentWidget/userdef_dim"<<"contentWidget/morning_dim"
		<<"contentWidget/enjoy_dim"<<"contentWidget/night_dim"
		<<"contentWidget/dinner_dim"<<"contentWidget/read_dim"
		<<"contentWidget/welcome_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/away_dim"<<"contentWidget/metting_dim"
		<<"contentWidget/relax_dim"<<"contentWidget/bath_dim"
		<<"contentWidget/party_dim"<<"contentWidget/motion_dim"
		<<"contentWidget/music_dim"<<"contentWidget/muse_dim"
	    <<"contentWidget/romantic_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/cooking_dim"<<"contentWidget/save_dim"
	    <<"contentWidget/purify_dim";
		;
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";
		;
	
	//printf("PressIcon, current_icon=%d\n", current_index);

    QTimer::singleShot(TIME_LIGHT_CLICK_MS, this, SLOT(light_dim_check()));
	//current_index = g_current_index;

	cur_button = string_list1.at(ast_dev[current_index].image_id);
	button_select = button_list.at(current_index);
#if 0
	//qDebug()<<"clicktime=["<<g_click_timer.elapsed()<<"]"<<endl;
	light_dim_press = 1;
	QTimer::singleShot( 5*100, this, SLOT(light_dim_check()));

	for(int i=0; i<button_list.count(); i++)
	{
		SceneButton *tool_button = button_list.at(i);
		if(current_index == i)
		{
		    button_select = tool_button;
			#if 0
		    //wait_for_status = 1;
			startA = 180;
		    startB = 360;
		    printf("tool_button->isChecked()=%d, i=%d\n", button_state[i], i);
			if (button_state[i] == 1)
			{
				qDebug()<<"g_click_timer.elapsed()="<<g_click_timer.elapsed()<<endl;
			    if (flag)
			    {  
			        continue;
			    }
			}
			
			//button_state[i] = !button_state[i];
		    if (!button_state[i])
		    {
		        //tool_button->setChecked(false);
			   // QPixmap pixmap(string_list2.at(i));
		    #if 1
			    QImage *img = new QImage;
			    img->load(string_list.at(ast_dev[i].image_id));
				*img = img->scaled(115,115,Qt::KeepAspectRatio);
			    tool_button->setIcon(QPixmap::fromImage(*img));
			    tool_button->setIconSize(QSize(115,115));
			    tool_button->update();
			#endif
				//enable current index, disable last index;
				/*if (last_index != -1)
				{
				    if (button_state[last_index] == 1 && last_index != current_index)
				    {
				        button_state[last_index] = 0;
				        tool_button->sceneCtrl(button_state[last_index]);
				    }
				}*/
		    }
			else 
			{

			    //tool_button->setChecked(true);
			    QPixmap pixmap(string_list.at(ast_dev[i].image_id));
			 //   tool_button->setIcon(pixmap);
			//    tool_button->setIconSize(pixmap.size());
			//    tool_button->update();
		    #if 1
			    QImage *img = new QImage;
			    img->load(string_list2.at(ast_dev[i].image_id));
				*img = img->scaled(115,115,Qt::KeepAspectRatio);
			    tool_button->setIcon(QPixmap::fromImage(*img));
			    tool_button->setIconSize(QSize(115,115));
			    tool_button->update();
			#endif
			}

		    wid_get[current_index/4]->setButton(tool_button);		   
			cur_button = string_list1.at(ast_dev[i].image_id);
			tool_button->sceneCtrl(!button_state[i]);
			#endif
		}
		
	}
	#if 0
	if (flag)
	{
	    //wid_get2->raise();
		prev_Widget = current_Widget;
		current_Widget = widget_light;
		//light_on_paint();
		//current_Widget->raise();
		
	    qDebug()<<"set light_flag=1"<<endl;
		
		light_brightness = ast_dev[current_index].dim_value;
		last_dim = light_brightness;
		bar->setValue(light_brightness);	
		qDebug()<<"set barval="<<bar->value();
		if (light_brightness == 100)
		{
			QPixmap pixmap_light("contentWidget/light_100");
			label_light->setPixmap(pixmap_light);
		}
		else
		{
		    QPixmap pixmap_light("contentWidget/light_0");
			label_light->setPixmap(pixmap_light);
		}
		content->hide();
		widget_light->move(0, 200);
		widget_light->show();
		widget_light->raise();
		light_flag = 1;
		timer_light_out->start();
		update();
	}
	else
	{
	    wait_for_status = 1;	
		qDebug()<<"timer_exe start";
		timer_exe->start();
	}
	last_index = current_index;  
	#endif
#endif
}

void ContentWidget::turnIcon(QString current_icon)
{
	bool ok;  
	current_index = current_icon.toInt(&ok, 10);
	QStringList string_list;
	QStringList string_list1;
	QStringList string_list2;
	QStringList string_list3;
	QString str_name; 
	QString str_first_name;
	int scene_name_3_flag = 0; 
		
	int flag = 0;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";

	string_list1<<"contentWidget/userdef_dim"<<"contentWidget/morning_dim"
		<<"contentWidget/enjoy_dim"<<"contentWidget/night_dim"
		<<"contentWidget/dinner_dim"<<"contentWidget/read_dim"
		<<"contentWidget/welcome_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/away_dim"<<"contentWidget/metting_dim"
		<<"contentWidget/relax_dim"<<"contentWidget/bath_dim"
		<<"contentWidget/party_dim"<<"contentWidget/motion_dim"
		<<"contentWidget/music_dim"<<"contentWidget/muse_dim"
	    <<"contentWidget/romantic_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/cooking_dim"<<"contentWidget/save_dim"
	    <<"contentWidget/purify_dim";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";

	string_list3<<"contentWidget/userdef_dim_off"<<"contentWidget/morning_dim_off"
		<<"contentWidget/enjoy_dim_off"<<"contentWidget/night_dim_off"
		<<"contentWidget/dinner_dim_off"<<"contentWidget/read_dim_off"
		<<"contentWidget/welcome_dim_off"<<"contentWidget/rest_dim_off"
		<<"contentWidget/away_dim_off"<<"contentWidget/metting_dim_off"
		<<"contentWidget/relax_dim_off"<<"contentWidget/bath_dim_off"
		<<"contentWidget/party_dim_off"<<"contentWidget/motion_dim_off"
		<<"contentWidget/music_dim_off"<<"contentWidget/muse_dim_off"
	    <<"contentWidget/romantic_dim_off"<<"contentWidget/rest_dim_off"
		<<"contentWidget/cooking_dim_off"<<"contentWidget/save_dim_off"
	    <<"contentWidget/purify_dim_off";

	if (g_do_scene_flag)
	{
	    return;
	}
	
	if (light_dim_press == 2)
	{
	    return;
	}
	else if (light_dim_press)
	{
	    light_dim_press = 0;
	}
	else
	{
	    return;
	}

    if (button_select->mouse_is_move)
    {
        return;
    }
	
	//printf("turnIcon, current_icon=%d\n", current_index);
	//qDebug()<<"clicktime=["<<g_click_timer.elapsed()<<"]"<<endl;

	if (g_click_timer.elapsed() < TIME_LIGHT_CLICK_MS)
	{
	   // flag = 1;
	   //return;
	}

					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
					//label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));
			    label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 70, OLED_PIX_X, 60));
				label_scene_first_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 70+60, OLED_PIX_X, 60));
				label_scene_onoff[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 70+60+60, OLED_PIX_X, 60));
				label_scene_name[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 90, OLED_PIX_X, 60));		
				label_scene_onoff[current_page]->setGeometry(QRect(OLED_PIX_X*(current_page+1), 150, OLED_PIX_X, 60));
		return;
	}
#endif
	for(int i=0; i<button_list.count(); i++)
	{
		SceneButton *tool_button = button_list.at(i);
		if(current_index == i)
		{
		    button_select = tool_button;
		    bool b = ast_dev[i].m_name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));  
			if (b)
			{
				if (ast_dev[i].m_name.size() <= 4)
				{
				    str_name = ast_dev[i].m_name;
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				}
				else if (ast_dev[i].m_name.size() > 4 && ast_dev[i].m_name.size() <= 8)
				{
				   str_name = ast_dev[i].m_name.left(4);
				   str_first_name = ast_dev[i].m_name.mid(4);
				   scene_name_3_flag = 1;
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				}
				else
				{
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				    str_name = ast_dev[i].m_name.left(4);
				    str_first_name = ast_dev[i].m_name.mid(4, 4) + QString("..");
					scene_name_3_flag = 1;
				}
			}
			else
			{
			    if (ast_dev[i].m_name.size() <= 11)
				{
				    str_name = ast_dev[i].m_name;
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				}
				//else if (ast_dev[i].m_name.size() <= 8)
				//{
				//    str_name = ast_dev[i].m_name;
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				//}
				else
				{
				    str_name = ast_dev[i].m_name.left(11) + QString("...");
					//label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));
				}
			}

			if (scene_name_3_flag)
			{
			    label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 70, 320, 60));
				label_scene_first_name[current_page]->setGeometry(QRect(320*(current_page+1), 70+60, 320, 60));
				label_scene_onoff[current_page]->setGeometry(QRect(320*(current_page+1), 70+60+60, 320, 60));
				
				label_scene_first_name[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
				label_scene_first_name[current_page]->setText(str_first_name);
			}
			else
			{
			    label_scene_first_name[current_page]->hide();
				label_scene_name[current_page]->setGeometry(QRect(320*(current_page+1), 90, 320, 60));		
				label_scene_onoff[current_page]->setGeometry(QRect(320*(current_page+1), 150, 320, 60));
			}
			
			label_scene_name[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
		  
			label_scene_name[current_page]->setText(str_name);
			if (!button_state[i])
			{
				label_scene_onoff[current_page]->setStyleSheet("font-size:44px;font-weight:bold;color:rgb(0, 255, 0);text-align:left;background:transparent;");
				label_scene_onoff[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
				label_scene_onoff[current_page]->setText(tr("Turn On"));
			}
			else
			{
				label_scene_onoff[current_page]->setStyleSheet("font-size:44px;font-weight:bold;color:rgb(255,255, 255);text-align:left;background:transparent;");
				label_scene_onoff[current_page]->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
				label_scene_onoff[current_page]->setText(tr("Turn Off"));
			}
		    //wait_for_status = 1;
			startA = 180;
		    startB = 360;
		    //printf("tool_button->isChecked()=%d, i=%d\n", button_state[i], i);
			if (button_state[i] == 1)
			{
		//		qDebug()<<"g_click_timer.elapsed()="<<g_click_timer.elapsed()<<endl;
			    if (flag)
			    {  
			        continue;
			    }
			}
			
			//button_state[i] = !button_state[i];
		    if (!button_state[i])
		    {
		        //tool_button->setChecked(false);
			   // QPixmap pixmap(string_list2.at(i));
		    #if 1
			    QImage img;
			    img.load(string_list.at(ast_dev[i].image_id));
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
			    tool_button->setIcon(QPixmap::fromImage(img));
			    tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
			    tool_button->update();
			#endif
				//enable current index, disable last index;
				/*if (last_index != -1)
				{
				    if (button_state[last_index] == 1 && last_index != current_index)
				    {
				        button_state[last_index] = 0;
				        tool_button->sceneCtrl(button_state[last_index]);
				    }
				}*/
		    }
			else 
			{
			    //tool_button->setChecked(true);
			    QPixmap pixmap(string_list.at(ast_dev[i].image_id));
			 //   tool_button->setIcon(pixmap);
			//    tool_button->setIconSize(pixmap.size());
			//    tool_button->update();
		    #if 1
			    QImage img;
			    img.load(string_list2.at(ast_dev[i].image_id));
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
			    tool_button->setIcon(QPixmap::fromImage(img));
			    tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
			    tool_button->update();
			#endif
			}

		   // wid_get[current_index/4]->setButton(tool_button);
		    wid_get[0]->setButton(tool_button);
			cur_button = string_list1.at(ast_dev[i].image_id);
			tool_button->sceneCtrl(!button_state[i]);
		}
		else 
		{
		    button_state[i] = 0;
		    QPixmap pixmap(string_list.at(ast_dev[i].image_id));
		//	tool_button->setIcon(pixmap);
		//	tool_button->setIconSize(pixmap.size());
		//	tool_button->update();
		}
		last_state[i] = button_state[i];
	}
	
	if (flag)
	{
	    //wid_get2->raise();
		prev_Widget = current_Widget;
		current_Widget = widget_light;
		//light_on_paint();
		//current_Widget->raise();
		
	    //qDebug()<<"set light_flag=1"<<endl;
		
		light_brightness = ast_dev[current_index].dim_value;
		last_dim = light_brightness;
		//bar->setValue(light_brightness);
		  if(light_brightness>100)
			  light_brightness =100;
		  else if(light_brightness<0)
			  light_brightness = 0;

		   set_progressbar_value(light_brightness);
		//qDebug()<<"set barval="<<bar->value();
		if (light_brightness == 100)
		{
			QPixmap pixmap_light("contentWidget/light_100");
			label_light->setPixmap(pixmap_light);
		}
		else
		{
		    QPixmap pixmap_light("contentWidget/light_0");
			label_light->setPixmap(pixmap_light);
		}
		content->hide();
		widget_light->move(0, 200);
		widget_light->show();
		widget_light->raise();
		light_flag = 1;
		timer_light_out->start();
		
		//qDebug()<<"turnIcon scene";
		update();
	}
	else
	{
	    wait_for_status = 1;
		g_blur_cnt = 1;
		
//	    Util::UI_motor_shake(1);
/*
		QWidget *w = content;
		QPixmap pixmap(w->size());
	
		w->render(&pixmap);
		QImage img = pixmap.toImage();
        QImage *img1 = blur(&img);
		QImage *img2 = blur(img1);
		new_image = blur(img2);
*/		
#if 1
#if 1
   //     QTimer::singleShot(SCENE_DO_TIME-10, this, SLOT(scene_turn_onoff()));
        QTimer::singleShot(SCENE_DO_TIME, this, SLOT(scene_main_back()));

		//group_scene_anima = new QParallelAnimationGroup;
		#ifdef SCENE_ANIMA_GROUP
		group_scene_anima->stop();
		group_scene_anima->clear();
		#endif
		
		//QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		
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
				img.load(string_list2.at(ast_dev[i].image_id));
			}
			else
			{
			    //qDebug()<<"button="<<i<<",state="<<button_state[i]<<",load list0";
				img.load(string_list.at(ast_dev[i].image_id));
			}
			//*img = img->scaled(400,400,Qt::KeepAspectRatio);

            //QLabel *b_label = new QLabel();
		    //模糊值，值越大越模糊
		    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		    effect->setBlurRadius(10);
		   
		    //QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
			//effect->setOpacity(0.7);
			tool_button->setGraphicsEffect(effect);
			
			//b_label->setPixmap(QPixmap::fromImage(*img));
			//delete effect;
			tool_button->setIcon(QPixmap::fromImage(img));
			tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
			
			//tool_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
			//tool_button->setStyleSheet("color:rgb(0,0,0);");
			//tool_button->update();

			QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"blurRadius");
			
			animation_bar->setDuration(SCENE_DO_TIME);
			animation_bar->setKeyValueAt(0, 10);
			animation_bar->setKeyValueAt(0.1, 25);
			animation_bar->setKeyValueAt(0.2, 40);
			animation_bar->setKeyValueAt(0.3, 50);
			animation_bar->setKeyValueAt(0.4, 60);
			animation_bar->setKeyValueAt(0.5, 70);
			animation_bar->setKeyValueAt(0.6, 60);
			animation_bar->setKeyValueAt(0.7, 50);
			animation_bar->setKeyValueAt(0.8, 40);
			animation_bar->setKeyValueAt(0.9, 25);
			animation_bar->setKeyValueAt(1, 0);

#if 0			
			QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"opacity");
			animation_bar->setDuration(SCENE_DO_TIME);
			animation_bar->setKeyValueAt(0, 0.9);
			animation_bar->setKeyValueAt(0.3, 0.5);
			animation_bar->setKeyValueAt(0.7, 0.5);
			animation_bar->setKeyValueAt(1, 0.9);
#endif

			//animation_bar->setEasingCurve(QEasingCurve::Linear);

			#ifdef SCENE_ANIMA_GROUP
		    group_scene_anima->addAnimation(animation_bar);
			#else
			animation_bar->start();
			#endif
		}
		
		g_do_scene_flag = 1;
		
		label_scene_onoff[current_page]->raise();
		label_scene_onoff[current_page]->show();
		label_scene_name[current_page]->raise();
		label_scene_name[current_page]->show();

		if (scene_name_3_flag)
		{
		    label_scene_first_name[current_page]->raise();
		    label_scene_first_name[current_page]->show();
		}

		
#ifdef SCENE_ANIMA_GROUP
		group_scene_anima->start();
#endif

		//usleep(10);

#if 0
        QGraphicsOpacityEffect *effect2 = new QGraphicsOpacityEffect(this);
		QPropertyAnimation *animation_label = new QPropertyAnimation(effect2,"opacity");
		label_scene_name[current_page]->setGraphicsEffect(effect2);
		//animation_label->setDuration(SCENE_DO_TIME);
		/*animation_label->setKeyValueAt(0, 0.2);
		//animation_label->setKeyValueAt(0.05, 0.5);
		animation_label->setKeyValueAt(0.3, 1);
		animation_label->setKeyValueAt(0.7, 1);
		animation_label->setKeyValueAt(0.95, 0.5);
		animation_label->setKeyValueAt(1, 0);*/
		animation_label->setDuration(SCENE_DO_TIME);
		animation_label->setKeyValueAt(0, 0.2);
		//animation_label->setKeyValueAt(0.05, 0.5);
		animation_label->setKeyValueAt(0.1, 1);
		animation_label->setKeyValueAt(0.9, 1);
		animation_label->setKeyValueAt(0.95, 0.5);
		animation_label->setKeyValueAt(1, 0);
		animation_label->setEasingCurve(QEasingCurve::Linear);

		QGraphicsOpacityEffect *effect1 = new QGraphicsOpacityEffect(this);
		QPropertyAnimation *animation_label1 = new QPropertyAnimation(effect1,"opacity");
		label_scene_onoff[current_page]->setGraphicsEffect(effect1);
		//animation_label1->setDuration(SCENE_DO_TIME);
		/*animation_label1->setKeyValueAt(0, 0.2);
		//animation_label1->setKeyValueAt(0.05, 0.5);
		animation_label1->setKeyValueAt(0.3, 1);
		animation_label1->setKeyValueAt(0.7, 1);
		animation_label1->setKeyValueAt(0.95, 0.5);
		animation_label1->setKeyValueAt(1, 0);*/
		animation_label1->setDuration(SCENE_DO_TIME);
		animation_label1->setKeyValueAt(0, 0.2);
		//animation_label->setKeyValueAt(0.05, 0.5);
		animation_label1->setKeyValueAt(0.1, 1);
		animation_label1->setKeyValueAt(0.9, 1);
		animation_label1->setKeyValueAt(0.95, 0.5);
		animation_label1->setKeyValueAt(1, 0);
		animation_label1->setEasingCurve(QEasingCurve::Linear);
		
		group_scene_anima->addAnimation(animation_label);
		group_scene_anima->addAnimation(animation_label1);
		#endif
		/*QGraphicsOpacityEffect *effect2 = new QGraphicsOpacityEffect(this);
		QPropertyAnimation *animation_label2 = new QPropertyAnimation(effect2,"opacity");
		wid_get_mask->setGraphicsEffect(effect2);
		animation_label2->setDuration(SCENE_DO_TIME-100);
		animation_label2->setKeyValueAt(0, 0);
		animation_label2->setKeyValueAt(0.3, 0.5);
		animation_label2->setKeyValueAt(0.7, 0.5);
		animation_label2->setKeyValueAt(1, 0);
		animation_label2->setEasingCurve(QEasingCurve::Linear);
		//wid_get_mask->raise();
		//wid_get_mask->show();

		group_scene_anima->addAnimation(animation_label2);*/
		//qDebug()<<"anima start test";
#endif
		//label_scene_onoff[current_page]->setGeometry(QRect(110, 110, 100, 100));
#if 0
		QWidget *w = content;
		QPixmap pixmap(w->size());
	
		w->render(&pixmap);
		QImage img = pixmap.toImage();
		new_image = new QImage(img);
#endif
#endif
		//qDebug()<<"timer_exe start";
		//timer_exe->start();
		
	}
	last_index = current_index;
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
void ContentWidget::mousePressEvent(QMouseEvent *event)
{
    if (g_do_scene_flag)
    {
        return;
    }
	
    if (!i_scene_num)
   	{
   	    return;
   	}

    //printf("ContentWidget::mousePressEvent,x=%d, y=%d\n", event->pos().x(), event->pos().y());
    #if 1
	if (light_flag==1)
	{
	    #if 1
	    if (event->pos().y() < 160 && event->pos().y() >= 5)
	    {
	        if (current_Widget == widget_light)
			{
			    widget_light->move(startx, starty);
			   // wid_get->move(startx, starty);
		       // wid_get1->move(startx, starty);
				widget_light->hide();
				prev_Widget->raise();
				current_Widget = prev_Widget;
				
			   /* QImage *img = new QImage;
			    img->load(button_select->m_img);
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
			    button_select->setIcon(QPixmap::fromImage(*img));
			    button_select->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
			    button_select->update();*/
				content->show();
				light_flag = 2;
				g_no_shake = 0;
		        update();
				
				lightdim_to_scene_start();
				return;
			}
	    }
		else if (event->pos().y() == 0 && event->pos().x() == 0)
		{
		    return;
		}
		else
			#endif
		{
		    m_mouseSrcPos = event->pos();
			m_mouseDstPos = m_mouseSrcPos;
			
		   if(event->pos().y() >=255 &&  event->pos().y()<=310)
		   {
			   if(m_mouseSrcPos.x()<=25)
				   light_brightness = 0;
			   else if(m_mouseSrcPos.x()>=295)
				   light_brightness = 100;
			   else
			   {
				   light_brightness = (m_mouseSrcPos.x()-25)*100/270;
			   }

			  if(light_brightness>100)
				  light_brightness =100;
			  else if(light_brightness<0)
				  light_brightness = 0;

			   set_progressbar_value(light_brightness);
				if (light_brightness == 100)
				{
					QPixmap pixmap_light("contentWidget/light_100");
					label_light->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("contentWidget/light_0");
					label_light->setPixmap(pixmap_light);
				}
			   update();
		   }

            #if 0
		    printf("src(%d,%d), dst(%d,%d)\n", 
			    m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y());
			if (m_mouseDstPos.x() <= 50)
			{
			    light_brightness = 0;
			}
			else if (m_mouseDstPos.x() >= 270)
			{
			    light_brightness = 100;
			}
			else
			{
				light_brightness = (m_mouseDstPos.x()-40)/2.2;
				if (light_brightness > 100)
				{
				    light_brightness = 100;
				}
			}
			if (light_brightness == 100)
			{
				QPixmap pixmap_light("contentWidget/light_100");
				label_light->setPixmap(pixmap_light);
			}
			else
			{
				QPixmap pixmap_light("contentWidget/light_0");
				label_light->setPixmap(pixmap_light);
			}
			
			bar->setValue(light_brightness);
			last_dim = light_brightness;	
			update();
			#endif
		}


		timer_light_out->start();
			
	    return;
	}
	#endif
	if (do_scene_flag)
	{
	    return;
	}
	if(event->button() == Qt::LeftButton) 
	{
	    left_mouse_press = 1;
		m_mouseSrcPos = event->pos();
		m_mouseDstPos = m_mouseSrcPos;
		//printf("src(%d,%d), dst(%d,%d)\n", 
		//	m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y());

        //scene move disable, when i_scene_num beyond 4
		wid_get[0]->move(startx-current_page*OLED_PIX_X, starty);
		{
		  //  left_mouse_press = 0;//mask by iovine
		}
	}
/*	else if(event->button() == Qt::RightButton)
	{
	    left_mouse_press = 0;
	    prev_Widget = current_Widget;
		current_Widget = widget_light;
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

void ContentWidget::scene_move_end()
{
	//	printf("current_uid=%d\r\n",current_uid);
		wid_get[0]->move(startx-current_page*320, starty);
}



#define WIDGET_MOVE_TIME  150
#define WIDGET_MOVE_OFFSET 100
void ContentWidget::mouseReleaseEvent(QMouseEvent *event)
{
   // printf("ContentWidget::mouseReleaseEvent\n");
	if (g_do_scene_flag)
	{
	    return;
	}

    if (!i_scene_num)
   	{
   	    return;
   	}

	if (current_Widget == widget_light && light_flag)
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
        ast_dev[current_index].dim_value = light_brightness;
        g_thread->msg_light_dim(current_index, last_dim, light_brightness);
		timer_light_out->start();
	}

	if (current_page == next_page || left_mouse_press != 1)
	{
	    if (current_page == next_page)
	    {
	        //qDebug()<<"line="<<__LINE__<<",return current_page == next_page\n";
	    }
		else if (left_mouse_press != 1)
		{
		    //qDebug()<<"line="<<__LINE__<<",left_mouse_press != 1\n";
		}
	    QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(scene_move_end()));
	    return;
	}
	left_mouse_press = 0;
			anim_r->setKeyValueAt(0, QRect(startx - offset -current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-OLED_PIX_X-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx+OLED_PIX_X - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*OLED_PIX_X, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx - current_page*OLED_PIX_X, starty, 320, 320));
	
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

			anim_r->setDuration(WIDGET_MOVE_TIME);
			anim_r->setKeyValueAt(0, QRect(startx - offset -current_page*320, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx-current_page*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);
			//group_move_widget->addAnimation(anim_rb);

			group_move_widget->start();
			QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(scene_move_end()));
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

			anim_l->setDuration(move_time);
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*320, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-320-current_page*320, starty, 320, 320));

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

			anim_l->setDuration(move_time);
			anim_l->setKeyValueAt(0, QRect(startx - offset-current_page*320, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx-current_page*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);
			//group_move_widget->addAnimation(anim_ln);

			group_move_widget->start();
		}
	    QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(scene_move_end()));
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

			anim_l->setDuration(WIDGET_MOVE_TIME);
			anim_l->setKeyValueAt(0, QRect(startx + offset - current_page*320, starty, 320, 320));
			anim_l->setKeyValueAt(1, QRect(startx - current_page*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_l);
			//group_move_widget->addAnimation(anim_lb);

			group_move_widget->start();
			QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(scene_move_end()));
    		return;
    	}
	    if ((m_mouseDstPos.x() >  m_mouseSrcPos.x()) 
			&& (m_mouseDstPos.x() - m_mouseSrcPos.x() > 100))
	    {

		 	 offset = m_mouseDstPos.x() - m_mouseSrcPos.x();
		 	move_time = WIDGET_MOVE_TIME*(320-offset)/WIDGET_MOVE_OFFSET;
			QPropertyAnimation *anim_r= new QPropertyAnimation(wid_get[0], "geometry");
			//QPropertyAnimation *anim_rn = new QPropertyAnimation(wid_get[next_page], "geometry");

			anim_r->setDuration(move_time);
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*320, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx+320 - current_page*320, starty, 320, 320));

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

			anim_r->setDuration(move_time);
			anim_r->setKeyValueAt(0, QRect(startx + offset - current_page*320, starty, 320, 320));
			anim_r->setKeyValueAt(1, QRect(startx - current_page*320, starty, 320, 320));

			group_move_widget->addAnimation(anim_r);
			//group_move_widget->addAnimation(anim_rn);
			group_move_widget->start();

		}
	    QTimer::singleShot(WIDGET_MOVE_TIME+100, this, SLOT(scene_move_end()));
	}
	update();
#endif
}


void ContentWidget::set_progressbar_value(int value)
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

void ContentWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (g_do_scene_flag)
    {
        return;
    }
	
    if (!i_scene_num)
   	{
   	    return;
   	}

    //printf("2、ContentWidget::mouseMoveEvent. ");
    if (light_flag)
    {
        g_no_shake = 1;
	//	return;
        #if 1
		m_mouseDstPos = event->pos();
#if 1
		if (m_mouseDstPos.x() > m_mouseSrcPos.x())
		{
		    if (light_brightness < 100)
		    {
		    	light_brightness = ast_dev[current_index].dim_value + (m_mouseDstPos.x() - m_mouseSrcPos.x())/3;
			   // printf("brightness=%d src=%d dst=%d\r\n",light_brightness,m_mouseSrcPos.x(),m_mouseDstPos.x());
			    if(m_mouseSrcPos.x()>300)//add 10 just for edge problem
			    {
			    	light_brightness+= 10;
			    }
				if (light_brightness > 100)
				{
					light_brightness = 100;
				}

				//bar->setValue(light_brightness);
				set_progressbar_value(light_brightness);
				if (light_brightness == 100)
				{
					QPixmap pixmap_light("contentWidget/light_100");
					label_light->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("contentWidget/light_0");
					label_light->setPixmap(pixmap_light);
				}
		    }
		}
		else
		{
		    if (light_brightness > 0)
		    {
			    light_brightness = ast_dev[current_index].dim_value - (m_mouseSrcPos.x() - m_mouseDstPos.x())/3;
				if (light_brightness < 0)
				{
				    light_brightness = 0;
				}
				//bar->setValue(light_brightness);
				set_progressbar_value(light_brightness);
				if (light_brightness == 100)
				{
					QPixmap pixmap_light("contentWidget/light_100");
					label_light->setPixmap(pixmap_light);
				}
				else
				{
				    QPixmap pixmap_light("contentWidget/light_0");
					label_light->setPixmap(pixmap_light);
				}
		    }
		}

		  if(light_brightness>100)
			  light_brightness =100;
		  else if(light_brightness<0)
			  light_brightness = 0;
		set_progressbar_value(light_brightness);
		if (light_brightness == 100)
		{
			QPixmap pixmap_light("contentWidget/light_100");
			label_light->setPixmap(pixmap_light);
		}
		else
		{
		    QPixmap pixmap_light("contentWidget/light_0");
			label_light->setPixmap(pixmap_light);
		}
		//bar->setValue(light_brightness);
	    //g_thread->msg_light_dim(current_index, last_dim, light_brightness);
	   // last_dim = light_brightness;
#endif
       

		//light_on_paint();
			
		update();

		if (timer_light_out->isActive())
		{
			timer_light_out->stop();
		}
        return;
		#endif
    }


	if (left_mouse_press != 1)
	{
	    return;
	}

    if (do_scene_flag)
    {
        return;
    }
	
	m_mouseDstPos = event->pos();
   // printf("ContentWidget,src(%d,%d), dst(%d,%d), move(%d,%d)\n", 
	//       m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y(),
	//       m_mouseDstPos.x() - m_mouseSrcPos.x(), m_mouseDstPos.y() - m_mouseSrcPos.y()); 
#ifdef SLIDER_VERTICAL

#else
	float speed=0;
	int move_offset = 0;

	if (m_mouseDstPos.x() <  m_mouseSrcPos.x())//to left
	{
	    if (current_page + 1 != page_num)
	    {
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x()- current_page*OLED_PIX_X , starty);
				wid_get[0]->move(startx - move_offset-current_page*OLED_PIX_X, starty);
			    wid_get[0]->move(startx + current_page*OLED_PIX_X + m_mouseDstPos.x() - m_mouseSrcPos.x(), starty);
			    wid_get[0]->move(startx + move_offset-current_page*OLED_PIX_X, starty);
			wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*OLED_PIX_X, starty);
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*OLED_PIX_X, starty);
		}
	}
	else if (m_mouseDstPos.x() >  m_mouseSrcPos.x()) // to right
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
		if(m_mouseDstPos.x() <  m_mouseSrcPos.x())//to left
		{
			if(m_mouseSrcPos.x()-m_mouseDstPos.x() <=WIDGET_MOVE_OFFSET)
			{
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x()- current_page*320 , starty);
			}
			else if((m_mouseSrcPos.x()-m_mouseDstPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseSrcPos.x()-m_mouseDstPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseSrcPos.x()-m_mouseDstPos.x()))/(m_mouseSrcPos.x()-m_mouseDstPos.x());
				move_offset = 200-speed*(m_mouseSrcPos.x()-m_mouseDstPos.x());
				wid_get[0]->move(startx - move_offset-current_page*320, starty);
			}
		}
		else if(m_mouseDstPos.x() >  m_mouseSrcPos.x())//to right
		{
			if(m_mouseDstPos.x() - m_mouseSrcPos.x() <=WIDGET_MOVE_OFFSET)
			{
			    wid_get[0]->move(startx + current_page*320 + m_mouseDstPos.x() - m_mouseSrcPos.x(), starty);
			}
			else if((m_mouseDstPos.x() - m_mouseSrcPos.x() >WIDGET_MOVE_OFFSET)&&(m_mouseDstPos.x() - m_mouseSrcPos.x() <=2*WIDGET_MOVE_OFFSET))
			{
				speed = (150-0.5*(m_mouseDstPos.x() - m_mouseSrcPos.x()))/(m_mouseDstPos.x() - m_mouseSrcPos.x());
				move_offset = 200-speed*(m_mouseDstPos.x() - m_mouseSrcPos.x());
			    wid_get[0]->move(startx + move_offset-current_page*320, starty);
			}
		}

	}
	else
	{

		if ((current_page < next_page) && (m_mouseDstPos.x() <  m_mouseSrcPos.x()))//to left
		{
			wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*320, starty);
		}
		else if ((current_page > next_page) && (m_mouseDstPos.x() >  m_mouseSrcPos.x()))//to right
		{
				wid_get[0]->move(startx + m_mouseDstPos.x() - m_mouseSrcPos.x() - current_page*320, starty);
		}
	}



#endif
}

void ContentWidget::switch_to_scene(int last_page)
{

    if (last_page == 1)
    {
        
    }
}

void ContentWidget::snap_shot(QImage **w_image)
{
    QWidget *w = content;
	QPixmap pixmap(w->size());

	w->render(&pixmap);
	QImage img = pixmap.toImage();
	*w_image = new QImage(img);
	
	//(*w_image)->save("s_image.jpg","JPG",100);

	if (current_Widget == widget_light)
	{
		widget_light->move(startx, starty);
		widget_light->hide();
		prev_Widget->raise();
		current_Widget = prev_Widget;
	
		content->show();
		light_flag = 2;
		g_no_shake = 0;
        update();
		
		QTimer::singleShot(1, this, SLOT(lightdim_to_scene()));
	}
}
void ContentWidget::switch_to_page(int new_page, QImage *new_image)
{
    if (new_page == 1)
    {
	    switch_new_image = new_image;
        forward();
    }
	else if (new_page == 2)
    {
	    switch_new_image = new_image;
        next();
    }
}

void ContentWidget::light_on_paint()
{
	int i;
	QStringList string_list;
    string_list<<"contentWidget/userdef_dim"<<"contentWidget/morning_dim"
		<<"contentWidget/enjoy_dim"<<"contentWidget/night_dim"
		<<"contentWidget/dinner_dim"<<"contentWidget/read_dim"
		<<"contentWidget/welcome_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/away_dim"<<"contentWidget/metting_dim"
		<<"contentWidget/relax_dim"<<"contentWidget/bath_dim"
		<<"contentWidget/party_dim"<<"contentWidget/motion_dim"
		<<"contentWidget/music_dim"<<"contentWidget/muse_dim"
	    <<"contentWidget/romantic_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/cooking_dim"<<"contentWidget/save_dim"
	    <<"contentWidget/purify_dim";

	//printf("light_on_paint\n");
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
	//if (light_brightness < 30)
	//{
	  QImage image = QPixmap(string_list.at(ast_dev[i].image_id)).toImage();
	//}

#if 0
	QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
//模糊值，值越大越模糊
    effect->setBlurRadius(5);
    b_label->setGraphicsEffect(effect);
	b_label->setPixmap(QPixmap("contentWidget/blue.png"));
#endif

	QPainter painter(&image);
	cur_button = string_list.at(ast_dev[i].image_id);

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
	//bar->setValue(light_brightness);

	int n = 100;
	//保存图片
	image.save("text.jpg","JPG",n);
#endif

}
void ContentWidget::scene_turn_onoff()
{
	QStringList string_list;
	QStringList string_list2;

	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";

	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";

    
	label_scene_onoff[current_page]->hide();
	label_scene_name[current_page]->hide();
	label_scene_first_name[current_page]->hide();

    
	//qDebug()<<"scene_turn_onoff";
    return;
	group_scene_anima = new QParallelAnimationGroup;
	for(int i=0; i<button_list.count(); i++)
	{
	    if (current_page != i/4)
	    {
	        continue;
	    }
	    SceneButton *tool_button = button_list.at(i);
		QImage *img = new QImage;
		//qDebug()<<"button="<<i<<",state="<<button_state[i]<<",load string_list1";
		if (button_state[i])
		{
			img->load(string_list2.at(ast_dev[i].image_id));
		}
		else
		{
			img->load(string_list.at(ast_dev[i].image_id));
		}
		//*img = img->scaled(400,400,Qt::KeepAspectRatio);

	    //QLabel *b_label = new QLabel();
	    //模糊值，值越大越模糊
	    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
	    //effect->setBlurRadius(50);
	    /*QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
	    effect->setOpacity(0.7);*/
	    tool_button->setGraphicsEffect(effect);
		
		//b_label->setPixmap(QPixmap::fromImage(*img));
		//delete effect;
		tool_button->setIcon(QPixmap::fromImage(*img));
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		
		tool_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
		//tool_button->update();
		QPropertyAnimation *animation_bar = new QPropertyAnimation(effect,"blurRadius");
		
		animation_bar->setDuration(1000);
		animation_bar->setKeyValueAt(0, 35);
		//animation_bar->setKeyValueAt(0.5, 35);
		animation_bar->setKeyValueAt(1, 0);
		animation_bar->setEasingCurve(QEasingCurve::Linear);
		
	    group_scene_anima->addAnimation(animation_bar);
	}
	group_scene_anima->start();
	
	QTimer::singleShot(1000, this, SLOT(scene_main_back()));
}
void ContentWidget::scene_main_back()
{
	QStringList string_list;
	QStringList string_list2;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";
	
    do_scene_flag = 0;
    startA = 0;
	startB = 30;
    timer_exe->stop();
  //  qDebug()<<"scene_main_back , stop"<<endl;
	wid_get_wait->hide();

  	for(int i=0; i<button_list.count(); i++)
	{
	    SceneButton *tool_button = button_list.at(i);
		QImage img;
		
		//qDebug()<<"button state="<<button_state[i];
		if (button_state[i])
		{
			img.load(string_list2.at(ast_dev[i].image_id));
			//qDebug()<<"load string_list2";
		}
		else
		{
			img.load(string_list.at(ast_dev[i].image_id));
			//qDebug()<<"load string_list";
		}
		//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
		
		QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		
	//模糊值，值越大越模糊
	    effect->setBlurRadius(0);
	    tool_button->setGraphicsEffect(NULL);
		tool_button->setIcon(QPixmap::fromImage(img));
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		//tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		
		//tool_button->setStyleSheet("color:rgb(255,255,255);");
		g_blur_cnt = 1;
		g_blur_dir = 1;
		tool_button->raise();
		tool_button->show();
		//delete effect;
		//tool_button->update();
	}
	
	//label_scene_onoff[current_page]->setGeometry(QRect(410, 410, 100, 100));
	label_scene_onoff[current_page]->hide();
	label_scene_name[current_page]->hide();
	label_scene_first_name[current_page]->hide();
	
	//usleep(100);
    content->show();
	content->raise();
	g_do_scene_flag = 0;
   
	//delete new_image;
}
void ContentWidget::scene_back()
{
	QStringList string_list;
	QStringList string_list2;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";

    do_scene_flag = 0;
    startA = 0;
	startB = 30;
    timer_exe->stop();
  //  qDebug()<<"wait_for_status=0, stop"<<endl;
	wid_get_wait->hide();

  	for(int i=0; i<button_list.count(); i++)
	{
	    SceneButton *tool_button = button_list.at(i);
		QImage img;
		
		//qDebug()<<"button state="<<button_state[i];
		if (button_state[i])
		{
			img.load(string_list2.at(ast_dev[i].image_id));
			//qDebug()<<"load string_list2";
		}
		else
		{
			img.load(string_list.at(ast_dev[i].image_id));
			//qDebug()<<"load string_list";
		}
		//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
		
		QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		
	//模糊值，值越大越模糊
	    effect->setBlurRadius(0);
	/*QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
		    effect->setOpacity(0);*/
			
	    tool_button->setGraphicsEffect(effect);
		tool_button->setIcon(QPixmap::fromImage(img));
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		g_blur_cnt = 1;
		g_blur_dir = 1;
		//delete effect;
		//tool_button->update();
	}
    content->show();
	content->raise();
	//delete new_image;
}

void ContentWidget::paintEvent(QPaintEvent *e)
{
    int i;
	static int imageRotate = 0;
	static int cnt = 0;
	
    if (!i_scene_num)
   	{
   	    return;
   	}

	if (light_flag==1)
	{
		QImage image = QPixmap(cur_button).toImage();			
		image.save("text.jpg", "JPG", 100);
		QImage image1 = QPixmap("text.jpg").toImage();	

		QPainter paint(this);
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
		QRect source(0.0, 0.0, 261.0, 261.0); //建立源矩形，用来划定来自外部的源图像文件中需要显示的区域

		//将文字绘制在图片中心位置
		QString str;
		str.sprintf("%d%", light_brightness);
		painter.drawText(image1.rect(), Qt::AlignCenter, str);
		paint.drawImage(target,image1,source);
	}
	else if (!light_flag)
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
		    	if (i == current_page)
				{
					painter.setPen(QPen(Qt::white, 6));//设置画笔形式
					painter.setBrush(Qt::white);
					painter.drawEllipse(current_point_x-17, 309, 6, 6);//画圆
					painter.drawRect(QRect(current_point_x-12, 309, 20, 6));
					painter.drawEllipse(current_point_x+5, 309, 6, 6);//画圆
				}
		    	else if (i < current_page)
		    	{
					painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
					painter.setBrush(Qt::gray);
		    		if(i==current_page-1)
		    		{
		    			painter.drawEllipse(current_point_x-44, 309, 6, 6);//画圆
		    		}
		    		else
		    		{
		    			painter.drawEllipse(current_point_x-(44+32*(current_page-i-1)), 309, 6, 6);//画圆
		    		}
		    	}
		    	else if (i > current_page)
				{
					painter.setPen(QPen(Qt::gray, 6));//设置画笔形式
					painter.setBrush(Qt::gray);
					if(i==current_page+1)
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
#if 0	
        //do scene 
  		if (wait_for_status || cnt != 0)
		{
		    //qDebug()<<"do scene in"<<endl;
		    do_scene_flag = 1;
			
			#if 1   //get status, until 30times, then end
		    if (cnt >= 30 && wait_for_status == 0)
		    {
		        cnt = 0;
			    startA = 0;
				startB = 30;
				g_stop_flag = 1;
			    //timer_exe->stop();
				//wid_get_wait->hide();
			    //content->show();
				//content->raise();
				
				//QTimer::singleShot(10, this, SLOT(scene_main_back()));
				return;
		    }
			#endif

            //cann't get status, wait until 250times, force out
	        if (cnt >= 100 && wait_for_status != 0)
	        {	        
	            cnt = -1;
				wait_for_status = 0;
				qDebug()<<"do scene, stop"<<endl;
				g_stop_flag = 1;
				//QTimer::singleShot(10, this, SLOT(scene_main_back()));
				/*do_scene_flag = 0;

				timer_exe->stop();
			    
				wid_get_wait->hide();
			    content->show();
				content->raise();*/
	        }
#if 1
#if 0
			    QWidget *w = content;
				QPixmap pixmap(w->size());
			
				w->render(&pixmap);
				QImage img = pixmap.toImage();

				new_image = &img;//blur(&img);
        	
			
			//QPainter painter(&img);
#if 0
			QPixmap disc("contentWidget/ring.png");
		    
//			qDebug()<<"line=["<<__LINE__<<"]";
			imageRotate += 30;
		    if(imageRotate>= 360)
		        imageRotate = 0;

			if (i_scene_num == 1)
			{		
			
//			qDebug()<<"line=["<<__LINE__<<"]";
			    /* 设定旋转中心点 */
			    painter.translate(65+95,95+65);
			    /* 旋转的角度 */
			    painter.rotate(imageRotate);
			    /* 恢复中心点 */
			    painter.translate(-(95+65),-(95+65));
			    /* 画图操作 */
			    painter.drawPixmap(95,95,130,130, disc);
			}
			else
			{
			    /* 设定旋转中心点 */
			    painter.translate((button_select->item_id%2)*175+65,(button_select->item_id/2)*IMG_POS_Y+65);
			    /* 旋转的角度 */
			    painter.rotate(imageRotate);
			    /* 恢复中心点 */
			    painter.translate(-((button_select->item_id%2)*175+65),-((button_select->item_id/2)*IMG_POS_Y+65));
			    /* 画图操作 */
			    painter.drawPixmap((button_select->item_id%2)*175,
			         (button_select->item_id/2)*IMG_POS_Y,130,130, disc);
			}
#endif 
			QRect target(0.0, 0.0, 320.0, 320.0); //建立目标矩形，该区域是显示图像的目的地
			QRect source(0.0, 0.0, 320.0, 320.0);

            //QImage *old_image = new_image;
			//new_image = blur(old_image);
			paint.drawImage(target, *new_image, source);

			this->setAttribute(Qt::WA_TranslucentBackground, true);
			paint.setBrush(QColor(0,0,0,30));
			if (!g_blur_dir)
			{
				paint.setBrush(QColor(0,0,0,5));
			}
			else
			{
			    paint.setBrush(QColor(0,0,0,30));
			}
			paint.drawRect(-10, -10, 340, 340);
#endif
#if 1
            QPen pen1 = paint.pen();
			pen1.setColor(Qt::white);
			QFont font1 = paint.font();
			font1.setBold(true);//加粗
			font1.setPixelSize(45);//改变字体大小
			paint.setPen(pen1);
			paint.setFont(font1);
#endif

			//if (cnt >= 2)
			{
				paint.drawText(110, 140, tr("scene"));
				paint.drawText(100, 180, tr("turn on"));
			}

			//this->setAttribute(Qt::WA_TranslucentBackground, true);
			//paint.setBrush(QColor(0,0,0,100));
			//paint.drawRect(0, 0, 320, 320);
#endif
		    int n = 100;
			if (cnt == 0)
			{
	//		    img.save("wid1.jpg","JPG",n);
			}
			
			cnt++;
			content->hide();
			wid_get_wait->show();
		    wid_get_wait->raise();

		    //qDebug()<<"do scene out"<<endl;
		}
		else 
		{/*
			do_scene_flag = 0;
		    startA = 0;
			startB = 30;
		    timer_exe->stop();
		  //  qDebug()<<"wait_for_status=0, stop"<<endl;
			wid_get_wait->hide();
		  	for(int i=0; i<button_list.count(); i++)
			{
			    SceneButton *tool_button = button_list.at(i);
				QImage *img = new QImage;
				
				//qDebug()<<"button state="<<button_state[i];
				if (button_state[i])
				{
					img->load(string_list2.at(ast_dev[i].image_id));
				}
				else
				{
					img->load(string_list.at(ast_dev[i].image_id));
				}
				//*img = img->scaled(SCENE_BUTTON_X,SCENE_BUTTON_Y,Qt::KeepAspectRatio);
				tool_button->setIcon(QPixmap::fromImage(*img));
				tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
				tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
				//tool_button->update();
			}
		    content->show();
			content->raise();*/
		}
#endif

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
	
}
QImage * ContentWidget::blur(QImage * origin){  
	QImage * newImage = new QImage(*origin);  
   
	int kernel [5][5]= {{0,0,1,0,0},  
						{0,1,3,1,0},  
						{1,3,7,3,1},  
						{0,1,3,1,0},  
						{0,0,1,0,0}};  
	int kernelSize = 5;  
	int sumKernel = 27;  
	int r,g,b;	
	QColor color;  
   
	for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){  
		for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){	
   
			r = 0;	
			g = 0;	
			b = 0;	
   
			for(int i = -kernelSize/2; i<= kernelSize/2; i++){	
				for(int j = -kernelSize/2; j<= kernelSize/2; j++){	
					color = QColor(origin->pixel(x+i, y+j));  
					r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];  
					g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];	
					b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];  
				}  
			}  
   
			r = qBound(0, r/sumKernel, 255);  
			g = qBound(0, g/sumKernel, 255);  
			b = qBound(0, b/sumKernel, 255);  
   
			newImage->setPixel(x,y, qRgb(r,g,b));  
   
		}  
	}  
	return newImage;  
}  
void ContentWidget::onTimerExe()
{
	QObject *object = QObject::sender();
	QTimer *timer1 = qobject_cast<QTimer *>(object);
	int the_state[16];
	QStringList string_list;
	QStringList string_list1;
	QStringList string_list2;
	QStringList string_list3;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";

	string_list1<<"contentWidget/userdef_dim"<<"contentWidget/morning_dim"
		<<"contentWidget/enjoy_dim"<<"contentWidget/night_dim"
		<<"contentWidget/dinner_dim"<<"contentWidget/read_dim"
		<<"contentWidget/welcome_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/away_dim"<<"contentWidget/metting_dim"
		<<"contentWidget/relax_dim"<<"contentWidget/bath_dim"
		<<"contentWidget/party_dim"<<"contentWidget/motion_dim"
		<<"contentWidget/music_dim"<<"contentWidget/muse_dim"
	    <<"contentWidget/romantic_dim"<<"contentWidget/rest_dim"
		<<"contentWidget/cooking_dim"<<"contentWidget/save_dim"
	    <<"contentWidget/purify_dim";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";;

	string_list3<<"contentWidget/userdef_dim_off"<<"contentWidget/morning_dim_off"
		<<"contentWidget/enjoy_dim_off"<<"contentWidget/night_dim_off"
		<<"contentWidget/dinner_dim_off"<<"contentWidget/read_dim_off"
		<<"contentWidget/welcome_dim_off"<<"contentWidget/rest_dim_off"
		<<"contentWidget/away_dim_off"<<"contentWidget/metting_dim_off"
		<<"contentWidget/relax_dim_off"<<"contentWidget/bath_dim_off"
		<<"contentWidget/party_dim_off"<<"contentWidget/motion_dim_off"
		<<"contentWidget/music_dim_off"<<"contentWidget/muse_dim_off"
	    <<"contentWidget/romantic_dim_off"<<"contentWidget/rest_dim_off"
		<<"contentWidget/cooking_dim_off"<<"contentWidget/save_dim_off"
	    <<"contentWidget/purify_dim_off";

    //this->show();
    //title_widget->turnPage("3");
 //   qDebug()<<"onTimerExe";
    for(int i=0; i<button_list.count(); i++)
	{
	    SceneButton *tool_button = button_list.at(i);
		QImage img;
		//qDebug()<<"line="<<__LINE__<<",button="<<button_state[i]<<",load string_list1"<<"blur_cnt="<<g_blur_cnt;
	
		//*img = img->scaled(400,400,Qt::KeepAspectRatio);

        //QLabel *b_label = new QLabel();
	    //模糊值，值越大越模糊
	    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect(this);
		if (g_blur_cnt < 40 && g_blur_dir)
		{
			g_blur_cnt += 1;
			the_state[i] = last_state[i];
		}
		else if (g_blur_cnt >= 2 && !g_blur_dir)
		{
		    g_blur_cnt -= 1;
			the_state[i] = button_state[i];
		}
		else if (g_blur_cnt >= 40)
		{
		    g_blur_dir = 0;
			the_state[i] = last_state[i];
		}
		else if (g_stop_flag)
		{
		    if (g_blur_cnt <= 2)
	    	{
			    timer_exe->stop();
			    g_stop_flag = 0;
				scene_back();
				return;
			   // QTimer::singleShot(1, this, SLOT(scene_main_back()));
	    	}
		}
		
		if (the_state[i])
		{
			img.load(string_list2.at(ast_dev[i].image_id));
		}
		else
		{
			img.load(string_list.at(ast_dev[i].image_id));
		}

	    effect->setBlurRadius(g_blur_cnt);
	    /*QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
	    effect->setOpacity(0.7);*/
	    tool_button->setGraphicsEffect(effect);
		
		//b_label->setPixmap(QPixmap::fromImage(*img));
		tool_button->setIcon(QPixmap::fromImage(img));
		tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		
		tool_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
		//tool_button->update();
	}
#if 0
	QWidget *w = content;
	QPixmap pixmap(w->size());

	w->render(&pixmap);
	QImage img = pixmap.toImage();
	new_image = new QImage(img);
 #endif
 
	update();

	//timer1->stop();
	//qDebug()<<"onTimerExe"<<endl;
}

enno_result ContentWidget::set_dim_value(char *buf_in, uint32 len_buf)
{
    enno_result ret;
	uint32 id;
	uint32 len;
	uint32 total_len;
	uint32 dim;
	char *buf = buf_in;

	//qDebug()<<"set_dim_value in"<<endl;
    if (NULL == buf)
    {
       // qDebug()<<"[qt err]err value,func="<<__func__<<",line="<<__LINE__<<endl;
    }

    while (len_buf)
    {
		ret = enno_decode_uint(buf, &id, &len, len_buf);
		ENNO_CHECK_RET(ret);
		buf += len;
	    len_buf -= len;

		//qDebug()<<"set_dim_value id="<<id<<endl;

		if (id < MAX_SCENE)
		{
		    ret = enno_decode_uint(buf, &dim, &len, len_buf);
		    ENNO_CHECK_RET(ret);
			buf += len;
			len_buf -= len;
	        
			//qDebug()<<"ast_dev["<<id<<"].dim_value="<<dim<<endl;
	        if (dim <= 100 && dim >= 0)
	    	{
				ast_dev[id].dim_value = dim;
	    	}
		}
		else
		{
		    //qDebug()<<"[error]scene id err="<<id<<endl;
		}
    }
}

enno_result ContentWidget::get_scene_status_byname(char *buf_in, uint32 len_buf)
{   
    enno_result ret;
	uint32 len;
	char *buf = buf_in;
	char name[128];
	uint32 sid;
	uint32 status;
	bool ok;  
	uint32 i;
	QStringList string_list;
	QStringList string_list2;
	QStringList string_list3;
	QStringList string_list4;
	char *ch;
	//QByteArray ba;
	QByteArray ba;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";
	
	string_list3<<"contentWidget/userdef_off_b"<<"contentWidget/morning_off_b"
		<<"contentWidget/enjoy_off_b"<<"contentWidget/night_off_b"
		<<"contentWidget/dinner_off_b"<<"contentWidget/read_off_b"
		<<"contentWidget/welcome_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/away_off_b"<<"contentWidget/metting_off_b"
		<<"contentWidget/relax_off_b"<<"contentWidget/bath_off_b"
		<<"contentWidget/party_off_b"<<"contentWidget/motion_off_b"
		<<"contentWidget/music_off_b"<<"contentWidget/muse_off_b"
	    <<"contentWidget/romantic_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/cooking_off_b"<<"contentWidget/save_off_b"
	    <<"contentWidget/purify_off_b";
	
	string_list4<<"contentWidget/userdef_on_b"<<"contentWidget/morning_on_b"
		<<"contentWidget/enjoy_on_b"<<"contentWidget/night_on_b"
		<<"contentWidget/dinner_on_b"<<"contentWidget/read_on_b"
		<<"contentWidget/welcome_on_b"<<"contentWidget/rest_on_b"
		<<"contentWidget/away_on_b"<<"contentWidget/metting_on_b"
		<<"contentWidget/relax_on_b"<<"contentWidget/bath_on_b"
		<<"contentWidget/party_on_b"<<"contentWidget/motion_on_b"
		<<"contentWidget/music_on_b"<<"contentWidget/muse_on_b"
	    <<"contentWidget/romantic_on_b"<<"contentWidget/rest_on_b"
		<<"contentWidget/cooking_on_b"<<"contentWidget/save_on_b"
	    <<"contentWidget/purify_on_b";

	ENNO_CHECK_NULL_RET(buf);
	
    ret = enno_decode_uint(buf, &status, &len, len_buf);
	ENNO_CHECK_RET(ret);
	buf += len;
    len_buf -= len;

	ret = enno_decode_str(buf, name, &len, len_buf);
	ENNO_CHECK_RET(ret);

	for (i = 0; i < i_scene_cnt; i++)
	{
		//qDebug()<<"get_scene_status_byname,name="<<name<<", status="<<status<<endl;
	    ba = ast_dev[i].m_name.toLocal8Bit(); // must
		ch = ba.data();
#if 0		
		printf("m_name(%d)={", ba.size());
		for (int j = 0; j < ba.size(); j++)
		{
		    printf("0x%x ", ch[j]);
		}
		printf("}\r\n");

		printf("get_name(%d)={", strlen(name));
		for (int j = 0; j < strlen(name); j++)
		{
		    printf("0x%x ", name[j]);
		}
		printf("}\r\n");
#endif
		
	    if (!memcmp(ch, name, strlen(ch)))
	    {
	       // qDebug()<<"same scene name";
	        sid = i;
	    }
		else
		{
	       // qDebug()<<"status name="<<name<<"m_name="<<ch;
		    continue;
		}
		
		 //qDebug()<<"get_scene_status,sid="<<sid<<", status="<<status<<endl;
		if (i >= MAX_SCENE)
		{
		    qDebug()<<"fatal err, sid > MAX_SCENE, i="<<i;
			return ENNO_UNKOWN;
		}
	    
		if (i >= button_list.count())
		{
		//    qDebug()<<"i="<<i<<endl;
		    return ENNO_OK;
		}
			
		button_state[i] = status;
		SceneButton *tool_button = button_list.at(i);
		
	//	printf("tool_button->isChecked()=%d\n", button_state[i]);
		wait_for_status = 0;
	    //timer_exe->stop();
	   // update();
	    //if (!light_flag)
		//    content->show();
		
		if(button_state[i])
		{
	        //tool_button->setChecked(false);
		    // QPixmap pixmap(string_list2.at(i));
		    QPixmap pixmap(string_list2.at(ast_dev[i].image_id));
		    tool_button->setIcon(pixmap);
			tool_button->setImg(string_list4.at(ast_dev[i].image_id));
		    tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		    tool_button->update();  
		}
		else 
		{
		    QPixmap pixmap(string_list.at(ast_dev[i].image_id));
			tool_button->setIcon(pixmap);
			tool_button->setImg(string_list3.at(ast_dev[i].image_id));
			tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));//pixmap.size());
			tool_button->update();
		}
	}
    
}
enno_result ContentWidget::get_scene_status(char *buf_in, uint32 len_buf)
{
    enno_result ret;
	uint32 len;
	char *buf = buf_in;
	uint32 sid;
	uint32 status;
	bool ok;  
	uint32 i;
	int i_image_id = 0;
	QStringList string_list;
	QStringList string_list2;
	QStringList string_list3;
	QStringList string_list4;
	
	string_list<<"contentWidget/userdef_off"<<"contentWidget/morning_off"
		<<"contentWidget/enjoy_off"<<"contentWidget/night_off"
		<<"contentWidget/dinner_off"<<"contentWidget/read_off"
		<<"contentWidget/welcome_off"<<"contentWidget/rest_off"
		<<"contentWidget/away_off"<<"contentWidget/metting_off"
		<<"contentWidget/relax_off"<<"contentWidget/bath_off"
		<<"contentWidget/party_off"<<"contentWidget/motion_off"
		<<"contentWidget/music_off"<<"contentWidget/muse_off"
	    <<"contentWidget/romantic_off"<<"contentWidget/short_rest_off"
		<<"contentWidget/cooking_off"<<"contentWidget/save_off"
	    <<"contentWidget/purify_off";
	
	string_list2<<"contentWidget/userdef_on"<<"contentWidget/morning_on"
		<<"contentWidget/enjoy_on"<<"contentWidget/night_on"
		<<"contentWidget/dinner_on"<<"contentWidget/read_on"
		<<"contentWidget/welcome_on"<<"contentWidget/rest_on"
		<<"contentWidget/away_on"<<"contentWidget/metting_on"
		<<"contentWidget/relax_on"<<"contentWidget/bath_on"
		<<"contentWidget/party_on"<<"contentWidget/motion_on"
		<<"contentWidget/music_on"<<"contentWidget/muse_on"
	    <<"contentWidget/romantic_on"<<"contentWidget/short_rest_on"
		<<"contentWidget/cooking_on"<<"contentWidget/save_on"
	    <<"contentWidget/purify_on";
	
	string_list3<<"contentWidget/userdef_off_b"<<"contentWidget/morning_off_b"
		<<"contentWidget/enjoy_off_b"<<"contentWidget/night_off_b"
		<<"contentWidget/dinner_off_b"<<"contentWidget/read_off_b"
		<<"contentWidget/welcome_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/away_off_b"<<"contentWidget/metting_off_b"
		<<"contentWidget/relax_off_b"<<"contentWidget/bath_off_b"
		<<"contentWidget/party_off_b"<<"contentWidget/motion_off_b"
		<<"contentWidget/music_off_b"<<"contentWidget/muse_off_b"
	    <<"contentWidget/romantic_off_b"<<"contentWidget/rest_off_b"
		<<"contentWidget/cooking_off_b"<<"contentWidget/save_off_b"
	    <<"contentWidget/purify_off_b";
	
	string_list4<<"contentWidget/userdef_on_b"<<"contentWidget/morning_on_b"
		<<"contentWidget/enjoy_on_b"<<"contentWidget/night_on_b"
		<<"contentWidget/dinner_on_b"<<"contentWidget/read_on_b"
		<<"contentWidget/welcome_on_b"<<"contentWidget/rest_on_b"
		<<"contentWidget/away_on_b"<<"contentWidget/metting_on_b"
		<<"contentWidget/relax_on_b"<<"contentWidget/bath_on_b"
		<<"contentWidget/party_on_b"<<"contentWidget/motion_on_b"
		<<"contentWidget/music_on_b"<<"contentWidget/muse_on_b"
	    <<"contentWidget/romantic_on_b"<<"contentWidget/rest_on_b"
		<<"contentWidget/cooking_on_b"<<"contentWidget/save_on_b"
	    <<"contentWidget/purify_on_b";

	ENNO_CHECK_NULL_RET(buf);
	
    ret = enno_decode_uint(buf, &sid, &len, len_buf);
	ENNO_CHECK_RET(ret);
	buf += len;
    len_buf -= len;

	ret = enno_decode_uint(buf, &status, &len, len_buf);
	ENNO_CHECK_RET(ret);

    //qDebug()<<"get_scene_status,sid="<<sid<<", status="<<status<<endl;
    if (g_sid_index[sid] != 0xff)
    {
	    i = g_sid_index[sid];
		if (i >= MAX_SCENE)
		{
		    qDebug()<<"fatal err, sid > MAX_SCENE";
			return ENNO_UNKOWN;
		}
        if (ast_dev[i].scene_id != sid)
        {
         //   qDebug()<<"ast_sid="<<ast_dev[i].scene_id<<"sid="<<sid<<endl;
            return ENNO_OK;
        }
		if (i >= button_list.count())
		{
		//    qDebug()<<"i="<<i<<endl;
		    return ENNO_OK;
		}
			
		button_state[i] = status;
		SceneButton *tool_button = button_list.at(i);
		
	//	printf("tool_button->isChecked()=%d\n", button_state[i]);
		wait_for_status = 0;
	    //timer_exe->stop();
	   // update();
	    //if (!light_flag)
		//    content->show();
		
		i_image_id = (ast_dev[i].image_id > string_list.size()) ? 0 : ast_dev[i].image_id;
		if(button_state[i])
		{
	        //tool_button->setChecked(false);
		    // QPixmap pixmap(string_list2.at(i));
		    QPixmap pixmap(string_list2.at(i_image_id));
		    tool_button->setIcon(pixmap);
			tool_button->setImg(string_list4.at(i_image_id));
		    tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));
		    tool_button->update();  
		}
		else 
		{
		    QPixmap pixmap(string_list.at(i_image_id));
			tool_button->setIcon(pixmap);
			tool_button->setImg(string_list3.at(i_image_id));
			tool_button->setIconSize(QSize(SCENE_BUTTON_X,SCENE_BUTTON_Y));//pixmap.size());
			tool_button->update();
		}
		if( g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->updateScene(sid,status);
		}
    }
	
	return ENNO_OK;

}
void ContentWidget::setDuration(int d)
{
    duration = d;
}

void ContentWidget::paintPrevious(QPainter &paint, QImage *img)
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

void ContentWidget::paintNext(QPainter &paint, QImage *img)
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

void ContentWidget::valueChanged_slot(QVariant value)
{
    //qDebug()<<"QA valueChanged_slot";

    currentValue = value;
    update();
}

void ContentWidget::animationFinished()
{
    //qDebug()<<"QA animationFinished";
    isAnimation = false;
   // widget(currentIndex())->show();
   // setCurrentIndex(nextIndex);
    content->show();
    delete current_image;
    delete switch_new_image;
}

void ContentWidget::showContent()
{
    content->show();  
}

void ContentWidget::next()
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

void ContentWidget::forward()
{
    //qDebug()<<"QA forward";

    if( isAnimation )
    {
        return;
    }
	snap_shot(&current_image);

	current_image->save("c2_image.jpg","JPG",100);
	switch_new_image->save("s2_image.jpg","JPG",100);
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


sceneWidget::sceneWidget(QWidget *parent) : QWidget(parent)
{
	
}

void sceneWidget::setButton(SceneButton *scene_button)
{
    cur_button = scene_button;
}
bool sceneWidget::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug()<<"sceneWidget::eventFilter"<<endl;

    if(event->type() == QEvent::Paint)
    {
        magicTime();
    }
    return QWidget::eventFilter(watched,event);
}

void sceneWidget::magicTime()
{
   // qDebug()<<"sceneWidget::magicTime"<<endl;

	QPainter painter(this);
	QRectF rectangle(0.0, 0.0, 130.0, 130.0);
	painter.setPen(QPen(Qt::blue,10));//设置画笔形式
	painter.drawEllipse(rectangle);//画圆
}

void sceneWidget::paintEvent(QPaintEvent *e)
{
#if 0
	QPainter painter(this);
	QRectF rectangle(0.0, 0.0, 140.0, 140.0);
	painter.setPen(QPen(Qt::blue,10));//设置画笔形式
	painter.drawEllipse(rectangle);//画圆
#endif
#if 0
    QPainter paint(this);

    qDebug()<<"sceneWidget::paintEvent"<<endl;
    return;
    if (cur_button == 0)
    {
        return;
    }
#if 0
	QPainter painter(img);
	painter.setPen(QPen(Qt::red,10));//设置画笔形式
	QRect target(0.0, 0.0, 130.0, 130.0); //建立目标矩形，该区域是显示图像的目的地
	QRect source(0.0, 0.0, 130.0, 130.0);

    painter.drawEllipse(0, 0, 125, 125); //x,y,w,h 

    paint.drawImage(target,*img,source);
	
	int n = 100;
	img->save("scene.jpg","JPG",n);
	cur_button = 0;
#endif
	QWidget *w = this;
    QPixmap pixmap(w->size());

	w->render(&pixmap);
	QImage img = pixmap.toImage();
	QPainter painter(&img);

	QRect target(0.0, 0.0, 320.0, 320.0); //建立目标矩形，该区域是显示图像的目的地
	QRect source(0.0, 0.0, 320.0, 320.0);
	
	painter.setPen(QPen(Qt::blue,10));//设置画笔形式
    painter.drawEllipse(0, 0, 130, 130); //x,y,w,h 
	paint.drawImage(target,img,source);

	int n = 100;
	img.save("wid.jpg","JPG",n);
	cur_button = 0;
#if 0
	QWidget *w = this;
	QPixmap pixmap(w->size());
	//将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
	w->render(&pixmap);
	QRect r = w->geometry();
	//绘制当前的Widget
	// QRectF r1(0.0, 0.0, r.width(), r.height() + value);
	//QRectF r2(0, r.height() + value, r.width(), value);
	
	QRectF r1(0.0, 0, 320, 320);
	QRectF r2(0, 0, 320, 320);
	painter.drawPixmap(r1, pixmap, r2);

#endif
#if 0
    QRectF rect(0.0, 160.0, 130.0, 130.0); //x,y,w,h  
    int startAngle = 1 * 16; //值为，实际角度 * 16  
    int spanAngle = 90 * 16;  
      
    //三个参数：rect表示弧线所在的矩形，startAngle起始角度，spanAngle跨越角度  
    //painter.drawArc(rect, startAngle, spanAngle);
	//painter.drawArc(rect, 1*16, 360*16);
	//painter.drawArc(rect, 180*16, 270*16);
	//painter.drawArc(rect, 270*16, 360*16);

	painter.drawEllipse(0, 160, 130, 130); //x,y,w,h  
#endif
#endif
}




