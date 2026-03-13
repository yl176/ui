//#include <QPixmap>
//#include <QApplication>
#include "wthread.h"
#include <QMessageBox>
#include <unistd.h>
#include "sem.h"
#include <termios.h>


QWidget *currwid;
#ifndef UI_UNIX_DOMAIN_PATH
#define UI_UNIX_DOMAIN_PATH          "/home/user/bin/ui"
#endif
/*void WThread::setWidSlot(MainWidget *wnd)
{
    //window = wnd;
	connect(return_button,SIGNAL(clicked()),wnd,SLOT(reviewWidget()));
}
*/
static char *g_msp_uart_port = "/dev/ttymxc1";

extern int g_clear_flag;
extern uint32 g_dali_set_flag;

WThread::WThread(QWidget *parent) 
             :QWidget(parent)
{
#if 1
//	myWid = NULL;

	socket = new QLocalSocket();
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readFortune()));
   // QObject::connect(socket, SIGNAL(waitForReadyRead(300)), this, SLOT(readFortune()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
        this, SLOT(displayError(QLocalSocket::LocalSocketError)));

	QByteArray ba;
	ba.resize(5);
	ba[0] = 0x3c;
	ba[1] = 0xb8;
	ba[2] = 0x64;
	ba[3] = 0x18;
	ba[4] = 0xca;
    socket->connectToServer(UI_UNIX_DOMAIN_PATH);
	//printf("sleep\n");
	//sleep(3);
	//printf("write1\n");
    if(socket->state() == QLocalSocket::ConnectedState)
    {
        socket->write(ba);
    }
	//sleep(10);
	
	//printf("write2\n");
	ba[0] = 0x11;
   // socket->write(ba);
	blockSize = 0;
	//printf("WThread::run, send data\n");
    //socket->flush();
    //socket->disconnectFromServer();
   // createjs();
    pThread = new Thread();
	connect(pThread, SIGNAL(signal_wakeup()), this, SLOT(show_wakeup()));
	//pThread->start();

#ifdef QT_COM_ENABLE
	serial = new QSerialPort();
	serial->setPortName(g_msp_uart_port);
	serial->open(QIODevice::ReadWrite);
	serial->setBaudRate(QSerialPort::Baud115200);
	serial->setDataBits(QSerialPort::Data8);
	serial->setParity(QSerialPort::NoParity);
	serial->setStopBits(QSerialPort::OneStop); 
	serial->setFlowControl(QSerialPort::NoFlowControl);

	connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData()));
#endif

    QSettings *configIni = new QSettings("/home/user/ui/qwid/config.ini", QSettings::IniFormat);

	slider_speed_time = configIni->value("roomlight/slider_speed").toInt();

    ENNO_LOG_ERR("slider_speed_time=%d\n", slider_speed_time);
	if (slider_speed_time <= 0)
	{
	    slider_speed_time = 300;
	}

	delete configIni;

#endif
}
/*******************************************************************************
AirConditionArray:[
	{
	devName:”floor”,
	serialid:”2342”,
	value:”2342”,
	mode:”0”,
	speed:”2342”
	}
]
*******************************************************************************/

