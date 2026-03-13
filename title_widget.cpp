#include "title_widget.h"
#include "tool_button.h"
#include <QListWidget>
#include <QGraphicsBlurEffect>
#include "mainwinget.h"


extern MainWidget * g_ptr_main_widget;


int g_current_room;
int g_room_num;
int g_current_title;
int g_aqi_value = 0;

#define ICON_LEVEL0_SIZE_Y 70    //128
#define ICON_LEVEL1_SIZE_Y 70    //104
#define ICON_LEVEL2_SIZE_Y 70     //80

//#define TIME_OF_SCENE_SWITCH 100
#define MAX_SWITCH_BUTTOM_LEN 200
int g_current_house_index = 0;
int g_house_button_state = 0;
HouseButton::HouseButton(QWidget *parent)
	:QPushButton(parent)
{
	mouse_over = false;
	mouse_press = false;
}

void HouseButton::mousePressEvent(QMouseEvent *event)
{
	mouse_is_move = 0;
	g_current_house_index = m_index;
	g_house_button_state = 1;

	emit pressButton();

    QPushButton::mousePressEvent(event);
	QWidget::mousePressEvent(event);
}

void HouseButton::mouseMoveEvent(QMouseEvent *event)
{
	button_move_pos = event->pos();
	mouse_is_move = 1; 
    QWidget::mouseMoveEvent(event);
}

void HouseButton::mouseReleaseEvent(QMouseEvent *event)
{		
	QPushButton::mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}

void HouseButton::setIndex(int index)
{
    m_index = index;
}

void HouseButton::setLevel(int level)
{
    m_level = level;
}

TitleWidget::TitleWidget(TreeModel *model, QWidget *parent)
	: QWidget(parent)
{
    m_mouseMoveCnt = 0;
	
	QWidget::setAttribute( Qt::WA_AlwaysShowToolTips );

    this->setFixedSize(320, 320);
	
    create_title_widget();  
	create_house_widget(model);

	current_Widget = wid_scene;
	wid_scene->raise();
	wid_house->hide();
	qDebug()<<"TitleWidget end";

	//hvac_wid->raise();
	//curtain_wid->raise();
	//scene_wid->raise();
	//this->translateLanguage();
}
void TitleWidget::set_aqi()
{
    char auc_aqi[20];
	int data = g_aqi_value;
	QString str;
	//qDebug()<<"TitleWidget::paintEvent";
	if (data < 50)
	{
	    str = "font-size:36px;color:rgb(49,209,49);";
	}
	else if (data < 100)
	{
	    str = "font-size:36px;color:rgb(216,214,38);";
	}
	else if (data < 150)
	{
	    str = "font-size:36px;color:rgb(231,128,25);";
	}
	else if (data < 200)
	{
	    str = "font-size:36px;color:rgb(223,45,28);";
	}
	else if (data < 300)
	{
	    str = "font-size:36px;color:rgb(148,12,50);";
	}
	else if (data)
	{
	    str = "font-size:36px;color:rgb(170,50,185);";
	}

	sprintf(auc_aqi, "AQI %d", g_aqi_value);
	ENNO_LOG_ERR("set aqi=%d\n", g_aqi_value);
	//lable_aqi->setStyleSheet(str);
	//lable_aqi->setText(auc_aqi);

	update();

}

void TitleWidget::paintEvent(QPaintEvent *)
{

	//sprintf(auc_aqi, "AQI %d", g_aqi_value);
	//lable_aqi->setStyleSheet(str);
    // lable_aqi->setText(auc_aqi);
}

TitleWidget::~TitleWidget()
{
    delete wid_scene;
	delete scene_wid;
	delete curtain_wid;
	delete hvac_wid;
	delete aqi_wid;
	delete switch_button;
	delete scene_button;
	delete curtans_button;
	delete hvac_button;
    delete lable_aqi;
}

#ifdef TITLE_IMG_ENABLE
void TitleWidget::create_title_widget()
{
	QPalette text_palette = palette();
	text_palette.setColor(QPalette::ButtonText, QColor(230, 230, 230));

    wid_scene = new QWidget(this);

	scene_wid = new QWidget(wid_scene);
	curtain_wid = new QWidget(wid_scene);
	hvac_wid = new QWidget(wid_scene);
	aqi_wid = new QWidget(wid_scene);

    aqi_wid->setFixedSize(320, 40);
	scene_wid->setFixedSize(320, 90);
	curtain_wid->setFixedSize(320, 90);
	hvac_wid->setFixedSize(320, 90);

	switch_button = new QPushButton(aqi_wid);
	
	QPixmap light_button_pixmap;
	QPixmap curtans_button_pixmap;
	QPixmap hvac_button_pixmap;
	if (g_language_zh)
	{
	    light_button_pixmap.load(QString("menuButton/scene.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac02.png"));
	}
	else
	{
	    light_button_pixmap.load(QString("menuButton/scene_en.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain_en01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac_en02.png"));
	}
	//switch_button->setFont(font);
	//switch_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//QPixmap switch_button_pixmap(QString("menuButton/switch1.png"));
	//switch_button->setIcon(switch_button_pixmap);
	//switch_button->setIconSize(switch_button_pixmap.size());
	//switch_button->setFixedSize(switch_button_pixmap.width()+200, 40);
	//switch_button->setObjectName("functionButton");
	//switch_button->setPalette(text_palette);
	//switch_button->setText(tr("bed room"));
	
	switch_button->setStyleSheet("font-size:30px;color:white;text-align:right;background-color: black;");
	//switch_button->setStyleSheet("font-size:35px;background-color: white;border-width:2px;border-style:solid;border-color:black;color:black;font-weight:bold;text-align:left;");

    scene_button = new QToolButton(scene_wid);
	scene_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
	//light_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	//QPixmap light_button_pixmap(QString("menuButton/scene.png"));
	scene_button->setIcon(light_button_pixmap);
	scene_button->setIconSize(light_button_pixmap.size());
	scene_button->setFixedSize(light_button_pixmap.width()+0, light_button_pixmap.height()+0);
	scene_button->setObjectName("functionButton");
	scene_button->setPalette(text_palette);
	
    curtans_button = new QToolButton(curtain_wid);
	curtans_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
	//QPixmap curtans_button_pixmap(QString("menuButton/curtain01.png"));
	curtans_button->setIcon(curtans_button_pixmap);
	curtans_button->setIconSize(curtans_button_pixmap.size());
	curtans_button->setFixedSize(curtans_button_pixmap.width()+0, curtans_button_pixmap.height()+0);
	curtans_button->setObjectName("functionButton");
	curtans_button->setPalette(text_palette);

	hvac_button = new QToolButton(hvac_wid);
	hvac_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
	//QPixmap hvac_button_pixmap(QString("menuButton/hvac02.png"));
	hvac_button->setIcon(hvac_button_pixmap);
	hvac_button->setIconSize(hvac_button_pixmap.size());
	hvac_button->setFixedSize(hvac_button_pixmap.width()+0, hvac_button_pixmap.height()+0);
	hvac_button->setObjectName("functionButton");
	hvac_button->setPalette(text_palette);

	connect(scene_button, SIGNAL(clicked()), this, SLOT(light_button_slot()));
	connect(curtans_button, SIGNAL(clicked()), this, SLOT(curtain_button_slot()));
	connect(hvac_button, SIGNAL(clicked()), this, SLOT(content_button_slot()));
    connect(switch_button, SIGNAL(clicked()), this, SLOT(switch_button_slot()));

    aqi_value = 50;
	g_aqi_value = aqi_value;
    //lable_aqi = new QLabel(tr("AQI %1") .arg(aqi_value+1), aqi_wid);
	switch_button->setGeometry(QRect(0, -4, 50, 30));  //(0, -4, 50, 30)
	
	//lable_aqi->setFont(font);
	current_button = scene_button;
    //lable_aqi->setGeometry(QRect(380, 2, 140, 40));
    //lable_aqi->setStyleSheet("font-size:30px;color:rgb(0, 255, 0);text-align:left;");
	//wid_scene->setLayout(h_layout);
	scene_wid->move(0, 50);
	curtans_button->setGeometry(QRect(30, 24, 260, 104));
	curtain_wid->move(0, 130);
	hvac_wid->move(59,240);
}
#else
void TitleWidget::create_title_widget()
{
	QSize pic_s(90,90);
    wid_scene = new QWidget(this);
	scene_wid = new QWidget(wid_scene);
	curtain_wid = new QWidget(wid_scene);
	hvac_wid = new QWidget(wid_scene);
	aqi_wid = new QWidget(wid_scene);

	group_wins = new QParallelAnimationGroup;
	anim_group = new QParallelAnimationGroup;

	label_scene= new QLabel(wid_scene);
	label_scene->setGeometry(QRect(0, 0, OLED_PIX_X, OLED_PIX_Y));
	label_scene->setStyleSheet("background:transparent");
	label_scene->hide();

    aqi_wid->setFixedSize(OLED_PIX_X, 40);
	scene_wid->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);
	curtain_wid->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);
	hvac_wid->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);

	label_icon_scene = new QPushButton(scene_wid);
	label_icon_curtain = new QPushButton(curtain_wid);
	label_icon_hvac = new QPushButton(hvac_wid);

	label_icon_scene_big = new QPushButton(scene_wid);
	label_icon_curtain_big = new QPushButton(curtain_wid);
	label_icon_hvac_big = new QPushButton(hvac_wid);

	aqi_wid->move(0, 0);
	switch_button = new QPushButton(aqi_wid);
	switch_icon = new QToolButton(aqi_wid);
	label_icon_light = new QLabel(aqi_wid);
	group_anima = new QParallelAnimationGroup;

	//switch_button->setFont(font);
	//switch_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	QPixmap switch_button_pixmap(QString("menuButton/switch1.png"));
	switch_icon->setIcon(switch_button_pixmap);
	switch_icon->setIconSize(switch_button_pixmap.size());
	switch_icon->setFixedSize(40, 40);
	//switch_button->setFixedSize(280, 40);
	//switch_button->setPalette(text_palette);
	switch_button->setObjectName("switchButton");
	//switch_button->setStyleSheet("font-size:32px;color:white;font-weight:440;text-align:right;");//38

	switch_button->setStyleSheet("font-size:34px;color:white;text-align:right;background-color: black;");

	
	switch_icon->setToolButtonStyle(Qt::ToolButtonIconOnly);


    scene_button = new QPushButton(scene_wid);
	//scene_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//scene_button->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);
	//scene_button->setPalette(text_palette);
	scene_button->setText(tr("        Scenes"));
	//setIconLevel(scene_button, 0, scene_wid);
	
    curtans_button = new QPushButton(curtain_wid);
	//curtans_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	//curtans_button->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);
	//curtans_button->setPalette(text_palette);
	curtans_button->setText(tr("        Shades"));
	setIconLevel(curtans_button, 1, curtain_wid);

	hvac_button = new QPushButton(hvac_wid);
	//hvac_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	//hvac_button->setFixedSize(OLED_PIX_X, TITLE_MENU_HEIGHT);
	//hvac_button->setPalette(text_palette);
	hvac_button->setText(tr("        HVAC  "));
	setIconLevel(hvac_button, 2, hvac_wid);
	setIconLevel(scene_button, 0, scene_wid);

	connect(scene_button, SIGNAL(clicked()), this, SLOT(light_button_slot()));
	connect(curtans_button, SIGNAL(clicked()), this, SLOT(curtain_button_slot()));
	connect(hvac_button, SIGNAL(clicked()), this, SLOT(content_button_slot()));

	connect(label_icon_scene, SIGNAL(clicked()), this, SLOT(light_button_slot()));
	connect(label_icon_curtain, SIGNAL(clicked()), this, SLOT(curtain_button_slot()));
	connect(label_icon_hvac, SIGNAL(clicked()), this, SLOT(content_button_slot()));
    //connect(switch_button, SIGNAL(clicked()), this, SLOT(switch_button_slot()));
    //connect(switch_icon, SIGNAL(clicked()), this, SLOT(switch_button_slot()));

    aqi_value = 50;
	g_aqi_value = aqi_value;
    //lable_aqi = new QLabel(tr("AQI %1") .arg(aqi_value+1));
	switch_icon->setGeometry(QRect(10, 0, 40, 40));
	switch_button->setGeometry(QRect(90, 0, 230, 50));  //0, -4

	label_icon_light->setGeometry(QRect(280, 0, 40, 40));
