#ifndef ROOM_LIGHT_H
#define ROOM_LIGHT_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QWidget>
#include <QCalendarWidget>
#include <QProgressBar>
#include <QTimer>
//class MainWindow;
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QImage>

//#include <QAxWidget>
#include <QToolButton>
#include <QSlider>
#include <QWidget>
#include <QDialog>
#include <QMouseEvent>
#include <qmath.h>
#include <QTranslator>
#include <QStackedWidget>
#include "common.h"

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"

#define MAX_LIGHT_OF_ROOM 128

#define NUM_OF_LIGHT_BEFORE_MOIVE 10
/*
#define DEFAULT_SKIN            "skin/17_big"

Q_ENUMS(ReadyStateConstants)

enum PlayStateConstants 
{
	Stopped = 0,
	Paused = 1,
	Playing = 2
};

enum ReadyStateConstants
{
    Uinnitialized = 0,
	Loading = 1,
	Interactive = 3,
	Complete = 4
};
*/
class Worker : public QObject
{
	Q_OBJECT
public:
	explicit Worker(QObject *parent = NULL)
	{
		Q_UNUSED(parent)
	}

signals:
	void resultReady(const QVector<QVector<QString> > &result); //返回处理结果信号

public slots:
	void doWork(const QString &cmd) //接收页面命令槽函数
	{
		Q_UNUSED(cmd)
		QVector< QVector<QString> > result;
		for (int i = 0; i < 1000000; ++i)
		{
			QVector<QString> tmp(10);
			tmp[0] = QString::number(i+1);
			tmp[1] = QString("test%1").arg(i+1);
			tmp[2] = tmp[1];
			tmp[7] = "1";
			tmp[8] = "";
			tmp[9] = "";
			result.push_back(tmp);
		}
		usleep(1000*3); //模拟耗时操作
		emit resultReady(result);
	}

private:

};

class BIL_SHARE CDataClass : public QObject
{
	Q_OBJECT
public:
	explicit CDataClass(QObject *parent = NULL)
	{
		Q_UNUSED(parent)
		Worker *worker = new Worker; //定义数据处理类
		worker->moveToThread(&mWorkerThread); //把数据处理类移到线程
		connect(&mWorkerThread, &QThread::finished, worker, &QObject::deleteLater);
		//定义信号槽，把命令发送到线程里面的槽函数
		connect(this, &CDataClass::operate, worker, &Worker::doWork);
		//定义信号槽，接收线程里面发送的结果
		connect(worker, &Worker::resultReady, this, &CDataClass::handleResults);
		mWorkerThread.start(); //开启线程
	}

	~CDataClass()
	{
		mWorkerThread.quit();
		mWorkerThread.wait();
	}

signals:
	//把页面接受的命令，发送到线程里面的槽函数
	void operate(const QString &cmd);
	//把线程里面的处理结果返回给页面
	void operateResult(const QVector<QVector<QString> > &result);

public slots:
	 void handleResults(const QVector<QVector<QString> > &result) //接受线程里面处理结果
	 {
		emit operateResult(result);
	 }

private:
	QThread mWorkerThread; //定义处理线程

};

class BIL_SHARE RoomLightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomLightWidget(int room_id, TreeModel *model, QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
    void lighton();
	void lighton_press();
	void set_light_status(int val);
	void set_light_value(int val);
	int lightNum();
	void lightoff();
	void lightoff_press();
	int updateLightStatus(char *buf_in, uint32 len);
	
	int updateDeviceStatus(char *value, uint32 len_buf);

	void updateStatus(uint32 device_id, uint32 value);

	int isAllLightOff(void);
	
	void setLightStatus(uint32 status);
	int getMetalButtonStatus(void);
	int get_metal_scene_set();
	int get_metal_scene_status();
	
private:
    //MainWindow *ui;
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;

	QLineEdit *lineEdit;
	QTextEdit *textEdit;
	QPlainTextEdit *plainTextEdit;
	QTextCodec *codec;

	QHBoxLayout *hboxLayout;
	QVBoxLayout *vboxLayout;
	//QWidget *widget;
	QCalendarWidget *calendarWidget;

	QLineEdit *filename;
	QPushButton *button_file;

	QProgressBar *bar;
	QTimer *timer;
	QLabel *label;
    QImage *img;

	//QAxWidget *axWidget;
	QToolButton *openButton;
	QToolButton *playPauseButton;
	QToolButton *stopButton;
	QSlider *seekSlider;
	QString fileFilters;
	int updateTimer;
	QWidget *widget;
	QWidget *right_center_function_widget;
	QLabel *dis_label;
	QLabel *img_label_back;
	QLabel *img_label;
	QLabel *label_lighton;
	QLabel *label_lightoff;
	QMovie *movie_on;
	QMovie *movie_off;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	int i_light_cnt;
	int i_light_num;
	uint32 i_light_status;
	uint32 i_room_id;
	ST_DeviceItem ast_dev[MAX_LIGHT_OF_ROOM];
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();
signals:
	void closeRoomlight();


private slots:
	void lighton_close();
	void lightoff_close();
	void changeLanguage();
	void lighton_ctrl();
	void lightoff_ctrl();
	//void changeSkin(QString skin_name);

};

class BIL_SHARE RoomLightTextWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomLightTextWidget( QWidget *parent = 0);
	void translateLanguage();
    void lighton();
	void lighton_press();
	void light_release();
	void nolight();
	void lightoff();
	void lightoff_press();
	
	
private:
	QLabel * labelText;
	QProgressBar *perBar;
	QTimer *timer_bar_out;
	int timer_cnt = 0;

	
public slots:
    
	void onTimerBarOut();

signals:



private slots:

};


#endif // MAINWINDOW_H