void WThread::createjs()
{
    QFile file("/home/user/ui/devjs");
	QJsonArray devArray;
	QJsonArray sceneArray;
	QJsonArray roomArray;
	QJsonObject json_home;
	QJsonObject json_room;
	QJsonObject json_scene;
	QJsonObject json_dev;
	json_dev.insert("devName", "floor");
	json_dev.insert("serialid", "1234");
	json_dev.insert("type", "104");
	json_dev.insert("mode", "3");
	json_dev.insert("speed", "1");
	
	devArray.append(json_dev);
	
	json_scene.insert("sceneName", "happy");
    json_scene.insert("devList", QJsonValue(devArray));

    sceneArray.append(json_scene);
	
    json_room.insert("roomId", "234");
    json_room.insert("roomName", "room");
    json_room.insert("sceneArray", QJsonValue(sceneArray));

    roomArray.append(json_room);
    json_home.insert("homeName", "涓囬噷鍒");
	json_home.insert("homeId", "83DA0987-9B38-56AC-945C-8198A02C5FA3");
	json_home.insert("roomArray", QJsonValue(roomArray));
	// 构建 JSON 文档
	QJsonDocument document;
	document.setObject(json_home);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QString strJson(byteArray);

	//qDebug() << strJson;

	file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
	file.write(byteArray);

	parsejs(byteArray);
	//关闭文件
	file.close();
}
int WThread::parse_dev(QJsonArray dev_array)
{								 
	int nSize = dev_array.size();
	for (int i = 0; i < nSize; i++)
	{
	    QJsonValue value = dev_array.at(i);
		if (value.isObject())
		{
			QJsonObject object_dev = value.toObject();
			if (object_dev.contains("devName"))
			{
	            QJsonValue value = object_dev.value("devName");
	            if (value.isString()) 
				{
				    QString devName = value.toString();  
	                //qDebug() << "[devList]devName : " << devName;
				}
			}
			
			if (object_dev.contains("serialid"))
			{
	            QJsonValue value = object_dev.value("serialid");
	            if (value.isString()) 
				{
				    QString dSerialid = value.toString();
	                //qDebug() << "[devList]serialid : " << dSerialid;
				}
			}

			if (object_dev.contains("type"))
			{
	            QJsonValue value = object_dev.value("type");
	            if (value.isString()) 
				{
				    QString dTpye = value.toString();
	                //qDebug() << "[devList]type : " << dTpye;
				}
			}
		}
	}

	return 0;
}

int WThread::parse_scene(QJsonArray scene_array)
{
    int i;
	int ret;
    int nSize = scene_array.size();
	
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = scene_array.at(i);
		if (value.isObject())
		{
			QJsonObject object_scene = value.toObject();
			if (object_scene.contains("sceneName")) 
			{  
				QJsonValue value = object_scene.value("sceneName");	
				if (value.isString()) 
				{  
					QString strName = value.toString();  
					//qDebug() << "[sceneArray]sceneName : " << strName;
				}
			}
			if (object_scene.contains("devList"))				//level_4 devList
			{
				QJsonValue value = object_scene.value("devList");
				if (value.isArray())
				{
					QJsonArray dev_array = value.toArray();
					ret = parse_dev(dev_array);
					if (ret)
					{
					   // qDebug() << "parse_dev error ret=%d\n" << ret;
					}
				}	
			}
	    }
	}

	return 0;
}
int WThread::parse_room(QJsonArray room_array)
{
    int i;
	int ret;
	int nSize = room_array.size();
	for (i = 0; i < nSize; i++)
	{
		QJsonValue value = room_array.at(i);
		if (value.isObject())
		{
			QJsonObject object_room = value.toObject();
			if (object_room.contains("roomName")) 
			{  // 包含指定的 homeName
				QJsonValue value = object_room.value("roomName");  // 获取指定 key 对应的 value
				if (value.isString()) 
				{  // 判断 value 是否为字符串
					QString strName = value.toString();  // 将 value 转化为字符串
					//qDebug() << "[roomArray]roomName : " << strName;
				}
			}
			if (object_room.contains("roomId")) 
			{
				QJsonValue value = object_room.value("roomId");
				if (value.isString()) 
				{
					QString strFrom = value.toVariant().toString();
					//qDebug() << "[roomArray]roomId : " << strFrom;
				}
			}
			if (object_room.contains("sceneArray"))
			{
				QJsonValue value = object_room.value("sceneArray");   //level_3 scene
				if (value.isArray())
				{
					QJsonArray array_scene = value.toArray();							
					ret = parse_scene(array_scene);	
					if (ret)
					{
					   // qDebug() << "parse_scene error ret=%d\n" << ret;
					}
				}	
			}
		}
	}
	
    return 0;
}
void WThread::parsejs(QByteArray byteArray)
{
	//parse
	int ret;
	QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);

	#if 1
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
	{  // 解析未发生错误
	    if (doucment.isObject())
		{ // JSON 文档为对象
	        QJsonObject object_home = doucment.object();  // level_1 home
	        if (object_home.contains("homeName")) 
			{  // 包含指定的 homeName
	            QJsonValue value = object_home.value("homeName");  // 获取指定 key 对应的 value
	            if (value.isString()) 
				{  // 判断 value 是否为字符串
	                QString strName = value.toString();  // 将 value 转化为字符串
	                //qDebug() << "homeName : " << strName;
	            }
	        }
	        if (object_home.contains("homeId")) 
			{
	            QJsonValue value = object_home.value("homeId");
	            if (value.isString()) 
				{
	                QString strFrom = value.toVariant().toString();
	                //qDebug() << "homeId : " << strFrom;
	            }
	        }
	        if (object_home.contains("roomArray")) 
			{
	            QJsonValue value = object_home.value("roomArray");  // level_2 rome
				if (value.isArray()) 
				{
				    QJsonArray room_array = value.toArray(); 
					ret = parse_room(room_array); 
					if (ret)
					{
					    //qDebug() << "parse_room error ret=%d\n" << ret;
					}
				}
	        }
	    }
	}
	#endif
}
void WThread::send(void *buf, int len)
{
	QByteArray ba;
	static int sn = 0;
	int i;
	char *data;
	int msg_len;
	uint32 len_use;
	uint32 buf_len = 12;
	int msg_id;
	
	ba.resize(12+len);
	ba[0] = 0x55;
	ba[1] = 0x66;
	ba[2] = 0xa5;
	ba[3] = 0x5a;

    //qDebug()<<"wthread send, len="<<len<<endl;
	msg_len = len + 4;

	ba[4] = (msg_len<<24)&0xff;
	ba[5] = (msg_len<<16)&0xff;
	ba[6] = (msg_len<<8)&0xff;
	ba[7] = msg_len&0xff;

	ba[11] = (sn<<24)&0xff;
	ba[10] = (sn<<16)&0xff;
	ba[9] = (sn<<8)&0xff;
	ba[8] = sn&0xff;
	
	//qDebug()<<"sn="<<sn<<",len="<<len<<endl;

	sn++;
	
    data = ba.data();
    //ba.append((char *)buf, len);
    for (i = 0; i < len; i++)
	{
	    // ba.at(4+i) = ((char *)buf)[i];
	    data[12+i] = ((char *)buf)[i];
		//printf("wd[%d]=0x%x\n", i, ba.at(i));
	}
	msg_id = CHANGE_TO_LITTLE_ENDIAN(*((int *)(&data[12])));

	syslog(LOG_INFO|LOG_LOCAL1, "ui_msg_id send = 0x%x\n", msg_id);

	//printf("ui_msg_id send = 0x%x\n", msg_id);
	
    if(socket->state() == QLocalSocket::ConnectedState)
    {
        socket->write(ba);
    }
}
void WThread::msg_reset_qt()
{
    int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_REBOOT_QT);
	
	qDebug()<<"msg_reset_qt";
	send(&msg_id, 4);
}

