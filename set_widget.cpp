#include "set_widget.h"
//#include "ui_mainwindow.h"
#include <QPalette>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QApplication>
#include <QGraphicsBlurEffect>
#include <QMessageBox>
#include "wthread.h"
#include "status_xml.h"
#include "common.h"

//QLabel *set_content;
extern SetAboutWidget *set_about_widget;
extern char g_pairid[11];
uint32 g_dali_set_flag = 0;
#define		SET_WIDGET_DBG		1

void SetWidget::setWidSlot(MainWidget *wnd)
{
    window = wnd;
	//connect(return_button,SIGNAL(clicked()),wnd,SLOT(reviewWidget()));
	//connect(button_list.at(5), SIGNAL(changeLanguage()), wnd, SLOT(changeLanguage()));
}

void SetWidget::translateLanguage()
{

	#if 1 
    button_list.at(0)->setText(tr("Network Fix"));
	button_list.at(1)->setText(tr("Dali Address"));
	button_list.at(2)->setText(tr("Factory Reset"));
	button_list.at(3)->setText(tr("ETS Programe Set"));	
	button_list.at(4)->setText(tr("Boot Mode"));
	button_list.at(5)->setText(tr("Language"));
	button_list.at(6)->setText(tr("About"));
	button_list.at(7)->setText(tr("Net Reset"));
	button_list.at(8)->setText(tr("Lights identify"));
    button_list.at(9)->setText(tr("Dali reconfiguration"));
    button_list.at(10)->setText(tr("Child lock"));
	#endif
}
void SetWidget::setfunc(QString current_icon)
{
	bool ok;  
	char buf[10];
    int current_index = current_icon.toInt(&ok, 10);
    QPushButton *push_button;
    scrollArea->verticalScrollBar()->setVisible(true);

#if SET_WIDGET_DBG
qDebug() << "[UI] file=" << __FILE__ << ",line=" << __LINE__ << ",idx=" << current_index << endl;
#endif


	if (0 == current_index)
	{
	   // printf("button %d on\n", current_index);
	}
	for (int i = 0; i < button_num; i++)
	{
		push_button = button_list.at(i);
	    if (i != current_index)
	    {
	       // push_button->setStyleSheet("font-size:30px;color:rgb(180,180,180);font-weight:bold;text-align:left;");
			if (i%2 == 0)
			{
			/*push_button->setStyleSheet("font-size:30px;color:rgb(180,180,180);"\
			                           "font-weight:bold;text-align:left;"\
			                           "background-color:rgb(100,100,100);");*/
				push_button->setStyleSheet("background-color:rgb(109,109,109);\
                    font-size:30px;color:white;text-align:center;border-radius: 15px;");
                   // border-radius: 15px;  border: 2px;");
			}
			else
			{
				push_button->setStyleSheet("background-color:rgb(99,99,99);\
                    font-size:30px;color:white;text-align:center;border-radius: 15px;\
				    ");
			}
		}
		else
		{
		   // push_button->setStyleSheet("font-size:35px;color:rgb(255,255,255);font-weight:bold;text-align:left;");
		   push_button->setStyleSheet("background-color:rgb(52,204,255);\
					   font-size:30px;color:white;width:320px;\
                       height:90px;text-align:center;border-radius: 15px;");

		}
	}
	set_about_widget->doSetMsg(current_index);
	#if 0
	switch(current_index)
	{
	case 0: //about
	    printf("Network\n");
	   // system("killall -9 panno_s_app.out");
		////system("cp -a /home/user/bin /home/user/binbak");
		//system("tar -zxvf /mnt/update/p2bin -C /home/user/");
		//sleep(3);
		//system("chmod +x /home/user/bin/*");
		//system("cd /home/user/bin && ./panno_s_app.out &");
		//QMessageBox::information(this, tr("serial num: "),
      //                           tr("type:panno_s"
      //                              "version:01A02W01"));
        //set_content->setText(tr("Network : connect"));
        set_about_widget->doSetMsg(current_index);
		break;
		
	case 1:
	    printf("Net Reset\n");
		//system("echo 0 > /sys/class/gpio/gpio51/value ");
		//sleep(1);
		//system("echo 1 > /sys/class/gpio/gpio51/value ");
		
        //set_content->setText(tr("wait Net Reset ..."));
		break;
		
	case 2:
		printf("Factory Reset\n");
		//g_thread->msg_reset_qt();
		
       // set_content->setText(tr("wait Factory Reset ..."));
		break;

	case 3:
		qDebug()<<"boot mode";
		//printf("reset fpga");
	    //system("echo 0 > /sys/class/gpio/gpio48/value");
		//sleep(1);
	    //system("echo 1 > /sys/class/gpio/gpio48/value");
	    //printf("Boot Mode");
		
       //set_content->setText(tr("wait Factory Reset ..."));
		break;

	case 4:
		qDebug()<<"setup code";
		break;

	//case 5:
	//	qDebug()<<"about";
	//	break;

	case 5:
		qDebug()<<"changeLanguage";	
		language_flag ^= 1;
		emit changeLanguage(language_flag);
		break;
		
//	case 4:
//		printf("reboot, todo\n");
//		system("reboot");
//		break;

	default:
		printf("unknown msg=%d\n", current_index);
		break;
	}
	#endif
}

SetWidget::SetWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	//this->setMaximumSize(320,320);
	//this->setMinimumSize(320,320);
	QStringList string_list;
    //4+4+2+1.    
    string_list<<"Network Fix"<<"Dali Address"<<"Factory Reset"<<"ETS Programe Set"\
              <<"Boot Mode"<<"Language"<<"About"<<"Net Reset"\
             <<"Lights identify"<<"Dali reconfiguration" << "Child lock";
    //4 skip.
	//string_list<<"Network"<<"Net Reset"<<"Factory Reset"<<"Boot Mode";
    QSignalMapper *signal_mapper = new QSignalMapper(this);
	
	scrollArea = new QScrollArea(this);
	wid_page = new QWidget(scrollArea);
	button_num = string_list.size();
    wid_page->setMaximumSize(320,880);  // 80*n
    wid_page->setMinimumSize(320,800);

	scrollArea->setGeometry(QRect(0,0,320,320));
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(wid_page);
    scrollArea->setObjectName("scroll");

	left_mouse_press = 0;
	language_flag = 0;

	//scrollArea->verticalScrollBar()->setValue(220);

	//wid_page->setGeometry(QRect(0,0,320,320));
	wid_page->setFixedSize(320,880);

	for(int i=0; i<string_list.size(); i++)
	{
		QPushButton *push_button = new QPushButton(wid_page);
       // push_button->setFixedSize(300, 81);
		push_button->setText(string_list.at(i));
		//push_button->setObjectName("functionButton");
		if (i%2 == 0)
		{
		    /*push_button->setStyleSheet("font-size:30px;color:rgb(180,180,180);"\
			                           "font-weight:bold;text-align:left;"\
			                           "background-color:rgb(100,100,100);");*/
            //push_button->setStyleSheet("background-color:rgb(109,109,109);\
            //	font-size:30px;color:white;text-align:left;");
                //border-radius: 15px;  border: 10px;");
            push_button->setStyleSheet("background-color:rgb(85,85,85);\
                font-size:30px;color:white;text-align:center;border-radius: 35px;");
		}
		else
		{
            //push_button->setStyleSheet("background-color:rgb(99,99,99);\
            //	font-size:30px;color:white;text-align:left;");
               // border-radius: 15px;  border: 10px;");
            push_button->setStyleSheet("background-color:rgb(85,85,85);\
                font-size:30px;color:white;text-align:center;border-radius: 35px;\
                ");
		}

		button_list.append(push_button);
		connect(push_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
		
	    //v_layout->addWidget(push_button);
        push_button->setGeometry(QRect(0, i*80, 312, 70));//0, i*80, 320, 50//
		
		signal_mapper->setMapping(push_button, QString::number(i, 10));
#if SET_WIDGET_DBG
   qDebug() << "[UI] file=" << __FILE__ << ",line=" << __LINE__ << ",i="<< i << ",str=" << string_list[i] << endl;
#endif


		//connect(tool_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	}
	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(setfunc(QString)));
	//connect(this, SIGNAL(changeLanguage(int)), window, SLOT(changeLanguage(int)));
	/*return_button = new QPushButton();
	v_layout->addWidget(return_button);
	return_button->setFixedSize(300, 45);
	return_button->setText("return");
	return_button->setObjectName("functionButton");
	return_button->setStyleSheet("font-size:35px;color:white;font-weight:bold;text-align:left;");
	return_button->setVisible(0);*/
	//wid_page->setLayout(v_layout);
    wid_page->raise();
	y_pos = 0;
	wid_page->move(0, y_pos);
	
	this->translateLanguage();
    scrollArea->verticalScrollBar()->setVisible(false);
}
/*void SetWidget::changeLanguage()
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
}*/
void SetWidget::mousePressEvent(QMouseEvent *event)
{
#if 0
    printf("SetWidget::mousePressEvent,x=%d, y=%d\n", event->pos().x(), event->pos().y());
#endif
   
	left_mouse_press = 1;
	m_mouseSrcPos = event->pos();						
	m_mouseDstPos = event->pos();
    scrollArea->verticalScrollBar()->setVisible(true);
}
void SetWidget::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
    printf("SetWidget::mouseReleaseEvent\n");
