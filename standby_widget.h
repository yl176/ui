#ifndef STANDBY_WINGET_H
#define STANDBY_WINGET_H

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

#define MAX_OUTDOOR_TEMP 80
#define MIN_OUTDOOR_TEMP 0

#define MAX_OUTDOOR_AQI 2000
#define MIN_OUTDOOR_AQI 0

//#define NEW_LAKE 1
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

//#define VOC_STAND_VALUE  200

#define VOC_GOOD_VALUE 100
#define VOC_BAD_VALUE  250

#define AIR_CONDITION_SHOW 108

#define MAX_DEV_NUM_SHOW (DEV_P2-DEV_DALI_LIGHT+1)

#define BITMAP_MASK ((1<<MAX_DEV_NUM_SHOW)-1)

class StandbyUpWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StandbyUpWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
	void get_sensor_value(char *value, uint32 len_buf);
	void show_temp_value(int temp_val);
	void update_sensor_value();
	void update_color();
    void gettime();
	void indoor_data_save_ini(int c_id, unsigned int val);

private:
	//MainWindow *ui;
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;
	QLabel *label_temp_decade;
	QLabel *label_temp_decade_brother;
	QLabel *label_temp_bits;
	QLabel *label_temp_bits_brother;

	QLabel *label_temp_negative_flg;

	QLabel *label_time_hour;
    QTimer *timer_update;
    QTimer *timer_check;

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
	CTL_MSG_CONF msg_bak;

	//QAxWidget *axWidget;
	QToolButton *openButton;
	QToolButton *playPauseButton;
	QToolButton *stopButton;
	QSlider *seekSlider;
	QString fileFilters;
	int updateTimer;
	QWidget *widget;
	QWidget *right_center_function_widget;
	QLabel *function_label;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
	QString skin_name;
	QLabel *label_icon_tvoc;
	QLabel *label_font_tvoc;
	QLabel *label_icon_humi;
	QLabel *label_font_humi;
	QLabel *status_widget;
	QWidget *temp_font_widget;

	int humidity;
	int temperature;
	double gas;
	QParallelAnimationGroup *group1;
	QParallelAnimationGroup *group_temp = NULL;
	QGraphicsColorizeEffect *effect_color = NULL;
	QPropertyAnimation *anim_color =NULL;
	QPropertyAnimation *anim_tvoc = NULL;
	QPropertyAnimation *anim_humi = NULL;
	int aqi_hide_flag = 0;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:
    void onTimerUpdate();
    void onTimerCheck();
	//void showWidget();

private slots:
	
	//void changeLanguage();
	//void changeSkin(QString skin_name);

};

class StandbyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StandbyWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
	void get_outdoor_value(char *value, uint32 len_buf);
	void get_dev_open_notify(char *value, uint32 len_buf);
	void outdoor_info_data_set();
	void outdoor_data_save_ini(int c_id, unsigned int val);
	
	void outdoor_data_update_from_ini();
	QString get_aqi_style(int valAqi);

private:
    //MainWindow *ui;
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;
	QLabel *label_temp_font;
	QLabel *label_aqi_data;
	QLabel *label_aqi_font;
	
	QParallelAnimationGroup *group1;

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
	QLabel *function_label;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
	int aqi_hide_flag = 0;
    QString skin_name;
	QLabel *label_weather;
	QLabel *label_weather_font;
	QLabel *label_open_font;
	QLabel *label_background;
	QLabel *label_open[MAX_DEV_NUM_SHOW];
	QLabel *label_open1[MAX_DEV_NUM_SHOW];
	QLabel *label_open2[MAX_DEV_NUM_SHOW];
	
	QPropertyAnimation *anim[MAX_DEV_NUM_SHOW];
	QWidget *open_widget;
	
	QWidget *open_widget_list1;
	QWidget *open_widget_list2;

	int weather_id;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();

private slots:
	
	void changeLanguage();
	//void changeSkin(QString skin_name);

};

#endif // MAINWINDOW_H