//	label_icon_light->setStyleSheet("QWidget{background-color:rgb(0, 0, 0);}");
	QPixmap light_icon("menuButton/light_point_icon.png");
	label_icon_light->setPixmap(light_icon);
	label_icon_light->hide();
	
	current_button = scene_button;
	//lable_aqi->setGeometry(QRect(380, 0, 140, 40));
	//lable_aqi->setStyleSheet("font-size:30px;color:rgb(0, 255, 0);text-align:left;");
	
	hvac_wid->move(0,230);
	//hvac_wid->raise();

	curtain_wid->move(0,140);
    //QFile file(PAIR_OK_FILE);

    #if 0
	if (file.exists())
	{
	    qDebug()<<"file exist";
	}
	else
	{
		wid_scene->hide();//add by iovine
	}
    #endif
	
	//curtain_wid->raise();
	scene_wid->move(0, 50);

	QPixmap button_pixmap1;
	button_pixmap1.load(QString("menuButton/scene_icon.png"));
	label_icon_scene_big->setStyleSheet("background:transparent;");
	label_icon_scene_big->setIcon(button_pixmap1);
	label_icon_scene_big->setIconSize(pic_s);
	label_icon_scene_big->setGeometry(QRect(40, 0, 90, 90));
	label_icon_scene_big->hide();

	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/curtain_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);
	label_icon_curtain_big->setIconSize(pic_s);
	label_icon_curtain_big->setGeometry(QRect(40, 0, 90, 90));
	label_icon_curtain_big->hide();

	QPixmap button_pixmap3;
	button_pixmap3.load(QString("menuButton/hvac_icon.png"));
	label_icon_hvac_big->setStyleSheet("background:transparent;");
	label_icon_hvac_big->setIcon(button_pixmap3);
	label_icon_hvac_big->setIconSize(pic_s);
	label_icon_hvac_big->setGeometry(QRect(40, 0, 90, 90));
	label_icon_hvac_big->hide();

	qDebug()<<"create_title_widget end";
	//scene_wid->raise();
}

void TitleWidget::control_lihght_point(int room_id,int status)
{
	//printf("XXXXXXXXXroom_id=%d status=%d \r\n",room_id,status);

	light_status[room_id] =  status;
	
    if(g_current_room == room_id)
    {
        if(status)
		{
			label_icon_light->show();
			switch_button->setGeometry(QRect(50, 0, 230, 50));
		}
		else
		{

			label_icon_light->hide();
			switch_button->setGeometry(QRect(90, 0, 230, 50));
		}
    }
}


void TitleWidget::setIconLevel(QPushButton *button, int level, QWidget* wid)
{
	QPixmap button_pixmap0;
	QSize pic_s(90,90);
	QSize unchoose_pic(60,60);
	int start_x_pos = 40;

    switch(level)
    {
    case 0:
		//wid->setFixedSize(320, ICON_LEVEL0_SIZE_Y);
		button->setGeometry(QRect(0, 20, 320, ICON_LEVEL0_SIZE_Y));
		current_button = button;
		//button_pixmap0.load(QString("menuButton/no.png"));

		if (wid == scene_wid)
		{		
		    QPixmap button_pixmap;
			button_pixmap.load(QString("menuButton/scene_icon.png"));
			button->setStyleSheet("background-color:rgb(255,191,37);\
				font-size:40px;color:white;width:320px;font-weight:440;\
				height:90px;border-radius: 15px;  border: 2px;");
			//label_icon_scene->setPixmap(button_pixmap);
		    //label_icon_scene->setIconSize(pic_s);
		    //button->setIcon(button_pixmap0);
		    
			label_icon_scene->setStyleSheet("background:transparent;");
			label_icon_scene->setIcon(button_pixmap);
			label_icon_scene->setIconSize(pic_s);
			label_icon_scene->setGeometry(QRect(start_x_pos, 0, 90, 90));
			label_icon_scene->raise();
			
			//button->setIconSize(pic_s);
			QPixmap button1_pixmap;
			button1_pixmap.load(QString("menuButton/curtain_icon_off.png"));
			//label_icon_curtain->setPixmap(button1_pixmap);
			
			label_icon_curtain->setStyleSheet("background:transparent;");
			label_icon_curtain->setIcon(button1_pixmap);
			label_icon_curtain->setIconSize(unchoose_pic);
			label_icon_curtain->setGeometry(QRect(start_x_pos+15, 25, 60, 60));
			label_icon_curtain->raise();

			QPixmap button2_pixmap;
			button2_pixmap.load(QString("menuButton/hvac_icon_off.png"));
		    //label_icon_hvac->setPixmap(button2_pixmap);
			label_icon_hvac->setStyleSheet("background:transparent;");
		    label_icon_hvac->setIcon(button2_pixmap);
			label_icon_hvac->setIconSize(unchoose_pic);
			label_icon_hvac->setGeometry(QRect(start_x_pos+15, 25, 60, 60));
			label_icon_hvac->raise();
		}
		else if (wid == curtain_wid)
		{
		    QPixmap button_pixmap;
		    button_pixmap.load(QString("menuButton/curtain_icon.png"));
			button->setStyleSheet("background-color:rgb(123,226,80);\
				font-size:40px;color:white;width:320px;font-weight:440;\
				height:90px;border-radius: 15px;  border: 2px;transparent;");

			//label_icon_curtain->setPixmap(button_pixmap);
			//label_icon_scene->setIconSize(pic_s);
			label_icon_curtain->setStyleSheet("background:transparent;");
			label_icon_curtain->setIcon(button_pixmap);
			label_icon_curtain->setIconSize(pic_s);
			label_icon_curtain->setGeometry(QRect(start_x_pos, 0, 90, 90));

			QPixmap button1_pixmap;
			button1_pixmap.load(QString("menuButton/scene_icon_off.png"));
			//label_icon_scene->setPixmap(button1_pixmap);
			label_icon_scene->setStyleSheet("background:transparent;");
			label_icon_scene->setIcon(button1_pixmap);
			label_icon_scene->setIconSize(unchoose_pic);
			label_icon_scene->setGeometry(QRect(start_x_pos+15, 25, 60, 60));

			QPixmap button2_pixmap;
			button2_pixmap.load(QString("menuButton/hvac_icon_off.png"));
			//label_icon_hvac->setPixmap(button2_pixmap);
			label_icon_hvac->setStyleSheet("background:transparent;");
			label_icon_hvac->setIcon(button2_pixmap);
			label_icon_hvac->setIconSize(unchoose_pic);
			label_icon_hvac->setGeometry(QRect(start_x_pos+15, 25, 60, 60));
		}
		else
		{
		    QPixmap button_pixmap;
		    button_pixmap.load(QString("menuButton/hvac_icon.png"));
			button->setStyleSheet("background-color:rgb(90,224,255);\
				font-size:40px;color:white;width:320px;font-weight:440;\
				height:90px;border-radius: 15px;  border: 2px;");
				
			//label_icon_hvac->setPixmap(button_pixmap);
			//label_icon_scene->setIconSize(pic_s);
			label_icon_hvac->setIcon(button_pixmap);
			label_icon_hvac->setIconSize(pic_s);
			label_icon_hvac->setGeometry(QRect(start_x_pos, 0, 90, 90));
			label_icon_hvac->setStyleSheet("background:transparent;");

			QPixmap button1_pixmap;
			button1_pixmap.load(QString("menuButton/curtain_icon_off.png"));
			//label_icon_curtain->setPixmap(button1_pixmap);
			label_icon_curtain->setIcon(button1_pixmap);
			label_icon_curtain->setIconSize(unchoose_pic);
			label_icon_curtain->setGeometry(QRect(start_x_pos+15, 25, 60, 60));
			label_icon_curtain->setStyleSheet("background:transparent;");

			QPixmap button2_pixmap;
			button2_pixmap.load(QString("menuButton/scene_icon_off.png"));
			//label_icon_scene->setPixmap(button2_pixmap);
			label_icon_scene->setIcon(button2_pixmap);
			label_icon_scene->setIconSize(unchoose_pic);
			label_icon_scene->setGeometry(QRect(start_x_pos+15, 25, 60, 60));
			label_icon_scene->setStyleSheet("background:transparent;");
		}	
		
		//button->setIcon(button_pixmap);
		//button->setIconSize(pic_s);
		//button->setGeometry(QRect(0, 0, 320, ICON_LEVEL0_SIZE_Y));
		break;
		
	case 1:
		//button_pixmap.load(QString("menuButton/no.png"));
//		QPixmap button_pixmap(unchoose_pic);
		#if 0
		if (wid == scene_wid)
		{
			button_pixmap.load(QString("menuButton/scene_icon.png"));
		}
		else if (wid == curtain_wid)
		{
		    button_pixmap.load(QString("menuButton/curtain_icon.png"));
		}
		else
		{
		    button_pixmap.load(QString("menuButton/hvac_icon.png"));
		}	
		//button->setIcon(button_pixmap);
		//button->setIconSize(unchoose_pic);
		#endif
		button->setStyleSheet("background-color:rgb(85,85,85);\
				font-size:40px;color:white;font-weight:440;\
                border-radius: 15px; border: 2px;");
		button->setGeometry(QRect(0, 20, 320, ICON_LEVEL0_SIZE_Y));
		//wid->setFixedSize(320, ICON_LEVEL1_SIZE_Y);
		break;
		
	case 2:
		//button_pixmap.load(QString("menuButton/no.png"));
		#if 0
		if (wid == scene_wid)
		{
			button_pixmap.load(QString("menuButton/scene_icon.png"));
					}
		else if (wid == curtain_wid)
		{
		    button_pixmap.load(QString("menuButton/curtain_icon.png"));
		}
		else
		{
		    button_pixmap.load(QString("menuButton/hvac_icon.png"));
		}	
		//button->setIcon(button_pixmap);
		//button->setIconSize(unchoose_pic);
		#endif
		button->setStyleSheet("background-color:rgb(85,85,85);\
				font-size:40px;color:white;font-weight:440;\
				border-radius: 15px; border: 2px;");
		button->setGeometry(QRect(0, 20, 320, ICON_LEVEL0_SIZE_Y));
		//wid->setFixedSize(320, ICON_LEVEL2_SIZE_Y);
		break;
	
	default:
		break;
    }
}
#endif
QString g_room_name;