#endif

    if (!left_mouse_press)
    {
        return;
    }
    left_mouse_press = 0;
	m_mouseDstPos = event->pos();		

   // ENNO_LOG_ERR("dst.y=%d, src.y=%d, y_pos=%d\n", m_mouseDstPos.y(), m_mouseSrcPos.y(), y_pos);
    // up to middle
	if ((last_mouseDstPos.y() <= 5) && (m_mouseDstPos.y() > 150))
	{
		m_mouseDstPos = last_mouseDstPos;
	}
	
    if (m_mouseDstPos.y() <  m_mouseSrcPos.y()) 	
    {
        if(m_mouseSrcPos.y() - m_mouseDstPos.y() > 280)
        {
            y_pos += 320;
        }
        else if(m_mouseSrcPos.y() - m_mouseDstPos.y() > 200)
        {
            y_pos += 240;
        }
		else if(m_mouseSrcPos.y() - m_mouseDstPos.y() > 120)
        {
            y_pos += 160;
        }
		else if (m_mouseSrcPos.y() - m_mouseDstPos.y() > 40)
		{
		    y_pos += 80;
		}
		if (y_pos >= 480)//320
		{
		    y_pos = 560;
		}
		//wid_page->move(0, -y_pos);
		wid_page->setGeometry(QRect(0,-y_pos,320,320));
		scrollArea->verticalScrollBar()->setValue(y_pos);
    }
	else  //down
	{
		if(m_mouseDstPos.y() - m_mouseSrcPos.y() > 280)
		{
		    y_pos -= 320;
		}
		else if(m_mouseDstPos.y() - m_mouseSrcPos.y() > 200)
		{
		    y_pos -= 240;
		}
		else if (m_mouseDstPos.y() - m_mouseSrcPos.y() > 120)
		{
		    y_pos -= 160;
		}
		else if (m_mouseDstPos.y() - m_mouseSrcPos.y() > 40)
		{
			y_pos -= 80;
		}

        if (y_pos <= 0)
        {
            y_pos = 0;
        }
		
		//ENNO_LOG_ERR("dst.y=%d, src.y=%d, y_pos=%d\n", m_mouseDstPos.y(), m_mouseSrcPos.y(), y_pos);
		wid_page->setGeometry(QRect(0,-y_pos,320,320));
		scrollArea->verticalScrollBar()->setValue(y_pos);
	    //wid_page->move(0, -y_pos);
	}
	
}
void SetWidget::mouseMoveEvent(QMouseEvent *event)
{
    //printf("ContentWidget::mouseMoveEvent,");
    if (!left_mouse_press)
    {
        left_mouse_press = 1;
		m_mouseSrcPos = event->pos();			
		m_mouseDstPos = event->pos();
        scrollArea->verticalScrollBar()->setVisible(true);
        //return;
	}
	
	m_mouseDstPos = event->pos();			
    if ((last_mouseDstPos.y() <= 5) && (m_mouseDstPos.y() > 150))
	{
	    return;
	}
	last_mouseDstPos = m_mouseDstPos;
#if 0	
    printf("src(%d,%d), dst(%d,%d), move(%d,%d)\n", 
	       m_mouseSrcPos.x(), m_mouseSrcPos.y(), m_mouseDstPos.x(), m_mouseSrcPos.y(),
	       m_mouseDstPos.x() - m_mouseSrcPos.x(), m_mouseDstPos.y() - m_mouseSrcPos.y()); 
  ENNO_LOG_ERR("dst.y=%d, src.y=%d, y_pos=%d\n", m_mouseDstPos.y(), m_mouseSrcPos.y(), y_pos);
#endif

	if (m_mouseDstPos.y() <  m_mouseSrcPos.y())
	{
	    //up
	    if (y_pos == 560)  //320
	    {
	        return;
	    }

        if (y_pos == 0)
        {
			if (m_mouseSrcPos.y() - m_mouseDstPos.y() < 160)
			{
			    wid_page->move(0, m_mouseDstPos.y()-m_mouseSrcPos.y());
				scrollArea->verticalScrollBar()->setValue(m_mouseSrcPos.y() - m_mouseDstPos.y());
			}						
        }
		else if (y_pos == 80)
		{
		    if (m_mouseSrcPos.y() - m_mouseDstPos.y() < 80)
		    {
		        wid_page->move(0, m_mouseDstPos.y() - m_mouseSrcPos.y());
				scrollArea->verticalScrollBar()->setValue(m_mouseSrcPos.y() - m_mouseDstPos.y()+80);
		    }
			
		}
		else if (y_pos == 160)
		{
		    if (m_mouseSrcPos.y() - m_mouseDstPos.y() < 160)
		    {
		        wid_page->move(0, m_mouseDstPos.y() - m_mouseSrcPos.y());
				scrollArea->verticalScrollBar()->setValue(m_mouseSrcPos.y() - m_mouseDstPos.y()+160);
		    }
		}
		else if (y_pos == 240)
		{
		    if (m_mouseSrcPos.y() - m_mouseDstPos.y() < 240)
		    {
		        wid_page->move(0, m_mouseDstPos.y() - m_mouseSrcPos.y());
				scrollArea->verticalScrollBar()->setValue(m_mouseSrcPos.y() - m_mouseDstPos.y()+240);
		    }
		}
	   
	}
	else if (m_mouseDstPos.y() >  m_mouseSrcPos.y())
	{
	    //down
	    if (y_pos == 0)
	    {	 
	        return;
	    }
		
		if (y_pos == 320)
		{
		   if (m_mouseDstPos.y() - m_mouseSrcPos.y() < 320)
		   {
			   wid_page->move(0, m_mouseDstPos.y()- m_mouseSrcPos.y() - 320);
			   scrollArea->verticalScrollBar()->setValue(320 - m_mouseDstPos.y() + m_mouseSrcPos.y());
		   }
		}
        else if (y_pos == 240)
        {
			if (m_mouseDstPos.y() - m_mouseSrcPos.y() < 240)
			{
			    wid_page->move(0, m_mouseDstPos.y()- m_mouseSrcPos.y() - 240);
				scrollArea->verticalScrollBar()->setValue(240 - m_mouseDstPos.y() + m_mouseSrcPos.y());
			}
        }
		else if (y_pos == 160)
		{
			if (m_mouseDstPos.y() - m_mouseSrcPos.y() < 160)
			{
			    wid_page->move(0, m_mouseDstPos.y()- m_mouseSrcPos.y() - 160);
				scrollArea->verticalScrollBar()->setValue(160 - m_mouseDstPos.y() +  m_mouseSrcPos.y());
			}
        }
		else if (y_pos == 80)
		{
		    if (m_mouseSrcPos.y() - m_mouseDstPos.y() < 80)
		    {
		        wid_page->move(0, m_mouseDstPos.y() - m_mouseSrcPos.y() - 80);
				scrollArea->verticalScrollBar()->setValue(80 - m_mouseDstPos.y() +  m_mouseSrcPos.y());
		    }
		}
	}

	
  //  ENNO_LOG_ERR("dst.y=%d, src.y=%d, y_pos=%d\n", m_mouseDstPos.y(), m_mouseSrcPos.y(), y_pos);
}

void SetAboutWidget::setWidSlot(MainWidget *wnd)
{
    //window = wnd;
	//connect(return_button,SIGNAL(clicked()),wnd,SLOT(reviewWidget()));
}
//
void SetAboutWidget::setQrwid(QqrWidget *qrwid)
{
    qr_widget = qrwid;
}

