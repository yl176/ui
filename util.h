#ifndef UTIL_H
#define UTIL_H
#include <unistd.h>  
#include <fcntl.h>  

#include <QSettings>
#include <QFontMetrics>

#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QString>
#define Q_OS_LINUX 1

#if defined(Q_OS_LINUX)
#include "sys/statfs.h"
#else
#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"Psapi.lib")
#include <windows.h>
#include <tlhelp32.h>
#include<direct.h>
#include<winternl.h>
#include <psapi.h>
//#include <atlconv.h>
#include <cmath>
#include <string.h>
#endif

//#include "common.h"

#define DEFAULT_SKIN            "skin/17_big"
class Util
{

public:

	static bool writeInit(QString path, QString user_key, QString user_value);
	static bool readInit(QString path, QString user_key, QString &user_value);
	static bool updateText(QString text, int max_width, QString &elided_text);
	static QString getSkinName();
	static void UI_motor_init(void);
	static void UI_motor_send(unsigned char addr, unsigned char dat);
	static void UI_motor_shake(unsigned char type);

};

class MySysInfo : public QObject
{
    Q_OBJECT
public:
    explicit MySysInfo(QObject *parent = NULL);
private slots:
    void GetResource();
public:
    bool GetMemUsage(double &nMemTotal,double &nMemUsed);
    bool GetNetUsage();
    bool GetDiskSpeed();
    bool GetCpuUsage(double &nCpuRate);
    bool GetdiskSpace(unsigned long &lFreeAll,unsigned long &lTotalAll);
    bool GetPathSpace(const QString & path);
private:
    const int m_timer_interval__ = 10000;
    QTimer monitor_timer__;
    double m_send_bytes__ = 0;
    double m_recv_bytes__ = 0;
    double m_disk_read__ = 0;
    double m_disk_write__ = 0;
    double m_cpu_total__ = 0;
    double m_cpu_use__ = 0;
};


#endif