void WThread::msg_menu_change(int page)
{
	int data[10];
    int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_MENU_CHANGE);
    
	
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_MENU_CHANGE);
	data[1] = page;

	qDebug()<<"change to page="<<page;
	send(data, 8);
}

void WThread::ui_is_wake_up()
{
	int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_IS_WAKE_UP);
	
	send(&msg_id, 4);
}

void WThread::msg_enter_set()
{
   // int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_ENTER_SET);
	//send(&msg_id, 4);
}

void WThread::msg_get_device_info()
{
    int msg_id = CHANGE_TO_LITTLE_ENDIAN(F_UI_GET_DEV_INFO_CMD);
	send(&msg_id, 4);
}

void WThread::msg_set_metal_light_ctrl(uint32 room_id, uint32 status)
{
	int data[10];
	data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_METAL_LIGHT_ONOFF);
	data[1] = room_id;
	data[2] = status;
	
	send(data, 12);
}


void WThread::msg_metal_scene_ctrl(int on, int scene_id)
{
	int data[10];
	data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_SCENE_CTRL);
	data[1] = on ? EN_SCENE_METAL_ON : EN_SCENE_METAL_OFF; 
	data[2] = scene_id;
	//qDebug()<<"scene send msg = "<<F_UI_SCENE_CTRL<<endl;
	//qDebug()<<"scene_id=["<<scene_id<<"]"<<endl;
	
	syslog(LOG_INFO|LOG_LOCAL1, "metal, ui_msg_id = 0x%x, little=0x%x\n", F_UI_SCENE_CTRL, data[0]);
	send(data, 12);
}