void SetAboutWidget::translateLanguage()
{
    uint32 ret;
    uint32 val;


    netfix_title->setText(tr("Current Network :"));
	show_net_state();
	show_net_wac_state();
    netfix_yes->setText(tr("Start Fix"));

	labelNetStatusHead->setText(tr("Net Status:"));
	labelWacStatusHead->setText(tr("Net Status:"));

	dali_addr_set_title->setText(tr("Dali Address Set :"));
	dali_addr_set_status->setText(tr("Set OK!"));
    dali_1_all->setText(tr("CH1 All"));
    dali_1_new->setText(tr("CH1 Test"));
    dali_2_all->setText(tr("CH2 All"));
    dali_2_new->setText(tr("CH2 Test"));

    fac_title->setText(tr("Pannel ID : "));
    fac_status->setText(tr("Reseting"));
    fac_yes->setText(tr("Factory Reset"));
    

	program_button->setText(tr("Program"));
	log_button->setText(tr("Log On"));
	ets_title->setText(tr("ETS Program Set : "));

    bootmode_title->setText(tr("Boot Mode :"));
	ios_button->setText(tr("IOS")); 
	and_button->setText(tr("Andriod")); 
	//setcode_yes->setText(tr("Yes"));

	english_button->setText(tr("English"));
	chinese_button->setText(tr("Zh-Hans"));
	tr_cn_button->setText(tr("Zh-Hant"));
	language_title->setText(tr("Language Set : "));

	about_title->setText(tr("About : "));
	model_status->setText(tr("Model   : Pannos 2"));
	version_status->setText(tr("Version : 01A01W01"));

	net_wac_title->setText(tr("Net ReSet : "));
	identify_title->setText(tr("Lights identify : "));


	statrt_identify_button->setText(tr("start"));
	stop_identify_button->setText(tr("stop"));
	previous_light_button->setText(tr("previous"));
	next_light_button->setText(tr("next"));

	
	dali_reconfiguration_title->setText(tr("dali reconfiguration"));
	dali_reconfiguration_button->setText(tr("send"));
	dali_reconfiguration_title->setText(tr("dali reconfiguration"));


	facreset_sure_title->setText(tr("Factory Reset"));

	facreset_sure_button->setText(tr("sure"));
	facreset_cancel_button->setText(tr("cancel"));


	dali_1_addr_reconfiguration_title->setText(tr("reassign dali address"));
	dali_2_addr_reconfiguration_title->setText(tr("reassign dali address"));


	dali_1_addr_reconfiguration_sure_button->setText(tr("sure"));
	dali_1_addr_reconfiguration_cancel_button->setText(tr("cancel"));

	dali_2_addr_reconfiguration_sure_button->setText(tr("sure"));
	dali_2_addr_reconfiguration_cancel_button->setText(tr("cancel"));
    //
    label_1->setText(tr("status:"));
    ret = enno_read_uint_from_file(&val,CFG_CHILD_LOCK_FILE);
	b_child_lock_on = false;
    if(!ret)
    {
       if(val)
       	{
           label_2->setText(tr("enable"));
		   b_child_lock_on = true;
       	}
       else
           label_2->setText(tr("disable"));
    }
    else
        label_2->setText(tr("disable"));
	pbtn_1->setText(tr("enable"));
	pbtn_2->setText(tr("disable"));

	
	g_thread->msg_get_device_info();
}

void SetAboutWidget::show_net_wac_state()
{
	switch(net_state)
	{
	case EN_NET_UNCONNECT:
		net_wac_status->setText(tr("Unconnect"));
		net_wac_status->setStyleSheet("color:rgb(255,61,16);" \
				"text-align:left;background-color:black;");
		break;
		
	case EN_NET_UNPAIRED:
		net_wac_status->setText(tr("Unpaired"));
		net_wac_status->setStyleSheet("color:rgb(255,61,16);" \
				"text-align:left;background-color:black;");
		break;

	case EN_NET_PAIRED:
		net_wac_status->setText(tr("Paired"));
		net_wac_status->setStyleSheet("color:rgb(123,226,80);" \
				"text-align:left;background-color:black;");
		break;

	case EN_NET_CONNECTED:
		net_wac_status->setText(tr("Connected"));
		net_wac_status->setStyleSheet("color:rgb(123,226,80);" \
				"text-align:left;background-color:black;");
		break;

	case EN_NET_FIXED:
		net_wac_status->setText(tr("Reset"));
		net_wac_status->setStyleSheet("color:rgb(123,226,80);" \
				"text-align:left;background-color:black;");
		break;

	default:
		break;
	}
}

void SetAboutWidget::show_net_state()
{
    
    switch(net_state)
	{
	case EN_NET_UNCONNECT:
        netfix_status->setText(tr("Unconnect"));
		netfix_status->setStyleSheet("color:rgb(255,61,16);" \
                "text-align:left;background-color:black;");
		break;
		
	case EN_NET_UNPAIRED:
        netfix_status->setText(tr("Unpaired"));
		netfix_status->setStyleSheet("color:rgb(255,61,16);" \
                "text-align:left;background-color:black;");
		break;

	case EN_NET_PAIRED:
        netfix_status->setText(tr("Paired"));
		netfix_status->setStyleSheet("color:rgb(123,226,80);" \
                "text-align:left;background-color:black;");
		break;

	case EN_NET_CONNECTED:
        netfix_status->setText(tr("Connected"));
		netfix_status->setStyleSheet("color:rgb(123,226,80);" \
                "text-align:left;background-color:black;");
		break;

	case EN_NET_FIXED:
        netfix_status->setText(tr("Fixed"));
		netfix_status->setStyleSheet("color:rgb(123,226,80);" \
                "text-align:left;background-color:black;");
		break;

	default:
		break;
	}
}

void SetAboutWidget::get_net_state(char *value, uint32 len_buf)
{
	enno_result ret;
    char *buf = value;
	uint32 len = len_buf;
	uint32 len_use;

	if (NULL == value)
	{
	   qDebug()<<"qterr, get_dev_info, value null";
	   return;
	}

    ret = enno_decode_uint(buf, (uint32*)&net_state, &len_use, len);
    if (ret)
	{
	    return;
    }

	if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateSetup((unsigned int)net_state);
	}

	wac_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	reboot_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	netfix_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");

	show_net_state();
	show_net_wac_state();
}

void SetAboutWidget::get_dev_info(char *value, uint32 len_buf)
{
    enno_result ret;
    char *buf = value;
	uint32 len = len_buf;
	uint32 len_use;
	
    if (NULL == value)
	{
	    qDebug()<<"qterr, get_dev_info, value null";
	    return;
	}

	ret = enno_decode_str(buf, device_id, &len_use, len);
	if (ret)
	{
	    qDebug()<<"qterr, enno_decode_str device_id errnum="<<ret;
	    return;
	}
	QString str1("ID:");
	QString str2(device_id);
	label_devid->setText(str1 + str2);

    buf += len_use;
	len -= len_use;
	ret = enno_decode_str(buf, device_version, &len_use, len);
	if (ret)
	{
	    qDebug()<<"qterr, enno_decode_str device_id errver="<<ret;
	    return;
	}
	QString str3(tr("Version : "));
	QString str4(device_version);
	str4 = str4.trimmed();
    version_status->setText(str3 + str4);    
	
}

void SetAboutWidget::get_ssid_info(char *value, uint32 len_buf)
{
    enno_result ret;
    char *buf = value;
	uint32 len = len_buf;
	uint32 len_use;
	uint32 rssi;
	int i=0;
	char *buf1 = value;
	
	qDebug()<<"get_ssid_info";
    if (NULL == value)
	{
	    qDebug()<<"qterr, get_dev_info, value null";
	    return;
	}



	ret = enno_decode_str(buf, (char *)ssid_name, &len_use, len);
	if (ret)
	{
	    qDebug()<<"qterr, enno_decode_str device_id errnum="<<ret;
	    return;
	}
	 buf += len_use;
	 len -= len_use;
	 

	ret = enno_decode_str(buf, (char *)ip_name, &len_use, len);
	if (ret)
	{
	 	ip_name[0] = 0;
	}
	else
	{
		buf += len_use;
		len -= len_use;
		ret = enno_decode_uint(buf, &rssi, &len_use, len);
		if (ret)
		{
		    qDebug()<<"qterr, enno_decode_str device_id errnum="<<ret;
		    return;
		}

		snprintf(db_name,16,"-%d db",rssi);
	}


	QString str1(ssid_name);
	qDebug()<<"set ssid="<<str1;
	if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateSsid(ssid_name);
	}
    labelNetworkName->setText(str1);    


    QString strip(ip_name);
    qDebug()<<"set ip="<<strip;
	if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updateip(ip_name);
	}
    labelNetipName->setText(strip);

    QString strdb(db_name);
    qDebug()<<"set wifi db="<<strdb;
	if( g_statusXmlHandle
		&&!g_statusXmlHandle->getReadOut())
	{
	    g_statusXmlHandle->updatedb(db_name);
	}
    labelNetdbName->setText(strdb);
   
    
}
void SetAboutWidget::get_dali_info(char *value, uint32 len_buf)
{
    uint32 channel;
	uint32 len_use;
	uint32 num;
	uint32 cmd_type;
	uint32 start_addr = 0;
	uint32 end_addr = 0;
	char *buf = value;

    enno_decode_uint(buf, &channel, &len_use, len_buf);

	buf += len_use;
	len_buf -= len_use;
    enno_decode_uint(buf, &num, &len_use, len_buf);

	buf += len_use;
	len_buf -= len_use;
    enno_decode_uint(buf, &cmd_type, &len_use, len_buf);

	buf += len_use;
	len_buf -= len_use;
    enno_decode_uint(buf, &start_addr, &len_use, len_buf);

	buf += len_use;
	len_buf -= len_use;
    enno_decode_uint(buf, &end_addr, &len_use, len_buf);

	//g_dali_set_flag = 0;

    if(cmd_type == 1)//all
    {
        g_dali_set_flag = 0;
    	if(num<=0)
    	{
    		dali_addr_set_status->setText((tr("CH%1=0") .arg(channel+1)));
    	}
    	else
    	{
    		dali_addr_set_status->setText((tr("CH%1=%2, addr form %3 to %4") .arg(channel+1) .arg(num) .arg(start_addr) .arg(end_addr)));
    	}
    }
    else if(cmd_type == 2)//new
	{
        g_dali_set_flag = 0;
    	dali_addr_set_status->setText((tr("CH%1 add %2") .arg(channel+1) .arg(num)));
	}
	else if(cmd_type == 3)//single dali addr show 
	{	   
		dali_addr_set_status->setText((tr("CH%1 new add %2") .arg(channel+1) .arg(start_addr))); 
	}



}

