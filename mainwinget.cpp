#include "mainwinget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>

#include "wthread.h"
#include "set_widget.h"
#include "status_xml.h"


WThread *g_thread = NULL;
int g_pairid_flag = 0;
char g_pairid[MAX_PAIR_ID_LEN] = {"111-11-111"};
SetWidget *set_widget;
SetAboutWidget *set_about_widget;
int g_clear_flag = 1;
extern int g_time_out_cnt;
extern int g_time_set_cnt;
extern int g_time_set_enable;

extern int g_upgrade_flag;
extern int g_oled_state;
extern int g_current_dim;

pthread_mutex_t mutex_turnPage;
int g_upSreenTurning;
int g_BottomSreenTurning;

CStatusXml * g_statusXmlHandle = NULL;
#define MAIN_WIDGET_DBG		1



void MainWidget::translateLanguage()
{
    
}
EN_SYS_MODE MainWidget::get_state()
{
    return en_state;
}
enno_result MainWidget::load_boot_mode_status(uint32 *p_status)
{
	int nbytes = 1;
	char tmp[32];
	char *path = BOOT_MODE_FILE;
	QFile file(BOOT_MODE_FILE);

	memset(tmp, 0, 32);
	
	if (!file.open(QFile::ReadOnly))
	{
	    qDebug()<<"fileopen fail";
	    return ENNO_UNKOWN;
	}
	nbytes = file.read(tmp,10);

	if (!nbytes)
	{
        *p_status = 0;
		file.close();
		return ENNO_UNKOWN;
	}

	*p_status = atoi(tmp);

    qDebug()<<"boot_mode="<<*p_status;
	file.close();
	return ENNO_OK;
}

#if 0
void MainWidget::paintEvent(QPaintEvent *e)
{
     int nIndex = stack_title->currentIndex();

     printf("nIndex=%d\n", nIndex);
	 if (nIndex == 4)
	 {
	     QPixmap image("/home/user/ui/qwid/hkt.png");
		 QPainter painter(this);
		 painter.drawPixmap(0,0,80,70,image);
		 
	 }
}
#endif

void MainWidget::msTimerTaskSet()
{
	static int ms_cnt=0;


	if(mstimer_flag==1)
	{
		ms_cnt++;
			
		if(ms_cnt>50)
		{
			ms_cnt = 0;
			mstimer_flag = 0;
			qDebug()<<"---msTimerTaskSet--:";

			pthread_mutex_lock(&mutex_turnPage);

            if(set_about_widget)
			{
                if(set_about_widget->b_child_lock_on)
				{
#if MAIN_WIDGET_DBG
                    printf("[UI] fun=%s,line=%d. child lock on.\n",__FUNCTION__,__LINE__);
#endif		
					statked_widget->setCurrentIndex(statked_widget->count()-1);
                    stack_title->setCurrentIndex(stack_title->count()-1);
				}
				else
				{
                    
					statked_widget->setCurrentIndex(prev_up_index);
					stack_title->setCurrentIndex(prev_down_index);
				}
			}

			
			ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
			
			unlockTurnPageMutex(this);
			
		
			en_state = EN_SYS_NORM;
		}

	}

		
		

}

MainWidget::MainWidget(QWidget *parent)
	: QDialog(parent)
{

	//system("echo 0 > /sys/class/gpio/gpio48/value");
	//system("echo 1 > /sys/class/gpio/gpio48/value");
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setMaximumSize(OLED_SIZE_X, OLED_SIZE_Y);
	this->setMinimumSize(OLED_SIZE_X, OLED_SIZE_Y);

	this->setStyleSheet("background:black");
	
	g_room_num = 0;
	g_upSreenTurning = 0;
    g_upSreenTurning = 0;

	pthread_mutex_init(&mutex_turnPage, NULL);

    g_statusXmlHandle = new CStatusXml(xmlFileName);

/*    g_statusXmlHandle->updateOutDoor(CHAR_P2_OUTDOOR_TEMP,25);
    g_statusXmlHandle->updateOutDoor(CHAR_P2_OUTDOOR_AQI,80);
    g_statusXmlHandle->updateOutDoor(CHAR_P2_OUTDOOR_WEATHER,3);
    g_statusXmlHandle->updateOpenDev(0x33);

    g_statusXmlHandle->updateOpenDev(21);

    g_statusXmlHandle->setRoomIndex(1);
    g_statusXmlHandle->updateDevInfo(DEV_TYPE_HAVC,123,0x1500,2);
    g_statusXmlHandle->updateDevInfo(DEV_TYPE_HAVC,123,0x1500,1);
    g_statusXmlHandle->updateDevInfo(DEV_TYPE_HAVC,123,0x1502,3);
    g_statusXmlHandle->updateScene(34,0);
    g_statusXmlHandle->updateScene(34,1);
    g_statusXmlHandle->updateLight(1);*/

	create_ui();

    refresh_ui();

	//sys_info = new MySysInfo();

	timer_dim_out = new QTimer();
	connect(timer_dim_out, SIGNAL(timeout()), this, SLOT(onTimerDimOut()));
	system("killall qdisp");
	usleep(20000);
	system("sync");
	syslog(LOG_INFO|LOG_LOCAL1,"ui init ok\n");
	//qDebug()<<"MainWidget init end";
	
#ifdef MOTOR_ENABLE
	Util::UI_motor_init();
#endif

    timer_ms = new QTimer();
    timer_ms->setInterval(10);//10ms
    timer_ms->start();
	connect(timer_ms, SIGNAL(timeout()), this, SLOT(msTimerTaskSet()));

////yu
//statked_widget->setCurrentIndex(0);
//stack_title->setCurrentIndex(0);


    syslog(LOG_INFO|LOG_LOCAL1,"MainWidget start end\n");

}

