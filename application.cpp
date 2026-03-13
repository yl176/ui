#include "application.h"
#include "wthread.h"
#include "util.h"

int g_no_shake = 0;
int g_time_out_cnt = 0;
int g_time_set_cnt = 0;
int g_time_set_enable = 0;
int g_oled_state = 0;

int g_fatocry_test_mode = 0;

Application::Application(int &argc, char **argv)
    :  QApplication(argc,argv)
    , window(0)
{
    this->setOverrideCursor(Qt::BlankCursor);
    g_no_shake = 0;
    timer = new QTimer();
    timer->setInterval(TIMER_STANDBY_MS);  //30s 
  // timer->setInterval(100000);   //1000s
    
    //timer->setInterval(300000);
    timer->stop();

	timer_off = new QTimer();
	timer_off->setInterval(TIMER_SLEEP_MS);
   // timer_off->setInterval(350000);   //350s
   // timer_off->setInterval(200000);   //200s
    timer_off->stop();

	timer_set = new QTimer();
	//timer_off->setInterval(60000);
    timer_set->setInterval(1000);
    timer_set->start();

	g_time_out_cnt = 0;
	g_time_set_cnt = 0;
	g_time_set_enable = 0;
	g_fatocry_test_mode = 0;
}

extern WThread *g_thread;
void Application::setWindowInstance(MainWidget *wnd)
{
    window = wnd;
    connect(timer, SIGNAL(timeout()), wnd, SLOT(onTimerOut()));
	connect(timer_off, SIGNAL(timeout()), wnd, SLOT(onTimerOff()));
	connect(timer_set, SIGNAL(timeout()), wnd, SLOT(onTimerSet()));
	wnd->setTimer(timer, timer_off);
	//connect(wnd, SIGNAL(time_restart()), this, SLOT(onTimeRestart()));
}

void Application::TimerRestart()
{
   // timer->start(TIMER_STANDBY_MS);
   // timer_off->start(TIMER_SLEEP_MS);
	//qDebug()<<"TimerRestart";
}//

bool Application::notify(QObject *obj, QEvent *e)
{
    char buf[10] = {0};
	buf[0] = 1;
	buf[1] = 2;
	buf[2] = 3;
	buf[3] = 4;
	int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_TOUCH_EVENT);
    static int64_t last_send_tick = 0;
	//QFont font;

	//QFontDatabase::addApplicationFont("");
   // font.setFamily("Helvetica Neue LT Pro");
	//this->setFont(font);	

	//printf("e->type()=%d\n", e->type());
	if (e->type() == QEvent::TouchBegin)   //194
	{
	    //Util::UI_motor_shake();
	    if (g_thread != NULL && (g_oled_state == 3))
    	{

    		//qDebug()<<"---wake_up1 oled--window->get_state()--:"<<window->get_state();
    	   // if (!g_no_shake)
		    g_thread->send(&msg_id, 4);
		   
		    
	    }	


		if (GetTickCount() - last_send_tick >= 30000) //30 ms send
		{
			//qDebug()<<"---wake_up2 oled--window->get_state()--:"<<window->get_state();
		    g_thread->send(&msg_id, 4);
			last_send_tick = GetTickCount();
		}

		
		g_time_set_enable = 1;
		g_time_out_cnt = 0;
	}
	else if (e->type() == QEvent::MouseMove)
	{
		g_time_out_cnt = 0;
	}
	else if (e->type() == QEvent::TouchEnd)
	{
		g_time_set_enable = 0;
		g_time_out_cnt = 0;


	}

	if(g_fatocry_test_mode==1) //add by iovine
	{
		g_time_out_cnt = 58;//goto standby mode after 2s
		g_fatocry_test_mode = 0;
	}

		
	return QApplication::notify(obj,e);

	
    if(e->type() == QEvent::MouseMove)
    {
        if(window)
        {   
            if (timer->remainingTime() > 0 && timer->remainingTime()<5000)
           	{
	            timer->start();
            }
//			qDebug()<<"timer->stop()";
			if (window->get_state() == EN_SYS_NORM || window->get_state() == EN_SYS_SET)
			{
//			    qDebug()<<"timer_off->stop()";
				if (timer->remainingTime() > 0 && timer->remainingTime()<5000)
				{
					timer_off->start();
				}
			    //timer_off->stop();
				//qDebug()<<"astop";
			}
			else
			{
                //qDebug()<<"stop";
			}
        }
    }
	else if (e->type() == QEvent::TouchBegin)   //194
	{
	    if (g_thread != NULL)
    	{
    	    if (!g_no_shake)
    	    {
				g_thread->send(&msg_id, 4);

    	    }
		       
	    }
		else
		{
		    qDebug()<<"[qterror], g_thread is NULL"<<endl;
		}
		timer_set->start();
		//window->pairclrWidget();
	}
	else if (e->type() == QEvent::TouchEnd)  //196
	{
		timer_set->stop();  //press 10s, display set menu
		if (g_thread != NULL)
		{
		//    g_thread->msg_enter_set();
		}
		else
		{
		    qDebug()<<"[qterror], g_thread is NULL"<<endl;
		}
	}
    else if (window)
    {        
        if(((window->get_state() == EN_SYS_SET) 
			|| (window->get_state() == EN_SYS_NORM))
			&& (!timer->isActive()))
        {
            timer->start();
			timer_off->start();
//            qDebug()<<"timer & timer_off start";
        }
    }
	//printf("notify\n");

    return QApplication::notify(obj,e);
}