void WThread::msg_scene_ctrl(int on, int scene_id)
{
    int data[10];
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_SCENE_CTRL);
    data[1] = on ? EN_SCENE_ON : EN_SCENE_OFF; 
	data[2] = scene_id;
	//qDebug()<<"scene send msg = "<<F_UI_SCENE_CTRL<<endl;
	//qDebug()<<"scene_id=["<<scene_id<<"]"<<endl;
	
	syslog(LOG_INFO|LOG_LOCAL1, "ui_msg_id = 0x%x, little=0x%x\n", F_UI_SCENE_CTRL, data[0]);
	send(data, 12);
}
void WThread::msg_dev_ctrl(int dev_id, int char_id, int value)
{
    int data[10];
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_DEV_CTL);
	data[1] = EN_DEVICE_CTL;
	data[2] = dev_id;
	data[3] = char_id; //CHAR_SHADE_TARGET_POSITION;
    data[4] = value; 
	//qDebug()<<"qt send msg = "<<F_UI_DEV_CTL;
//	qDebug()<<",devid="<<dev_id;
//	qDebug()<<",char_id="<<char_id;
//	qDebug()<<",value="<<value<<endl;
	send(data, 20);
}

void WThread::msg_set_conf(EN_SET_ITEM_DEF cmd, int value)
{
    int data[10];
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_SET_CONFIG);
    data[1] = (int)cmd; 
	data[2] = value;

	ENNO_LOG_ERR("cmd=%d, value=%d\n", cmd, value);
	//qDebug()<<"send msg = "<<T_UI_CONF_UPDATE<<endl;
	//qDebug()<<"msg_set_conf cmd=["<<cmd<<"]"<<endl;
	send(data, 12);
}

void WThread::msg_get_light_dimvalue(int scene_num)
{
    int data[10];
	
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_SCENE_DIM_VALUE);
	data[1] = scene_num;
	//qDebug()<<"scene_send_msg = "<<F_UI_SCENE_DIM_VALUE<<endl;
	send(data, 8);
}

void WThread::msg_light_dim(int scene_id, int last_dim, int dim)
{
    int data[10];
	
    data[0] = CHANGE_TO_LITTLE_ENDIAN(F_UI_DIM_START);
	data[1] = scene_id;
	data[2] = last_dim;
	data[3] = dim;
	//qDebug()<<"dim start = "<<F_UI_DIM_START<<"scene_id="<<scene_id<<", last="<<last_dim<<", dim="<<dim<<endl;
	send(data, 16);
}

void WThread::show_wakeup()
{
    qDebug()<<"wthread show_wakeup";
    window->get_wake_up_msg(NULL, 0);
	if (g_thread != NULL)
	{
		g_thread->ui_is_wake_up();
	}
}

Thread::Thread(QObject* parent)
	:QThread(parent)
{
    UI_sem_init();
}

void Thread::setWindowInstance(MainWidget *wnd)
{
    window = wnd;
}

void Thread::run()
{
    while (1)
	{
		//printf("Thread::run\n");
		if (UI_sem_p())
		{
		    qDebug("UI_sem_p error exit\n");
		    exit(0);
		}
		//qDebug()<<"UI_sem_p revmsg";
		
		emit signal_wakeup();
		//window->get_wake_up_msg(NULL, 0);
		//g_thread->readData();
		//sleep(1);
	}
}

void WThread::run()
{
   // currwid->hide();
    //init(); 
    #if 0
    socket = new QLocalSocket();
    //connect(socket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    //connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
     //   this, SLOT(displayError(QLocalSocket::LocalSocketError)));

	QByteArray ba;
	ba.resize(5);
	ba[0] = 0x3c;
	ba[1] = 0xb8;
	ba[2] = 0x64;
	ba[3] = 0x18;
	ba[4] = 0xca;
    socket->connectToServer(UI_UNIX_DOMAIN_PATH);
    socket->write(ba);
	
	printf("WThread::run, send data\n");
    socket->flush();
    socket->disconnectFromServer();
	#endif
	while (1)
	{
		//printf("WThread::run\n");
		sleep(1);
		
	}
}

// 发生错误时，进行错误处理
void WThread::displayError(QLocalSocket::LocalSocketError socketError)
{
#if 0
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        QMessageBox::information(this, "Fortune Client",
                                "The host was not found. Please check the "
                                    "host name and port settings.");
        break;
    case QLocalSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Fortune Client",
                                 "The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct.");
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
       // QMessageBox::information(this, "Fortune Client",
       //                          "The following error occurred: %1."
      //                           .arg(socket->errorString()));
      break;
    }