void TitleWidget::set_room_name()
{
	QString elideNote;

	QFontMetrics fontWidth(switch_button->font());
	elideNote = fontWidth.elidedText(g_room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
	switch_button->setText(elideNote);
	switch_button->setToolTip(g_room_name);

}
#define ROOM_ID "/home/user/ui/qwid/current_room_id"

int read_room_id_form_file()
{
    QFile file(ROOM_ID);

	//file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		g_current_room = 0;
		qDebug()<<"read_room_id_form_file error g_current_room"<<g_current_room;
		return ENNO_OK;
	}
	QByteArray line = file.readLine();
	g_current_room = line[0] - 0x30;
	if(g_current_room<0)
		g_current_room = 0;
	file.close();
	qDebug()<<"read_room_id_form_file g_current_room="<<g_current_room;
	
	return ENNO_OK;
}


void TitleWidget::create_house_widget(TreeModel *model)
{
	int j;
	g_room_num = 0;
	g_current_room = 0;

	

	QString room_name;
	QString elideNote;

	read_room_id_form_file();

	wid_house = new QWidget(this);
	//wid_house->setFixedSize(OLED_PIX_X, OLED_PIX_Y);

	QModelIndex title;
	QModelIndex room;
	
	group1 = new QParallelAnimationGroup;
	
	//qDebug()<<"title widget"<<endl;
	room = model->index(0, 0, title);
	//qDebug() << "model->columnCount(room)="<<model->columnCount(room)<<endl;
	//qDebug() << "model->rowCount(room)="<< model->rowCount(title)<<endl;
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
	g_room_num = room_num;
	//qDebug() << "room_num="<<room_num;
	
	if (room_num <= 1)
	{
	    switch_icon->setEnabled(false);
	    switch_icon->setVisible(0);
		if (1 == room_num)
		{
			room_name = astRoom[0].m_name;
			g_room_name = astRoom[0].m_name;
/*			QFontMetrics fontWidth(switch_button->font());
			elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
			switch_button->setText(elideNote);
			switch_button->setToolTip(room_name);
*/			
			QTimer::singleShot(1, this, SLOT(set_room_name()));
			//switch_button->setText(astRoom[0].m_name);
		}
		//qDebug()<<"[qterr]room num is error"<<room_num<<endl;
	   // return;
	}
	
	QSignalMapper *signal_mapper = new QSignalMapper(this);
	//QVBoxLayout *v_layout = new QVBoxLayout();
	//h_layout->addWidget(switch_button);

	for(int i=0; i<room_num; i++)
	{
	   // room_widget[i] = new QWidget(wid_house);
		
		room_button[i] = new HouseButton(wid_house);
		//push_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		//QPixmap button_pixmap(string_list.at(i));
		//push_button->setIcon(button_pixmap);
		room_button[i]->setIndex(i);

		button_list_house.append(room_button[i]);
		connect(room_button[i], SIGNAL(clicked()), signal_mapper, SLOT(map()));
		//room_button[i]->setFixedSize(290, 90);

		if(i==0)
		{
			room_button[i]->setGeometry(QRect(15, 0, 290, 100));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
						font-size:32px;color:white;font-weight:440;\
						border-radius: 15px;  border: 2px;");
			room_top = 0;
		}
		else if (i == 1)
		{
			room_button[i]->setGeometry(QRect(0, 110, OLED_PIX_X, 100));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
					font-size:40px;color:white;font-weight:440;\
	                border-radius: 15px;  border: 2px;");
		}
		else
		{
			room_button[i]->setGeometry(QRect(15, i*110, 290, 100));
			room_button[i]->setStyleSheet("background-color:rgb(85,85,85);\
						font-size:32px;color:white;font-weight:440;\
						border-radius: 15px;  border: 2px;");
		}
		//setButtonLevel(room_button[i], i);
		//QString s = "ab?D??";
		//int length = s.length(); //length = 4;
		//length = s.toLocal8Bit().length(); //length = 6

	    room_name = astRoom[i].m_name;
		QFontMetrics fontWidth(room_button[i]->font());
		elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, 100);
		room_button[i]->setText(elideNote);
		room_button[i]->setToolTip(room_name);
		room_button[i]->raise();
		
		//push_button->setGeometry(QRect(100,100,100,25));
		signal_mapper->setMapping(room_button[i], QString::number(i, 10));
		//qDebug()<<"addWidget push=["<<room_button[i]<<"]"<<endl;
		
		connect(room_button[i], SIGNAL(pressButton()), this, SLOT(press_button_slot()));
       // v_layout->addWidget(push_button);	
	}
	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(turnIcon(QString)));

/**************************************************/
//add by iovine for scroll bar
	label_bar = new QLabel(this);
	label_bar->setGeometry(QRect(310, 0, 10, 320));
	label_bar->setStyleSheet("QWidget{background-color:rgb(180, 180, 180);"\
			"border-radius:5px;}");

	label_bar->hide();

	label_bar_s = new QLabel(this);
	if (room_num>3)
	{
		label_bar_s->setGeometry(QRect(310, 60, 10, 320/(room_num-2)));
	}
	else
	{
		label_bar_s->setGeometry(QRect(310, 60, 10, 100));
	}

	label_bar_s->setStyleSheet("QWidget{background-color:rgb(255, 255, 255);"\
			"border-radius:5px;}");

	label_bar_s->hide();

	label_house= new QLabel(this);
	effect_bar= new QGraphicsOpacityEffect();
	effect_bars= new QGraphicsOpacityEffect();

	///room_offset = 0;

	///startx = 0;
	///starty = 0;

    ///int k = room_num;
	
	/*while (k--)
	{
	    //if (k!=1 )
	    //wid_get[k]->raise();
		if (k == 1)
		{		    
			wid_get[k]->move(0, (100+10)*k);
		}
		else
		{
		    wid_get[k]->move(10, (100+10)*k+10);
		}
	}*/
    room_name = astRoom[g_current_room].m_name;
	g_room_name = astRoom[g_current_room].m_name;
	/*QFontMetrics fontWidth(switch_button->font());
	elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
	switch_button->setText(elideNote);
	switch_button->setToolTip(room_name);*/
	QTimer::singleShot(1, this, SLOT(set_room_name()));
	current_room = 0;
	page_first_room = 0;
	move_first_room = 0;
	//switch_button->setText(astRoom[g_current_room].m_name);
    if (room_num >= 4)
	{
		house_height = 110*room_num - 10;
    }
	else
	{
		house_height = 320;
	}
	
	wid_house->setFixedSize(320, house_height);
	wid_house->raise();
	room_pos_y = 0;

	label_bar->raise();
	label_bar_s->raise();

	if (room_num >= 2)
	{
	    connect(switch_button, SIGNAL(clicked()), this, SLOT(switch_button_slot()));
	    connect(switch_icon, SIGNAL(clicked()), this, SLOT(switch_button_slot()));
	}
	else
	{
	    switch_icon->setVisible(0);
	}

}
void TitleWidget::setButtonLevel(HouseButton *button, int level)
{
    switch(level)
    {

    case 0:
		button->setStyleSheet("background-color:rgb(100,100,100);\
			font-size:30px;color:white;font-weight:440;width:320px;\
			height:90px;border-radius: 15px;  border: 2px;");	
		//button->setGeometry(QRect(0, 0, 320, 90));//52,204,255
		button->setGeometry(QRect(0, 0, 300, 90));	
		button->setLevel(level);
		break;
				
	case 1:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:30px;color:white;font-weight:440;\
                border-radius: 15px;  border: 2px;");
		//button->setGeometry(QRect(15, 0, 320-30, 90));
		button->setGeometry(QRect(0, 0, OLED_PIX_X, 100));	
		button->setLevel(level);
		break;
		
	case 2:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:30px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");
		//button->setGeometry(QRect(30, 0, 320-60, 90));
		button->setGeometry(QRect(0, 0, 300, 90));
		button->setLevel(level);
		break;

	case 3:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:30px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		//button->setGeometry(QRect(45, 0, 320-90, 90));
		button->setGeometry(QRect(0, 0, 300, 90));		
		button->setLevel(level);
		break;
		
	default:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:30px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		//button->setGeometry(QRect(45, 0, 320-90, 90));
		button->setGeometry(QRect(0, 0, 300, 90));
		break;
    }
}

