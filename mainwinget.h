#ifndef MAINWINGET_H
#define MAINWINGET_H

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
#include <QObject>  
#include <QPixmap>  
#include <QSplashScreen>  
#include <QDebug>
#include <QFile>

#include <unistd.h>
#include "common.h"

#include "util.h"
#include "tool_button.h"
#include "title_widget.h"
#include "content_widget.h"
#include "curtain_widget.h"
#include "sleep_widget.h"
#include "update_widget.h"
#include "standby_widget.h"
#include "havc_widget.h"
#include "light_widget.h"
#include "treemodel.h"
#include "enno_encode.h"
#include "qrgen.h"
#include "program_widget.h"
#include "roomlight.h"
#include "start_widget.h"
#include "room_select_widget.h"
#include "room_back_widget.h"
#include "house_room_widget.h"
#include "flash_start.h"
#include "roomlight_dim.h"
#include "childLock_widget.h"
//#include "set_widget.h"

//#include "wthread.h"
//#define START_SCREEN_ENABLE
//#define X64

#ifdef X64
#define OLED_SIZE_X 320
#define OLED_SIZE_Y 640
#elif LAT_6900C
#define OLED_SIZE_X 640
#define OLED_SIZE_Y 320
#else
#define OLED_SIZE_X 440
#define OLED_SIZE_Y 1920
#endif

#define MAX_PAIR_ID_LEN 20

typedef enum 
{
    EN_SYS_NORM = 0,
	EN_SYS_SET,
	EN_SYS_STANDBY,
	EN_SYS_SLEEP,
	EN_SYS_DIM,
	EN_SYS_LOCK,
	EN_SYS_ROOM_LIGHT_DIM,
	EN_SYS_ROOM_LIGHT_ONOFF,
	EN_SYS_MAX,
}EN_SYS_MODE;

//extern WThread *g_thread;

#if 0
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
#endif
extern int g_pairid_flag;

class BIL_SHARE MainWidget : public QDialog
{
    Q_OBJECT

public:	
    MainWidget(QWidget *parent = 0);
	~MainWidget();

	void setTranslator(QTranslator* translator);
	void translateLanguage();
	void pairdispWidget(char *pair_id);
	void pairclrWidget();
	void enno_enter_sleep_mode();
	void enno_enter_standby_mode();
	void enno_enter_set_mode();
	void enterProgramWidget();
	void exitProgramWidget();
	void set_dim_value(char *value, uint32 len_buf);
	void get_scene_status(char *value, uint32 len_buf);
	void get_scene_status_byname(char *value, uint32 len_buf);
	void set_dev_update(char *value, uint32 len_buf);
	void get_sensor_value(char *value, uint32 len_buf);
	void get_outdoor_value(char *value, uint32 len_buf);
	void get_firmware_update_per(char *value, uint32 len_buf);
	void get_dim_value(char *value, uint32 len_buf);
	void get_dev_info(char *value, uint32 len_buf);
	void get_ssid_info(char *value, uint32 len_buf);
	void get_dali_info(char *value, uint32 len_buf);
	void set_cycle_light_dim(char *value, uint32 len_buf);
	void get_net_state(char *value, uint32 len_buf);
	void get_dev_open_notify(char *value, uint32 len_buf);
	void set_light_status(char *value, uint32 len_buf);
	
	void set_light_button_press(char *value, uint32 len_buf);
	void set_light_button_release(char *value, uint32 len_buf);
	void get_wake_up_msg(char *value, uint32 len_buf);
	void create_ui();
	void delete_ui();
	void TimerRestart();
	void setTimer(QTimer *timer_standby_in, QTimer *timer_sleep_in);
	//read data from xml, and refresh the ui
	void refresh_ui(void);

	void get_lock_status(char *value, uint32 len_buf);

	void update_light_on(char *value, uint32 len_buf);
	
	enno_result update_light_status(char *value, uint32 len_buf);

	int isAllClose(int devType);
//public:
//		void paintEvent(QPaintEvent *);

private:
    //MainWindow *ui;
	TreeModel *model;
	QTimer *timer_standby;
	QTimer *timer_sleep;
    QStackedWidget *stack_title;
	QStackedWidget *statked_widget;   //up
	TitleWidget *title_widget; 
	ContentWidget *content_widget[MAX_ROOM]; 
	HavcWidget *havc_widget[MAX_ROOM]; 
	CurtainWidget *curtain_widget[MAX_ROOM]; 
	StandbyUpWidget *standby_content;
	StandbyWidget *standby_title;
	SleepWidget *sleep_widget; 
	SleepWidget *sleep_title;
	UpdateWidget *update_widget;
	LightWidget *light_widget;
	ProgramWidget *program_widget;
	RoomLightWidget *roomlight_widget[MAX_ROOM];
	RoomLightDimWidget *room_light_dim_widget[MAX_ROOM];

    FlashStartWidget *flash_widget;
	FlashTextWidget *flash_text_widget;
	StartWidget *start_widget;
	RoomBackWidget *room_back_widget;
	RoomSelectWidget *room_select_widget;
	HouseRoomWidget *house_room_widget;

	StartTitleWidget *start_title_widget;
	RoomLightTextWidget * lightText_widget;

	UpdateWidget_Pic* logo_widget;
	StartWordWidget * lock_word_widget;
	QLabel *label_err_room;

	int cur_in;
	int prev_up_index;  ///从待机恢复到之前的界面  //
	int prev_down_index;  ///从待机恢复到之前的界面  //
    int pair_index;
	int cur_bottom;

	int mstimer_flag;
	
	QHBoxLayout *h_layout;
	QVBoxLayout *v_layout;
	
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
	QLabel *function_label;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	//SetWidget *set_widget;
	QqrWidget *pair_widget;
	MySysInfo *sys_info;
	QLabel *hkt_label;
	
    QTimer *timer_dim_out;
    
    QTimer *timer_ms;
	
	EN_NET_STATE en_net_state;
	QParallelAnimationGroup * group_upScreen;
	QLabel * tempLabelAni;
	QLabel * tempLabelLeft;
    QWidget * widToSet;
	uint32 boot_mode;
//protected:
//	void paintEvent(QPaintEvent *event);
    //
	childLock_widget *widget_childLock;
    childLockBlack_widget *widget_chldLkBlack;
public slots:
	void showWidget();
	void turnPage(int current_page, int last_page);
	void reviewWidget();
	void showPairWidget();
	void onTimerOut();
	void onTimerOff();
	void onTimerSet();
	void onTimerDimOut();
	void light_on_showoff();
	void light_dim_showoff();
	void enter_room_light_dim();
	void exit_room_light_dim();
	void enter_room_light_onoff();
	void exit_room_light_onoff();
	void exit_room_dim_onoff();
	void flash_end();
	void clearPairWidget();
	void returnRoomWidget();
	void showSwitchRoom(int index, QString room_name);
	void selectRoom(int index,int flg);
	void show_font();
	void show_room();
	void switch_to_scene_slot();
	void switch_to_curtain_slot();
	void switch_to_hvac_slot();
	void switch_UpSreen_Animation(QWidget *widgetOut,QWidget *widgetIn,int moveLeft);
	void msTimerTaskSet();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void create_pair_file();
	enno_result load_boot_mode_status(uint32 *p_status);

//signals:
//	void time_restart();


private slots:
	
	void changeLanguage(int flag);
	
	void changeSkin(QString skin_name);
        
	void upAniFinish(void);

private:
    EN_SYS_MODE en_state;

public:
	EN_SYS_MODE get_state();

};

#endif // MAINWINDOW_H