void SetAboutWidget::setfunc(QString current_icon)
{
	bool ok;  
	char buf[10];
    int current_index = current_icon.toInt(&ok, 10);
    QPushButton *push_button;

	if (0 == current_index)
	{
	    //printf("button %d on\n", current_index);
	}
	/*for (int i = 0; i < button_num; i++)
	{
		push_button = button_list.at(i);
	    if (i != 0)
	    {
	        push_button->setStyleSheet("font-size:28px;color:rgb(200,200,200);font-weight:bold;text-align:left;");
	    }
		else
		{
		    push_button->setStyleSheet("font-size:25px;color:rgb(200,200,200);font-weight:bold;text-align:left;");
		}
	}
	*/
	switch(current_index)
	{
	case 0: //about
	    //printf("about\n");
		break;
		
	case 1:
	    //printf("set language\n");
		break;
		
	case 2:
		//printf("Souftware Update\n");
		break;

	case 3:
	    //printf("reset");
		break;

	default:
		//printf("unknown msg=%d\n", current_index);
		break;
	}
	
}

void SetAboutWidget::doSetMsg(int icon_id)
{
    if(icon_id == 0)
    {
        labelNetworkName->setParent(wid_netfix);
        //labelNetStatusHead->setParent(wid_netfix);
    }
    else if(icon_id==1)
    {
    	 //  labelNetworkName->setParent(wid_daliset);
         //labelNetStatusHead->setParent(wid_netreset);
    }


    if(icon_id == 2)
    {
        label_devid->setParent(wid_facreset);
        fac_title->setParent(wid_facreset);
        //fac_status->setStyleSheet("font-size:25px;color:rgb(200,200,200);" \
        //            "text-align:left;background-color:black;");
    }
    else if(icon_id == 3)
    {
        label_devid->setParent(wid_etsset);
        fac_title->setParent(wid_etsset);
    }
    else if(icon_id == 4)
    {
        label_devid->setParent(wid_bootmode);
        fac_title->setParent(wid_bootmode);
    }
    else if(icon_id == 6)
    {
        label_devid->setParent(wid_about);
        fac_title->setParent(wid_about);
    }


    info_widget->setCurrentIndex(icon_id);
	#if 0
    switch(icon_id)
    {
    case EN_SET_NETWORK_FIX:
		qDebug()<<"doSetMsg NetWork Fix";
		QMessageBox::information(this, "NetWork Fix", "are you sure?", 
			 QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		break;
		
	case EN_SET_NET_RESET:
		
		break;
		
	case EN_SET_FACTORY_RESET:
		
		break;
		
	case EN_SET_BOOT_MODE:
		break;
		
	case EN_SET_SETUP_CODE:
		break;
		
	case EN_SET_LANGUAGE:
		break;
		
	case EN_SET_ABOUT:
		break;
		
	default:
		qDebug()<<"doSetMsg unknown item_id="<<icon_id;
		break;
    }
	#endif
}
void SetAboutWidget::netfixClicked()
{
    net_state = EN_NET_FIXED;
	
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<net_state;
	//show_net_state();
	netfix_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_NETWORK_FIX, 0);

	netfix_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");


    timer_countfix_net = 30;
	timer_tick_one->start();
}


void SetAboutWidget::dali_1_all_Clicked()
{
	/*dali_addr_set_status->setVisible(1);
	net_state = EN_NET_FIXED;
	g_dali_set_flag = 1;

	qDebug()<<"set g_dali_set_flag="<<g_dali_set_flag;
	
	dali_addr_set_status->setText("Dali ch1 reset for all.....");
	dali_addr_set_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH1_ADDR_RESET_ALL);
*/

	
	info_widget->setCurrentWidget(wid_dali_1_addr_reconfiguration_sure);
	//info_widget->setCurrentIndex(10);


    
}


static int test_cnt =1;

void SetAboutWidget::dali_1_new_Clicked()
{

	if(test_cnt)
	{
		dali_1_new->setStyleSheet("font-size:30px;color:white;" \
					"text-align:center;background-color:rgb(155,155,155);border-radius:30px;");//font-weight:bold;
		test_cnt = 0;
	}
	else
	{
		dali_1_new->setStyleSheet("font-size:30px;color:white;" \
					"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
		test_cnt = 1;
	}
	
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH1_ADDR_SET_FOR_NEW);
}

void SetAboutWidget::dali_2_all_Clicked()
{

	info_widget->setCurrentWidget(wid_dali_2_addr_reconfiguration_sure);

/*	dali_addr_set_status->setVisible(1);
	net_state = EN_NET_FIXED;
	g_dali_set_flag = 1;

	dali_addr_set_status->setText("Dali ch2 reset for all.....");
	dali_addr_set_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH2_ADDR_RESET_ALL);
    */
}

void SetAboutWidget::dali_2_new_Clicked()
{
	//dali_addr_set_status->setVisible(1);
	net_state = EN_NET_FIXED;
	//g_dali_set_flag = 1;

	//dali_addr_set_status->setText("Dali ch2 set for new.....");
	//dali_addr_set_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH2_ADDR_SET_FOR_NEW);
}


static int g_dali_dev_id =0;	
static int channel_id =1;
static int g_identify_start =0;


void SetAboutWidget::identify_start_Clicked()
{
	g_identify_start =1;
	
	identify_status->setVisible(1);
	g_dali_dev_id = 0;

	if(channel_id==1)
	{
		identify_status->setText("Dali ch1 idetify id: 0");
		identify_status->setVisible(1);
		g_thread->msg_set_conf(EN_SET_IDENTIFY_CH1, g_dali_dev_id);
	}
	else if(channel_id==2)
	{
		identify_status->setText("Dali ch2 idetify id: 0");
		identify_status->setVisible(1);
		g_thread->msg_set_conf(EN_SET_IDENTIFY_CH2, g_dali_dev_id);
	}	

}

void SetAboutWidget::identify_stop_Clicked()
{
	identify_status->setVisible(1);
	g_identify_start = 0;
	identify_status->setText("Dali idetify stop");
	identify_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_IDENTIFY_STOP, 0);


#if 0 //for P2S
    if(channel_id==1)
    	channel_id = 2;
    else if(channel_id==2)
    	channel_id = 1;
#endif
    	

}

void SetAboutWidget::identify_pre_Clicked()
{
	identify_status->setVisible(1);

	if(g_dali_dev_id>=1 && g_dali_dev_id<64 && g_identify_start==1)
	{
		g_dali_dev_id --;
		if(channel_id==1)
		{
			identify_status->setText((tr("Dali ch1 idetify id: %1") .arg(g_dali_dev_id)));
			identify_status->setVisible(1);
			g_thread->msg_set_conf(EN_SET_IDENTIFY_CH1, g_dali_dev_id);
		}	
		else if(channel_id ==2)
		{
			identify_status->setText((tr("Dali ch2 idetify id: %1") .arg(g_dali_dev_id)));
			identify_status->setVisible(1);
			g_thread->msg_set_conf(EN_SET_IDENTIFY_CH2, g_dali_dev_id);
		}
	}
	
}

void SetAboutWidget::identify_next_Clicked()
{
	if(g_dali_dev_id>=0 && g_dali_dev_id<63 && g_identify_start==1)
	{
		g_dali_dev_id ++;
		if(channel_id==1)
		{
			identify_status->setText((tr("Dali ch1 idetify id: %1") .arg(g_dali_dev_id)));
			identify_status->setVisible(1);
			g_thread->msg_set_conf(EN_SET_IDENTIFY_CH1, g_dali_dev_id);
		}	
		else if(channel_id ==2)
		{
			identify_status->setText((tr("Dali ch2 idetify id: %1") .arg(g_dali_dev_id)));
			identify_status->setVisible(1);
			g_thread->msg_set_conf(EN_SET_IDENTIFY_CH2, g_dali_dev_id);
		}
	}

}

void SetAboutWidget::dali_reconfiguration_Clicked()
{
	dali_reconfiguration_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(155,155,155);border-radius:30px;");//font-weight:bold;

	reconfiguration_status->setVisible(1);
	g_thread->msg_set_conf(EN_SET_DALI_RECONFIGURATION, 0);
	
}


