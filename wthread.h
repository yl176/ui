#ifndef _WTHREAD_H_
#define _WTHREAD_H_

#include <QWidget>
#include <QThread>
#include <QtNetwork>
//#include <QJson>
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
*/
//#include <pthread.h>

//#include "content_widget.h"
#include "mainwinget.h"
#include "common.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


//#define QT_COM_ENABLE 1

#ifdef QT_COM_ENABLE
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#endif

#ifndef QT_UNIX_DOMAIN_SOCKET_PATH
#define QT_UNIX_DOMAIN_SOCKET_PATH          "/home/user/bin/ui"
#endif 

#define OS_ENNO_DB_FILE "/home/user/conf/enno_db"

class BIL_SHARE Thread:public QThread
{
    Q_OBJECT
public:
    Thread(QObject* parent=0);
	void setWindowInstance(MainWidget *wnd);
	
signals:
    void signal_wakeup(); 
	
protected: 
    void run();

private:	
    MainWidget* window;
	
};

class WThread:public QWidget
{
    Q_OBJECT
private:
	int	socket_fd;
	unsigned int socket_len; 
	//struct sockaddr_un	sockaddr_addr;
	QLocalSocket *socket;
	QString currentFortune;
	quint16 blockSize;
    MainWidget* window;
	int slider_speed_time;
//	MyWidget *myWid;
	Thread *pThread;
public slots:	
	void displayError(QLocalSocket::LocalSocketError socketError);
	void readFortune();
	void show_wakeup();
	
#ifdef QT_COM_ENABLE
	void readSerialData();
#endif
	
public:
	void readData();
    WThread(QWidget *parent=0);
	~WThread() {
        // 请求终止
        //quit();
        //wait();
		}
    QWidget *parent1;
	
#ifdef QT_COM_ENABLE
	QSerialPort *serial;//声明对象 
#endif	
	void run();
	void init();
	void stop();
	void send(void *buf, int len);
	void msg_reset_qt();
	void ui_is_wake_up();
	void msg_menu_change(int page);
	void createjs();
	int parse_dev(QJsonArray dev_array);
	int parse_scene(QJsonArray scene_array);
	int parse_room(QJsonArray room_array);
	void parsejs(QByteArray byteArray);
	void setWindowInstance(MainWidget *wnd);
	//void setMyWidInstance(MyWidget *wnd);
	void msg_scene_ctrl(int on, int scene_id);
	void msg_metal_scene_ctrl(int on, int scene_id);
	void msg_set_metal_light_ctrl(uint32 room_id, uint32 status);
	void msg_get_light_dimvalue(int scene_num);
	void msg_light_dim(int scene_id, int last_dim, int dim);
	void msg_dev_ctrl(int dev_id, int char_id, int value);
	void msg_enter_set();
	void msg_set_conf(EN_SET_ITEM_DEF cmd, int value);
	void msg_get_device_info();
};
#endif


extern WThread *g_thread;


