#include "util.h"
#include "common.h"

int g_Drv2624DriverFD = 0;
#define DRV2624YFF_VIBRATOR "/dev/VIBRATOR-DRV2624"

bool Util::writeInit(QString path, QString user_key, QString user_value)
{
	if(path.isEmpty() || user_key.isEmpty())
	{
		return false;
	}
	else
	{
		//创建配置文件操作对象
		QSettings *config = new QSettings(path, QSettings::IniFormat);

		//将信息写入配置文件
		config->beginGroup("config");
		config->setValue(user_key, user_value);
		config->endGroup();

		return true;
	} 
}

bool Util::readInit(QString path, QString user_key, QString &user_value)
{
	user_value = QString("");
	if (path.isEmpty() || user_key.isEmpty())
	{
		return false;
	}
	else
	{
		//创建配置文件操作对象
		QSettings *config = new QSettings(path, QSettings::IniFormat);

		//读取用户配置信息
		user_value = config->value(QString("config/") + user_key).toString();

		return true;
	}  
}

bool Util::updateText(QString text, int max_width, QString &elided_text)
{
	elided_text = QString("");
	if(text.isEmpty() || max_width <= 0)
	{
		return false;
	}

	QFont ft;
	QFontMetrics fm(ft);
	elided_text = fm.elidedText(text, Qt::ElideRight, max_width);

	return true;
}

QString Util::getSkinName()
{
	QString skin_name = DEFAULT_SKIN;
	bool is_read = Util::readInit(QString("./user.ini"), QString("skin"), skin_name);

	return skin_name;
}

void Util::UI_motor_init(void)
{	
	g_Drv2624DriverFD = open(DRV2624YFF_VIBRATOR, O_RDWR);	
	if (g_Drv2624DriverFD < 0)	
	{		
		printf(" open -> %s device error.\n",DRV2624YFF_VIBRATOR);		
		return; 
	//maybe node descriptor error	
	}	
	printf("Open ->%s OK! \n",DRV2624YFF_VIBRATOR);
}

void Util::UI_motor_send(unsigned char addr, unsigned char dat)
{	
	unsigned char buf[3];		
	buf[0] = addr;	
	buf[1] = dat;	
	write( g_Drv2624DriverFD, buf, 2);
}

void Util::UI_motor_shake(unsigned char type)
{
    if( g_Drv2624DriverFD == 0 )		
		return;		
	switch(type)
	{
	case 1:
		UI_motor_send(0x05, 0xff);//0x40
		UI_motor_send(0x05, 0xff);//0x40
		UI_motor_send(0x05, 0xff);//0x40
		break;
		
	case 2:
		UI_motor_send(0x05, 0xff);
		UI_motor_send(0x05, 0xff);
		UI_motor_send(0x05, 0xff);
		UI_motor_send(0x05, 0xff);
		UI_motor_send(0x05, 0xff);
		break;
		
	case 3:
		UI_motor_send(0x03, 0xff);//0x40
		break;
		
	default:
		UI_motor_send(0x03, 0xff);//0x40
		break;
	}
	//UI_motor_send( 0x03, 0xff);//0x40
}