void SetAboutWidget::dali_1_addr_reconfiguration_sure_Clicked()
{

	info_widget->setCurrentWidget(wid_daliset);
	
	dali_addr_set_status->setVisible(1);
	net_state = EN_NET_FIXED;
	g_dali_set_flag = 1;

	qDebug()<<"set g_dali_set_flag="<<g_dali_set_flag;
	
	dali_addr_set_status->setText("Dali ch1 reset for all.....");
	dali_addr_set_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH1_ADDR_RESET_ALL);	
}
void SetAboutWidget::dali_1_addr_reconfiguration_cancel_Clicked()
{
	dali_addr_set_status->setVisible(0);
	info_widget->setCurrentWidget(wid_daliset);
	
}
void SetAboutWidget::dali_2_addr_reconfiguration_sure_Clicked()
{
	info_widget->setCurrentWidget(wid_daliset);

	dali_addr_set_status->setVisible(1);
	net_state = EN_NET_FIXED;
	g_dali_set_flag = 1;

	dali_addr_set_status->setText("Dali ch2 reset for all.....");
	dali_addr_set_status->setVisible(1);
    g_thread->msg_set_conf(EN_SET_DALI_ADDR, DALI_CH2_ADDR_RESET_ALL);	
    
}
void SetAboutWidget::dali_2_addr_reconfiguration_cancel_Clicked()
{
	dali_addr_set_status->setVisible(0);
	info_widget->setCurrentWidget(wid_daliset);
	
}

void SetAboutWidget::facreset_sure_Clicked()
{
	info_widget->setCurrentWidget(wid_facreset);

    fac_status->setVisible(1);
	fac_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
    //g_thread->msg_set_conf(EN_SET_FACTORY_RESET, 0);

    timer_count = DEF_RESET_TIME;
	timer_tick_one->start();

    
}
void SetAboutWidget::facreset_cancel_Clicked()
{
	fac_status->setVisible(0);
	info_widget->setCurrentWidget(wid_facreset);
	
}


void SetAboutWidget::chineseClicked()
{
	emit changeLanguage(1);

	chinese_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
	english_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	tr_cn_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	
}

void SetAboutWidget::englishClicked()
{
	emit changeLanguage(0);
	chinese_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	tr_cn_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	english_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
}

void SetAboutWidget::trcnClicked()
{
	emit changeLanguage(2);
	chinese_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	tr_cn_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
	english_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
}

void SetAboutWidget::wacClicked()
{
	net_state = EN_NET_FIXED;
		
	show_net_wac_state();
	//netfix_status->setVisible(1);
	g_thread->msg_set_conf(EN_SET_WAC_RESET, 0);

	wac_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");

}

void SetAboutWidget::netrebootClicked()
{
	net_state = EN_NET_FIXED;
		
	show_net_wac_state();
	//netfix_status->setVisible(1);
	g_thread->msg_set_conf(EN_SET_NETWORK_FIX, 0);

	reboot_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");

}

void SetAboutWidget::programClicked()
{
	//todo
	g_thread->msg_set_conf(EN_SET_ETS_MODE, 0);

	program_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
	log_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
}


extern int g_fatocry_test_mode;

void SetAboutWidget::logClicked()
{
    //qDebug()<<"andClicked";
    system("rm /home/user/conf/pannos_hktbak.conf");
	system("cp /home/user/conf/pannos_hkt.conf /home/user/conf/pannos_hktbak.conf");
	usleep(20000);
	system("sync");
    //todo 
    g_thread->msg_set_conf(EN_SET_ETS_MODE, 1);
	g_fatocry_test_mode = 1;
   

   /* 
	ios_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	and_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
                */
}

void SetAboutWidget::iosClicked()
{

#if 0 //mask by iovine
    g_thread->msg_set_conf(EN_SET_BOOT_MODE, 0);
	system("echo 0 > /home/user/conf/boot_mode");

	ios_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
	and_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
#endif
}

void SetAboutWidget::andClicked()
{
#if 0 //mask by iovine
	system("echo 1 > /home/user/conf/boot_mode");

    g_thread->msg_set_conf(EN_SET_BOOT_MODE, 1);
	ios_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
	and_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
#endif
}

void SetAboutWidget::setcodeClicked()
{
    char tmp[20];   
    //todo
   
	//set_status->setText(g_pairid);
	/*if (qr_widget != NULL)
	{
	    //qr_widget->GetSetupCode(tmp);
		//set_status->setText(tmp);
		emit showSetupCode();
	}*/

	//set_status->setVisible(1);
}

void SetAboutWidget::facClicked()
{
/*
    fac_status->setVisible(1);
	fac_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(52,204,255);border-radius:35px;");
    g_thread->msg_set_conf(EN_SET_FACTORY_RESET, 0);

    timer_count = 35;
	timer_tick_one->start();
	*/
	info_widget->setCurrentWidget(wid_facreset_sure);

	
}

void SetAboutWidget::retClicked()
{
    //qDebug()<<"retClicked";
	syslog(LOG_INFO|LOG_LOCAL1, "retClicked\n");
	netfix_status->setVisible(1);
	dali_addr_set_status->setVisible(0);
//	set_status->setVisible(0);

    //2023-08-11 set default-info
	info_widget->setCurrentWidget(wid_about);
    
    fac_status->setVisible(0);

	g_dali_set_flag = 0;

	emit retMainwid();
}

void SetAboutWidget::quitSet()
{
	netfix_status->setVisible(1);
	dali_addr_set_status->setVisible(0);
	//set_status->setVisible(0);
    fac_status->setVisible(0);
}

void SetAboutWidget::onTimerTaskSet()
{
    
    if(timer_count == 1)
    {
        fac_status->setText(tr("Reset OK!"));
        fac_status->setStyleSheet("font-size:25px;color:rgb(123,226,80);" \
                    "text-align:left;background-color:black;");
		fac_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");
        timer_tick_one->stop();

		if(g_statusXmlHandle)
		{
		    g_statusXmlHandle->init_status();
		}
    }

	if (timer_count == DEF_RESET_TIME - 5)
	{
	    if (info_widget->currentWidget() == wid_facreset)
	   	{
		    g_thread->msg_set_conf(EN_SET_FACTORY_RESET, 0);
			ENNO_LOG_ERR("send factory reset cmd\n");
	    }
		else
		{
		    ENNO_LOG_ERR("current widget=%d, err no reset\n", info_widget->currentIndex());
			timer_count = 1;
			timer_tick_one->stop();
		}
	}

	timer_count--;
	switch (timer_count%5)
	{
    case 0:
        fac_status->setText(tr("Reseting....."));
		break;
		
	case 1:
        fac_status->setText(tr("Reseting...."));
		break;
		
	case 2:
        fac_status->setText(tr("Reseting..."));
		break;
		
	case 3:
        fac_status->setText(tr("Reseting.."));
		break;
		
	case 4:
        fac_status->setText(tr("Reseting."));
		break;
	}

	
    if(timer_countfix_net == 1)
    {
        timer_tick_one->stop();
        if(g_statusXmlHandle)
		{
		    g_statusXmlHandle->init_status();
		}
    }

    
	

	timer_countfix_net--;
	
	switch (timer_countfix_net%5)
	{
    case 0:
        netfix_status->setText(tr("....."));
		break;
		
	case 1:
        netfix_status->setText(tr("...."));
		break;
		
	case 2:
        netfix_status->setText(tr("..."));
		break;
		
	case 3:
        netfix_status->setText(tr(".."));
		break;
		
	case 4:
        netfix_status->setText(tr("."));
		break;

	default:
		break;
	}
}

void SetAboutWidget::on_pbtn_1_clicked()
{
    pbtn_1->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(52,204,255);border-radius:30px;");
    pbtn_2->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");
	//
	b_child_lock_on = true;
    //
    label_2->setText(tr("enable"));
    system("echo 1 > /home/user/conf/child_lock");
    system("sync");
}