#endif
    //getFortuneButton->setEnabled(true);
}

void WThread::init()
{
	int error = 0;
	
#if 0	
	/* create a socket */
	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("%s", "unix socket create fail");
		//return -EPERM;
	}

	memset(&sockaddr_addr, 0x00, sizeof(struct sockaddr_un));
	sockaddr_addr.sun_family = AF_UNIX;
	sprintf(sockaddr_addr.sun_path, "%s%05d", QT_UNIX_DOMAIN_SOCKET_PATH, getpid());
	socket_len = offsetof(struct sockaddr_un, sun_path) + strlen(sockaddr_addr.sun_path);

	unlink(sockaddr_addr.sun_path);

	error = bind(socket_fd, (struct sockaddr*)&sockaddr_addr, socket_len);
	if(error < 0)
	{
		printf("%s", "bind socket error");
		//return -EPERM;
	}

	error = chmod(sockaddr_addr.sun_path , S_IRWXU);
	if(error < 0)
	{
		printf("%s", "chmod error");
		//return -EPERM;
	}

	/* fill socket address struct with server's address */
	memset(&sockaddr_addr, 0x00, sizeof(sockaddr_addr));
	sockaddr_addr.sun_family = AF_UNIX;
	strcpy(sockaddr_addr.sun_path, QT_UNIX_DOMAIN_SOCKET_PATH);
	socket_len = offsetof(struct sockaddr_un, sun_path) + strlen(QT_UNIX_DOMAIN_SOCKET_PATH);

	//error = connect(socket_fd, (struct sockaddr*)&sockaddr_addr, socket_len);
	if(error < 0)
	{
		printf("conect error:%d", error);
		//return -EPERM;
	}
#endif
}
#define MAX_DEF_VAL 13

char true_table[MAX_DEF_VAL][4]=
{
{0, 0, 0, 0},   //0
{0, 0, 1, 0},  ///1
{0, 0, 2, 0},  ///2
{0, 1, 0, 0},  ///3
{0, 1, 1, 1},  ///4
//{0, 1, 2, 2}, //6,a,b,c
{0, 2, 0, 0}, //5,a,c,a
{1, 0, 0, 0}, ///6     //10,b,a,a
{1, 1, 0, 1}, ///7//13,bba
{1, 1, 1, 1}, ///8//14,bbb   
{1, 1, 2, 1}, ///9//15,bbc
{1, 2, 1, 1}, ///10//17,bcb
{2, 0, 0, 0}, ///11//19,caa
{2, 1, 1, 1}, ///12//23,cbb
};

char check_out(unsigned short a, unsigned short b)
{
    if (a > b)
    {
        return 0;
    }
	else if(a < b)
	{
	    return 1;
	}
	else 
	{
	    return 2;
	}
}
char compare_value(char a, char b, char c)
{
    char i = 0;
	
    for(i = 0; i < MAX_DEF_VAL; i++)
    {
        if (a == true_table[i][0] && b == true_table[i][1] && c == true_table[i][2])
        {
            ENNO_LOG_ERR("rule=%d,o=%d\n", i, true_table[i][3]);
            return true_table[i][3];
        }
    }

	return 2;
}

char UI_cal_data(char data)
{
    int64_t current_time = 0;
	static int64_t last_time = 0;
    static int sn = 0;
	static int re_cnt = 0;
    static char avl[4];
	static char out[4];
	
	current_time = GetTickCount();
    if (current_time - last_time >= 500)
    {
        re_cnt = 0;
	    ENNO_LOG_ERR("reset,current=%lld,last=%lld, l=%lld\n", current_time, last_time, current_time-last_time);
		last_time = current_time;
    }

    re_cnt++;
	out[3] = 2;

	avl[3] = data;

	if (re_cnt >= 4)
	{
		out[0] = check_out(avl[0], avl[1]);
		out[1] = check_out(avl[1], avl[2]);
		out[2] = check_out(avl[2], avl[3]);

		out[3] = compare_value(out[0], out[1], out[2]);
		
		ENNO_LOG_ERR("avl=[%d,%d,%d,%d], dir=%d\r\n", avl[0], avl[1], avl[2], avl[3], out[3]);
	}
	
	avl[0] = avl[1];
	avl[1] = avl[2];
	avl[2] = avl[3];
    
	return out[3];
}