MySysInfo::MySysInfo(QObject *parent) : QObject(parent)
{
    connect(&monitor_timer__, &QTimer::timeout, this, &MySysInfo::GetResource);
    monitor_timer__.start(m_timer_interval__);
}
void MySysInfo::GetResource()
{
    double nCpuRate = 0;
    GetCpuUsage(nCpuRate);
    GetDiskSpeed();
    double nMemTotal;
    double nMemUsed;
    GetMemUsage(nMemTotal,nMemUsed);
    GetNetUsage();
    unsigned long lFreeAll;
    unsigned long lTotalAll;
    GetdiskSpace(lFreeAll,lTotalAll);
    GetPathSpace("/");
    qDebug()<<"\n";
}
bool MySysInfo::GetMemUsage(double &nMemTotal,double &nMemUsed)
{
#if defined(Q_OS_LINUX)
    QProcess process;
    process.start("free -m"); //使用free完成获取
    process.waitForFinished();
    process.readLine();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");//将连续空格替换为单个空格 用于分割
    QStringList lst = str.split(" ");
    if(lst.size() > 6)
    {
        qDebug("mem total:%.0lfMB free:%.0lfMB",lst[1].toDouble(),lst[6].toDouble());
        nMemTotal = lst[1].toDouble();
        nMemUsed = nMemTotal-lst[6].toDouble();
        return true;
    }
    else
    {
        return false;
    }
#else
    MEMORYSTATUSEX memsStat;
        memsStat.dwLength = sizeof(memsStat);
        if(!GlobalMemoryStatusEx(&memsStat))//如果获取系统内存信息不成功，就直接返回
        {
            return false;
        }
        double nMemFree = memsStat.ullAvailPhys/( 1024.0*1024.0 );
        nMemTotal = memsStat.ullTotalPhys/( 1024.0*1024.0 );
        nMemUsed= nMemTotal- nMemFree;
        qDebug("windows:mem total:%.0lfMB,use:%.0lfMB",nMemTotal,nMemUsed);
        return true;
#endif
}
bool MySysInfo::GetNetUsage()
{
#if defined(Q_OS_LINUX)
    QProcess process;
    process.start("cat /proc/net/dev"); //读取文件/proc/net/dev获取网络收发包数量，再除取样时间得到网络速度
    process.waitForFinished();
    process.readLine();
    process.readLine();
    while(!process.atEnd())
    {
        QString str = process.readLine();
        str.replace("\n","");
        str.replace(QRegExp("( ){1,}")," ");
        QStringList lst = str.split(" ");
        if(lst.size() > 9 && lst[0] == "enp2s0:")
        {
            double recv = 0;
            double send = 0;
            if(lst.size() > 1)
                recv = lst[1].toDouble();
            if(lst.size() > 9)
                send = lst[9].toDouble();
            qDebug("%s 接收速度:%.0lfbyte/s 发送速度:%.0lfbyte/s",lst[0].toStdString().c_str(),(recv - m_recv_bytes__) / (m_timer_interval__ / 1000.0),(send - m_send_bytes__) / (m_timer_interval__ / 1000.0));
            m_recv_bytes__ = recv;
            m_send_bytes__ = send;
        }
    }
#endif
    return true;
}
#if defined(Q_OS_WIN32)
__int64 Filetime2Int64(const FILETIME* ftime)
{
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

__int64 CompareFileTime(FILETIME preTime,FILETIME nowTime)
{
    return Filetime2Int64(&nowTime) - Filetime2Int64(&preTime);
}
#endif
bool MySysInfo::GetCpuUsage(double &nCpuRate)
{
    nCpuRate = -1;
#if defined(Q_OS_LINUX)
    QProcess process;
    process.start("cat /proc/stat");
    process.waitForFinished();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");
    QStringList lst = str.split(" ");
    if(lst.size() > 3)
    {
        double use = lst[1].toDouble() + lst[2].toDouble() + lst[3].toDouble();
        double total = 0;
        for(int i = 1;i < lst.size();++i)
            total += lst[i].toDouble();
        if(total - m_cpu_total__ > 0)
        {
            qDebug("cpu rate:%.2lf%%",(use - m_cpu_use__) / (total - m_cpu_total__) * 100.0);
            m_cpu_total__ = total;
            m_cpu_use__ = use;
            nCpuRate = (use - m_cpu_use__) / (total - m_cpu_total__) * 100.0;
            return true;
        }
    }
#else
    HANDLE hEvent;
        bool res;
        static FILETIME preIdleTime;
        static FILETIME preKernelTime;
        static FILETIME preUserTime;
        FILETIME idleTime;
        FILETIME kernelTime;
        FILETIME userTime;
        res = GetSystemTimes(&idleTime,&kernelTime,&userTime);
        preIdleTime = idleTime;
        preKernelTime = kernelTime;
        preUserTime = userTime;
        hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//初始值为nonsignaled
        WaitForSingleObject(hEvent,500);//等待500毫秒
        res = GetSystemTimes(&idleTime,&kernelTime,&userTime);
        long long idle = CompareFileTime(preIdleTime,idleTime);
        long long kernel = CompareFileTime(preKernelTime,kernelTime);
        long long user = CompareFileTime(preUserTime,userTime);
        nCpuRate =ceil( 100.0*( kernel + user - idle ) / ( kernel + user ) );
        qDebug()<<"windows:CPU use rate:"<<nCpuRate<<"%";
#endif
    return true;
}
bool MySysInfo::GetDiskSpeed()
{
#if defined(Q_OS_LINUX)
    QProcess process;
    process.start("iostat -k -d");
    process.waitForFinished();
    process.readLine();
    process.readLine();
    process.readLine();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");
    QStringList lst = str.split(" ");
    if(lst.size() > 5)
    {
        qDebug("disk read:%.0lfkb/s disk write:%.0lfkb/s",(lst[4].toDouble() - m_disk_read__ ) / (m_timer_interval__ / 1000.0),(lst[5].toDouble() - m_disk_write__) / (m_timer_interval__ / 1000.0));
        m_disk_read__ = lst[4].toDouble();
        m_disk_write__ = lst[5].toDouble();
        return true;
    }
#endif
    return false;
}
bool MySysInfo::GetdiskSpace(unsigned long &lFreeAll,unsigned long &lTotalAll)
{
#if defined(Q_OS_LINUX)
    QProcess process;
    process.start("df -k");
    process.waitForFinished();
    process.readLine();
    while(!process.atEnd())
    {
        QString str = process.readLine();
        if(str.startsWith("/dev/sda"))
        {
            str.replace("\n","");
            str.replace(QRegExp("( ){1,}")," ");
            QStringList lst = str.split(" ");
            if(lst.size() > 5)
                qDebug("挂载点:%s 已用:%.0lfMB 可用:%.0lfMB",lst[5].toStdString().c_str(),lst[2].toDouble()/1024.0,lst[3].toDouble()/1024.0);
            lFreeAll += lst[2].toDouble()/1024.0;
            lTotalAll += lst[3].toDouble()/1024.0+lFreeAll;
        }
    }
#else

    static char path[_MAX_PATH];//存储当前系统存在的盘符
    int curdrive = _getdrive();
    lFreeAll = 0UL;
    lTotalAll = 0UL;
    for(int drive = 1; drive <= curdrive; drive++ )//遍历所有盘符
    {
        if( !_chdrive( drive ) )
        {
            sprintf(path, "%c:\\", drive + 'A' - 1 );
            ULARGE_INTEGER caller, total, free;
            WCHAR wszClassName[_MAX_PATH];
            memset(wszClassName,0,sizeof(wszClassName));
            MultiByteToWideChar(CP_ACP,0,path,strlen(path)+1,wszClassName,
                sizeof(wszClassName)/sizeof(wszClassName[0]));
            if (GetDiskFreeSpaceEx(wszClassName, &caller, &total, &free) == 0)
            {
                qDebug()<<"GetDiskFreeSpaceEx Filed!";
                return false;
            }

            double dTepFree = free.QuadPart/( 1024.0*1024.0 );
            double dTepTotal = total.QuadPart/( 1024.0*1024.0 );
            qDebug()<<"Get Windows Disk Information:"<<path<<"--free:"<<dTepFree<<"MB,--total:"<<dTepTotal<<"MB";
            lFreeAll += ceil(dTepFree);
            lTotalAll += ceil(dTepTotal);
        }
    }
    qDebug("Total disk capacity:%lu MB,Free disk capacity:%lu MB",lTotalAll,lFreeAll);
#endif
    return true;
}
bool MySysInfo::GetPathSpace(const QString & path)
{
#if defined(Q_OS_LINUX)
    struct statfs diskInfo;//需要#include "sys/statfs.h"
    statfs(path.toUtf8().data(), &diskInfo);
    qDebug("%s 总大小:%.0lfMB 可用大小:%.0lfMB",path.toStdString().c_str(),(diskInfo.f_blocks * diskInfo.f_bsize)/1024.0/1024.0,(diskInfo.f_bavail * diskInfo.f_bsize)/1024.0/1024.0);
#endif
    return true;
}