void SetAboutWidget::on_pbtn_2_clicked()
{
    pbtn_2->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(52,204,255);border-radius:30px;");
    pbtn_1->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");
	//
	b_child_lock_on = false;
    //
    label_2->setText(tr("disable"));
    system("echo 0 > /home/user/conf/child_lock");
    system("sync");
}
SetAboutWidget::SetAboutWidget(QWidget *parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(320,320);
	this->setMinimumSize(320,320);
	QStringList string_list;
	string_list<<"ID: 1234-1234-1234-1234";//<<"network: connect";
		//<<"Version: 01A01W01"<<"Setup Code: 11-222-33";
    //string_list<<"ID: 1234-1234-1234-1234"<<"model: pannos 2"<<"Version: 01A01W01"<<"Setup Code: 11-222-33";
	
	//QVBoxLayout *v_layout = new QVBoxLayout();
    QSignalMapper *signal_mapper = new QSignalMapper(this);
	button_num = string_list.size();
	
	qr_widget = NULL;
    strNetworkName = " ";//this string should be got dynamically

	//info_widget = new QWidget(this);

	info_widget = new QStackedWidget(this);
	info_widget->setFixedSize(320,300);
	info_widget->raise();

    //QWidget *ret_widget = new QWidget(this);
   // ret_widget->setFixedSize(68,64);

	label_devid = new QLabel(this);
    label_devid->setText("1234-1234-1234-1234");
    label_devid->setGeometry(QRect(0, 42, 320, 320));
    label_devid->setFixedSize(320, 40);
	//label_devid->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;");//font-weight:bold;
    label_devid->setObjectName("setAboutLabel");
	label_devid->setVisible(false);
    #if 0
	for(int i=0; i<string_list.size(); i++)
	{
		QPushButton *push_button = new QPushButton(this);
		push_button->setText(string_list.at(i));
		//push_button->setObjectName("functionButton");
		
		push_button->setGeometry(QRect(i*80, 10, 320, 320));
		if (i != 0)
		{
		    push_button->setFixedSize(300, 200);
			push_button->setStyleSheet("font-size:30px;color:rgb(200,200,200);" \
				"font-weight:bold;text-align:left;background-color:rgb(99,99,99);");
		}
		else
		{
		    push_button->setFixedSize(300, 45);
		    push_button->setStyleSheet("font-size:25px;color:rgb(200,200,200);" \
				"font-weight:bold;text-align:left;");
		}
		button_list.append(push_button);
		connect(push_button, SIGNAL(clicked()), signal_mapper, SLOT(map()));
	  //  v_layout->addWidget(push_button);
		signal_mapper->setMapping(push_button, QString::number(i, 10));
		//connect(tool_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	}
	
	connect(signal_mapper, SIGNAL(mapped(QString)), this, SLOT(setfunc(QString)));
	#endif
	
#if 0
	set_content = new QLabel("Net Work:");
	set_content->setText("Net Work: connect");
	set_content->setFixedSize(300, 200);
	set_content->setStyleSheet("font-size:30px;color:rgb(200,200,200);" \
				"font-weight:bold;text-align:left;background-color:rgb(99,99,99);");
    v_layout->addWidget(set_content);
#endif
    //10+3+1.
    wid_netfix = new QWidget();
    wid_daliset = new QWidget();
    wid_facreset = new QWidget();
    wid_etsset = new QWidget();
    wid_bootmode = new QWidget();
    wid_language = new QWidget();
    wid_about = new QWidget();
	wid_netreset = new QWidget();
	wid_lights_identify = new QWidget();
	wid_dali_reconfiguration = new QWidget();    
    //
	wid_dali_1_addr_reconfiguration_sure = new QWidget();
	wid_dali_2_addr_reconfiguration_sure = new QWidget();
	wid_facreset_sure = new QWidget();
    //
    wid_child_lock = new QWidget();
	


	

    //0.network fix
    netfix_title = new QLabel(wid_netfix); //tr("current network : ")
    netfix_title->setText(tr("Current Network :"));
   // netfix_title->setStyleSheet("font-size:25px;color:white;" \
	//				"text-align:left;background-color:black;");
    netfix_status = new QLabel(tr("Status: Fixed"), wid_netfix);
	
    netfix_yes = new QPushButton(tr("Start Fix"), wid_netfix);
	netfix_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
    netfix_status->setStyleSheet("font-size:30px;color:white;" \
                "text-align:left;background-color:black;");
    netfix_title->setGeometry(QRect(0, 0, 320, 40));
    netfix_yes->setGeometry(QRect(0, 180, 320, UP_SET_BTN_HEIGHT));
    netfix_status->setGeometry(QRect(160, 126, 160, 40));
	netfix_status->setVisible(1);
    netfix_yes->setVisible(true);
    netfix_title->setVisible(true);
	netfix_title->setObjectName("setAboutLabel");
	netfix_status->setObjectName("setAboutLabel");

    labelNetworkName = new QLabel(strNetworkName,wid_netfix);
    labelNetworkName->setGeometry(QRect(0, 42, 320, 40));
    //labelNetworkName->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");
	labelNetworkName->setObjectName("setAboutLabel");


	labelNetipName = new QLabel(strNetipName,wid_netfix);
	labelNetipName->setGeometry(QRect(0, 84, 230, 40));
	labelNetipName->setObjectName("setAboutLabel");

	
	labelNetdbName = new QLabel(strNetdbName,wid_netfix);
	labelNetdbName->setGeometry(QRect(230, 84, 90, 40));
	labelNetdbName->setObjectName("setAboutLabel");
		

	if (g_statusXmlHandle)
		g_statusXmlHandle->getSsid(ssid_name);
    QString str_ssid(ssid_name);
	labelNetworkName->setText(str_ssid);  


	if (g_statusXmlHandle)
		g_statusXmlHandle->getip(ip_name);
    QString str_ip(ip_name);
	labelNetipName->setText(str_ip);  	

	if (g_statusXmlHandle)
		g_statusXmlHandle->getdb(db_name);
    QString str_db(db_name);
	labelNetdbName->setText(str_db); 

	//net_state = EN_NET_UNCONNECT;
	//unsigned int openBitMap;
	if (g_statusXmlHandle)
	    g_statusXmlHandle->getSetup((unsigned int *)&net_state);
	
    labelNetStatusHead = new QLabel(tr("Net Status:"),wid_netfix);
    labelNetStatusHead->setGeometry(QRect(0, 126, 160, 40));
    //labelNetStatusHead->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");
	labelNetStatusHead->setObjectName("setAboutLabel");
	//qDebug()<<"setenstate,func="<<__func__<<",line="<<__LINE__<<",state="<<net_state;
	show_net_state();
    connect(netfix_yes, SIGNAL(clicked()), this, SLOT(netfixClicked()));

	//1.dali_addr_set_title
    dali_addr_set_title = new QLabel(tr("Dali Address Set :"), wid_daliset);
    //netreset_title->setStyleSheet("font-size:25px;color:white;" \
	//				"text-align:left;background-color:black;");
    dali_addr_set_title->setObjectName("setAboutLabel");
	dali_addr_set_status = new QLabel(tr("Set OK!"), wid_daliset);
	dali_addr_set_status->setObjectName("setAboutLabel");
	
	dali_1_all = new QPushButton(tr("CH1 All"), wid_daliset);
	dali_1_all->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_1_all->setGeometry(QRect(0, 100, 150, 60));

	dali_1_new = new QPushButton(tr("CH1 Test"), wid_daliset);
	dali_1_new->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_1_new->setGeometry(QRect(160, 100, 150, 60));



	dali_2_all = new QPushButton(tr("CH2 All"), wid_daliset);
	dali_2_all->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_2_all->setGeometry(QRect(0, 175, 150, 60));
	
	dali_2_new = new QPushButton(tr("CH2 Test"), wid_daliset);
	dali_2_new->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_2_new->setGeometry(QRect(160, 175, 150, 60));



	dali_addr_set_title->setGeometry(QRect(0, 0, 320, 40));
    dali_addr_set_status->setGeometry(QRect(0, 50, 320, 40));
    dali_addr_set_status->setVisible(0);
    connect(dali_1_all, SIGNAL(clicked()), this, SLOT(dali_1_all_Clicked()));

    
    connect(dali_1_new, SIGNAL(clicked()), this, SLOT(dali_1_new_Clicked()));
    connect(dali_2_all, SIGNAL(clicked()), this, SLOT(dali_2_all_Clicked()));
    connect(dali_2_new, SIGNAL(clicked()), this, SLOT(dali_2_new_Clicked()));
	
	//2.factory reset
    fac_title = new QLabel(tr("Pannel ID : "), wid_facreset);
    //fac_title->setStyleSheet("font-size:25px;color:white;" \
	//				"text-align:left;background-color:black;");
	fac_title->setObjectName("setAboutLabel");
    fac_status = new QLabel(tr("Start Reset"), wid_facreset);
	
    fac_yes = new QPushButton(tr("Factory Reset"), wid_facreset);
	fac_yes->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
    //fac_status->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");
	fac_status->setObjectName("setAboutLabel");
    fac_title->setGeometry(QRect(0, 0, 320, 40));
    fac_yes->setGeometry(QRect(0, 164, 320, UP_SET_BTN_HEIGHT));
    fac_status->setGeometry(QRect(0, 82, 320, 40));
	fac_status->setVisible(0);

    label_devid->setParent(wid_facreset);
    label_devid->setVisible(true);
    connect(fac_yes, SIGNAL(clicked()), this, SLOT(facClicked()));

    //3.boot mode
	program_button = new QPushButton(wid_etsset);
	log_button = new QPushButton(wid_etsset);
    program_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	log_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	program_button->setText(tr("Program")); 
	log_button->setText(tr("LogOn"));  
    ets_title = new QLabel(tr("ETS Program Set : "), wid_etsset); 
    //ets_title->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");
	ets_title->setObjectName("setAboutLabel");
    ets_title->setGeometry(QRect(0, 120, 320, 40));
    program_button->setGeometry(QRect(10, 164, 140, UP_SET_BTN_HEIGHT));
    log_button->setGeometry(QRect(171, 164, 140, UP_SET_BTN_HEIGHT));
    connect(program_button, SIGNAL(clicked()), this, SLOT(programClicked()));
    connect(log_button, SIGNAL(clicked()), this, SLOT(logClicked()));

    //4.bootmode
    bootmode_title = new QLabel(tr("Boot Mode :"), wid_bootmode);
    //setcode_title->setStyleSheet("font-size:25px;color:white;" \
	//				"text-align:left;background-color:black;");
	bootmode_title->setObjectName("setAboutLabel");
	//set_status = new QLabel("111-22-333", wid_bootmode); 
	
	ios_button = new QPushButton(wid_bootmode);
	and_button = new QPushButton(wid_bootmode);
    ios_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	and_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	ios_button->setText(tr("IOS")); 
	and_button->setText(tr("Andriod")); 

   // set_status->setStyleSheet("font-size:30px;color:white;" \
   //             "text-align:center;background-color:rgb(85,85,85);border-radius:35px;font-weight:bold;");
    bootmode_title->setGeometry(QRect(0, 120, 320, 40));
	//setcode_yes->setGeometry(QRect(200, 20, 80, UP_SET_BTN_HEIGHT));
	ios_button->setGeometry(QRect(10, 164, 140, UP_SET_BTN_HEIGHT));
    and_button->setGeometry(QRect(171, 164, 140, UP_SET_BTN_HEIGHT));
  //  set_status->setGeometry(QRect(0, 164, 320, UP_SET_BTN_HEIGHT));
    //set_status->setVisible(1);
  //  set_status->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
   // setcode_yes->setVisible(false);
   // connect(setcode_yes, SIGNAL(clicked()), this, SLOT(setcodeClicked()));
    connect(ios_button, SIGNAL(clicked()), this, SLOT(iosClicked()));
    connect(and_button, SIGNAL(clicked()), this, SLOT(andClicked()));
	
    //5.language
	chinese_button = new QPushButton(wid_language);
	english_button = new QPushButton(wid_language);
	tr_cn_button = new QPushButton(wid_language);
	
	chinese_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	english_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	tr_cn_button->setStyleSheet("font-size:30px;color:white;" \
					"text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;

				
	english_button->setText(tr("English"));
	chinese_button->setText(tr("zh-Hans"));
	tr_cn_button->setText(tr("zh-Hant"));
    language_title = new QLabel(tr("Language Set : "), wid_language); 
    language_title->setStyleSheet("font-size:25px;color:white;" \
				"text-align:left;background-color:black;");
	language_title->setObjectName("setAboutLabel");
    language_title->setGeometry(QRect(0, 0, 320, 30));
    english_button->setGeometry(QRect(0, 60, 320, UP_SET_BTN_HEIGHT));
    chinese_button->setGeometry(QRect(0, 140, 320, UP_SET_BTN_HEIGHT));
	tr_cn_button->setGeometry(QRect(0, 220, 320, UP_SET_BTN_HEIGHT));
    connect(chinese_button, SIGNAL(clicked()), this, SLOT(chineseClicked()));
    connect(english_button, SIGNAL(clicked()), this, SLOT(englishClicked()));
    connect(tr_cn_button, SIGNAL(clicked()), this, SLOT(trcnClicked()));

	//6.about
	about_title = new QLabel(tr("About : "), wid_about); 
	about_title->setStyleSheet("font-size:30px;color:white;" \
					"text-align:left;background-color:black;");
	model_status = new QLabel(tr("Model   : Pannos 2"), wid_about); 
	version_status = new QLabel(tr("Version : 01A01W01"), wid_about); 
    //model_status->setStyleSheet("font-size:25px;color:white;" \
   //             "text-align:left;background-color:black;");
    //version_status->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");//background-color:rgb(99,99,99);
    model_status->setObjectName("setAboutLabel");
	version_status->setObjectName("setAboutLabel");
    about_title->setGeometry(QRect(0, 0, 160, 40));
    model_status->setGeometry(QRect(0, 84, 320, 40));
    version_status->setGeometry(QRect(0, 124, 320, 40));
    about_title->setVisible(false);


	//7.wac reset
	wac_button = new QPushButton(wid_netreset);
	reboot_button = new QPushButton(wid_netreset);
	wac_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	reboot_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:35px;");//font-weight:bold;
	wac_button->setText(tr("NetReset"));
	reboot_button->setText(tr("NetReboot"));
    net_wac_title = new QLabel(tr("Net ReSet : "), wid_netreset); 
    //language_title->setStyleSheet("font-size:25px;color:white;" \
	//			"text-align:left;background-color:black;");
	net_wac_title->setObjectName("setAboutLabel");
    net_wac_title->setGeometry(QRect(0, 0, 320, 40));
    wac_button->setGeometry(QRect(10, 164, 140, UP_SET_BTN_HEIGHT));
    reboot_button->setGeometry(QRect(171, 164, 140, UP_SET_BTN_HEIGHT));
    connect(wac_button, SIGNAL(clicked()), this, SLOT(wacClicked()));
    connect(reboot_button, SIGNAL(clicked()), this, SLOT(netrebootClicked()));

	net_wac_status = new QLabel(tr("Status: Reset"), wid_netreset);
    net_wac_status->setStyleSheet("font-size:30px;color:white;" \
                "text-align:left;background-color:black;");
    net_wac_status->setGeometry(QRect(160, 84, 160, 40));
	net_wac_status->setVisible(1);
	net_wac_status->setObjectName("setAboutLabel");

	labelWacStatusHead = new QLabel(tr("Net Status:"),wid_netreset);
    labelWacStatusHead->setGeometry(QRect(0, 126, 160, 40));
    //labelNetStatusHead->setStyleSheet("font-size:25px;color:white;" \
    //            "text-align:left;background-color:black;");
	labelWacStatusHead->setObjectName("setAboutLabel");


	

	//8.lights_identify
	statrt_identify_button = new QPushButton(wid_lights_identify);
	stop_identify_button = new QPushButton(wid_lights_identify);
	next_light_button = new QPushButton(wid_lights_identify);
	previous_light_button = new QPushButton(wid_lights_identify);


                
	statrt_identify_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	stop_identify_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;

	next_light_button->setStyleSheet("font-size:30px;color:white;" \
							"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	previous_light_button->setStyleSheet("font-size:30px;color:white;" \
							"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;

                
	statrt_identify_button->setText(tr("start"));
	stop_identify_button->setText(tr("stop"));
	previous_light_button->setText(tr("previous"));
	next_light_button->setText(tr("next"));

	
    identify_title = new QLabel(tr("Lights identify : "), wid_lights_identify); 
    //language_title->setStyleSheet("font-size:25px;color:white;" \
	//			"text-align:left;background-color:black;");
	identify_title->setObjectName("setAboutLabel");
    identify_title->setGeometry(QRect(0, 0, 320, 40));

    identify_status = new QLabel(tr("current identify id: "), wid_lights_identify);
	identify_status->setObjectName("setAboutLabel");
	identify_status->setGeometry(QRect(0, 50, 320, 40));
    identify_status->setVisible(0);


	statrt_identify_button->setGeometry(QRect(0, 100, 150, 60));
	stop_identify_button->setGeometry(QRect(0, 175, 150, 60));

	previous_light_button->setGeometry(QRect(160, 100, 150, 60));
	next_light_button->setGeometry(QRect(160, 175, 150, 60));

    
    connect(statrt_identify_button, SIGNAL(clicked()), this, SLOT(identify_start_Clicked()));
    connect(stop_identify_button, SIGNAL(clicked()), this, SLOT(identify_stop_Clicked()));
    connect(previous_light_button, SIGNAL(clicked()), this, SLOT(identify_pre_Clicked()));
    connect(next_light_button, SIGNAL(clicked()), this, SLOT(identify_next_Clicked()));


	//9.lights_reconfiguration 
	dali_reconfiguration_button = new QPushButton(wid_dali_reconfiguration);

                
	dali_reconfiguration_button->setStyleSheet("font-size:30px;color:white;" \
                "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
                
	dali_reconfiguration_button->setText(tr("send"));
	
    dali_reconfiguration_title = new QLabel(tr("dali reconfiguration"), wid_dali_reconfiguration); 
	dali_reconfiguration_title->setObjectName("setAboutLabel");
    dali_reconfiguration_title->setGeometry(QRect(0, 0, 320, 40));


    reconfiguration_status = new QLabel(tr("send ok! Wait one minute"), wid_dali_reconfiguration);
	reconfiguration_status->setObjectName("setAboutLabel");
	reconfiguration_status->setGeometry(QRect(0, 50, 320, 40));
    reconfiguration_status->setVisible(0);

	dali_reconfiguration_button->setGeometry(QRect(0, 100, 150, 60));

    connect(dali_reconfiguration_button, SIGNAL(clicked()), this, SLOT(dali_reconfiguration_Clicked()));

	
	//10.dali 1 addr reconfiguration sure
	dali_1_addr_reconfiguration_sure_button = new QPushButton(wid_dali_1_addr_reconfiguration_sure);
	dali_1_addr_reconfiguration_cancel_button = new QPushButton(wid_dali_1_addr_reconfiguration_sure);
				
	dali_1_addr_reconfiguration_sure_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_1_addr_reconfiguration_cancel_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
				
	dali_1_addr_reconfiguration_sure_button->setText(tr("sure"));
	dali_1_addr_reconfiguration_cancel_button->setText(tr("cancel"));
	
	dali_1_addr_reconfiguration_title = new QLabel(tr("reassign dali address"), wid_dali_1_addr_reconfiguration_sure); 
	dali_1_addr_reconfiguration_title->setObjectName("setAboutLabel");
	dali_1_addr_reconfiguration_title->setGeometry(QRect(5, 60, 300, 60));
	dali_1_addr_reconfiguration_title->setStyleSheet("font-size:30px;color:white;" \
                "text-align:left;background-color:black;");


	dali_1_addr_reconfiguration_sure_button->setGeometry(QRect(0, 150, 150, 60));
	dali_1_addr_reconfiguration_cancel_button->setGeometry(QRect(160, 150, 150, 60));

	
	connect(dali_1_addr_reconfiguration_sure_button, SIGNAL(clicked()), this, SLOT(dali_1_addr_reconfiguration_sure_Clicked()));
	connect(dali_1_addr_reconfiguration_cancel_button, SIGNAL(clicked()), this, SLOT(dali_1_addr_reconfiguration_cancel_Clicked()));

#if 1
	//11.dali 2 addr reconfiguration sure
	dali_2_addr_reconfiguration_sure_button = new QPushButton(wid_dali_2_addr_reconfiguration_sure);
	dali_2_addr_reconfiguration_cancel_button = new QPushButton(wid_dali_2_addr_reconfiguration_sure);
				
	dali_2_addr_reconfiguration_sure_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	dali_2_addr_reconfiguration_cancel_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
				
	dali_2_addr_reconfiguration_sure_button->setText(tr("sure"));
	dali_2_addr_reconfiguration_cancel_button->setText(tr("cancel"));
	
	dali_2_addr_reconfiguration_title = new QLabel(tr("reassign dali address"), wid_dali_2_addr_reconfiguration_sure); 
	dali_2_addr_reconfiguration_title->setObjectName("setAboutLabel");
	dali_2_addr_reconfiguration_title->setGeometry(QRect(5, 60, 300, 60));
	dali_2_addr_reconfiguration_title->setStyleSheet("font-size:30px;color:white;" \
                "text-align:left;background-color:black;");


	dali_2_addr_reconfiguration_sure_button->setGeometry(QRect(0, 150, 150, 60));
	dali_2_addr_reconfiguration_cancel_button->setGeometry(QRect(160, 150, 150, 60));

	
	connect(dali_2_addr_reconfiguration_sure_button, SIGNAL(clicked()), this, SLOT(dali_2_addr_reconfiguration_sure_Clicked()));
	connect(dali_2_addr_reconfiguration_cancel_button, SIGNAL(clicked()), this, SLOT(dali_2_addr_reconfiguration_cancel_Clicked()));
#endif

	//12.wid_facreset_sure
	facreset_sure_button = new QPushButton(wid_facreset_sure);
	facreset_cancel_button = new QPushButton(wid_facreset_sure);
				
	facreset_sure_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
	facreset_cancel_button->setStyleSheet("font-size:30px;color:white;" \
				"text-align:center;background-color:rgb(85,85,85);border-radius:30px;");//font-weight:bold;
				
	facreset_sure_button->setText(tr("sure"));
	facreset_cancel_button->setText(tr("cancel"));
	
	facreset_sure_title = new QLabel(tr("Factory Reset"), wid_facreset_sure); 
	facreset_sure_title->setObjectName("setAboutLabel");
	facreset_sure_title->setGeometry(QRect(50, 60, 300, 60));
	facreset_sure_title->setStyleSheet("font-size:30px;color:white;" \
                "text-align:left;background-color:black;");


	facreset_sure_button->setGeometry(QRect(0, 150, 150, 60));
	facreset_cancel_button->setGeometry(QRect(160, 150, 150, 60));

	
	connect(facreset_sure_button, SIGNAL(clicked()), this, SLOT(facreset_sure_Clicked()));
	connect(facreset_cancel_button, SIGNAL(clicked()), this, SLOT(facreset_cancel_Clicked()));


    //13.wid child lock
    //(tr("status:")
    label_1 = new QLabel(tr("status:"),wid_child_lock);
    label_1->setObjectName("label_1");
    label_2 = new QLabel(tr("enable"),wid_child_lock);
    label_2->setObjectName("label_2");
    pbtn_1 = new QPushButton(wid_child_lock);
    pbtn_1->setObjectName("pbtn_1");
    pbtn_2 = new QPushButton(wid_child_lock);
    pbtn_2->setObjectName("pbtn_2");
    //
    pbtn_1->setText(tr("enable"));
    pbtn_2->setText(tr("disable"));
    label_1->show();
    label_2->show();
    //
    label_1->setStyleSheet("color: white; font-size: 30px;");
    label_2->setStyleSheet("color: white; font-size: 30px;");
    pbtn_1->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");
    pbtn_2->setStyleSheet("font-size:30px;color:white;" \
                    "text-align:center;background-color:rgb(85,85,85);border-radius:30px;");
    //
    label_1->setGeometry(QRect(0, 0, 320, 40));
    label_2->setGeometry(QRect(0, 50, 320, 40));
    pbtn_1->setGeometry(QRect(1, 130, 140, 60));
    pbtn_2->setGeometry(QRect(180, 130, 140, 60));
    //
    connect(pbtn_1, SIGNAL(clicked()), this, SLOT(on_pbtn_1_clicked()));
    connect(pbtn_2, SIGNAL(clicked()), this, SLOT(on_pbtn_2_clicked()));


    //10+1+3[skip]
    
    //
	info_widget->addWidget(wid_netfix);
    info_widget->addWidget(wid_daliset);
    info_widget->addWidget(wid_facreset);
    info_widget->addWidget(wid_etsset);
    info_widget->addWidget(wid_bootmode);
    info_widget->addWidget(wid_language);
    info_widget->addWidget(wid_about);
	info_widget->addWidget(wid_netreset);
	info_widget->addWidget(wid_lights_identify);    
    info_widget->addWidget(wid_dali_reconfiguration);
	//
	info_widget->addWidget(wid_child_lock);
    //
	info_widget->addWidget(wid_dali_1_addr_reconfiguration_sure);
	info_widget->addWidget(wid_dali_2_addr_reconfiguration_sure);
	info_widget->addWidget(wid_facreset_sure);

	info_widget->setCurrentIndex(0);

    //rtn.
    ret_button = new QToolButton(this);
    ret_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	QPixmap ret_button_pixmap(QString("menuButton/exitSetting.png"));
	ret_button->setIcon(ret_button_pixmap);
	ret_button->setIconSize(ret_button_pixmap.size());
    ret_button->setFixedSize(ret_button_pixmap.width()+4, ret_button_pixmap.height());
	//switch_button->setObjectName("functionButton");
	//ret_button->setPalette(text_palette);
	//switch_button->setText(tr("bed room"));
    ret_button->setGeometry(QRect(252, 0, 64, 64));
    ret_button->setStyleSheet("font-size:30px;background-color:black;text-align:right;");//color:white;
	
    connect(ret_button, SIGNAL(clicked()), this, SLOT(retClicked()));
	
	//v_layout->addWidget(info_widget);
    info_widget->move(0, 20);
    //ret_widget->move(250, 2);
    //ret_widget->raise();
	//v_layout->addWidget(ret_widget);
	//this->setLayout(v_layout);
	ret_button->raise();

    timer_tick_one = new QTimer();
	//timer_off->setInterval(60000);
    timer_tick_one->setInterval(TIMER_TICK_ONE);
    //timer_exe->setInterval(60000);
    timer_tick_one->stop();
	connect(timer_tick_one, SIGNAL(timeout()), this, SLOT(onTimerTaskSet()));


	dali_2_all->setVisible(0);//add  by iovine for P2S 
	dali_2_new->setVisible(0);

	this->translateLanguage();
}

SetAboutWidget::~SetAboutWidget()
{
    //delete signal_mapper;
    delete timer_tick_one;
    delete ret_button;
    delete version_status;
    delete model_status;
    delete about_title;
    delete language_title;
    delete english_button;
    delete chinese_button;
	delete tr_cn_button;
    delete setcode_yes;
   // delete set_status;
//    delete setcode_title;
    delete bootmode_title;

    delete labelNetStatusHead;
    delete and_button;
    delete ios_button;
    delete fac_yes;
    delete fac_status;
    delete fac_title;
    delete dali_1_all;
    delete dali_1_new;
    delete dali_2_all;
    delete dali_2_new;
    delete dali_addr_set_status;
    delete dali_addr_set_title;
    delete labelNetworkName;
    delete netfix_yes;
    delete netfix_status;
    delete netfix_title;
    delete wid_about;
    delete wid_lights_identify;

    delete wid_netfix;
    delete wid_daliset;
    delete wid_facreset;
    delete wid_bootmode;
    delete wid_etsset;
    delete wid_language;
    //
    delete wid_child_lock;
    delete label_1;
    delete label_2;
    delete pbtn_1;
    delete pbtn_2;
}