void WThread::stop()
{
    //QThread::stop();
}
#ifdef QT_COM_ENABLE
void WThread::readSerialData()
{
   #if 1
    QByteArray array;
    QByteArray ba;
    char *buf;
	char tmpbuf[12];
	uint32 len;
	uint32 dir;
	char auc_cycle[2] = {0xaa,0x81};
	char auc_dir1[3] = {0xaa,0x81,0x1};
	char auc_dir0[3] = {0xaa,0x81,0x0};
	char auc_key[3] = {0xaa,0x82,0x1};
	
	array = serial->readAll();
	
	if(!array.isEmpty())
	{
	    buf = array.data();
		len = array.size();
		ENNO_LOG_ERR("data = {");
		for (int i = 0; i < len; i++)
		{
		    printf("0x%x ", array.data()[i]);
		}
		printf("}\r\n");

		/*if (!memcmp(buf, auc_dir0, 3))
		{
		    enno_encode_uint(tmpbuf, 0, &len, 12);
			window->set_cycle_light_dim((char *)&tmpbuf, 4);
		}
		else if (!memcmp(buf, auc_dir1, 3))
		{//1/
		    enno_encode_uint(tmpbuf, 1, &len, 12);
			window->set_cycle_light_dim((char *)&tmpbuf, 4);
		}*/
		if (!memcmp(buf, auc_cycle, 2))
		{
		    dir = UI_cal_data(buf[2]);
			if (dir >= 0 && dir <= 1)
			{
			    enno_encode_uint(tmpbuf, dir, &len, 12);
				window->set_cycle_light_dim((char *)&tmpbuf, 4);
			}
		}
		else if (!memcmp(buf, auc_key, 3))
		{
			enno_encode_uint(tmpbuf, 1, &len, 12);
			window->set_light_status((char *)&tmpbuf, 4);
		}
	    //ENNO_LOG_ERR("read serial data: 0x%x, 0x%x, 0x%x\n", buf[0], buf[1], buf[2]);
	}
	array.clear();    
	
	#endif

}
#endif
void WThread::readFortune()
{

    //printf("read forturn\n");
	ST_MSG_TYPE msg_in;
	QByteArray dataArray;
	int i;
	int msg_id;
	uint32 sn;
	static uint32 last_sn = 0;
	uint32 len_use;
	qint32 tmp;
	CTL_DEVICE_CONF *pst_msg;
	int64_t current_time = 0;
	static int64_t last_dim_time = 0;

    //读取接收到的数据
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    while (socket->bytesAvailable() >= sizeof(qint32))
    {
        if(socket->bytesAvailable() >= sizeof(qint32))
            in>>msg_in.head;
		if (msg_in.head != UI_TO_QT_HEAD_FLAG)
		{
		    //qDebug()<<"[error]func="<<__func__<<", line="<<__LINE__<<",receive data="<<msg_in.head;
		    return;
		}
		else
		{
		    //qDebug()<<"receive msg head\n";
		}
		//qDebug()<<"receive msg -----------\n";

		if (socket->bytesAvailable() >= sizeof(qint32))
		{
		    in>>tmp;
			msg_in.length = CHANGE_TO_LITTLE_ENDIAN(tmp);
		//    qDebug()<<"msg_in.length :"<<msg_in.length<<"bytevailable :"<<socket->bytesAvailable();
		}
		else
		{
		    qDebug()<<"[error]func="<<__func__<<", line="<<__LINE__<<",byte="<<socket->bytesAvailable();
		    return;
		}

		if ((socket->bytesAvailable() >= msg_in.length + 4) && (msg_in.length != 0))
		{
		    int bytesRead = in.readRawData((char *)msg_in.data, msg_in.length + 4);
			dataArray = QByteArray((char *)msg_in.data);
			//for (i = 0; i < 4; i++)
			//{
			//	printf("data:0x%x\n", msg_in.data[i]);
			//}
			msg_id = msg_in.data[0] + (msg_in.data[1]<<8) + (msg_in.data[2]<<16) + (msg_in.data[3]<<24);
			enno_decode_uint(&msg_in.data[msg_in.length], &sn, &len_use, 4);
			//qDebug()<<"receive data from arm qwid_sn="<<sn;
			if (sn != last_sn+1)
			{
			    qDebug()<<"[socket_error]sn_error last_sn="<<last_sn<<", but sn="<<sn;
				
				syslog(LOG_INFO|LOG_LOCAL1,"line=%d,[socket_error]sn_error last_sn=%d, but sn=%d",
					__LINE__, last_sn, sn);
			}
			last_sn = sn;
    	//	printf("ui_msg_id rev = 0x%x\n", msg_id);
    	    syslog(LOG_INFO|LOG_LOCAL1,"ui_msg_id rev = 0x%x\n", msg_id);
			switch(msg_id)
			{
			case T_UI_RESET:
			
				break;

			case T_UI_CONF_UPDATE:
				window->get_outdoor_value((char *)&msg_in.data[4], msg_in.length-4);
				break;
				
			case T_UI_DISPLAY_PAIRID:
				//qDebug()<<"receive T_UI_DISPLAY_PAIRID\n";
				qDebug()<<"pair msg recv in "<<"readFortune"<<endl;
				window->pairdispWidget((char *)&msg_in.data[4]);
				break;

			case T_UI_CLR_PAIRID:
				qDebug()<<"receive T_UI_CLR_PAIRID, call pairclrWidget\n";
				window->pairclrWidget();
				g_clear_flag = 1;
				break;

			case T_UI_FLUTTER_CLR_PAIRID:
				qDebug()<<"receive T_UI_FLUTTER_CLR_PAIRID, call pairclrWidget\n";
				g_clear_flag = 1;
				break;

            case T_UI_SET_SCENE_DIM:
				window->set_dim_value((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_SCENE_UPDATE:
				window->get_scene_status((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_SCENE_NAME_UPDATE:
				window->get_scene_status_byname((char *)&msg_in.data[4], msg_in.length-4);
				break;

            case T_SYS_DEV_UPDATE:
				window->set_dev_update((char *)&msg_in.data[4], msg_in.length-4);
				break;

            case T_UI_GET_SENSOR_VAL:
				window->get_sensor_value((char *)&msg_in.data[4], msg_in.length-4);
				break;
				
			case T_UI_FIRMWARE_UPDATE:
				window->get_firmware_update_per((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_DIM_LIGHT:	
				window->get_dim_value((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_GET_DEV_INFO:
				window->get_dev_info((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_DEV_OPEN_NOTIRY:
				window->get_dev_open_notify((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_ENTER_PROGRAM:
				window->enterProgramWidget();
				break;

			case T_UI_EXIT_PROGRAM:
				window->exitProgramWidget();
				break;
				
			case T_UI_METAL_BUTTON:
				syslog(LOG_INFO|LOG_LOCAL1,"receive T_UI_METAL_BUTTON\n");
				window->set_light_status((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_METAL_BUTTON_PRESS:
				window->set_light_button_press((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_METAL_BUTTON_RELEASE:
				window->set_light_button_release((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_NET_STATE:				
				window->get_net_state((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case F_UI_QT_WAKE_UP:
				window->get_wake_up_msg((char *)&msg_in.data[4], msg_in.length-4);
				break;
				
			case T_UI_DISPLAY_SSID:
				//qDebug()<<"get arm_msg T_UI_DISPLAY_SSID";
				window->get_ssid_info((char *)&msg_in.data[4], msg_in.length-4);				
				break;

			case T_UI_SEND_LOCK_STATUS:
				//qDebug()<<"get arm_msg T_UI_SEND_LOCK_STATUS,len="<<msg_in.length<<endl;
				window->get_lock_status((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_DALI_AUTO_ADDR_RESP:
				//g_dali_set_flag = 0;
				qDebug()<<"clear g_dali_set_flag="<<g_dali_set_flag;
				window->get_dali_info((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_CYCLE_LIGHT_DIM:
#if 1
				current_time = GetTickCount();
				ENNO_LOG_ERR("T_UI_CYCLE_LIGHT_DIM current_time=%lld, last_time=%lld, ms=%lld\n", current_time, last_dim_time, current_time - last_dim_time);
				if (current_time - last_dim_time <= slider_speed_time)
				{
				    //ENNO_LOG_ERR("sleep %lld ms\r\n", current_time - last_dim_time);
				    //usleep((current_time - last_dim_time)*1000);
				    //ENNO_LOG_ERR("sleep ms end");
					break;
				}
				last_dim_time = current_time;
#endif
				window->set_cycle_light_dim((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case F_UI_LIGHT_ON:
				pst_msg = (CTL_DEVICE_CONF *)&msg_in.data[4];
				
			   // qDebug()<<"get arm_msg F_UI_LIGHT_ON,id="<<pst_msg->device_id<<",cid="<<pst_msg->cid<<",val="<<pst_msg->value<<endl;
				window->update_light_on((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case F_UI_LIGHT_STATUS:
				//pst_msg = (CTL_DEVICE_CONF *)&msg_in.data[4];
				//ENNO_LOG_ERR("F_UI_DEVICE_STATUS\r\n");
				//window->update_device_status((char *)&msg_in.data[4], msg_in.length-4);
				//ENNO_LOG_ERR("F_UI_DEVICE_STATUS return\r\n");
				window->update_light_status((char *)&msg_in.data[4], msg_in.length-4);
				break;
				
			default:
				//printf("[qt_error]unknown msg received = 0x%x\n", msg_id);
				break;
			}
		}
		else
		{
		    qDebug()<<"[socket_error receive]func="<<__func__<<", line="<<__LINE__<<"bytes="<<socket->bytesAvailable();
		    return;
		}
    }
	
}
void WThread::readData()
{

  //  printf("read data\n");
	ST_MSG_TYPE msg_in;
	QByteArray dataArray;
	int i;
	int msg_id;
	qint32 tmp;

    // 读取接收到的数据
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    while (socket->bytesAvailable() >= sizeof(qint32))
    {
        if(socket->bytesAvailable() >= sizeof(qint32))
            in>>msg_in.head;
		if (msg_in.head != UI_TO_QT_HEAD_FLAG)
		{
		    //qDebug()<<"receive data="<<msg_in.head;
		    return;
		}
		else
		{
		    //qDebug()<<"receive msg head\n";
		}

		if (socket->bytesAvailable() >= sizeof(qint32))
		{
		    in>>tmp;
			msg_in.length = CHANGE_TO_LITTLE_ENDIAN(tmp);
		    //qDebug()<<"msg_in.length :"<<msg_in.length<<"bytevailable :"<<socket->bytesAvailable();
		}
		else
		{
		    //qDebug()<<"[error]func="<<__func__<<__LINE__;
		    return;
		}

		if ((socket->bytesAvailable() >= msg_in.length) && (msg_in.length != 0))
		{
		    int bytesRead = in.readRawData((char *)msg_in.data, msg_in.length);
			dataArray = QByteArray((char *)msg_in.data);
			//for (i = 0; i < 4; i++)
			//{
			//	printf("data:0x%x\n", msg_in.data[i]);
			//}
			msg_id = msg_in.data[0] + (msg_in.data[1]<<8) + (msg_in.data[2]<<16) + (msg_in.data[3]<<24);
			//printf("ui_msg_id rev = 0x%x\n", msg_id);
			switch(msg_id)
			{
			case T_UI_DISPLAY_PAIRID:
				qDebug()<<"pair msg recv in "<<"readData"<<endl;
				window->pairdispWidget((char *)&msg_in.data[4]);
				break;

			case T_UI_CLR_PAIRID:
				//window->pairclrWidget();
				break;


            case T_UI_SET_SCENE_DIM:
				window->set_dim_value((char *)&msg_in.data[4], msg_in.length-4);
				break;

			case T_UI_SCENE_UPDATE:
				window->get_scene_status((char *)&msg_in.data[4], msg_in.length-4);
				break;
				
			default:
				printf("[error]unknown msg received = 0x%x\n", msg_id);
				break;
			}
		}
		else
		{
		    //qDebug()<<"[error]func=\n"<<__func__<<__LINE__;
		    return;
		}
    }
	
}

void WThread::setWindowInstance(MainWidget *wnd)
{
    window = wnd;
	pThread->setWindowInstance(window);
	pThread->start();
}

//void WThread::setMyWidInstance(MyWidget *wnd)
//{
//    myWid = wnd;
//}