void TitleWidget::setButtonStytle(HouseButton *button, int level)
{
   // qDebug()<<"setButtonStytle level"<<level;
    switch(level)
    {
    case 0:
		button->setStyleSheet("background-color:rgb(100,100,100);\
			font-size:32px;color:white;font-weight:440;width:320px;\
			height:90px;border-radius: 15px;  border: 2px;");	
		button->setLevel(level);
		break;
				
	case 1:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:40px;color:white;font-weight:440;\
                border-radius: 15px;  border: 2px;");
		button->setLevel(level);
		break;
		
	case 2:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:32px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		button->setLevel(level);
		break;

	case 3:
		button->setStyleSheet("background-color:rgb(100,100,100);\
				font-size:32px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");
		button->setLevel(level);
		break;

	case 5:
		button->setStyleSheet("background-color:rgb(52,204,255);\
				font-size:32px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		break;

	case 6:
		button->setStyleSheet("background-color:rgb(52,204,255);\
				font-size:40px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		break;
		
	default:
		button->setStyleSheet("background-color:rgb(109,109,109);\
				font-size:32px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		break;
    }
}

void TitleWidget::setButtonStytleOffset(HouseButton *button, int level, int offset)
{
    int tmp = (offset%110)/2;
    QString str1("font-size:30px;color:white;font-weight:bold;border-radius: 15px;  border: 2px;");
    QString str2;

	if (!move_direct)
	{
	    return;
	}

	if (offset%110 == 0)
	{
	    return;
	}
    switch(level)
    {
    case 0:
		if (move_direct == EN_MOVE_UP)
		{
			tmp = 100;
		}
		else if (move_direct == EN_MOVE_DOWN)
		{
		    tmp = 100;
		}
		else
		{
		    tmp = 100;
		}
		//QString str1 = QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
		//qDebug()<<"stytle="<<QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
		//button->setStyleSheet();	
		button->setLevel(level);
		break;
#if 1				
	case 1:
		if (move_direct == EN_MOVE_UP)
		{
			//tmp = 150 - tmp;
		}
		else if (move_direct == EN_MOVE_DOWN)
		{
		   // tmp += 100;
		}
		else
		{
		   // tmp = 100;
		}
		tmp = 100;
		//QString str2 = QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
		//qDebug()<<"stytle="<<str2;
		//button->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp));
		button->setLevel(level);
		break;
		
	case 2:
		if (move_direct == EN_MOVE_UP)
		{
			//tmp += 100;
		}
		else if (move_direct == EN_MOVE_DOWN)
		{
		  //  tmp = 150 - tmp;
		}
		else
		{
		  //  tmp = 100;
		}
		tmp = 100;
		//QString str3 = QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
		//qDebug()<<"stytle="<<str3;
		//QString str1 = QString("background-color:rgb(%1,%2,%3)").arg(tmp).arg(tmp).arg(tmp);
		//button->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp));
		button->setLevel(level);
		break;

	case 3:
		if (move_direct == EN_MOVE_UP)
		{
		    tmp = 100;
		}
		else if (move_direct == EN_MOVE_DOWN)
		{
		    tmp = 100;
		}
		else
		{
		    tmp = 100;
		}
		//QString str4 = QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
		//qDebug()<<"stytle="<<str4;
		//QString str1 = QString("background-color:rgb(%1,%2,%3)").arg(tmp).arg(tmp).arg(tmp);
		//button->setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp));
		button->setLevel(level);
		break;
#endif
	case 5:
		button->setStyleSheet("background-color:rgb(52,204,255);\
				font-size:30px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		return;
		
	default:
		button->setStyleSheet("background-color:rgb(109,109,109);\
				font-size:30px;color:white;font-weight:440;\
				border-radius: 15px;  border: 2px;");	
		return;
    }
	
   // qDebug()<<"setButtonStytle level"<<level<<", tmp="<<tmp;
	
    //qDebug()<<"off"<<offset<<", m="<<move_direct<<"l="<<level<<",st="<<tmp;
	str2 = str1 + QString("background-color:rgb(%1,%2,%3);").arg(tmp).arg(tmp).arg(tmp);
	button->setStyleSheet(str2);
	
}

void TitleWidget::press_button_check()
{
#if 1
    if (g_house_button_state)
    {
        g_house_button_state = 2;
    }
	else
	{
	    return;
	}

    if (room_button[g_current_house_index]->mouse_is_move != 0)
    {
        return;
    }
	//qDebug()<<"call setButtonStytle func="<<__func__<<",line="<<__LINE__;
	//setButtonStytle(room_button[g_current_house_index], 5);
#endif


}
void TitleWidget::press_button_slot()
{
   // QTimer::singleShot(200, this, SLOT(press_button_check()));
}


void TitleWidget::switch_to_scene_first()
{
	printf("switch_to_scene_first\r\n");
	pthread_mutex_lock(&mutex_turnPage);
    switch(g_current_title)
	{
	case 0:
		this->turnPage("0", 0);
		break;
	case 1:
		this->turnPage("1", 1);
		break;
	case 2:
		this->turnPage("2", 2);
		break;
	}

		//wid_scene->hide();
		wid_scene->show();
		wid_scene->raise();
		current_Widget = wid_scene;

		QPropertyAnimation *animation_scene = new QPropertyAnimation(wid_scene, "geometry");

		animation_scene->setDuration(400);
		animation_scene->setStartValue(QRect(+wid_scene->width(), 0, wid_scene->width(), wid_scene->height()));
		animation_scene->setEndValue(QRect(0, 0, wid_scene->width(), wid_scene->height()));

		animation_scene->start(QAbstractAnimation::DeleteWhenStopped);
		//connect(group_wins, SIGNAL(finished()), this, SLOT(bottomAniFinished()));
		
		QTimer::singleShot(410, this, SLOT(bottomAniFinished()));
		room_button[g_current_house_index]->setStyleSheet("background-color:rgb(85,85,85);\
								font-size:30px;color:white;font-weight:440;\
								border-radius: 15px;  border: 2px;");

		unlockTurnPageMutex(this);
								

}

void TitleWidget::scene_show_delay()
{
	wid_scene->show();
	wid_scene->raise();
}


void TitleWidget::bottomAniFinished()
{
    g_BottomSreenTurning = 0;

	switch_icon->setEnabled(true);
	switch_button->setEnabled(true);
	
	qDebug()<<"switch icon true";
}


static int g_second_room=1;

void TitleWidget::switch_to_scene()
{
    //pthread_mutex_lock(&mutex_turnPage);
    switch(g_current_title)
	{
	case 0:
		this->turnPage("0", 0);
		break;
		
	case 1:
		this->turnPage("1", 1);
		break;
		
	case 2:
		this->turnPage("2", 2);
		break;
	}
#if 0
	wid_scene->show();
    wid_scene->raise();
	current_Widget = wid_scene;
	
	room_button[g_current_house_index]->setStyleSheet("background-color:rgb(125,125,125);\
							font-size:30px;color:white;font-weight:bold;\
							border-radius: 15px;  border: 2px;");
#endif
	//label_house->resize(wid_house->size());
	label_house->setPixmap(this->grab());
	label_house->show();
	label_house->raise();
/*	QWidget *w = this;
	QPixmap pixmap(w->size());

	w->render(&pixmap);

	label_house->setPixmap(pixmap);
	label_house->show();*/

	//QImage img = pixmap.toImage();
	//*w_image = new QImage(img);
	//img.save("xxx.jpg","JPG",100);
	//printf("XXXXXXXX\r\n");
	//wid_scene->hide();
	current_Widget = wid_scene;
    g_BottomSreenTurning = 1;
	QPropertyAnimation *animation_scene = new QPropertyAnimation(wid_scene, "geometry");
	QPropertyAnimation *animation_house = new QPropertyAnimation(label_house, "geometry");
	//group_wins = new QParallelAnimationGroup;
	
	group_wins->clear();

	animation_house->setDuration(300);
	animation_house->setStartValue(QRect(0,0,320,320));
	animation_house->setEndValue(QRect(-OLED_PIX_X, 0, 320, 320));

	animation_scene->setDuration(300);
	animation_scene->setStartValue(QRect(+wid_scene->width(), 0, wid_scene->width(), wid_scene->height()));
	animation_scene->setEndValue(QRect(0, 0, wid_scene->width(), wid_scene->height()));

	group_wins->addAnimation(animation_house);
	group_wins->addAnimation(animation_scene);
	g_BottomSreenTurning = 1;
	QTimer::singleShot(10, this, SLOT(scene_show_delay()));
	group_wins->start();
	//connect(group_wins, SIGNAL(finished()), this, SLOT(bottomAniFinished()));
	
	QTimer::singleShot(310, this, SLOT(bottomAniFinished()));

    if (room_num > 1)
	{
		if(g_second_room == g_current_house_index)
		{
			room_button[g_current_house_index]->setStyleSheet("background-color:rgb(85,85,85);\
								font-size:40px;color:white;font-weight:440;\
								border-radius: 15px;  border: 2px;");
		}
		else
		{
			room_button[g_current_house_index]->setStyleSheet("background-color:rgb(85,85,85);\
								font-size:32px;color:white;font-weight:440;\
								border-radius: 15px;  border: 2px;");
		}
	}
	
	//unlockTurnPageMutex();	
}

void TitleWidget::turnIcon(QString current_icon)
{
	bool ok;  
	int current_index = current_icon.toInt(&ok, 10);
	HouseButton *h_button = button_list_house.at(current_index);

	char cmd_buf[64];

	QString room_name;
	QString elideNote;

	if (h_button->mouse_is_move)
	{
	    return;
	}

	//qDebug()<<"turnIcon";

	g_house_button_state = 0;
	
	//qDebug()<<"call setButtonStytle func="<<__func__<<",line="<<__LINE__;
	if (g_second_room == g_current_house_index)
	{
		setButtonStytle(room_button[g_current_house_index], 6);
	}
	else
	{
		setButtonStytle(room_button[g_current_house_index], 5);
	}
	//setButtonStytle(room_button[g_current_house_index], room_button[g_current_house_index]->m_level);
		
	g_current_room = current_index;//current_room + room_offset;
	//qDebug()<<"g_current_room="<<g_current_room<<"name="<<astRoom[g_current_room].m_name;
	//qDebug()<<"g_current_house_index="<<g_current_house_index;
	//if (g_current_room == current_index)
	{

		room_name = astRoom[g_current_room].m_name;

		control_lihght_point(g_current_room,light_status[g_current_room]);

		QFontMetrics fontWidth(switch_button->font());
		elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
		switch_button->setText(elideNote);
		switch_button->setToolTip(room_name);

		//switch_button->setText(astRoom[g_current_room].m_name);
		for(int i=0; i<room_num; i++)
		{
		    room_button[i]->setEnabled(false);
		}
		
		qDebug()<<"room_button false g_current_room=  current_icon="<<g_current_room<<current_icon;

		
		snprintf(cmd_buf, 64, "echo %d > /home/user/ui/qwid/current_room_id",g_current_room );
		system(cmd_buf);
		usleep(20000);
		system("sync");

		

	    QTimer::singleShot(100, this, SLOT(switch_to_scene()));
		update();
		#if 0
		switch(g_current_title)
		{
		case 0:
			this->turnPage("0");
			break;
			
		case 1:
			this->turnPage("1");
			break;
			
		case 2:
			this->turnPage("2");
			break;
		}
		wid_scene->show();
	    wid_scene->raise();
		current_Widget = wid_scene;
		#endif
	}

	//current_room = current_index;
	//g_current_room = current_room + room_offset;

	//turnRoom(current_room);
}

void TitleWidget::selectRoom(int index,int flg)
{
    g_house_button_state = 0;
	g_current_title = 0;

	QString room_name;
	QString elideNote;

	g_current_house_index = index;
	if (room_num > 1)
	{
	    qDebug()<<"room_num="<<room_num;
		setButtonStytle(room_button[g_current_house_index], 5);
	}
	//setButtonStytle(room_button[g_current_house_index], room_button[g_current_house_index]->m_level);
		
	g_current_room = index;//current_room + room_offset;

	room_name = astRoom[g_current_room].m_name;
	QFontMetrics fontWidth(switch_button->font());
	elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
	switch_button->setText(elideNote);
	switch_button->setToolTip(room_name);
	//switch_button->setText(astRoom[g_current_room].m_name);
	
	for(int i=0; i<room_num; i++)
	{
		room_button[i]->setEnabled(false);
	}
	qDebug()<<"select room_button false";

	if(flg)
	{
		QTimer::singleShot(1, this, SLOT(switch_to_scene_first()));
	}
	else
	{
	    QTimer::singleShot(100, this, SLOT(switch_to_scene()));
	}

	update();
}

void TitleWidget::turnRoom(int current_room)
{
   /* for (int i = 0; i < 4; i++)
    {
        wid_get[i] = room_widget[i+room_offset];
    }*/
	
	//return;//add by iovine for test
	QString room_name;
	QString elideNote;

	g_current_room = current_room + room_offset;
	
	room_name = astRoom[g_current_room].m_name;
	QFontMetrics fontWidth(switch_button->font());
	elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
	switch_button->setText(elideNote);
	switch_button->setToolTip(room_name);

	//switch_button->setText(astRoom[g_current_room].m_name);

    if (current_room == 0)
	{
	    if (room_num-room_offset >= 4)
		{
		    HouseButton *button = button_list_house.at(room_offset+3);
			button->setText(astRoom[room_offset+3].m_name);
		    setButtonLevel(button, 3);	
		    room_button[3]->raise();
		    room_button[3]->move(0, 230);
		}

		if (room_num-room_offset >= 3)
		{
		    setButtonLevel(button_list_house.at(room_offset+2), 2);
			button_list_house.at(room_offset+2)->setText(astRoom[room_offset+2].m_name);
			room_button[2]->raise();
			room_button[2]->move(0, 160);
		}

	    if (room_num-room_offset >= 2)
	    {
		    setButtonLevel(button_list_house.at(room_offset+1), 1);	
			button_list_house.at(room_offset+1)->setText(astRoom[room_offset+1].m_name);
			room_button[1]->raise();
			room_button[1]->move(0, 80);
	    }

		setButtonLevel(button_list_house.at(room_offset), 0);
		button_list_house.at(room_offset)->setText(astRoom[room_offset].m_name);
		room_button[0]->raise();
		room_button[0]->move(0, 0);
	}
	else if (current_room == 1)
	{
		if (room_num-room_offset >= 4)
		{
		    setButtonLevel(button_list_house.at(room_offset+3), 2);
			button_list_house.at(room_offset+3)->setText(astRoom[room_offset+3].m_name);
			room_button[3]->raise();
			room_button[3]->move(0, 240);
		}

		if (room_num-room_offset >= 3)
		{
		    setButtonLevel(button_list_house.at(room_offset+2), 1);
			button_list_house.at(room_offset+2)->setText(astRoom[room_offset+2].m_name);
			room_button[2]->raise();
			room_button[2]->move(0, 160);
		}
		
		setButtonLevel(button_list_house.at(room_offset+0), 1);
		button_list_house.at(room_offset)->setText(astRoom[room_offset].m_name);
		room_button[0]->raise();
		room_button[0]->move(0, 0);

		setButtonLevel(button_list_house.at(room_offset+1), 0);
		button_list_house.at(room_offset+1)->setText(astRoom[room_offset+1].m_name);
		room_button[1]->raise();
		room_button[1]->move(0, 80);
	}
	else if (current_room == 2)
	{
		if (room_num - room_offset >= 4)
		{
		    setButtonLevel(button_list_house.at(3+room_offset), 1);
			button_list_house.at(room_offset+3)->setText(astRoom[room_offset+3].m_name);
			room_button[3]->raise();
			room_button[3]->move(0, 230);
		} 	    
		
		setButtonLevel(button_list_house.at(room_offset), 2);
		button_list_house.at(room_offset)->setText(astRoom[room_offset].m_name);
		room_button[0]->raise();
		room_button[0]->move(0, 0);

		setButtonLevel(button_list_house.at(room_offset+1), 1);
		button_list_house.at(room_offset+1)->setText(astRoom[room_offset+1].m_name);
		room_button[1]->raise();
		room_button[1]->move(0, 80);

		setButtonLevel(button_list_house.at(room_offset+2), 0);
		button_list_house.at(room_offset+2)->setText(astRoom[room_offset+2].m_name);
		room_button[2]->raise();
		room_button[2]->move(0, 160);
	}
	else if (current_room == 3)
	{
		setButtonLevel(button_list_house.at(room_offset+0), 3);
		button_list_house.at(room_offset)->setText(astRoom[room_offset].m_name);
		room_button[0]->raise();
		room_button[0]->move(0, 0);
		
		setButtonLevel(button_list_house.at(room_offset+1), 2);		
		button_list_house.at(room_offset+1)->setText(astRoom[room_offset+1].m_name);
		room_button[1]->raise();
		room_button[1]->move(0, 80);
		
		setButtonLevel(button_list_house.at(room_offset+2), 1);
		button_list_house.at(room_offset+2)->setText(astRoom[room_offset+2].m_name);
		room_button[2]->raise();
		room_button[2]->move(0, 160);

		setButtonLevel(button_list_house.at(room_offset+3), 0);		
		button_list_house.at(room_offset+3)->setText(astRoom[room_offset+3].m_name);
		room_button[3]->raise();
		room_button[3]->move(0, 230);
	} 
}

void TitleWidget::switch_button_slot()
{
#if 0
    wid_scene->hide();
	wid_house->show();
    wid_house->raise();
	current_Widget = wid_house;
#endif
    
	if(g_upSreenTurning||g_BottomSreenTurning)
    {
        return;
    }

    if(g_ptr_main_widget!=NULL && g_ptr_main_widget->get_state() == EN_SYS_SET)
    {
        return;
    }
	
	switch_icon->setEnabled(false);
	switch_button->setEnabled(false);
	qDebug()<<"switch icon false";
    //pthread_mutex_lock(&mutex_turnPage);
    g_BottomSreenTurning = 1;
    emit switch_to_house(g_current_room, astRoom[g_current_room].m_name);
	
	//label_scene->resize(wid_scene->size());
	label_scene->setPixmap(wid_scene->grab());
	label_scene->show();
	//wid_house->hide();

	current_Widget = wid_house;
	wid_house->show();
	wid_house->raise();

	 QPropertyAnimation *animation_scene = new QPropertyAnimation(label_scene, "geometry");
	 QPropertyAnimation *animation_house = new QPropertyAnimation(wid_house, "geometry");
	 group_wins->clear();

	 animation_scene->setDuration(300);
	 animation_scene->setStartValue(QRect(0,0,320,320));
	 animation_scene->setEndValue(QRect(320, 0, 320, 320));

	 animation_house->setDuration(300);
	 animation_house->setStartValue(QRect(-OLED_PIX_X, wid_house->pos().y(), 320, 320));
	 animation_house->setEndValue(QRect(0, wid_house->pos().y(), 320, 320));

	 group_wins->addAnimation(animation_scene);
	 group_wins->addAnimation(animation_house);
	 group_wins->start();
	 //connect(group_wins, SIGNAL(finished()), this, SLOT(group_winsFinished()));
	 QTimer::singleShot(310, this, SLOT(group_winsFinished()));
	
     //unlockTurnPageMutex();
}
void TitleWidget::group_winsFinished()
{
    wid_scene->hide();
	g_BottomSreenTurning = 0;

	//switch_icon->setEnabled(false);
	//switch_button->setEnabled(false);
	for(int i=0; i<room_num; i++)
	{
	    room_button[i]->setEnabled(true);
	}
	
	qDebug()<<"room_button true";
}

void TitleWidget::return_room_func()
{
	QString room_name = astRoom[g_current_room].m_name;
	QFontMetrics fontWidth(switch_button->font());
	QString elideNote = fontWidth.elidedText(room_name, Qt::ElideRight, MAX_SWITCH_BUTTOM_LEN);
	switch_button->setText(elideNote);
	switch_button->setToolTip(room_name);
	//switch_button->setText(astRoom[g_current_room].m_name);
	for(int i=0; i<room_num; i++)
	{
	    room_button[i]->setEnabled(false);
	}
	
	qDebug()<<"return room_button false";
	
    QTimer::singleShot(100, this, SLOT(switch_to_scene()));
	update();
}

void TitleWidget::switch_button1_slot()
{
    wid_house->hide();
	wid_scene->show();
    wid_scene->raise();
	current_Widget = wid_scene;
}

void TitleWidget::translateLanguage(int flag)
{
	scene_button->setText(tr("        Scenes"));
	curtans_button->setText(tr("        Shades"));
	hvac_button->setText(tr("        HVAC  "));
#ifdef TITLE_IMG_ENABLE
	curtain_button_select();
	havc_button_select();
	light_button_select();
#endif
}
#if 1
void TitleWidget::turnPage(QString current_page, int last_page)
{
	bool ok;  
	int current_index = current_page.toInt(&ok, 10);
	//printf("turnPage\r\n");

	for(int i=0; i<button_list.count(); i++)
	{
		ToolButton *tool_button = button_list.at(i);
		if(current_index == i)
		{
			tool_button->setMousePress(true);
		}
		else
		{
			tool_button->setMousePress(false);
		}
	}

	emit turnPage(current_index, last_page);
}

void TitleWidget::scene_button_on()
{
	int start_x_pos = 40;
	qDebug()<<"group_anima count="<<group_anima->animationCount();
	group_anima->clear();
	qDebug()<<"group_anima clear count="<<group_anima->animationCount();

    /*setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 2, hvac_wid);	
	setIconLevel(scene_button, 0, scene_wid);*/
	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/curtain_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);
	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_curtain_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	//animation_bar->setKeyValueAt(1, QRect(start_x_pos, -180, 320, 320));
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_scene_big, "geometry");
		
	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	//animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 180, 320, 320));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(scene_button_off()));
}
void TitleWidget::scene_button_off()
{
    setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 2, hvac_wid);	
	setIconLevel(scene_button, 0, scene_wid);
    //qDebug()<<"current_button="<<current_button;
	//qDebug()<<"s="<<scene_button<<"c="<<curtans_button<<",h="<<hvac_button;

	label_icon_scene_big->hide();
	label_icon_curtain_big->hide();
	label_icon_hvac_big->hide();
}
void TitleWidget::hvac2scene_button_on()
{
	int start_x_pos = 40;

    /*setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 2, hvac_wid);	
	setIconLevel(scene_button, 0, scene_wid);*/
	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_hvac_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(0.5, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/hvac_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_curtain_big, "geometry");

	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(0.5, QRect(start_x_pos, 0, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);

	QPropertyAnimation *animation_bar2 = new QPropertyAnimation(label_icon_scene_big, "geometry");
	animation_bar2->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar2->setKeyValueAt(0, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar2->setKeyValueAt(0.5, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar2->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	animation_bar2->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->clear();
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	group_anima->addAnimation(animation_bar2);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(scene_button_off()));
}

void TitleWidget::light_button_select()
{
	int start_x_pos = 40;

    if (current_button == curtans_button)
    {
	   this->turnPage("0", 1);
    }
	else if (current_button == hvac_button)
	{
	   this->turnPage("0", 2);
	}
	else if (current_button == scene_button)
	{
		this->turnPage("0", 0);
	}
	g_current_title = 0;
#ifdef TITLE_IMG_ENABLE
	QPixmap light_button_pixmap;
	QPixmap curtans_button_pixmap;
	QPixmap hvac_button_pixmap;
	if (g_language_zh)
	{
	    light_button_pixmap.load(QString("menuButton/scene.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac02.png"));
	}
	else
	{
	    light_button_pixmap.load(QString("menuButton/scene_en.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain_en01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac_en02.png"));
	}
	//QPixmap light_button_pixmap(QString("menuButton/scene.png"));
	scene_button->setIcon(light_button_pixmap);
	scene_button->setIconSize(light_button_pixmap.size());
	scene_button->setFixedSize(light_button_pixmap.width()+0, light_button_pixmap.height()+0);

	//QPixmap curtans_button_pixmap(QString("menuButton/curtain01.png"));
	curtans_button->setIcon(curtans_button_pixmap);
	curtans_button->setIconSize(curtans_button_pixmap.size());
	curtans_button->setFixedSize(curtans_button_pixmap.width()+0, curtans_button_pixmap.height()+0);

	//QPixmap hvac_button_pixmap(QString("menuButton/hvac02.png"));
	hvac_button->setIcon(hvac_button_pixmap);
	hvac_button->setIconSize(hvac_button_pixmap.size());
	hvac_button->setFixedSize(hvac_button_pixmap.width()+0, hvac_button_pixmap.height()+0);
	scene_wid->move(0, 50);
	scene_button->setGeometry(QRect(0, 0, OLED_PIX_X, 128));
	curtans_button->setGeometry(QRect(30, 24, 260, 104));
	curtain_wid->move(0, 130);
	hvac_wid->move(59,240);
#else
    if (current_button == curtans_button)
    {
	    //setIconLevel(curtans_button, 1, curtain_wid);
		label_icon_curtain_big->setGeometry(QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
        label_icon_curtain_big->show();
		label_icon_curtain_big->raise();
		label_icon_scene_big->show();
		label_icon_scene_big->setGeometry(QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
		label_icon_scene_big->raise();
       	
        QTimer::singleShot(1, this, SLOT(scene_button_on()));
    }
	else if (current_button == hvac_button)
	{
	    //setIconLevel(curtans_button, 1, curtain_wid);
	    label_icon_hvac_big->setGeometry(QRect(start_x_pos, 0, 90, 90));
        label_icon_hvac_big->show();
		label_icon_hvac_big->raise();
		
		label_icon_curtain_big->setGeometry(QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
        label_icon_curtain_big->show();
		label_icon_curtain_big->raise();
		
		label_icon_scene_big->show();
		label_icon_scene_big->setGeometry(QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
		label_icon_scene_big->raise();
       	
        QTimer::singleShot(1, this, SLOT(hvac2scene_button_on()));
    }	
	else 
	{
		setIconLevel(curtans_button, 1, curtain_wid);
		setIconLevel(hvac_button, 2, hvac_wid);	
		setIconLevel(scene_button, 0, scene_wid);
	}
    //hvac_wid->move(0,240);
	//curtain_wid->move(0,140);
	//scene_wid->move(0, 40);
#endif
#if 0
	hvac_wid->raise();
	curtain_wid->raise();
	scene_wid->raise();
	aqi_wid->raise();
#endif
}
void TitleWidget::scene2curtain_button_on()
{
	int start_x_pos = 40;

    /*setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 2, hvac_wid);	
	setIconLevel(scene_button, 0, scene_wid);*/
	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_scene_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	//animation_bar->setKeyValueAt(1, QRect(start_x_pos, -180, 320, 320));
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/curtain_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_curtain_big, "geometry");
		
	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	//animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 180, 320, 320));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->clear();
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(curtain_button_off()));
}

void TitleWidget::hvac2curtain_button_on()
{
	int start_x_pos = 40;

	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_hvac_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	//animation_bar->setKeyValueAt(1, QRect(start_x_pos, -180, 320, 320));
	animation_bar->setEasingCurve(QEasingCurve::Linear);


	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/curtain_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_curtain_big, "geometry");
		
	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	//animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 180, 320, 320));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->clear();
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(curtain_button_off()));
}
void TitleWidget::curtain_button_off()
{
	setIconLevel(scene_button, 1, scene_wid);
	setIconLevel(hvac_button, 1, hvac_wid);
	setIconLevel(curtans_button, 0, curtain_wid);
    //qDebug()<<"current_button="<<current_button;

	label_icon_scene_big->hide();
	label_icon_curtain_big->hide();
	label_icon_hvac_big->hide();
}
void TitleWidget::curtain_button_select()
{
   // this->turnPage("2");
	g_current_title = 1;
	
	if (current_button == curtans_button)
    {
	   this->turnPage("1", 1);
    }
	else if (current_button == hvac_button)
	{
	   this->turnPage("1", 2);
	}
	else if (current_button == scene_button)
	{
		this->turnPage("1", 0);
	}
#ifdef TITLE_IMG_ENABLE
	QPixmap light_button_pixmap;
	QPixmap curtans_button_pixmap;
	QPixmap hvac_button_pixmap;
	if (g_language_zh)
	{
	    light_button_pixmap.load(QString("menuButton/scene01.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac01.png"));
	}
	else
	{
	    light_button_pixmap.load(QString("menuButton/scene_en01.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain_en.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac_en01.png"));
	}
	//QPixmap light_button_pixmap(QString("menuButton/scene01.png"));
	scene_button->setIcon(light_button_pixmap);
	scene_button->setIconSize(light_button_pixmap.size());
	scene_button->setFixedSize(light_button_pixmap.width()+0, light_button_pixmap.height()+0);

	//QPixmap curtans_button_pixmap(QString("menuButton/curtain.png"));
	curtans_button->setIcon(curtans_button_pixmap);
	curtans_button->setIconSize(curtans_button_pixmap.size());
	curtans_button->setFixedSize(curtans_button_pixmap.width()+0, curtans_button_pixmap.height()+0);

	//QPixmap hvac_button_pixmap(QString("menuButton/hvac01.png"));
	hvac_button->setIcon(hvac_button_pixmap);
	hvac_button->setIconSize(hvac_button_pixmap.size());
	hvac_button->setFixedSize(hvac_button_pixmap.width()+0, hvac_button_pixmap.height()+0);
	
	scene_wid->move(0, 26);
	scene_button->setGeometry(QRect(30, 24, 260, 104));
	
	curtans_button->setGeometry(QRect(0, 0, OLED_PIX_X, 128));
	curtain_wid->move(0, 120);
	hvac_wid->move(30,216);
#else
    if (current_button == scene_button)
    {
	    //setIconLevel(curtans_button, 0, curtain_wid);
	    label_icon_curtain_big->setGeometry(QRect(40, -TITLE_MENU_HEIGHT, 90, 90));
        label_icon_curtain_big->show();
		label_icon_curtain_big->raise();
		label_icon_scene_big->show();
		label_icon_scene_big->setGeometry(QRect(40, 0, 90, 90));
		label_icon_scene_big->raise();
       	
       	//qDebug()<<"call scene2curtain_button_on";
        QTimer::singleShot(1, this, SLOT(scene2curtain_button_on()));
    }
	else if (current_button == hvac_button)
	{
	    //setIconLevel(curtans_button, 0, curtain_wid);
		label_icon_curtain_big->setGeometry(QRect(40, TITLE_MENU_HEIGHT, 90, 90));
        label_icon_curtain_big->show();
		label_icon_curtain_big->raise();
		label_icon_hvac_big->setGeometry(QRect(40, 0, 90, 90));
		label_icon_hvac_big->show();
		label_icon_hvac_big->raise();
       	//qDebug()<<"call hvac2curtain_button_on";
        QTimer::singleShot(1, this, SLOT(hvac2curtain_button_on()));
	}
	else
	{
		setIconLevel(scene_button, 1, scene_wid);
		setIconLevel(hvac_button, 1, hvac_wid);
		setIconLevel(curtans_button, 0, curtain_wid);
	}
    //hvac_wid->move(0,320-104);
	//curtain_wid->move(0,115);
	//scene_wid->move(0, 40);
#endif
#if 0
	hvac_wid->raise();
	scene_wid->raise();
	curtain_wid->raise();
	aqi_wid->raise();
#endif
}
void TitleWidget::hvac_button_on()
{
	int start_x_pos = 40;

    /*setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 2, hvac_wid);	
	setIconLevel(scene_button, 0, scene_wid);*/
	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_curtain_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, TITLE_MENU_HEIGHT+2, 90, 90));
	//animation_bar->setKeyValueAt(1, QRect(start_x_pos, -180, 320, 320));
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/curtain_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_hvac_big, "geometry");
		
	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	//animation_bar1->setKeyValueAt(1, QRect(start_x_pos, 180, 320, 320));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->clear();
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(hvac_button_off()));
}
void TitleWidget::scene2hvac_button_on()
{
	int start_x_pos = 40;

	QPropertyAnimation *animation_bar = new QPropertyAnimation(label_icon_scene_big, "geometry");
			
	animation_bar->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar->setKeyValueAt(0, QRect(start_x_pos, 0, 90, 90));
	animation_bar->setKeyValueAt(0.5, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar->setKeyValueAt(1, QRect(start_x_pos, TITLE_MENU_HEIGHT, 90, 90));
	animation_bar->setEasingCurve(QEasingCurve::Linear);

	QPixmap button_pixmap2;
	button_pixmap2.load(QString("menuButton/scene_icon.png"));
	label_icon_curtain_big->setStyleSheet("background:transparent;");
	label_icon_curtain_big->setIcon(button_pixmap2);

	QPropertyAnimation *animation_bar1 = new QPropertyAnimation(label_icon_curtain_big, "geometry");
	animation_bar1->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar1->setKeyValueAt(0, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar1->setKeyValueAt(0.5, QRect(start_x_pos, 0, 90, 90));
	animation_bar1->setKeyValueAt(1, QRect(start_x_pos, TITLE_MENU_HEIGHT+2, 90, 90));
	animation_bar1->setEasingCurve(QEasingCurve::Linear);

	QPropertyAnimation *animation_bar2 = new QPropertyAnimation(label_icon_hvac_big, "geometry");
	animation_bar2->setDuration(TIME_OF_SCENE_SWITCH);
	animation_bar2->setKeyValueAt(0, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar2->setKeyValueAt(0.5, QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
	animation_bar2->setKeyValueAt(1, QRect(start_x_pos, 0, 90, 90));
	animation_bar2->setEasingCurve(QEasingCurve::Linear);
	
	group_anima->clear();
	group_anima->addAnimation(animation_bar);
	group_anima->addAnimation(animation_bar1);
	group_anima->addAnimation(animation_bar2);
	
	group_anima->start();

	QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(hvac_button_off()));
}

void TitleWidget::hvac_button_off()
{
	setIconLevel(scene_button, 2, scene_wid);
	setIconLevel(curtans_button, 1, curtain_wid);
	setIconLevel(hvac_button, 0, hvac_wid);

    //qDebug()<<"current_button="<<current_button;
	label_icon_scene_big->hide();
	label_icon_curtain_big->hide();
	label_icon_hvac_big->hide();
}

void TitleWidget::havc_button_select()
{
    int start_x_pos = 40;
	
    //this->turnPage("1");
	if (current_button == curtans_button)
    {
	   this->turnPage("2", 1);
    }
	else if (current_button == hvac_button)
	{
	   this->turnPage("2", 2);
	}
	else if (current_button == scene_button)
	{
		this->turnPage("2", 0);
	}
	g_current_title = 2;
		
#ifdef TITLE_IMG_ENABLE
	QPixmap light_button_pixmap;
	QPixmap curtans_button_pixmap;
	QPixmap hvac_button_pixmap;
	if (g_language_zh)
	{
	    light_button_pixmap.load(QString("menuButton/scene02.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac.png"));
	}
	else
	{
	    light_button_pixmap.load(QString("menuButton/scene_en02.png"));
		curtans_button_pixmap.load(QString("menuButton/curtain_en01.png"));
		hvac_button_pixmap.load(QString("menuButton/hvac_en.png"));
	}
	//QPixmap light_button_pixmap(QString("menuButton/scene02.png"));
	scene_button->setIcon(light_button_pixmap);
	scene_button->setIconSize(light_button_pixmap.size());
	scene_button->setFixedSize(light_button_pixmap.width()+0, light_button_pixmap.height()+0);

	//QPixmap curtans_button_pixmap(QString("menuButton/curtain01.png"));
	curtans_button->setIcon(curtans_button_pixmap);
	curtans_button->setIconSize(curtans_button_pixmap.size());
	curtans_button->setFixedSize(curtans_button_pixmap.width()+0, curtans_button_pixmap.height()+0);

	//QPixmap hvac_button_pixmap(QString("menuButton/hvac.png"));
	hvac_button->setIcon(hvac_button_pixmap);
	hvac_button->setIconSize(hvac_button_pixmap.size());
	hvac_button->setFixedSize(hvac_button_pixmap.width()+0, hvac_button_pixmap.height()+0);
	
	scene_wid->move(0, 26);
	scene_button->setGeometry(QRect(59, 12, 202, 80));
	
	curtans_button->setGeometry(QRect(29, 0, 260, 104));
	curtain_wid->move(0, 108);
	
    hvac_button->setGeometry(QRect(0, 0, OLED_PIX_X, 128));
	hvac_wid->move(0,192);
#else
	if (current_button == curtans_button)
	{
	   //setIconLevel(curtans_button, 0, curtain_wid); 
	   label_icon_hvac_big->show();
	   label_icon_hvac_big->setGeometry(QRect(40, -TITLE_MENU_HEIGHT, 90, 90));
	   label_icon_hvac_big->raise();
	   label_icon_curtain_big->setGeometry(QRect(40, 0, 90, 90));
	   label_icon_curtain_big->show();
	   label_icon_curtain_big->raise();
	   
	   QTimer::singleShot(1, this, SLOT(hvac_button_on()));
	}
	else if (current_button == scene_button)
	{
	    //setIconLevel(curtans_button, 1, curtain_wid);
	    label_icon_hvac_big->setGeometry(QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
        label_icon_hvac_big->show();
		label_icon_hvac_big->raise();
		
		label_icon_curtain_big->setGeometry(QRect(start_x_pos, -TITLE_MENU_HEIGHT, 90, 90));
        label_icon_curtain_big->show();
		label_icon_curtain_big->raise();
		
		label_icon_scene_big->show();
		label_icon_scene_big->setGeometry(QRect(start_x_pos, 0, 90, 90));
		label_icon_scene_big->raise();
       	
        QTimer::singleShot(1, this, SLOT(scene2hvac_button_on()));
    }	
	else
	{
	    setIconLevel(scene_button, 2, scene_wid);
		setIconLevel(curtans_button, 1, curtain_wid);
		setIconLevel(hvac_button, 0, hvac_wid);
	}

   	//hvac_wid->move(0, 320-ICON_LEVEL0_SIZE_Y);
	//curtain_wid->move(0, 115);
	//scene_wid->move(0, 40);
#endif
#if 0
	scene_wid->raise();
	curtain_wid->raise();
	hvac_wid->raise();
	aqi_wid->raise();
#endif
}
void TitleWidget::light_button_slot()
{
    light_button_select();
}
void TitleWidget::curtain_button_slot()
{
    curtain_button_select();
}
void TitleWidget::content_button_slot()
{
    havc_button_select();
}

void TitleWidget::mousePressEvent(QMouseEvent *event)
{
	//qDebug()<<"TitleWidget::mousePressEvent,x="<<event->pos().x()<<", y=%d"<<event->pos().y();
	left_mouse_press = 1;
	m_mouseMoveCnt = 0;
	m_mouseSrcPos = event->pos();
	m_mouseDstPos = m_mouseSrcPos;
	//printf("src(%d,%d), dst(%d,%d)\n", 
	//   m_mouseSrcPos.x(), m_mouseSrcPos.y(), 
	//   m_mouseDstPos.x(), m_mouseDstPos.y());
	move_direct = EN_NO_MOVE;
}

static int g_delay_hide = 50;
static float g_opacity_bar = 1.0;
static bool g_timer_stop_flg = true;
void TitleWidget::TimerTimeOut()
{
	g_delay_hide--;
	if(g_delay_hide<=10)
		{
			g_opacity_bar-=0.1;
			if(g_opacity_bar>=0)
			{
				effect_bar->setOpacity(g_opacity_bar);
				effect_bars->setOpacity(g_opacity_bar);
				label_bar->setGraphicsEffect(effect_bar);
				label_bar_s->setGraphicsEffect(effect_bars);
			}


		}
	if(g_delay_hide<=0)
	{
		m_timer->stop();
		g_timer_stop_flg = true;
		label_bar->hide();
		label_bar_s->hide();
	}

}

#define BAR_X 310
static int g_bar_y =0;
static int g_bars_len = 0;
void TitleWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int i;
	int j;
	int move_end;
	QPoint startPos;
	QPoint endPos;
	
	QPropertyAnimation *aul_anim[16];

	m_mouseDstPos = event->pos();
	m_mouseMoveCnt = 0;
//    printf("TitleWidget::mouseReleaseEvent\n");
//	printf("src(%d,%d), dst(%d,%d)\n", 
//	   m_mouseSrcPos.x(), m_mouseSrcPos.y(), 
//	   m_mouseDstPos.x(), m_mouseDstPos.y());
    if (move_direct == EN_NO_MOVE)
    {
        return;
    }
	
    if (current_Widget == wid_scene)
    {
        return;
    }

    g_delay_hide = 50;

    if(g_timer_stop_flg)
    {
		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
		g_timer_stop_flg = false;
		m_timer->start(50);
    }

    m_mouseDstPos = event->pos();

//	qDebug()<<"page_first_room="<<page_first_room<<",move_first_room="<<move_first_room;

	page_first_room = move_first_room;
	room_pos_y = room_pos_y_end;

//	qDebug()<<"room_pos_y_end="<<room_pos_y_end;

	//anim_group = new QParallelAnimationGroup;
	anim_group->clear();

	if (room_pos_y_end < 0)
	{
	    for (i = 0; i < room_num; i++)
	    {
	        if (room_pos_y_end >= 0 - i*110)
	        {
	            break;
	        }
	    }
//		group1->stop();
//		group1->clear();

		QPropertyAnimation *anim = new QPropertyAnimation(wid_house, "geometry");	

		anim->setDuration(100);
		anim->setKeyValueAt(0, QRect(0, room_pos_y_end, 320, house_height-room_pos_y_end));
		if ((move_direct == EN_MOVE_UP) || (0 == i))
		{
			if (i != 0)
			{
			    if (room_pos_y_end >= 0 - i*110 + 50)
			    {
			        i -= 1;
			    }
			}
		    move_end = -i*110;
		}
		else
		{
		    if (room_pos_y_end >= 0 - i*110 + 50)
		   	{
		        i -= 1;
		    }
			
//		    qDebug()<<"move dir="<<move_direct<<", rom_pos_y_end="<<-i*100;
			move_end = -i*110;
		}
		
	    anim->setKeyValueAt(1, QRect(0, move_end, 320, house_height+move_end));
		room_pos_y_end = move_end;
		room_pos_y = room_pos_y_end;
		anim_group->addAnimation(anim);



		for (j = i; j < room_num && j < 4 + i; j++)
		{
//		    qDebug()<<"i="<<i<<",j="<<j<<",x="<<room_button[j]->x();
			aul_anim[j-i] = new QPropertyAnimation(room_button[j], "geometry");	

            startPos.setX(room_button[j]->x());
			startPos.setY(room_button[j]->y());
			if (j - i == 1)
			{
			    endPos.setX(0);
			}
			else
			{
				endPos.setX(15);
			}
			endPos.setY(110+i*110);

//			qDebug()<<"start,x="<<startPos.x()<<",y="<<startPos.y();
//			qDebug()<<"end,x="<<endPos.x()<<",y="<<endPos.y();
			
			aul_anim[j-i]->setDuration(100);
			aul_anim[j-i]->setKeyValueAt(0, QRect(startPos.x(), startPos.y(), 320-2*startPos.x(), 100));
			aul_anim[j-i]->setKeyValueAt(1, QRect(endPos.x(), startPos.y(), 320-2*endPos.x(), 100));
			//qDebug()<<"call setButtonStytle func="<<__func__<<",line="<<__LINE__;

			setButtonStytle(room_button[j], j-i);
			
			anim_group->addAnimation(aul_anim[j-i]);
		}
		
		anim_group->start();
	}
	else if (room_pos_y_end > 0)
	{
	    i = 0;
	    QPropertyAnimation *anim = new QPropertyAnimation(wid_house, "geometry");	


		anim->setDuration(100);
		//anim->setKeyValueAt(0, QRect(0, room_pos_y_end, 320, house_height-room_pos_y_end));
		anim->setKeyValueAt(0, QRect(0, room_pos_y_end, 320, house_height-room_pos_y_end));
		move_end = 0;
				
	    anim->setKeyValueAt(1, QRect(0, move_end, 320, house_height+move_end));
		room_pos_y_end = move_end;
		room_pos_y = room_pos_y_end;
		anim_group->addAnimation(anim);


		for (j = i; j < room_num && j < 4 + i; j++)
		{
//		    qDebug()<<"i="<<i<<",j="<<j<<",x="<<room_button[j]->x();
			aul_anim[j-i] = new QPropertyAnimation(room_button[j], "geometry");	

            startPos.setX(room_button[j]->x());
			startPos.setY(room_button[j]->y());
			//endPos.setX((j-i)*15);
			if (j - i == 1)
			{
			    endPos.setX(0);
			}
			else
			{
				endPos.setX(15);
			}
			endPos.setY(110+i*110);

//			qDebug()<<"start,x="<<startPos.x()<<",y="<<startPos.y();
//			qDebug()<<"end,x="<<endPos.x()<<",y="<<endPos.y();
			
			aul_anim[j-i]->setDuration(100);
			aul_anim[j-i]->setKeyValueAt(0, QRect(startPos.x(), 
				startPos.y(), 320-2*startPos.x(), 100));
			aul_anim[j-i]->setKeyValueAt(1, QRect(endPos.x(),
			    startPos.y(), 320-2*endPos.x(), 100));
			//qDebug()<<"call setButtonStytle func="<<__func__<<",line="<<__LINE__;

			setButtonStytle(room_button[j], j-i);
			
			anim_group->addAnimation(aul_anim[j-i]);
		}
		
		anim_group->start();
	}

if(320/(room_num-2) != g_bars_len)
{
	if(move_direct ==EN_MOVE_UP)
	{
		QPropertyAnimation *anim_bars = new QPropertyAnimation(label_bar_s, "geometry");
		anim_bars->setDuration(100);
		anim_bars->setKeyValueAt(0, QRect(BAR_X, g_bar_y, 10, g_bars_len));
		anim_bars->setKeyValueAt(1, QRect(BAR_X, 320-OLED_PIX_X/(room_num-2), 10, 320/(room_num-2)));
		anim_bars->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else if(move_direct ==EN_MOVE_DOWN)
	{
		QPropertyAnimation *anim_bars = new QPropertyAnimation(label_bar_s, "geometry");
		anim_bars->setDuration(100);
		anim_bars->setKeyValueAt(0, QRect(BAR_X, 0, 10, g_bars_len));
		anim_bars->setKeyValueAt(1, QRect(BAR_X, 0, 10, 320/(room_num-2)));
		anim_bars->start(QAbstractAnimation::DeleteWhenStopped);
	}
}
	move_direct = EN_NO_MOVE;

	g_second_room = abs(move_end)/110+1;
	//printf("move_end=%d g_second_room=%d\r\n",move_end,g_second_room);





	//delete anim_group;
#if 0
	if(m_mouseDstPos.y() <  m_mouseSrcPos.y()) 
	{
	    if (current_button == hvac_button)
	    {
	        return;
	    }

		if (m_mouseSrcPos.y() - m_mouseDstPos.y() > 100)
		{
		    current_button = hvac_button;
		}
		else if (m_mouseSrcPos.y() - m_mouseDstPos.y() > 3)
		{
		    if (current_button == curtans_button)
		    {
		        current_button = hvac_button;
		    }
			else
			{
			    current_button = curtans_button;
			}
		}
		else
		{
		
		}
	}
	else if (m_mouseDstPos.y() >  m_mouseSrcPos.y())
	{
	    if (current_button == scene_button)
	    {
	        return;
	    }

		if (m_mouseDstPos.y() - m_mouseSrcPos.y() > 100)
		{
		    current_button = scene_button;
		}
		else if (m_mouseDstPos.y() - m_mouseSrcPos.y() > 3)
		{
		    if (current_button == curtans_button)
		    {
		        current_button = scene_button;
		    }
			else
			{
			    current_button = curtans_button;
			}
		}
	}
   // sleep(1);
    //title_widget->turnPage("3");

	if (current_button == scene_button)
	{
	    light_button_select();
	}
	else if (current_button == curtans_button)
	{
	    curtain_button_select();
	}
	else if (current_button == hvac_button)
	{
	    havc_button_select();
	}
#endif
}


void TitleWidget::mouseMoveEvent(QMouseEvent *event)
{
    //printf("TitleWidget::mouseMoveEvent\n");
    int offset;
	int max_move;
	int pos_x;
	int offset_bars;
	
	int bars_y =0;

	int bars_fix = 0;

	if (!m_mouseMoveCnt)
	{
	    m_mouseSrcPos = event->pos();
	    m_mouseDstPos = m_mouseSrcPos;
		//printf("set mouse move start\n");
		//printf("src(%d,%d), dst(%d,%d)\n", 
		//	m_mouseSrcPos.x(), m_mouseSrcPos.y(), 
		//	m_mouseDstPos.x(), m_mouseDstPos.y());
	}
	//printf("src(%d,%d), dst(%d,%d)\n", 
	//			m_mouseSrcPos.x(), m_mouseSrcPos.y(), 
	//			m_mouseDstPos.x(), m_mouseDstPos.y());

	if (event->pos().x() < 0)
	{
	    //printf("m_mouseDstPos.x()<0 return\n");
	    return;
	}
		
	m_mouseMoveCnt++;

    if (current_Widget == wid_scene)
    {
        return;
    }
	//qDebug()<<"room_num="<<room_num;
    //up
    m_mouseDstPos = event->pos();
	
#if 1
    if (room_num <= 3)
    {
        return;
    }

    //g_bar_y =(wid_house->pos().y()/110)*320/(room_num-2);
    g_opacity_bar = 1.0;
	effect_bar->setOpacity(g_opacity_bar);
	effect_bars->setOpacity(g_opacity_bar);
	label_bar->setGraphicsEffect(effect_bar);
	label_bar_s->setGraphicsEffect(effect_bars);

	label_bar->show();
	label_bar_s->show();
	label_bar->raise();
	label_bar_s->raise();

	if (m_mouseDstPos.y() < m_mouseSrcPos.y())
	{
	    offset = m_mouseSrcPos.y() - m_mouseDstPos.y();
		
	    //qDebug()<<"offset="<<offset;
		if (offset - room_pos_y > (room_num - 3)*110 + 50)
		{
		    offset = (room_num - 3)*110 + room_pos_y + 50;
		}
		if (wid_house->pos().y()<=0)
		{
			if (abs(wid_house->pos().y()) > 110*(room_num-3))
			{
			 	bars_fix = abs(wid_house->pos().y()) - 110*(room_num-3);
				if (bars_fix<=50)
				{
					bars_y = 320/(room_num-2)/2 *bars_fix /50;
					g_bar_y = 320-(320/(room_num-2)) + bars_y;
					label_bar_s->setGeometry(QRect(BAR_X, g_bar_y, 10, 320/(room_num-2)-bars_y));
					g_bars_len= 320/(room_num-2)-bars_y;
					//label_bar_s->move(BAR_X, g_bar_y+bars_y);
				}
			}
			else
			{
				g_bar_y = (320-OLED_PIX_X/(room_num-2)) * abs(wid_house->pos().y()) / (110*(room_num-3));
				label_bar_s->setGeometry(QRect(BAR_X, g_bar_y, 10, 320/(room_num-2)));
				g_bars_len = 320/(room_num-2);
				// label_bar_s->move(BAR_X, g_bar_y);
			}
		}

		/*offset_bars = offset*(320-OLED_PIX_X/(room_num-2))/(110*(room_num-3));
		bars_y = g_bar_y+ offset_bars;
		if(bars_y>=320-(320/(room_num-2)))
		{
			printf("XX1\r\n");
			bars_fix = bars_y - (320-(320/(room_num-2)));
			if(bars_fix<=320/(room_num-2)/2)
			{
				label_bar_s->setGeometry(QRect(BAR_X, bars_y, 10, 320/(room_num-2)-bars_fix));
				label_bar_s->move(BAR_X, bars_y);
				printf("XX2\r\n");
			}

		}
		else
		{
			printf("XX3\r\n");
			label_bar_s->setGeometry(QRect(BAR_X, bars_y, 10, 320/(room_num-2)));
			label_bar_s->move(BAR_X, bars_y);
		}
*/
#if 0	
	    if (offset < 100)
	    {
	        /*if (page_first_room + 1 <= room_num - 4)
        	{
		        move_first_room = page_first_room + 1;
        	}*/
	    }
		else if (offset < 200)
		{
		    if (page_first_room + 1 <= room_num - 4)
	    	{
				move_first_room = page_first_room + 1;
	    	}
		}
		else if (offset < 300)
		{
		    if (page_first_room + 2 <= room_num - 4)
	    	{
				move_first_room = page_first_room + 2;
	    	}
		}
		else
		{
		    if (page_first_room + 3 <= room_num - 4)
	    	{
				move_first_room = page_first_room + 3;
	    	}
		}
#endif
		move_first_room = (offset - room_pos_y)/110;

        if (move_first_room+3 < room_num + 3)
    	{
		    setButtonStytleOffset(room_button[move_first_room], 0, offset);
		    setButtonStytleOffset(room_button[move_first_room+1], 1, offset);
			if (move_first_room+2 < room_num)
			{
				setButtonStytleOffset(room_button[move_first_room+2], 2, offset);
			}
			
			if (move_first_room+3 < room_num)
			{
				setButtonStytleOffset(room_button[move_first_room+3], 3, offset);
			}
			pos_x = (offset%110)*15/110;
			
			//qDebug()<<"offset="<<offset;
			//qDebug()<<"pos_x="<<pos_x;

			room_button[move_first_room]->setGeometry(QRect(15, 
								room_button[move_first_room]->y(), 320-30, 100));

			room_button[move_first_room+1]->setGeometry(QRect(pos_x, 
								room_button[move_first_room+1]->y(), 320-2*pos_x, 100));
			//room_button[move_first_room]->resize(320+2*pos_x-30, 100);
			//room_button[move_first_room+1]->resize(320-2*pos_x, 100);
			
			if (move_first_room+2 < room_num)
			{
				room_button[move_first_room+2]->setGeometry(QRect(15-pos_x,
								room_button[move_first_room+2]->y(), 320+2*pos_x-30, 100));
				
				//room_button[move_first_room+2]->resize(320+2*pos_x-30, 100);
			}
			
			if (move_first_room+3 < room_num)
			{
				room_button[move_first_room+3]->setGeometry(QRect(15,
								room_button[move_first_room+3]->y(), 320-2*15, 100));
				//room_button[move_first_room+3]->resize(320+2*pos_x-30, 100);
			}

			//room_widget[move_first_room+3]->raise();
			//room_widget[move_first_room+2]->raise();
			//room_widget[move_first_room+1]->raise();
			//room_widget[move_first_room]->raise();
    	}
		else
		{
//		    qDebug()<<"move_first_room error="<<move_first_room;
		}
		
		//room_pos_y = room_pos_y - offset;
		//qDebug()<<"room_pos_y - offset="<<room_pos_y - offset;
		wid_house->move(0, room_pos_y - offset);
		room_pos_y_end = room_pos_y - offset;
		move_direct = EN_MOVE_UP;
	}
	else if (m_mouseDstPos.y() >  m_mouseSrcPos.y())
	{

		if(wid_house->pos().y()<=0)
		{
		 g_bar_y =(320-OLED_PIX_X/(room_num-2)) * abs(wid_house->pos().y()) / (110*(room_num-3));
		 label_bar_s->setGeometry(QRect(BAR_X, g_bar_y, 10, 320/(room_num-2)));
		 g_bars_len= 320/(room_num-2);
		}
		else
		{
			bars_fix = wid_house->pos().y();
			if(bars_fix<=50)
				{
					bars_y = 320/(room_num-2)/2 *bars_fix /50;
					g_bar_y = 0;
					label_bar_s->setGeometry(QRect(BAR_X, g_bar_y, 10, 320/(room_num-2)-bars_y));
					g_bars_len= 320/(room_num-2)-bars_y;
					//label_bar_s->move(BAR_X, g_bar_y+bars_y);

				}
		}
	    offset = m_mouseDstPos.y() - m_mouseSrcPos.y();
	    if (room_pos_y + offset > 0)
		{
			if (room_pos_y + offset > 50)
			{
			    offset = 50 - room_pos_y;
			}

			move_first_room = 0;
			
			room_button[move_first_room]->setGeometry(QRect(15, 
								room_button[move_first_room]->y(), 320-30, 100));
			room_button[move_first_room+1]->setGeometry(QRect(0, 
								room_button[move_first_room+1]->y(), OLED_PIX_X, 100));
			//room_button[move_first_room]->resize(320-30, 100);
			//room_button[move_first_room+1]->resize(320, 100);
			
			if (move_first_room+2 < room_num)
			{
				room_button[move_first_room+2]->setGeometry(QRect(15,
								room_button[move_first_room+2]->y(), 320-30, 100));
				//room_button[move_first_room+2]->resize(320-30, 100);
			}
			
			if (move_first_room+3 < room_num)
			{
				room_button[move_first_room+3]->setGeometry(QRect(15,
								room_button[move_first_room+3]->y(), 320-30, 100));
				//room_button[move_first_room+3]->resize( 320-90, 100);
			}

			wid_house->move(0, room_pos_y + offset);
		    room_pos_y_end = room_pos_y + offset;
		    move_direct = EN_MOVE_DOWN;
		
			return;
		}
//        qDebug()<<"offset="<<offset;
//		move_first_room = (room_pos_y + offset)/100;

		move_first_room = (0 - offset - room_pos_y)/110;
        if (move_first_room < 0)
        {
            move_first_room = 0;
        }

        if (move_first_room+1 < room_num)
    	{
		    setButtonStytleOffset(room_button[move_first_room], 0, offset);
		    setButtonStytleOffset(room_button[move_first_room+1], 1, offset);
			if (move_first_room+2 < room_num)
			{
				setButtonStytleOffset(room_button[move_first_room+2], 2, offset);
			}
			if (move_first_room+3 < room_num)
			{
				setButtonStytleOffset(room_button[move_first_room+3], 3, offset);
			}

			//room_widget[move_first_room+3]->raise();
			//room_widget[move_first_room+2]->raise();
			//room_widget[move_first_room+1]->raise();
			//room_widget[move_first_room]->raise();
			pos_x = 15 - (((offset-1)%110)*15/110);
//			qDebug()<<"pos_x="<<pos_x;

			room_button[move_first_room]->setGeometry(QRect(15-pos_x, 
						room_button[move_first_room]->y(), 320+2*pos_x-30, 100));
			room_button[move_first_room+1]->setGeometry(QRect(pos_x, 
						room_button[move_first_room+1]->y(), 320-2*pos_x, 100));

			//room_button[move_first_room]->resize(320+2*pos_x-30, 100);
			//room_button[move_first_room+1]->resize(320-2*pos_x, 100);
			if (move_first_room+2 < room_num)
			{
				room_button[move_first_room+2]->setGeometry(QRect(15-pos_x,
						room_button[move_first_room+2]->y(), 320+2*pos_x-30, 100));
				//room_button[move_first_room+2]->resize(320+2*pos_x-30, 100);
			}
			
			if (move_first_room+3 < room_num)
			{
				room_button[move_first_room+3]->setGeometry(QRect(15,
						room_button[move_first_room+3]->y(), 320-30, 100));
				//room_button[move_first_room+2]->resize(320+2*pos_x-90, 100);
			}
    	}
		else
		{
//		    qDebug()<<"down move_first_room error="<<move_first_room;
		}
		//room_pos_y = room_pos_y + offset;
//        qDebug()<<"widhouse->move="<<room_pos_y+offset<<"offset="<<offset<<",room_pos_y="<<room_pos_y;
		wid_house->move(0, room_pos_y + offset);
		room_pos_y_end = room_pos_y + offset;
		move_direct = EN_MOVE_DOWN;
	}
#endif


}


#endif
