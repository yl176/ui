#include "mainwinget.h"
#include <QTextCodec>
#include <QTranslator>

#include <QApplication>
#include <QSharedMemory>
#include "application.h"
#include <QProxyStyle>
#include <errno.h>

int g_language_zh;
int g_enno_debug_level;

int read_language()
{
    QFile file(LANGUAGE_CONF);
	QByteArray byteArray;

	//file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		system("echo 1 > /home/user/ui/qwid/language.conf");
		usleep(20000);
		system("sync");
		return ENNO_OK;
	}
	qDebug()<<"read file language.conf"<<endl;
	QByteArray line = file.readLine();
	g_language_zh = line[0] - 0x30;
	file.close();
	qDebug()<<"g_language_zh="<<g_language_zh;
	
	return ENNO_OK;
}

class MyProxyStyle : public QProxyStyle  
{   
    public:      
		int pixelMetric(PixelMetric m, const QStyleOption * opt = 0, const QWidget * widget = 0) const      
			{          
			        if ((m == PM_ButtonShiftHorizontal) ||  (m == PM_ButtonShiftVertical))            
						return 0;          
					return QProxyStyle::pixelMetric(m, opt, widget); 
			}  
};

MainWidget * g_ptr_main_widget = NULL;

int thread_run_cpu()
{
    int s, j;
    cpu_set_t cpuset;
    pthread_t thread;

    thread = pthread_self();

    /* Set affinity mask to include CPUs 0 to 7 */

    CPU_ZERO(&cpuset);
    for (j = 1; j < 2; j++)
   	{
        CPU_SET(j, &cpuset);
    }

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
   	{
        //handle_error_en(s, "pthread_setaffinity_np");
        ENNO_LOG_ERR("\n");
        return -1;
    }

    /* Check the actual affinity mask assigned to the thread */
    s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (s != 0)
   	{
        //handle_error_en(s, "pthread_getaffinity_np");
        ENNO_LOG_ERR("\n");
        return -1;
    }

    ENNO_LOG_ERR("Set returned by pthread_getaffinity_np() contained:\n");
    for (j = 0; j < CPU_SETSIZE; j++)
	{
        if (CPU_ISSET(j, &cpuset))
       	{
            ENNO_LOG_ERR("    CPU %d\n", j);
        }
	}

    return 0;
}

int main(int argc, char *argv[])
{
    int ret;
	g_enno_debug_level = 1;
    Application a(argc, argv);
	//QTextCodec *codec = QTextCodec::codecForName("System"); 
	//QTextCodec::setCodecForLocale(codec); 

	
//QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	//thread_run_cpu();

	ret = nice(-20);
	#if 0
	if (ret == -1 && errno !=0)
	    perror ("nice");
	else
	    printf ("nice value is now %d\n", ret);
	#endif
	
    //qDebug()<<"compile date="<<__DATE__<<",time="<<__TIME__; 
	ENNO_LOG_ERR("qwid start, compile date=%s time=%s\n", __DATE__, __TIME__);

	QTranslator translator;  
	read_language();
	if (g_language_zh==1)
	{
		translator.load(QString("qwid_jt.qm"));
		qDebug()<<"language chose qwid_jt";
	}
	else if (g_language_zh == 2)
	{
		translator.load(QString("qwid_ft.qm"));
		qDebug()<<"language chose qwid_ft";
	}
	else
	{
		translator.load(QString("qwid_zh_EN.qm"));
		qDebug()<<"language chose english";
	} 
	a.installTranslator(&translator);  
	a.setStyle(new MyProxyStyle);

	//QTranslator translator_zh;  
	//translator_zh.load(QString("qwid_zh_CN.qm"));  
	//a.installTranslator(&translator_zh);  
    //printf("func=%s,line=%d\n", __func__, __LINE__);
	QFile qss("qwid.qss");
	qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();
    //printf("func=%s,line=%d\n", __func__, __LINE__);
	
	QFont font;
	//QFontDatabase::addApplicationFont("");
    font.setFamily("Helvetica Neue LT Pro");
	a.setFont(font);	
#if 0
    QSharedMemory shared_memory;
	shared_memory.setKey(QString("main_window"));
    printf("func=%s,line=%d\n", __func__, __LINE__);

	if(shared_memory.attach())
	{
	    
		printf("func=%s,line=%d\n", __func__, __LINE__);
		return 0;
	}
#endif
	//printf("func=%s,line=%d\n", __func__, __LINE__);
#ifdef START_SCREEN_ENABLE
	QSplashScreen *splash = new QSplashScreen();
	splash->setPixmap(QPixmap("start.png"));
	splash->show();
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
	splash->showMessage(QObject::tr("Setting up the main window..."),
			topRight, Qt::white);
	usleep(5000000);
#endif	
	SleepWidget *sleep_widget = new SleepWidget();

	MainWidget main_widget;
	g_ptr_main_widget = &main_widget;
    a.setWindowInstance(&main_widget);  
    //sleep_widget.show();  
	//if(shared_memory.create(1))
	//{
	
		main_widget.setTranslator(&translator);
		main_widget.showWidget();
		#ifdef START_SCREEN_ENABLE
		splash->finish(&main_widget);
		delete splash;
		#endif

		return a.exec(); 
	//}
    printf("func=%s,line=%d\n", __func__, __LINE__);

    return a.exec();
}