void MainWidget::create_ui()
{
	QString data("test");
	int tmp_room_num;

	en_state = EN_SYS_NORM;
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	//printf("MainWidget window=%p, s=%d\n", this, en_state);
	pair_index = -1;

	model = new TreeModel(data);
	
	g_thread = new WThread(this);
//	thread1->start();
    g_thread->setWindowInstance(this);


	statked_widget = new QStackedWidget();
	stack_title = new QStackedWidget();
	title_widget = new TitleWidget(model);

	tmp_room_num = (g_room_num!=0) ? g_room_num : 1;
	for (int i = 0; i < tmp_room_num; i++)
	{
        content_widget[i] = new ContentWidget(i, model);
		havc_widget[i] = new HavcWidget(i, model);
		curtain_widget[i] = new CurtainWidget(i, model);
		roomlight_widget[i] = new RoomLightWidget(i, model);
		room_light_dim_widget[i] = new RoomLightDimWidget(i, model);
	}
	
	set_widget = new SetWidget();
	set_about_widget = new SetAboutWidget();
	pair_widget = new QqrWidget();
	standby_content = new StandbyUpWidget();
	standby_title = new StandbyWidget();
	sleep_widget = new SleepWidget();
	sleep_title = new SleepWidget();
	update_widget = new UpdateWidget();
	logo_widget = new UpdateWidget_Pic();
	program_widget = new ProgramWidget();
	light_widget = new LightWidget();
	start_widget = new StartWidget();
	room_back_widget = new RoomBackWidget();
	room_select_widget = new RoomSelectWidget();
	flash_widget = new FlashStartWidget();
	flash_text_widget = new FlashTextWidget();

    start_title_widget = new StartTitleWidget();
	house_room_widget = new HouseRoomWidget(model);
	group_upScreen = new QParallelAnimationGroup;
	tempLabelAni = NULL;
	widToSet = NULL;
	tempLabelLeft = NULL;

	lock_word_widget = new StartWordWidget();
	lightText_widget = new RoomLightTextWidget();
    //printf("havc_widget=0x%x\n", havc_widget[g_current_room]);
    //
    widget_childLock = new childLock_widget();
    widget_chldLkBlack = new childLockBlack_widget();
	
    set_widget->setWidSlot(this);
	set_about_widget->setWidSlot(this);
	connect(set_about_widget, SIGNAL(changeLanguage(int)), this, SLOT(changeLanguage(int)));
	connect(set_about_widget, SIGNAL(retMainwid()), this, SLOT(reviewWidget()));
	connect(set_about_widget, SIGNAL(showSetupCode()), this, SLOT(showPairWidget()));
	connect(pair_widget, SIGNAL(clearPairCode()), this, SLOT(clearPairWidget()));
	connect(room_back_widget, SIGNAL(return_room_singal()), this, SLOT(returnRoomWidget()));
	connect(house_room_widget, SIGNAL(changeRoom(int,int)), this, SLOT(selectRoom(int,int)));
	connect(title_widget, SIGNAL(switch_to_house(int, QString)), this, SLOT(showSwitchRoom(int, QString)));

#if 0
    pair_label = new QLabel("123-45-678", pair_widget);
	#if 1
	QFont font;

	QFontDatabase::addApplicationFont("SF-Mono-Bold.otf");
    font.setFamily("SF-Mono-Bold");
	pair_label->setFont(font);
	#endif
	
	pair_label->setStyleSheet("font-size:35px;background-color: white;border-width:2px;border-style:solid;border-color:black;color:black;font-weight:bold;text-align:left;");
	//pair_label->setFixedSize(200, 70);
#endif

#if 0
	QFont font;

	QFontDatabase::addApplicationFont("SF-Mono-Bold.otf");
    font.setFamily("SF-Mono-Bold");
	pair_label->setFont(font);
	#endif

	//tool1_button->setStyleSheet("font-size:35px;background-color: white;border-width:2px;border-style:solid;border-color:black;color:black;font-weight:bold;text-align:left;");
    //tool1_button->setText("123-45-678");
#if 0
	QPixmap pixmap("/home/user/ui/qwid/hkt_setup.png");
	//QPixmap pixmap("/home/jason/tmp/qt/hello/hkt_setup.png");
	hkt_label = new QLabel();
	hkt_label->resize(pixmap.size());
	hkt_label->setPixmap(pixmap);
	//hkt_label->setGeometry(QRect(0,0,22,22));
	//pair_label->setGeometry(QRect(32,32,288,22));
#endif

    #if 0
	QHBoxLayout *h_pair_layout = new QHBoxLayout();
	h_pair_layout->addWidget(tool1_button);
	h_pair_layout->addWidget(pair_label);

	pair_widget->setLayout(h_pair_layout);
	#endif

#if 1	
	QPalette palette;
	palette.setBrush(QPalette::Window, QBrush(Qt::white));

	statked_widget->setPalette(palette);
    statked_widget->setAutoFillBackground(true);
	for (int i = 0; i < tmp_room_num; i++)
	{
		statked_widget->addWidget(content_widget[i]);
		statked_widget->addWidget(havc_widget[i]);
		statked_widget->addWidget(curtain_widget[i]);
		statked_widget->addWidget(roomlight_widget[i]);
		statked_widget->addWidget(room_light_dim_widget[i]);
	}



    


	statked_widget->addWidget(standby_content);
    statked_widget->addWidget(sleep_widget);
	statked_widget->addWidget(set_about_widget);
	//statked_widget->addWidget(update_widget);
	statked_widget->addWidget(logo_widget);
	statked_widget->addWidget(light_widget);
	statked_widget->addWidget(start_widget);
	statked_widget->addWidget(room_select_widget);
	statked_widget->addWidget(room_back_widget);
	statked_widget->addWidget(flash_widget);
	statked_widget->addWidget(pair_widget);
    statked_widget->addWidget(widget_childLock);
    
	
	//statked_widget->addWidget(roomlight_widget);

	stack_title->addWidget(title_widget);  //
	stack_title->addWidget(standby_title); //
	stack_title->addWidget(sleep_title);   //
	stack_title->addWidget(set_widget);    //
	stack_title->addWidget(lock_word_widget);   //
	stack_title->addWidget(program_widget);//
	stack_title->addWidget(start_title_widget);
	stack_title->addWidget(house_room_widget);
	stack_title->addWidget(flash_text_widget);
	stack_title->addWidget(lightText_widget);
	stack_title->addWidget(update_widget);
	stack_title->addWidget(widget_chldLkBlack);
	//
	set_about_widget->setQrwid(pair_widget);
#if 0
	right_center_function_widget = new QWidget();
	function_label = new QLabel();
	more_button = new QPushButton();

	QFont function_font = function_label->font();
	function_font.setBold(true);
	function_label->setFont(function_font);
	function_label->setObjectName("infoLabel");
	more_button->setFixedSize(50, 25);
	more_button->setObjectName("blueButton");
	more_button->setCursor(Qt::PointingHandCursor);

	QHBoxLayout *h_layout = new QHBoxLayout();
	h_layout->addWidget(function_label);
	h_layout->addStretch();
	h_layout->addWidget(more_button);
	h_layout->setSpacing(8);
	h_layout->setContentsMargins(10, 5, 0, 0);

	QStringList string_list;
	string_list<<"contentWidget/recovery"<<"contentWidget/mobile"<<"contentWidget/game_box"<<"contentWidget/desktop"
		<<"contentWidget/net_repair"<<"contentWidget/auto_run"<<"contentWidget/net_speed"<<"contentWidget/net_pretext"
		<<"contentWidget/first_add";

	QGridLayout *grid_layout = new QGridLayout();
	for(int i=0; i<string_list.size(); i++)
	{
/*		QToolButton *tool_button = new QToolButton();
		tool_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		QPixmap button_pixmap(string_list.at(i));
		tool_button->setIcon(button_pixmap);
		tool_button->setIconSize(button_pixmap.size());
		tool_button->setFixedSize(button_pixmap.width()+50, button_pixmap.height()+35);
		tool_button->setObjectName("functionButton");
*/
	    ToolButton *tool_button = new ToolButton(string_list.at(i));
			//button_list.append(tool_button);

		button_list.append(tool_button);
		grid_layout->addWidget(tool_button, i/3, i%3);
	}
	
	grid_layout->setSpacing(10);
	grid_layout->setContentsMargins(5, 0, 5, 5);
#endif
#if 1
	v_layout = new QVBoxLayout();
	v_layout->addStretch();
	v_layout->addWidget(statked_widget);
	//v_layout->addStretch();
	v_layout->addWidget(stack_title);
	//v_layout->setStretchFactor(statked_widget, 1);
	//v_layout->setStretchFactor(title_widget, 2);
	 //v_layout->addStretch();
	v_layout->setSpacing(0);
	v_layout->setContentsMargins(0, 0, 0, 0);
	
#endif
#if 1
	h_layout = new QHBoxLayout();
	h_layout->addStretch();
	h_layout->addWidget(statked_widget);
	//v_layout->addStretch();
	h_layout->addWidget(stack_title);
	
	//h_layout->addWidget(pair_widget);
	//v_layout->setStretchFactor(statked_widget, 1);
	//v_layout->setStretchFactor(title_widget, 2);
	//v_layout->addStretch();
	h_layout->setSpacing(0);
	h_layout->setContentsMargins(0, 0, 0, 0);
	
#endif

#if 0
	right_center_function_widget->setLayout(v_layout);

	
	QHBoxLayout *main_layout = new QHBoxLayout();
	main_layout->addWidget(right_center_function_widget);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(0, 0, 0, 0);
#endif
	connect(title_widget, SIGNAL(turnPage(int,int)), this, SLOT(turnPage(int,int)));
	for (int i = 0; i < tmp_room_num; i++)
	{
		connect(roomlight_widget[i], SIGNAL(closeRoomlight()), this, SLOT(light_on_showoff()));
		connect(room_light_dim_widget[i], SIGNAL(closeRoomlightDim()), this, SLOT(light_dim_showoff()));
	}
	
	if (access(START_FLAG_FILE, F_OK) == 0)
	{
	    if (access(PAIR_OK_FILE, F_OK) == 0)
		{
		    //pair ok
		    ENNO_LOG_ERR("paired_OK\n");
		    if (access(HOUSE_SET_FILE, F_OK) == 0)
	    	{
			    title_widget->turnPage("0", 0);
				stack_title->setCurrentIndex(0);
				
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		    }
			else
			{
			    QTimer::singleShot(10, this, SLOT(show_font()));
			    statked_widget->setCurrentWidget(room_select_widget);
			    stack_title->setCurrentWidget(house_room_widget);
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
		}
		else if (access(BOOT_MODE_FILE, F_OK) == 0)
		{
		    load_boot_mode_status(&boot_mode);
			ENNO_LOG_ERR("load boot_mode file ok,boot_mode=%d", boot_mode);
			if (boot_mode)
			{
			    title_widget->turnPage("0", 0);
				stack_title->setCurrentWidget(title_widget);
				
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
			else
			{
			    statked_widget->setCurrentWidget(start_widget);
			    stack_title->setCurrentWidget(start_title_widget);
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
		}
		else
		{
		    qDebug()<<"not find boot_mode file";
			//title_widget->turnPage("0");
			statked_widget->setCurrentWidget(start_widget);
			stack_title->setCurrentWidget(start_title_widget);
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		}
	}
	else
	{
	    statked_widget->setCurrentWidget(flash_widget);
		stack_title->setCurrentWidget(flash_text_widget);
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		connect(flash_widget, SIGNAL(flash_singal()), this, SLOT(flash_end()));
		//stack_title->setCurrentWidget(start_title_widget);
	}

	prev_up_index = statked_widget->currentIndex();
	prev_down_index = stack_title->currentIndex();

	if(needEnterLockStatus())
	{
	    en_state = EN_SYS_LOCK;
		pair_widget->doQrgenInit();
		lock_word_widget->setTextType(lock_text);
		lock_word_widget->setText();
		statked_widget->setCurrentWidget(pair_widget);		
		stack_title->setCurrentWidget(lock_word_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
	
#endif

#ifdef X64
	this->setLayout(v_layout);
#else  
	this->setLayout(h_layout);
#endif
	//this->translateLanguage();
	
	//connect(more_button, SIGNAL(clicked()), this, SLOT(changeLanguage()));
	//this->showNormal();
	//this->raise();
	//this->activateWindow();
	//update();
    //this->setCentralWidget(right_center_function_widget);
}

/*MainWidget::~MainWidget()
{
    delete_ui();
}
*/	

void MainWidget::showSwitchRoom(int index, QString room_name)
{
     qDebug()<<"showSwitchRoom="<<index<<", name="<<room_name;
    QWidget * tempWidget = statked_widget->currentWidget();



        if(en_state == EN_SYS_SET)
        {
            return;
        }
        //wid_house->hide();

        //current_Widget = wid_house;
        //wid_house->show();
        //wid_house->raise();
        g_upSreenTurning = 1;

        room_back_widget->showTheRoom(index, room_name);

        //statked_widget->setCurrentWidget(room_back_widget);
        room_back_widget->resize(OLED_PIX_X*2, OLED_PIX_Y);

        if(tempLabelAni == NULL)
        {
             tempLabelAni = new QLabel(room_back_widget);
        }
        else
        {
             tempLabelAni->setParent(room_back_widget);
        }
        QLabel * sceneToMoveOut = tempLabelAni;

        sceneToMoveOut->setGeometry(320, 0, 320, 320);
        sceneToMoveOut->setPixmap(tempWidget->grab());
        //sceneToMoveOut->setAutoFillBackground(true);
        sceneToMoveOut->show();
        sceneToMoveOut->raise();

       // QFile file("temp.png");
       // file.open(QIODevice::WriteOnly);
       // tempWidget->grab().save(&file,"PNG");

        statked_widget->setCurrentWidget(room_back_widget);
        room_back_widget->setGeometry(-OLED_PIX_X,0,640,320);

        QPropertyAnimation * animation_In = new QPropertyAnimation(room_back_widget, "geometry");
        group_upScreen->clear();

        animation_In->setDuration(300);
        animation_In->setStartValue(QRect(-OLED_PIX_X, room_back_widget->pos().y(), 640, 320));
        animation_In->setEndValue(QRect(0, room_back_widget->pos().y(), 320, 320));

        group_upScreen->addAnimation(animation_In);
        group_upScreen->start();
         connect(group_upScreen, SIGNAL(finished()), this, SLOT(upAniFinish()));
         //QTimer::singleShot(310, this, SLOT(upAniFinish()));
}

void MainWidget::upAniFinish(void)
{
   //int i = 0;
   int aniCount = group_upScreen->animationCount();

   if(aniCount)
   {
       //for(i=0;i<aniCount;i++)
       //{
      //     free(group_upScreen->animationAt(i));
       //}

       group_upScreen->clear();
   }

   if(widToSet)
   {
      QWidget * tempWidget = statked_widget->currentWidget();
      statked_widget->setCurrentWidget(widToSet);
	  widToSet = NULL;
	  if(tempLabelLeft)
	  {
	      tempLabelLeft->hide();
		  tempLabelLeft->lower();
	  }
	  tempWidget->setGeometry(0,0,320,320);
   } 
   g_upSreenTurning = 0; 

}

void MainWidget::delete_ui()
{
	return;
	delete model;
    delete g_thread;
	delete statked_widget;
	delete stack_title;
    delete title_widget;
	for (int i = 0; i < g_room_num; i++)
	{
        delete content_widget[i];
		delete havc_widget[i];
		delete curtain_widget[i];
	}
	delete pair_widget;
	delete standby_content;
	delete standby_title;
	delete sleep_widget;
	delete sleep_title;
	delete v_layout;
	delete h_layout;
	group_upScreen->clear();
	delete group_upScreen;
	if(tempLabelAni)
	{
	   delete tempLabelAni;
	   tempLabelAni = NULL;
	}

	if(tempLabelLeft)
	{
	   delete tempLabelLeft;	
	}
}

void MainWidget::showWidget()
{
	this->showNormal();
	this->raise();
	this->activateWindow();
	this->show();
}
void MainWidget::changeLanguage(int flag)
{
    g_language_zh = flag;
    if (flag == 1)
    {
		system("echo 1 > /home/user/ui/qwid/language.conf");
        translator->load(QString("qwid_jt.qm"));
    }
	else if (flag == 2)
	{
		system("echo 2 > /home/user/ui/qwid/language.conf");
        translator->load(QString("qwid_ft.qm"));
    }	
	else
	{
		system("echo 0 > /home/user/ui/qwid/language.conf");
        translator->load(QString("qwid_zh_EN.qm"));
	}

	usleep(20000);
	system("sync");

	this->translateLanguage();
	set_widget->translateLanguage();
	set_about_widget->translateLanguage();
	title_widget->translateLanguage(flag);
	standby_content->translateLanguage();
	standby_title->translateLanguage();
	lock_word_widget->translateLanguage();
    widget_childLock->translateLanguage();
	for (int i = 0; i < g_room_num; i++)
	{
		content_widget[i]->translateLanguage();
		havc_widget[i]->translateLanguage();
		curtain_widget[i]->translateLanguage();
	}
}

void MainWidget::setTranslator(QTranslator* translator)
{
	this->translator = translator;
}

void MainWidget::changeSkin(QString skin_name)
{
	//Util::writeInit(QString("./user.ini"), QString("skin"), skin_name);
	//this->skin_name = skin_name;
	//this->setStyleSheet("background:url(skin/17_big)");
    //update();
}

void MainWidget::onTimerOut()
{
	QObject *object = QObject::sender();
	QTimer *timer1 = qobject_cast<QTimer *>(object);
    int time_remain;
	static int pair_flag = 0;
    //this->show();
	//qDebug()<<"onTimerOut func, g_pairid_flag="<<g_pairid_flag;
    if (!g_pairid_flag || pair_flag)
    {
        if (EN_SYS_DIM == en_state || EN_SYS_LOCK == en_state)
        {
            return;
        }

		if (statked_widget->currentWidget() != standby_content 
			&& statked_widget->currentWidget() != sleep_widget
			&& statked_widget->currentWidget() != set_about_widget
			&& statked_widget->currentWidget() != pair_widget
			&& statked_widget->currentWidget() != logo_widget)
		{
			prev_up_index = statked_widget->currentIndex();
		}

		if (stack_title->currentWidget() != standby_title 
				&& stack_title->currentWidget() != sleep_title
				&& stack_title->currentWidget() != set_widget
				&& stack_title->currentWidget() != lock_word_widget
				&& stack_title->currentWidget() != update_widget)
		{
			prev_down_index = stack_title->currentIndex();
		}
		pthread_mutex_lock(&mutex_turnPage);
	    title_widget->turnPage("3", 3);
		stack_title->setCurrentIndex(1);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		set_about_widget->quitSet();
		timer1->stop();
		//timer1->start();
		//qDebug()<<"onTimerOut stop1";
		qDebug()<<"enter standby 11111";
		en_state = EN_SYS_STANDBY;
		
		//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
		timer_sleep->start();

        pair_flag = 0;
		/*time_remain = timer_sleep->remainingTime();
		if (time_remain <= 0)
		{
		    qDebug()<<"time_remain="<<time_remain;
		    timer_sleep->start();
		}*/
    }
	else
	{
	    //g_pairid_flag = 0;
	    if(EN_SYS_LOCK == en_state)
	    {
	        return;
	    }
	    if (statked_widget->currentWidget() != pair_widget
			&&stack_title->currentWidget()!= update_widget)
	    {
	        pthread_mutex_lock(&mutex_turnPage);
	        title_widget->turnPage("3", 3);
			stack_title->setCurrentIndex(1);
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			unlockTurnPageMutex(this);
			set_about_widget->quitSet();
			timer1->stop();
			//timer1->start();
			//qDebug()<<"onTimerOut stop2";
			qDebug()<<"enter standby 2222";
			en_state = EN_SYS_STANDBY;
			
			//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
			timer_sleep->start();
	    }
		else
		{
		    pair_flag = 1;
		    timer1->start();
			timer_sleep->start();
		}
	}
}

void MainWidget::onTimerSet()
{
	QObject *object = QObject::sender();
	QTimer *timer1 = qobject_cast<QTimer *>(object);
	static int g_sys_time = 0;

	g_sys_time++;

	 if (g_sys_time%10 == 0)
    {
        //sys_info->GetResource();
    }

    if (g_dali_set_flag && stack_title->currentWidget() == set_widget)
	{
	    return;
	}

    if (g_time_out_cnt <= TIMER_SLEEP_MS/1000)
    {
	    g_time_out_cnt++;
    }

	if (g_time_set_enable)
	{
		g_time_set_cnt++;
	}
	else
	{
	    g_time_set_cnt = 0;
	}

	if (g_upgrade_flag == 1)
	{
		return;
	}
	else if (g_upgrade_flag == 0)
	{
	    if (statked_widget->currentWidget() == logo_widget)
	    {	        
			title_widget->turnPage("0", 0);
			g_upgrade_flag = 2;
	    }
	}


	//qDebug()<<"g_time_out_cnt="<<g_time_out_cnt;

    if (g_time_out_cnt >= TIMER_SLEEP_MS/1000)
    {
		//mask by iovine
		enno_enter_sleep_mode();  //enter sleep

		//g_oled_state = EN_SYS_SLEEP;
    }
    else if (g_time_out_cnt >= TIMER_STANDBY_MS/1000)  //standby
    {

    	//mask by iovine
        enno_enter_standby_mode();
    }
	
    if (g_time_set_cnt >= TIMER_SET_MS/1000)  //enter set
    {
        
		 if(set_about_widget)
		{
            if(set_about_widget->b_child_lock_on \
                && (statked_widget->currentWidget() == widget_childLock))
			{	
#if MAIN_WIDGET_DBG			
				printf("[UI] fun=%s,line=%d,in child lock.\n",__FUNCTION__,__LINE__);	
#endif	
				statked_widget->setCurrentIndex(0);		//prev_up_index
				stack_title->setCurrentIndex(0);		//prev_down_index
				g_time_set_cnt = 0;
				g_time_set_enable = 0;
            }
			else
			{
#if MAIN_WIDGET_DBG		
				printf("[UI] fun=%s,line=%d,not in child lock.\n",__FUNCTION__,__LINE__);	
#endif
				enno_enter_set_mode();
			}
	 	}
		
    }

	g_oled_state = en_state;
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
}


void MainWidget::onTimerOff()
{
    //this->show();
    //sleep
    QObject *object = QObject::sender();
    QTimer *timer2 = qobject_cast<QTimer *>(object);
	//qDebug()<<"onTimerOff func";

    if (EN_SYS_DIM == en_state)
    {
        return;
    }
	pthread_mutex_lock(&mutex_turnPage);
    title_widget->turnPage("4", 4);
	stack_title->setCurrentIndex(2);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	unlockTurnPageMutex(this);
    //qDebug()<<"fpga reset";
	//system("echo 0 > /sys/class/gpio/gpio48/value");
	timer2->stop();
	//qDebug()<<"onTimerOff stop";
	qDebug()<<"enter sleep 22222";
	en_state = EN_SYS_SLEEP;
	
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

}

void MainWidget::onTimerDimOut()
{
    QObject *object = QObject::sender();
    QTimer *timer2 = qobject_cast<QTimer *>(object);

    //qDebug()<<"onTimerDimOut in";
	if (en_state != EN_SYS_DIM)
    {
        timer2->stop();
        return;
    }

    TimerRestart();
	timer2->stop();
	pthread_mutex_lock(&mutex_turnPage);
	title_widget->turnPage("0", 0);
	stack_title->setCurrentIndex(0);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	unlockTurnPageMutex(this);
	en_state = EN_SYS_NORM;
	
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
}
void MainWidget::setTimer(QTimer *timer_standby_in, QTimer *timer_sleep_in)
{
    timer_standby = timer_standby_in;
	timer_sleep = timer_sleep_in;
}

void MainWidget::TimerRestart()
{
    timer_standby->start(TIMER_STANDBY_MS);
    timer_sleep->start(TIMER_SLEEP_MS);
	//qDebug()<<"TimerRestart";
}

void MainWidget::showPairWidget()
{
	//qDebug()<<"showPairWidget pair_widget->doQrgen()";
	pair_widget->doQrgen();
	lock_word_widget->setTextType(pair_text);
	lock_word_widget->setText();
	statked_widget->setCurrentWidget(pair_widget);
}
void MainWidget::clearPairWidget()
{
	pairclrWidget();
}

void MainWidget::reviewWidget()
{
	
	ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );	
	
    if(needEnterLockStatus())
    {
        pthread_mutex_lock(&mutex_turnPage);
		syslog(LOG_INFO|LOG_LOCAL1, "needEnterLockStatus=1\n");
        en_state = EN_SYS_LOCK;
	    pair_widget->doQrgenInit();
		lock_word_widget->setTextType(lock_text);
		lock_word_widget->setText();
	    statked_widget->setCurrentWidget(pair_widget);		
	    stack_title->setCurrentWidget(lock_word_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		return;
    }

    pthread_mutex_lock(&mutex_turnPage);
    en_state = EN_SYS_NORM;
	ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );

		
	qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
    stack_title->setCurrentIndex(prev_down_index);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	statked_widget->setCurrentIndex(prev_up_index);
	unlockTurnPageMutex(this);

	ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
}

void MainWidget::selectRoom(int index,int flg)
{
    qDebug()<<"-------------call selectRoom-------------="<<index;

    title_widget->selectRoom(index,flg);
	if(roomlight_widget[index]->isAllLightOff())
    {
        title_widget->control_lihght_point(index,0);//g_current_room
    }
	else
	{
	    title_widget->control_lihght_point(index,1);//g_current_room
	}
	usleep(100000);
	pthread_mutex_lock(&mutex_turnPage);
    title_widget->turnPage("0", 0);
    stack_title->setCurrentIndex(0);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	unlockTurnPageMutex(this);
}

void MainWidget::returnRoomWidget()
{
    if(    statked_widget->currentWidget() == set_about_widget 
		|| stack_title->currentWidget() == set_widget )
    {
        return;
    }
    title_widget->return_room_func();
}

#if 0
void MainWidget::qrcodeGen()
{
    int retval = 0;

    string bin = "0000000000001111010011000101101000100101000110";  
	char *qr_start= "X-HM://";
	char *config_bin = "1010";
	char categoryandsetupcode_bin[46];
	
	char pin_buf[27];
	char category_buf[8];
	char* pin_buf_t=NULL;
	char *category_buf_t=NULL;
	char data_buf[]="0123456789";
	char char_buf[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	char categoryandsetupcode[9];
	int data_len=0;
	int i=0,j=0;
	int tmp=0,char_id=0;
	int pin_len=0,category_len=0;
	char setup_ID[4];
	char setup_payload[13];
	char setup_code_tmp[8];
	char QR_setup_payload[20];	

	for(j=0;j<7;j++)
		categoryandsetupcode_bin[j]='0';
		
		
	long category=CATEGORY;
	category_buf_t=decToBin(category,&category_len);
	
	qDebug()<<category_buf_t<<endl;
	
	printf("category_len=%d\r\n",category_len);
	if(category_len<8)
	{
		i=8-category_len;
		for(j=0;j<i;j++)
			category_buf[j]='0';	
	}
	else
	{
		j=0;
	}

	sprintf(&category_buf[j],"%s",category_buf_t);
	
	qDebug()<<category_buf<<endl;
	
	sprintf(&categoryandsetupcode_bin[7],"%s",category_buf);
	sprintf(&categoryandsetupcode_bin[15],"%s",config_bin);
	
	long pin=0;//51808582;
	make_rand_setup_code(&pin);
	
	pin_buf_t=decToBin(pin,&pin_len);
	
	qDebug()<<pin_buf_t<<endl;
	
	printf("pin_len=%d\r\n",pin_len);
	if(pin_len<27)
	{
		i=27-pin_len;
		for(j=0;j<i;j++)
		pin_buf[j]='0';
	}
	else
	{
		j=0;
	}

	sprintf(&pin_buf[j],"%s",pin_buf_t);
	
	qDebug()<<pin_buf<<endl;

	sprintf(&categoryandsetupcode_bin[19],"%s",pin_buf);
	
	qDebug()<<categoryandsetupcode_bin<<endl;

	qDebug()<<bin<<endl;
	
    string b36 = baseConvert(categoryandsetupcode_bin, 2, 36);  
	
	data_len=b36.size();
	if(data_len<9)
	{
		qDebug()  << b36.size() << endl;
		i=9-data_len;
		for(j=0;j<i;j++)
		categoryandsetupcode[j]='0';
	}
	else
	{
		j=0;
	}
	char b36_buf[9];
	strcpy(b36_buf,b36.c_str());
	
	sprintf(&categoryandsetupcode[j],"%s",b36_buf);
	
    qDebug() << categoryandsetupcode << endl;  
	
	// string b2= baseConvert(categoryandsetupcode, 36, 2);  
	// qDebug() << b2 << endl; 
	
	int rand_buf[3];
	srand(time(NULL)); //\D3玫\B1前系统时\BC\E4\C9\E8\D6\C3\D6\D6\D7\D3
	for(i=0;i<3;i++)
	rand_buf[i]=(rand()%10);    //\D3\C3rand\BA\AF\CA\FD\C9\FA\B3\C90-9\B5\C4\CB\E6\BB\FA\CA\FD\A3\AC\B2\A2\B8\B3值\B8\F8\CA\FD\D7\E9rand_buf[i]

	printf("rand_numb=%d",rand_buf[0]);
	sprintf(setup_ID,"%c",data_buf[rand_buf[0]]);
	
	tmp=rand_buf[0]+rand_buf[1]+rand_buf[2];
	if(tmp<26)
	{
		sprintf(&setup_ID[1],"%c",char_buf[tmp]);
		printf("char1id=%d\r\n",tmp);
	}
	else
	{
		sprintf(&setup_ID[1],"%c",char_buf[tmp]);
		printf("char1id=%d\r\n",tmp%10);
	}
	sprintf(&setup_ID[2],"%c",char_buf[rand_buf[1]]);
	printf("char2id=%d\r\n",rand_buf[1]);
	sprintf(&setup_ID[3],"%c",char_buf[rand_buf[2]]);
	printf("char3id=%d\r\n",rand_buf[2]);
	
	qDebug() << setup_ID << endl;  

	sprintf(setup_payload,"%s",categoryandsetupcode);
	sprintf(&setup_payload[9],"%s",setup_ID);
	
	
	qDebug() << setup_payload << endl; 
	
	strcpy(QR_setup_payload,qr_start);
	strcat(QR_setup_payload,setup_payload);
	
	qDebug() << QR_setup_payload << endl; 
}
#endif






void MainWidget::pairdispWidget(char *pair_id)
{

	g_pairid_flag = 1;
	char p_id[MAX_PAIR_ID_LEN] = {0};
	//pair_label->setStyleSheet("font-size:35px;background-color: white;border-width:2px;border-style:solid;border-color:black;color:black;font-weight:bold;text-align:left;");
		//pair_label->setFixedSize(200, 70);


		memcpy(p_id, pair_id, 10);	
		p_id[10] = 0;
		memcpy(g_pairid, p_id, 11);

		
		qDebug()<<"--------g_pairid="<<g_pairid;


		if(!strcmp(g_pairid, "1234567890"))	//flutter app pair
		{
			memcpy(p_id, &pair_id[10], 4);
			p_id[4] = 0;
			qDebug()<<"--------p_id="<<p_id;
			pair_widget->GetSetupId(p_id);
			
			g_clear_flag = 0;
			pair_widget->doflutterQrgen();

			lock_word_widget->setTextType(pair_text);
			lock_word_widget->setText();
			statked_widget->setCurrentWidget(pair_widget);

		}
		else
		{
			pair_widget->GetSetupCode(p_id);
			
			memcpy(p_id, &pair_id[10], 4);
			p_id[4] = 0;
			pair_widget->GetSetupId(p_id);
			
			
			if (statked_widget->currentWidget() != pair_widget)
			{
				qDebug()<<"current12 widget is Not pair_widget,"<<" enter pair qr"<<endl;
				 pair_index = statked_widget->currentIndex();
				 
				qDebug()<<"pair_index="<<pair_index;
				g_clear_flag = 0;
			
				pair_widget->doQrgen();
			
				
				lock_word_widget->setTextType(pair_text);
				lock_word_widget->setText();
				statked_widget->setCurrentWidget(pair_widget);
			}
			else
			{
				qDebug()<<"curr widget is pair_widget,"<<" display pair qr"<<endl;
				if(needEnterLockStatus())
				{
					g_clear_flag = 0;
					pair_widget->doQrgen();
					lock_word_widget->setTextType(pair_text);
					lock_word_widget->setText();
					statked_widget->setCurrentWidget(pair_widget);
				}
			}


		}


		

	
}

void MainWidget::show_room()
{
	stack_title->setCurrentWidget(house_room_widget);
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	house_room_widget->show_move();

}

void MainWidget::show_font()
{
	qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

	room_select_widget->start_timer();
	statked_widget->setCurrentWidget(room_select_widget);
	QTimer::singleShot(800, this, SLOT(show_room()));
}

void MainWidget::create_pair_file()
{
	int fd = 0x00;
	//system("rm /home/user/conf/pannos_hkt.conf");
    //usleep(1000);
    QFile file(PAIR_OK_FILE);
	
	if (file.exists())
	{
	    ENNO_LOG_ERR("file exist");
	}
	else
	{
	    file.open(QIODevice::ReadWrite | QIODevice::Text);
		file.write(g_pairid);
		pair_index = -1;	
		start_widget->end_show();
		QTimer::singleShot(1000, this, SLOT(show_font()));
	}

	file.close();
}

void MainWidget::pairclrWidget()
{
    //printf("pairclrWidget, pair_id\n");
	qDebug()<<"g_clear_flag="<<g_clear_flag;

	
	create_pair_file();
	

	if(needEnterLockStatus())
	{
	    en_state = EN_SYS_LOCK;
		
		pthread_mutex_lock(&mutex_turnPage);
		pair_widget->doQrgenInit();
		lock_word_widget->setTextType(lock_text);
		lock_word_widget->setText();
		statked_widget->setCurrentWidget(pair_widget);		
		stack_title->setCurrentWidget(lock_word_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		pair_index = -1;
		
		unlockTurnPageMutex(this);
	    return;
	}
	
	
	pthread_mutex_lock(&mutex_turnPage);
	if (pair_widget == statked_widget->currentWidget())
	{
	    printf("exit pair qr, pair_index=%d,en_state is %d\n", pair_index,en_state);
	    if (pair_index != -1)
	    {
	        statked_widget->setCurrentIndex(pair_index);
			pair_index = -1;
	    }
	}
	
	unlockTurnPageMutex(this);
}
void MainWidget::enno_enter_sleep_mode()
{


	

	if (pair_widget == statked_widget->currentWidget())//add by iovine
	{
		qDebug()<<"enno_enter_sleep_mode, return back for pair_widget";
		return;
	}
	



	if ( en_state == EN_SYS_DIM 
		|| en_state == EN_SYS_SLEEP
		|| en_state == EN_SYS_ROOM_LIGHT_DIM
		|| en_state == EN_SYS_ROOM_LIGHT_ONOFF)
	{
	    if (en_state != EN_SYS_SLEEP)
	   	{
			qDebug()<<"enno_enter_sleep_mode, return back en_state from="<<en_state;
	    }
		return;
	}

    pthread_mutex_lock(&mutex_turnPage);
    title_widget->turnPage("4", 4); //up screen	    
	stack_title->setCurrentIndex(2);//down screen
	
	qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	unlockTurnPageMutex(this);
	qDebug()<<"enno_enter_sleep_mode, en_state from="<<en_state;
	en_state = EN_SYS_SLEEP;
	qDebug()<<"enno_enter_sleep_mode, en_state to="<<en_state;
}

void MainWidget::enno_enter_set_mode()
{
	if (light_dim_press == 2 
		|| shade_dim_press == 2 
		|| EN_SYS_SET == en_state
		|| EN_SYS_ROOM_LIGHT_DIM == en_state
		|| EN_SYS_ROOM_LIGHT_ONOFF == en_state)
	{
	   return;
	} //this->show();
	//title_widget->turnPage("3");
	g_time_set_cnt = 0;
	g_time_set_enable = 0;
	
    pthread_mutex_lock(&mutex_turnPage);
	if (statked_widget->currentWidget() != pair_widget 
        && statked_widget->currentWidget() != logo_widget)
	{
	    prev_up_index = statked_widget->currentIndex();
	}

	if (stack_title->currentWidget() != standby_title 
		&& stack_title->currentWidget() != sleep_title
		&& stack_title->currentWidget() != set_widget
		&& stack_title->currentWidget() != lock_word_widget
		&& stack_title->currentWidget() != update_widget)
	{
	    prev_down_index = stack_title->currentIndex();
		
		ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
	}   
	statked_widget->setCurrentWidget(set_about_widget);
	//title_widget->turnPage("5");  //set_about_widget
	stack_title->setCurrentIndex(3);
	
	ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
	unlockTurnPageMutex(this);
	//timer1->stop();
	qDebug()<<"onTimerSet enter set mode";
	en_state = EN_SYS_SET;

}

void MainWidget::enno_enter_standby_mode()
{
    int time_remain;
	static int pair_flag = 0;
	if(EN_SYS_LOCK == en_state || en_state == EN_SYS_SLEEP || en_state == EN_SYS_ROOM_LIGHT_DIM)
    {
        return;
    }


    if (pair_widget == statked_widget->currentWidget())//add by iovine
	{
		qDebug()<<"enno_enter_standby_mode, return back for pair_widget";
		return;
	}

    if (!g_pairid_flag || pair_flag)
    {
        if (EN_SYS_DIM == en_state)
        {
            return;
        }
		
		if (stack_title->currentWidget() == standby_title
			|| statked_widget->currentWidget() == roomlight_widget[g_current_room])
		{
		    return;
		}
		
		pthread_mutex_lock(&mutex_turnPage);
		if (statked_widget->currentWidget() != standby_content 
			&& statked_widget->currentWidget() != sleep_widget
			&& statked_widget->currentWidget() != set_about_widget
			&& statked_widget->currentWidget() != pair_widget
			&& statked_widget->currentWidget() != logo_widget)
		{
			prev_up_index = statked_widget->currentIndex();
		}

		if (stack_title->currentWidget() != standby_title 
				&& stack_title->currentWidget() != sleep_title
				&& stack_title->currentWidget() != set_widget
				&& stack_title->currentWidget() != lock_word_widget
				&& stack_title->currentWidget() != update_widget)
		{
			prev_down_index = stack_title->currentIndex();
		}
	    title_widget->turnPage("3", 3);
		stack_title->setCurrentIndex(1);
		
		ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
		unlockTurnPageMutex(this);
		set_about_widget->quitSet();
		//timer1->stop();
		//timer1->start();
		//qDebug()<<"onTimerOut stop1";
		qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<", en_state from= "<<en_state<<endl;
		en_state = EN_SYS_STANDBY;		
		qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<", en_state to= "<<en_state<<endl;
        pair_flag = 0;
    }
	else
	{
	    if ((statked_widget->currentWidget() != pair_widget)
			&&(standby_title != stack_title->currentWidget()) 
			&& (g_upgrade_flag == 0)/*stack_title->currentWidget()!= update_widget*/)
	    {
	        pthread_mutex_lock(&mutex_turnPage);
	        title_widget->turnPage("3", 3);
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			stack_title->setCurrentIndex(1);
			unlockTurnPageMutex(this);
			set_about_widget->quitSet();
			qDebug()<<"enter standby 4444";
			en_state = EN_SYS_STANDBY;
	    }
		else if(statked_widget->currentWidget() == pair_widget)
		{
		    pair_flag = 1;
		}
	}
}

void MainWidget::enterProgramWidget()
{
    //printf("pairclrWidget, pair_id\n");
	//qDebug()<<"g_clear_flag="<<g_clear_flag;
	//if (stack_title->currentWidget())
	{
	    stack_title->setCurrentWidget(program_widget);
	}
}

void MainWidget::exitProgramWidget()
{
    //printf("pairclrWidget, pair_id\n");
	//qDebug()<<"g_clear_flag="<<g_clear_flag;
	
    if (stack_title->currentWidget() == program_widget)
	{
	    stack_title->setCurrentIndex(0);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
}
void MainWidget::enter_room_light_dim()
{
	pthread_mutex_lock(&mutex_turnPage);
    en_state = EN_SYS_ROOM_LIGHT_DIM;
    
	if (statked_widget->currentWidget() != room_light_dim_widget[g_current_room]
        && statked_widget->currentWidget() != sleep_widget
        && stack_title->currentWidget() != lightText_widget
        && stack_title->currentWidget() != sleep_title)
    {
        cur_in = statked_widget->currentIndex();
        cur_bottom = stack_title->currentIndex();
		
		syslog(LOG_INFO|LOG_LOCAL1, "room light dim backup cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
    }

	statked_widget->setCurrentWidget(room_light_dim_widget[g_current_room]);	
	unlockTurnPageMutex(this);
}
void MainWidget::exit_room_light_dim()
{
    //5s 退出调光界面
    syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);

	if ( g_statusXmlHandle
			&&!g_statusXmlHandle->getReadOut())
	{
		g_statusXmlHandle->setRoomIndex(g_current_room);
		g_statusXmlHandle->updateLightDim(g_current_dim);
	}

	pthread_mutex_lock(&mutex_turnPage);

    en_state = EN_SYS_NORM;

    if (statked_widget->currentWidget() == room_light_dim_widget[g_current_room])
	{
		syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim up back to cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	    statked_widget->setCurrentIndex(cur_in);
	    stack_title->setCurrentIndex(cur_bottom);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
	else
	{
	    syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim error up back to cur_in=%d,, cur_bottom=%d, because up now is =%d, down_now=%d\n", 
			cur_in, cur_bottom, statked_widget->currentIndex(), stack_title->currentIndex());
	}
	
	unlockTurnPageMutex(this);
}

void MainWidget::enter_room_light_onoff()
{
	syslog(LOG_INFO|LOG_LOCAL1,"enter_room_light_onoff in\n");
	pthread_mutex_lock(&mutex_turnPage);

	en_state = EN_SYS_ROOM_LIGHT_ONOFF;
	
	if (statked_widget->currentWidget() != roomlight_widget[g_current_room]
		&& statked_widget->currentWidget() != sleep_widget
		&& stack_title->currentWidget() != lightText_widget
		&& stack_title->currentWidget() != sleep_title)
	{
		cur_in = statked_widget->currentIndex();
		cur_bottom = stack_title->currentIndex();
		
		syslog(LOG_INFO|LOG_LOCAL1, "backup cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	}
	else //if ( statked_widget->currentWidget() != )
	{
		//sleep(1);
		// cur_in = statked_widget->indexOf(content_widget[0]);
		//cur_bottom = stack_title->indexOf(title_widget);
	}

	if(stack_title->currentWidget() != lightText_widget)
	{
	//		cur_bottom = stack_title->currentIndex();
	}
	
	unlockTurnPageMutex(this);
}

void MainWidget::exit_room_light_onoff()
{
    //5s 退出调光界面
/*    syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);

    en_state = EN_SYS_NORM;

    if (statked_widget->currentWidget() == roomlight_widget[g_current_room])
	{
		syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim up back to cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	    statked_widget->setCurrentIndex(cur_in);
	    stack_title->setCurrentIndex(cur_bottom);
	}
	else
	{
	    syslog(LOG_INFO|LOG_LOCAL1, "exit_room_light_dim error up back to cur_in=%d,, cur_bottom=%d, because up now is =%d, down_now=%d\n", 
			cur_in, cur_bottom, statked_widget->currentIndex(), stack_title->currentIndex());
	}
*/
    pthread_mutex_lock(&mutex_turnPage);

    en_state = EN_SYS_NORM;
	syslog(LOG_INFO|LOG_LOCAL1, "light_on_showoff cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	
    if (statked_widget->currentWidget() == roomlight_widget[g_current_room] || stack_title->currentWidget() == lightText_widget)
	{
		syslog(LOG_INFO|LOG_LOCAL1, "up back to cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	    statked_widget->setCurrentIndex(cur_in);
	    stack_title->setCurrentIndex(cur_bottom);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
	else
	{
	    syslog(LOG_INFO|LOG_LOCAL1, "error up back to cur_in=%d,, cur_bottom=%d, because up now is =%d, down_now=%d\n", 
			cur_in, cur_bottom, statked_widget->currentIndex(), stack_title->currentIndex());
	}
	
	unlockTurnPageMutex(this);
}
void MainWidget::exit_room_dim_onoff()
{
    //5s 退出调光界面
    pthread_mutex_lock(&mutex_turnPage);

    en_state = EN_SYS_NORM;
	syslog(LOG_INFO|LOG_LOCAL1, "light_dim_showoff cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	
    if (statked_widget->currentWidget() == room_light_dim_widget[g_current_room])
	{
		syslog(LOG_INFO|LOG_LOCAL1, "up back to cur_in=%d, cur_bottom=%d\n", cur_in, cur_bottom);
	    statked_widget->setCurrentIndex(cur_in);
	    stack_title->setCurrentIndex(cur_bottom);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
	else
	{
	    syslog(LOG_INFO|LOG_LOCAL1, "error up back to cur_in=%d,, cur_bottom=%d, because up now is =%d, down_now=%d\n", 
			cur_in, cur_bottom, statked_widget->currentIndex(), stack_title->currentIndex());
	}
	
	unlockTurnPageMutex(this);
}

void MainWidget::set_dim_value(char *value, uint32 len_buf)
{
   (void)content_widget[g_current_room]->set_dim_value(value, len_buf);
}
void MainWidget::get_scene_status_byname(char *value, uint32 len_buf)
{
    for (int i = 0; i < g_room_num; i++)
	{
	   /* if(   g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(i);
		}*/
	    (void)content_widget[i]->get_scene_status_byname(value, len_buf);
	}
}

void MainWidget::get_scene_status(char *value, uint32 len_buf)
{
	for (int i = 0; i < g_room_num; i++)
	{
	    if(   g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(i);
		}
	    (void)content_widget[i]->get_scene_status(value, len_buf);
	}

	if(   g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(g_current_room);
		}
}

void MainWidget::set_dev_update(char *value, uint32 len_buf)
{
	for (int i = 0; i < g_room_num; i++)
	{
	    if(   g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(i);
		}
		
	    (void)havc_widget[i]->set_dev_update(value, len_buf);
	    (void)curtain_widget[i]->set_dev_update(value, len_buf);
	}

	if(   g_statusXmlHandle
		   &&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(g_current_room);
		}
}

void MainWidget::get_sensor_value(char *value, uint32 len_buf)
{
	standby_content->get_sensor_value(value,len_buf);
}
void MainWidget::get_outdoor_value(char *value, uint32 len_buf)
{
	standby_title->get_outdoor_value(value,len_buf);
	title_widget->set_aqi();
}

void MainWidget::get_dim_value(char *value, uint32 len_buf)
{
	if (NULL == value)
	{
	    qDebug()<<"qterr, get_dim_value, value null";
	    return;
	}
	//qDebug()<<"emit timer_dim_out start";
	return;
	//emit time_restart();
	en_state = EN_SYS_DIM;
	
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	timer_dim_out->setInterval(TIMER_DIM_OUT_MS);
   // timer_off->setInterval(350000);   //350s
   // timer_off->setInterval(200000);   //200s
    timer_dim_out->start();

	qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

	statked_widget->setCurrentWidget(light_widget);
	light_widget->get_dim_value(value, len_buf);
}

void MainWidget::get_dev_info(char *value, uint32 len_buf)
{
	set_about_widget->get_dev_info(value, len_buf);
}

void MainWidget::get_ssid_info(char *value, uint32 len_buf)
{
	set_about_widget->get_ssid_info(value, len_buf);
}

void MainWidget::get_dali_info(char *value, uint32 len_buf)
{
	set_about_widget->get_dali_info(value, len_buf);
}

void MainWidget::set_cycle_light_dim(char *value, uint32 len_buf)
{
    if (en_state == EN_SYS_ROOM_LIGHT_ONOFF)
    {
        qDebug()<<"set_light_status in room dim state, return";
		syslog(LOG_INFO|LOG_LOCAL1,"set_cycle_light_dim room_dim state, return\n");
        return;
    }

	if (en_state != EN_SYS_ROOM_LIGHT_DIM)
	{
	    enter_room_light_dim();
	}

    room_light_dim_widget[g_current_room]->light_dim(value, len_buf);
	
}

void MainWidget::get_net_state(char *value, uint32 len_buf)
{
	enno_result ret;
	char *buf = value;
	uint32 len = len_buf;
	uint32 len_use;

	set_about_widget->get_net_state(value, len_buf);

    ret = enno_decode_uint(buf, (uint32*)&en_net_state, &len_use, len);
    if (ret)
	{
	    return;
    }
	if (EN_NET_PAIRED == en_net_state)
	{
	    //qDebug()<<"get_net_state="<<en_state<<"call pairclrWidget";
		pairclrWidget();
	}
}

void MainWidget::get_dev_open_notify(char *value, uint32 len_buf)
{
	standby_title->get_dev_open_notify(value, len_buf);
}

void MainWidget::light_on_showoff()
{
    qDebug()<<"light_on_showoff";
    exit_room_light_onoff();
}

void MainWidget::light_dim_showoff()
{
    qDebug()<<"exit_room_dim_onoff,g_current_dim="<<g_current_dim;

    roomlight_widget[g_current_room]->set_light_value(g_current_dim);

	if (g_current_dim > 0)
	{
	    title_widget->control_lihght_point(g_current_room,1);
	    roomlight_widget[g_current_room]->set_light_status(1);
    }
	else
	{
	    title_widget->control_lihght_point(g_current_room,0);
		roomlight_widget[g_current_room]->set_light_status(0);
	}
	
    exit_room_dim_onoff();
}

void MainWidget::flash_end()
{
	if (access(PAIR_OK_FILE, F_OK) == 0)
	{
	    //pair ok
	    pthread_mutex_lock(&mutex_turnPage);
		if(needEnterLockStatus())
	    {
		    en_state = EN_SYS_LOCK;
			pair_widget->doQrgenInit();
			lock_word_widget->setTextType(lock_text);
		    lock_word_widget->setText();
		    statked_widget->setCurrentWidget(pair_widget);		
		    stack_title->setCurrentWidget(lock_word_widget);
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	    }
		else
		{
		    title_widget->turnPage("0", 0);
	        stack_title->setCurrentIndex(0);	
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		}
		unlockTurnPageMutex(this);
	    
	}
	else
	{
	    //title_widget->turnPage("0");
		qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

		if(needEnterLockStatus())
	    {
		    en_state = EN_SYS_LOCK;
			pair_widget->doQrgenInit();
			lock_word_widget->setTextType(lock_text);
		    lock_word_widget->setText();
		    statked_widget->setCurrentWidget(pair_widget);		
		    stack_title->setCurrentWidget(lock_word_widget);
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			
	    }
		else if (access(BOOT_MODE_FILE, F_OK) == 0)
		{
		    load_boot_mode_status(&boot_mode);
			qDebug()<<"load boot_mode file ok,boot_mode"<<boot_mode;
			
	        pthread_mutex_lock(&mutex_turnPage);
			if (boot_mode)
			{
			    title_widget->turnPage("0", 0);
				stack_title->setCurrentWidget(title_widget);
				
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
			else
			{
			    statked_widget->setCurrentWidget(start_widget);
			    stack_title->setCurrentWidget(start_title_widget);
				
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
			unlockTurnPageMutex(this);
		}
		else
		{
		    statked_widget->setCurrentWidget(start_widget);
	        stack_title->setCurrentWidget(start_title_widget);	
			
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		}
	    
	}
}

void MainWidget::set_light_button_release(char *value, uint32 len_buf)
{
	lightText_widget->light_release();
	light_on_showoff();
}

//to do
void MainWidget::set_light_button_press(char *value, uint32 len_buf)
{
	uint32 light_on;
	uint32 len;
	
	(void)enno_decode_uint(value, &light_on, &len, len_buf);
	//qDebug()<<"getlight value"<<light_on;
	if (g_room_num == 0)
	{
		qDebug()<<"there is no room set";
	}

	if (en_state == EN_SYS_ROOM_LIGHT_DIM)
	{
		qDebug()<<"set_light_status in room dim state, return";
		syslog(LOG_INFO|LOG_LOCAL1,"set_light_status room_dim state, return\n");
		return;
	} 

	enter_room_light_onoff();
   
	if(roomlight_widget[g_current_room]->getMetalButtonStatus())
	{
		light_on = 1;
	}
	else
	{
		light_on = 0;
	}
	
	/*if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
		g_statusXmlHandle->setRoomIndex(g_current_room);
		g_statusXmlHandle->updateLight(light_on);
	}*/
	
	syslog(LOG_INFO|LOG_LOCAL1, "light_on press=%d\n", light_on);
	
	if (light_on)
	{	
	
#ifdef MOTOR_ENABLE
		Util::UI_motor_shake(3);
#endif
		qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
		roomlight_widget[g_current_room]->lighton_press();
		//room_light_dim_widget[g_current_room]->set_light_dim_value(200);
		statked_widget->setCurrentWidget(roomlight_widget[g_current_room]);
		
		//syslog(LOG_INFO|LOG_LOCAL1, "lighton up statked_widget->setCurrentWidget=roomlight_widget[g_current_room]\n");
		//title_widget->control_lihght_point(g_current_room,light_on);
		
		if (!roomlight_widget[g_current_room]->lightNum())
		{
			lightText_widget->nolight();
		}
		else
		{
			lightText_widget->lighton_press();
		}
		
		syslog(LOG_INFO|LOG_LOCAL1, "lighton down stack_title->setCurrentWidget=lightText_widget\n");
		
		pthread_mutex_lock(&mutex_turnPage);
		stack_title->setCurrentWidget(lightText_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		//QTimer::singleShot(3000, this, SLOT(light_on_showoff()));
		//qDebug()<<"display lighton";
		//usleep(1000000);
		//statked_widget->setCurrentIndex(cur);
	}
	else
	{
	
#ifdef MOTOR_ENABLE
		Util::UI_motor_shake(3);
#endif

		qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
		roomlight_widget[g_current_room]->lightoff_press();
		//room_light_dim_widget[g_current_room]->set_light_dim_value(0);
		
		pthread_mutex_lock(&mutex_turnPage);
		statked_widget->setCurrentWidget(roomlight_widget[g_current_room]);
		//title_widget->control_lihght_point(g_current_room,light_on);
		lightText_widget->lightoff_press();
		syslog(LOG_INFO|LOG_LOCAL1, "lightoff stack_title->setCurrentWidget=lightText_widget\n");
		stack_title->setCurrentWidget(lightText_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		//QTimer::singleShot(3000, this, SLOT(light_on_showoff()));
		//qDebug()<<"display lightoff";
		//usleep(1000000);
		//statked_widget->setCurrentIndex(cur);
	}
}

void MainWidget::set_light_status(char *value, uint32 len_buf)
{
    uint32 light_on;
	uint32 len;


	 if(set_about_widget)
	{
        if(set_about_widget->b_child_lock_on \
            && ((statked_widget->currentWidget() == widget_childLock)\
                ||(statked_widget->currentWidget() == standby_content)))
		{
#if MAIN_WIDGET_DBG		
			printf("[UI] fun=%s,line=%d,in child lock rtn!\n",__FUNCTION__,__LINE__);
#endif
			return;
		}
 	}
	
	(void)enno_decode_uint(value, &light_on, &len, len_buf);
	//qDebug()<<"getlight value"<<light_on;
	if (g_room_num == 0)
	{
	    qDebug()<<"there is no room set";
	}

    /*if (en_state == EN_SYS_ROOM_LIGHT_DIM)
    {
        qDebug()<<"set_light_status in room dim state, return";
		syslog(LOG_INFO|LOG_LOCAL1,"set_light_status room_dim state, return\n");
		return;
    }*/

	enter_room_light_onoff();
   
	if(roomlight_widget[g_current_room]->getMetalButtonStatus())
	{
	    light_on = 1;
	}
	else
	{
	    light_on = 0;
	}
	
	if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->setRoomIndex(g_current_room);
	    g_statusXmlHandle->updateLight(light_on);
	}
	
	syslog(LOG_INFO|LOG_LOCAL1, "light_on=%d\n", light_on);
	
	if (light_on)
	{	
	
#ifdef MOTOR_ENABLE
	    Util::UI_motor_shake(3);
#endif
	    qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	    roomlight_widget[g_current_room]->lighton();
		room_light_dim_widget[g_current_room]->set_light_dim_value(200);
	    statked_widget->setCurrentWidget(roomlight_widget[g_current_room]);
		
		syslog(LOG_INFO|LOG_LOCAL1, "lighton up statked_widget->setCurrentWidget=roomlight_widget[g_current_room]\n");
	    title_widget->control_lihght_point(g_current_room,light_on);
		
		if (!roomlight_widget[g_current_room]->lightNum())
		{
		    lightText_widget->nolight();
		}
		else
		{
			lightText_widget->lighton();
		}
		
		syslog(LOG_INFO|LOG_LOCAL1, "lighton down stack_title->setCurrentWidget=lightText_widget\n");
		
		pthread_mutex_lock(&mutex_turnPage);
		stack_title->setCurrentWidget(lightText_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		//QTimer::singleShot(3000, this, SLOT(light_on_showoff()));
		//qDebug()<<"display lighton";
		//usleep(1000000);
		//statked_widget->setCurrentIndex(cur);
	}
	else
	{
	
#ifdef MOTOR_ENABLE
	    Util::UI_motor_shake(3);
#endif

	    qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	    roomlight_widget[g_current_room]->lightoff();
		room_light_dim_widget[g_current_room]->set_light_dim_value(0);
		
		pthread_mutex_lock(&mutex_turnPage);
	    statked_widget->setCurrentWidget(roomlight_widget[g_current_room]);
	    title_widget->control_lihght_point(g_current_room,light_on);
		lightText_widget->lightoff();
		syslog(LOG_INFO|LOG_LOCAL1, "lightoff stack_title->setCurrentWidget=lightText_widget\n");
	    stack_title->setCurrentWidget(lightText_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		//QTimer::singleShot(3000, this, SLOT(light_on_showoff()));
		//qDebug()<<"display lightoff";
		//usleep(1000000);
		//statked_widget->setCurrentIndex(cur);
	}
}

void MainWidget::get_wake_up_msg(char *value, uint32 len_buf)
{
    int time_remain;

    if (EN_SYS_SLEEP == en_state && !needEnterLockStatus())
    {
        g_time_out_cnt = 0;
		pthread_mutex_lock(&mutex_turnPage);
		title_widget->turnPage("3", 3);
		stack_title->setCurrentIndex(1);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		unlockTurnPageMutex(this);
		set_about_widget->quitSet();
		timer_standby->stop();
		//qDebug()<<"get_wake_up_msg onTimerOut stop";
		qDebug()<<"enter standby 5555";
		en_state = EN_SYS_STANDBY;
		
		//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

		time_remain = timer_sleep->remainingTime();
		if (time_remain <= 0)
		{
		    //qDebug()<<"time_remain="<<time_remain;
		    //timer_sleep->start();
		}
    }
	else if(needEnterLockStatus())
	{
	    g_time_out_cnt = 0;
	    en_state = EN_SYS_LOCK;
	    pair_widget->doQrgenInit();
		lock_word_widget->setTextType(lock_text);
		lock_word_widget->setText();
		statked_widget->setCurrentWidget(pair_widget);		
		stack_title->setCurrentWidget(lock_word_widget);
		
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
}

void MainWidget::get_firmware_update_per(char *value, uint32 len_buf)
{
    if (stack_title->currentWidget() != update_widget)
    {
    
	qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
        stack_title->setCurrentWidget(update_widget);
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
    }
	update_widget->get_firmware_update_per(value,len_buf);

	if(statked_widget->currentWidget()!=logo_widget)
	{
	    statked_widget->setCurrentWidget(logo_widget);
	}
}

void MainWidget::switch_to_scene_slot()
{
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

	statked_widget->setCurrentWidget(content_widget[g_current_room]);
}

void MainWidget::switch_to_curtain_slot()
{
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

	statked_widget->setCurrentWidget(curtain_widget[g_current_room]);
}

void MainWidget::switch_to_hvac_slot()
{
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;

	statked_widget->setCurrentWidget(havc_widget[g_current_room]);
}

void MainWidget::turnPage(int current_page, int last_page)
{
    QImage *new_image;
	QWidget *widgetOut = NULL, *widgetIn = NULL;
	int upFlag = 1;
	
	syslog(LOG_INFO|LOG_LOCAL1,"state=%d, current_page=%d, last_page=%d\n", 
		                   en_state, current_page, last_page);
	
	qDebug()<<"turnPage,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	qDebug()<<"-----current_page-----="<<current_page<<",last_page="<<last_page;
    //qDebug()<<"MainWidget::turnPage="<<current_page<<",last="<<last_page<<"en_state="<<en_state;
	if (stack_title->currentWidget() == title_widget && en_state == EN_SYS_SET)
	{
	   // qDebug()<<"set enstate force norm";
	}


	widgetOut = statked_widget->currentWidget();


	
    //pthread_mutex_lock(&mutex_turnPage);
	g_upSreenTurning = 1;
	if (current_page == 0)
	{

	    if (en_state == EN_SYS_SET)
		{
		    g_upSreenTurning = 0;
			
			//unlockTurnPageMutex(this);
		    return;
		}
		
		if (last_page == 1)
		{  
		    // content_widget[g_current_room]->showContent();
		    content_widget[g_current_room]->snap_shot(&new_image);
			curtain_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    curtain_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_scene_slot()));
		}
		else if (last_page == 2)
		{
		    //content_widget[g_current_room]->switch_to_scene(last_page);
		    //content_widget[g_current_room]->showContent();
		    content_widget[g_current_room]->snap_shot(&new_image);
			havc_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    havc_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_scene_slot()));
		}
		else
		{

			statked_widget->setCurrentWidget(content_widget[g_current_room]);
		}
	    widgetIn = content_widget[g_current_room];
		upFlag = 0;
		content_widget[g_current_room]->translateLanguage();
	}
	else if(current_page == 1)
	{
		if (en_state == EN_SYS_SET)
		{
		    g_upSreenTurning = 0;
			
			//unlockTurnPageMutex(this);
			return;
		}
		
		if (last_page == 0)
		{
		    //curtain_widget[g_current_room]->showContent();
		    curtain_widget[g_current_room]->snap_shot(&new_image);
			content_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    content_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_curtain_slot()));
		}
		else if (last_page == 2)
		{
		    //curtain_widget[g_current_room]->showContent();
		    curtain_widget[g_current_room]->snap_shot(&new_image);
			havc_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    havc_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_curtain_slot()));
		}
		else
		{
			statked_widget->setCurrentWidget(curtain_widget[g_current_room]);
		}
		widgetIn = curtain_widget[g_current_room];
		upFlag = 0;
		curtain_widget[g_current_room]->translateLanguage();
	}
	else if (current_page == 2)
	{
		if (en_state == EN_SYS_SET)
		{
		    g_upSreenTurning = 0;
			//unlockTurnPageMutex(this);
			return;
		}
		
		if (last_page == 0)
		{
		    //havc_widget[g_current_room]->showContent();
		    havc_widget[g_current_room]->snap_shot(&new_image);
			content_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    content_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_hvac_slot()));
		}
		else if (last_page == 1)
		{
		    //havc_widget[g_current_room]->showContent();
		    havc_widget[g_current_room]->snap_shot(&new_image);
			curtain_widget[g_current_room]->setDuration(TIME_OF_SCENE_SWITCH+TIME_OF_SCENE_DELAY);
		    curtain_widget[g_current_room]->switch_to_page(current_page, new_image);
			QTimer::singleShot(TIME_OF_SCENE_SWITCH, this, SLOT(switch_to_hvac_slot()));
		}
		else
		{
			statked_widget->setCurrentWidget(havc_widget[g_current_room]);
		}
		widgetIn = havc_widget[g_current_room];
		upFlag = 0;
		havc_widget[g_current_room]->translateLanguage();
	}
	else if(current_page == 3)
	{
	    statked_widget->setCurrentWidget(standby_content);
		//widgetIn = standby_content;
		widgetIn = NULL;
	}
	else if(current_page == 4)
	{

	    statked_widget->setCurrentWidget(sleep_widget);
	
		g_thread->msg_reset_qt();	
		
		widgetIn = NULL;
	}
	else if(current_page == 5)
	{
	    statked_widget->setCurrentWidget(set_about_widget);
		g_thread->msg_reset_qt();
		widgetIn = set_about_widget;
	}
	else if(current_page == 6)
	{
	    statked_widget->setCurrentWidget(logo_widget);
		g_thread->msg_reset_qt();
		widgetIn = logo_widget;
	}
	if((widgetIn != NULL)&&(widgetOut != widgetIn)&&upFlag)
	{
	    switch_UpSreen_Animation(widgetOut,widgetIn,1); 
	}
	else
    {
       g_upSreenTurning = 0;
    }
	
	if (statked_widget->currentWidget() != standby_content 
			&& statked_widget->currentWidget() != sleep_widget
			&& statked_widget->currentWidget() != set_about_widget
			&& statked_widget->currentWidget() != pair_widget
			&& statked_widget->currentWidget() != logo_widget)
	{
		prev_up_index = statked_widget->currentIndex();
	}

	if (stack_title->currentWidget() != standby_title 
			&& stack_title->currentWidget() != sleep_title
			&& stack_title->currentWidget() != set_widget
			&& stack_title->currentWidget() != lock_word_widget
			&& stack_title->currentWidget() != update_widget)
	{
		prev_down_index = stack_title->currentIndex();
		ENNO_LOG_ERR("prev_up_index=%d, prev_down_index=%d\n", prev_up_index, prev_down_index );
	}

	g_thread->msg_menu_change(current_page);

	//unlockTurnPageMutex(this);
}
void MainWidget::mousePressEvent(QMouseEvent *event)
{

    //qDebug()<<"MainWidget::mousePressEvent,x="<<event->pos().x()<<", y=%d"<<event->pos().y();
	//qDebug()<<"currentIndex()="<<stack_title->currentIndex();
	//qDebug()<<"statked_widget->cur="<<statked_widget->currentIndex();
	if (en_state == EN_SYS_SLEEP)
	{
	    //qDebug()<<"fpga dereset";
	    //system("echo 1 > /sys/class/gpio/gpio48/value");
	}
	if (stack_title->currentIndex() == 1 || stack_title->currentIndex() == 2)
	{
	
		mstimer_flag = 1; //add by iovine

		
		//en_state = EN_SYS_NORM;
		
		
		//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<en_state;
	}
}
void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //printf("MainWidget::mouseReleaseEvent\n");
   // sleep(1);
    //title_widget->turnPage("3");
}
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
   // printf("MainWidget::mouseMoveEvent\n");
}

void MainWidget::switch_UpSreen_Animation(QWidget *widgetOut,QWidget *widgetIn,int moveLeft)
{
    if(moveLeft == 1)
    {
        int count = 0;
        statked_widget->setCurrentWidget(widgetOut);
        //widgetOut->resize(OLED_PIX_X*2, OLED_PIX_Y);
		
		
		if(tempLabelAni == NULL)
        {
             tempLabelAni = new QLabel(widgetOut);
        }
        else
        {
             tempLabelAni->setParent(widgetOut);
        }
        tempLabelAni->setGeometry(320, 0, 320, 320);
        tempLabelAni->setPixmap(widgetIn->grab());
        //sceneToMoveOut->setAutoFillBackground(true);
        tempLabelAni->show();
        tempLabelAni->raise();

		if(tempLabelLeft == NULL)
		{
		    tempLabelLeft = new QLabel(widgetOut);
		}
		else
	    {
	        tempLabelLeft->setParent(widgetOut);
	    }
		
	    tempLabelLeft->setGeometry(0, 0, 320, 320);
		tempLabelLeft->setPixmap(widgetOut->grab());
		tempLabelLeft->show();
		tempLabelLeft->raise();
		tempLabelLeft->setObjectName("tempLeft");

		QPropertyAnimation * animation_Out = new QPropertyAnimation(widgetOut, "geometry");
        group_upScreen->clear();

        animation_Out->setDuration(300);
        animation_Out->setStartValue(QRect(0, widgetOut->pos().y(), 640, 320));
        animation_Out->setEndValue(QRect(-OLED_PIX_X, widgetOut->pos().y(), 640, 320));

        group_upScreen->addAnimation(animation_Out);
        group_upScreen->start();
        widToSet = widgetIn;
        connect(group_upScreen, SIGNAL(finished()), this, SLOT(upAniFinish()));
    }
	else
    {
        g_upSreenTurning = 0;
    }
}

MainWidget::~MainWidget()
{    
	pthread_mutex_destroy(&mutex_turnPage);
	if(g_statusXmlHandle)
	{
	    free(g_statusXmlHandle);
		g_statusXmlHandle = NULL;
	}
}

void unlockTurnPageMutex(QWidget * widgetIn)
{
    int count = 0;
	widgetIn->update();
    while(g_upSreenTurning||g_BottomSreenTurning)
    {
       usleep(5000);
	   count++;

	   if(count>=100)
	   {
	      break; 
	   }
    }
	g_upSreenTurning = 0;
	g_BottomSreenTurning = 0;
	pthread_mutex_unlock(&mutex_turnPage);
}

void MainWidget::refresh_ui(void)
{
	if(g_statusXmlHandle==NULL)
	{
	    return;
	}

	g_statusXmlHandle->setReadOut(1);

	CTL_DEVICE_CONF devInfoMsg;

	DevList listDevInfo;
	LightStatusList listLight;
	SceneStatusList listScene;

	g_statusXmlHandle->getAllDevs(listDevInfo,listLight,listScene);

	//refresh devs including curtains and havcs
	if(listDevInfo.count()>0)
	{
	   DevList::iterator itDevList;
	   for(itDevList=listDevInfo.begin();itDevList!=listDevInfo.end();itDevList++)
	   {
	       devInfoMsg.device_id = itDevList->devId;
	       devInfoMsg.cid = itDevList->cid;		   
		   devInfoMsg.value = itDevList->val;
		   if(itDevList->roomId >= g_room_num)
		   {
		       continue;
		   }
		   if(itDevList->devType == DEV_TYPE_CURTAIN)
		   {
		      (void)curtain_widget[itDevList->roomId]->set_dev_update((char*)&devInfoMsg, sizeof(CTL_DEVICE_CONF)); 
		   }
		   else if(itDevList->devType == DEV_TYPE_HAVC)
		   {
		       ENNO_LOG_INFO("roomid=%d, set_dev_update\r\n", itDevList->roomId);
			   (void)havc_widget[itDevList->roomId]->set_dev_refresh_ui();
			   //(void)havc_widget[itDevList->roomId]->set_dev_update((char*)&devInfoMsg, sizeof(CTL_DEVICE_CONF));
		   }
        }
    }

    char buff[32];
    uint32 lenRet = 0, offset = 0;
     //refresh the light status
     LightStatusList::iterator itLight;
     if(listLight.count()>0)
     {
         for(itLight =listLight.begin();itLight!=listLight.end();itLight++)
         {
             if(itLight->roomId>=g_room_num)
             {
                 continue;
             }

			 qDebug()<<"roomid="<<itLight->roomId<<",status="<<itLight->lightStatus;
             if(itLight->lightStatus)
             {
                 //roomlight_widget[itLight->roomId]->lighton(); //启动时不去执行开关灯的命令。
                 roomlight_widget[itLight->roomId]->set_light_status(1);
			     title_widget->control_lihght_point(itLight->roomId, 1);
             }
             else
             {
                 //roomlight_widget[itLight->roomId]->lightoff(); //启动时不去执行开关灯的命令。
                 roomlight_widget[itLight->roomId]->set_light_status(0);//update light status
			     title_widget->control_lihght_point(itLight->roomId, 0);
             }

             
			 qDebug()<<"roomid="<<itLight->roomId<<",dim="<<itLight->lightDim;
             if (itLight->lightDim <= 100 && itLight->lightDim >= 0)
             {
				 room_light_dim_widget[itLight->roomId]->set_light_dim_value(itLight->lightDim);
				 roomlight_widget[itLight->roomId]->set_light_value(itLight->lightDim);
             }
         }
     }
     //refresh the scenes
     SceneStatusList::iterator itScene;
     if(listScene.count()>0)
     {
         for(itScene=listScene.begin();itScene!=listScene.end();itScene++)
         {
             if(itScene->roomId>=g_room_num)
             {
                 continue;
             }
             offset = 0;
             enno_encode_uint(buff, itScene->sceneId, &lenRet, 32);
             offset+=lenRet;
             enno_encode_uint(buff+offset, itScene->sceneStatus, &lenRet, 32-offset);
             content_widget[itScene->roomId]->get_scene_status(buff, 16);
         }
     }
	
#if 0
	char outDoorBitMap;
	OutDoorValues stOutDoorValues;
	g_statusXmlHandle->getOutDoorValues(&outDoorBitMap,&stOutDoorValues);

	if(outDoorBitMap>0)
	{
	    qDebug()<<"xml temp="<<stOutDoorValues.temperature;
	    if (stOutDoorValues.temperature != 0 && stOutDoorValues.temperature != 212)
	    { 
		    if(outDoorBitMap&BIT_TEMP)
		    {
		         offset = 0;
		         enno_encode_uint(buff, CHAR_P2_OUTDOOR_TEMP, &lenRet, 32);
				 offset+=lenRet;
		         enno_encode_uint(buff+offset, stOutDoorValues.temperature, &lenRet, 32-offset);
				 this->get_outdoor_value(buff, 16);			 
		    }

			if(outDoorBitMap&BIT_AQI)
		    {
		         offset = 0;
		         enno_encode_uint(buff, CHAR_P2_OUTDOOR_AQI, &lenRet, 32);
				 offset+=lenRet;
		         enno_encode_uint(buff+offset, stOutDoorValues.aqi, &lenRet, 32-offset);
				 this->get_outdoor_value(buff, 16);			 
		    }

			if(outDoorBitMap&BIT_WEATHER)
		    {
		         offset = 0;
		         enno_encode_uint(buff, CHAR_P2_OUTDOOR_WEATHER, &lenRet, 32);
				 offset+=lenRet;
		         enno_encode_uint(buff+offset, stOutDoorValues.weather, &lenRet, 32-offset);
				 this->get_outdoor_value(buff, 16);			 
		    }		
	    }
	}
#endif

	unsigned int openBitMap;
	g_statusXmlHandle->getOpenDev(&openBitMap);
    enno_encode_uint(buff, openBitMap, &lenRet, 32);
	this->get_dev_open_notify(buff, 8);
#if 0
	//get sensor value
	char inDoorBitMap;
	InDoorValues stInDoorValues;
	g_statusXmlHandle->getInDoorValues(&inDoorBitMap,&stInDoorValues);

    //qDebug()<<"refresh_ui";

	if(inDoorBitMap>0)
	{
		if(inDoorBitMap&BIT_TEMP_IN)
		{
			 offset = 0;
			 enno_encode_uint(buff, CHAR_P2_TEMP_SENSOR, &lenRet, 32);
			 offset+=lenRet;
			 enno_encode_uint(buff+offset, stInDoorValues.temp, &lenRet, 32-offset);
			 //qDebug()<<"refresh ui temp="<<;
			 this->get_sensor_value(buff, 16); 		 
		}

		if(inDoorBitMap&BIT_HUM_IN)
		{
			 offset = 0;
			 enno_encode_uint(buff, CHAR_P2_HUMIDITY_SENSOR, &lenRet, 32);
			 offset+=lenRet;
			 enno_encode_uint(buff+offset, stInDoorValues.hum, &lenRet, 32-offset);
			 this->get_sensor_value(buff, 16); 		 
		}

		if(inDoorBitMap&BIT_VOC_IN)
		{
			 offset = 0;
			 enno_encode_uint(buff, CHAR_P2_VOC_SENSOR, &lenRet, 32);
			 offset+=lenRet;
			 enno_encode_uint(buff+offset, stInDoorValues.voc, &lenRet, 32-offset);
			 this->get_sensor_value(buff, 16); 		 
		}		
		
	}
#endif

	g_statusXmlHandle->setReadOut(0);	

}

int needEnterLockStatus(void)
{
    //read data from LOCK_STATUS_FILE, and judge if need enter lock status
    QFile file(LOCK_STATUS_FILE);
	int length = 0;
	char buff[16];
	if(!file.exists())
	{
	    return 0;
	}
	else
	{
	    memset(buff,0,16);
        file.open(QIODevice::ReadOnly);

		length=file.read(buff,10);
		if(length!=0)
		{
		    int lock_val = atoi(buff);
			if(lock_val>=EN_USER_GUEST)
			{
			    return 1;
			}
			else
			{
			    return 0;
			}
		}
	}
    return 0;
}

void MainWidget::get_lock_status(char *value, uint32 len_buf)
{
    uint32 lock_val;
	uint32 len;
	
	(void)enno_decode_uint(value, &lock_val, &len, len_buf);
    qDebug()<<"lock status val is "<<lock_val<<","<<value[0]<<","<<value[1]<<value[2]<<value[3]<<endl;
	if(lock_val<1)//unlock for the usr
	{
	    en_state = EN_SYS_NORM;
		if (access(PAIR_OK_FILE, F_OK) == 0)
		{
		    //pair ok
		    if (access(HOUSE_SET_FILE, F_OK) == 0)
	    	{
			    title_widget->turnPage("0", 0);
				stack_title->setCurrentIndex(0);
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		    }
			else
			{
			    QTimer::singleShot(10, this, SLOT(show_font()));
			    statked_widget->setCurrentWidget(room_select_widget);
			    stack_title->setCurrentWidget(house_room_widget);
				qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
			}
		}
		else
		{
		    title_widget->turnPage("0",0);			    
	        stack_title->setCurrentIndex(0);
			qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
		}
		//title_widget->turnPage("0",0);			    
	    //stack_title->setCurrentIndex(0);
        
	}
	else //lock when timeout.
	{
	    en_state = EN_SYS_LOCK;
	    pair_widget->doQrgenInit();
		lock_word_widget->setTextType(lock_text);
		lock_word_widget->setText();
		statked_widget->setCurrentWidget(pair_widget);		
		stack_title->setCurrentWidget(lock_word_widget);
		qDebug()<<"func="<<__func__<<",line="<<__LINE__<<",menu inxdex="<<stack_title->currentIndex();
	}
     
}

void MainWidget::update_light_on(char *value, uint32 len_buf)
{
    int ret = -1;
	int i = 0;
	
    for (i = 0; i < g_room_num; i++)
    {
        ret = roomlight_widget[i]->updateLightStatus(value,len_buf);
		/*if(ret>=0)
		{
		    break;
		}*/

        //qDebug()<<"value="<<value<<",roomlight["<<i<<"],alllight="<<(roomlight_widget[i]->isAllLightOff() ? 0 : 1);
		if ( g_statusXmlHandle
			&&!g_statusXmlHandle->getReadOut())
		{
		    g_statusXmlHandle->setRoomIndex(i);
		    g_statusXmlHandle->updateLight(roomlight_widget[i]->isAllLightOff() ? 0 : 1);
		}

		if (ret>=0)
		{
		    if (roomlight_widget[i]->isAllLightOff())
		    {
		        title_widget->control_lihght_point(i,0);//g_current_room  
		    }
			else
			{
			    title_widget->control_lihght_point(i,1);//g_current_room
			}
		    
		}
    }
	
}

enno_result MainWidget::update_light_status(char *value, uint32 len_buf)
{
    int ret = -1;
	int i = 0;
	char *buf = value;
	uint32 len_out;
	uint32 len_total = len_buf;
	uint32 room_id;
	uint32 light_status;

	ret = enno_decode_uint(buf, &room_id, &len_out, len_total);
	ENNO_CHECK_RET(ret);
	buf += len_out;
    len_total -= len_out;

	ret = enno_decode_uint(buf, &light_status, &len_out, len_total);
	ENNO_CHECK_RET(ret);
	buf += len_out;
    len_total -= len_out;

	ENNO_CHECK_PARA_RET(room_id >= g_room_num);

	roomlight_widget[room_id]->setLightStatus(light_status);


	if ( g_statusXmlHandle
				&&!g_statusXmlHandle->getReadOut())
	{
		g_statusXmlHandle->setRoomIndex(room_id);
		g_statusXmlHandle->updateLight(light_status);
	}
	
	i = room_id;
 
    if (!light_status)
    {
        title_widget->control_lihght_point(i,0);//g_current_room  
    }
	else
	{
	    title_widget->control_lihght_point(i,1);//g_current_room
	} 

	return ENNO_OK;
}

int MainWidget::isAllClose(int devType)
{
    int ret = 1;
	int currRoomClose = 1;

	for (int i = 0; i < g_room_num; i++)
	{
		switch(devType)
		{			
			case DEV_RS485_HEATER_COOLER:
				currRoomClose = havc_widget[i]->isAllAcClose();
				break;

			case DEV_FLOOR_HEAT:
				currRoomClose = havc_widget[i]->isAllFloorClose();
				break;
				
			default:
			break;
		}	
		if(currRoomClose==0)
		{
		   ret = 0;
		   break;
		}
	}
	return ret;
}

