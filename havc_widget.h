#ifndef HAVC_WINGET_H
#define HAVC_WINGET_H

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

#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <unistd.h>
#include "common.h"

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"
#include "treemodel.h"
#include "treeitem.h"
#include "content_widget.h"
#include "switch_widget.h"

//#include "sliderpicture.h"
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
#define MAX_AC_NUM 64
#define MAX_AC_TEMPERATURE 32
#define DB_CMD_LEN 256


//ac param
/*
typedef enum {
    LeftToRight,
    RightToLeft
}Direction;
*/
typedef enum
{
    AC_MODE_OFF = 0,
    AC_MODE_HEAT = 1,
	AC_MODE_COOL = 2,
	AC_MODE_AUTO = 3,
	AC_MODE_DEHUMIDIFY = 4,
	AC_MODE_FAN  = 5,
}AC_TAR_MODE;

typedef enum
{
    AP_MODE_IN_CYCLE  = 0,
    AP_MODE_OUT_CYCLE = 1,
}AP_TAR_MODE;

#ifndef _uint32
#define _uint32 unsigned int
#endif
typedef struct ST_AC_PARAMtag 
{
	_uint32 switch_on;
	_uint32 current_temperature;
	_uint32 target_temperature;   
	_uint32 current_heat_cool_mode;    //"0",inactive; "1",idele; "2",heating; "3","cooling".
	_uint32 target_heat_cool_mode;     //"0",heat or cool; "1",heat; "2",cool.
	_uint32 fan_speed;
	_uint32 temp_uint;
	_uint32 humidity;
	_uint32 mode;
	_uint32 ac_floor_mode;   //0,Air Conditioner  1,Heater or Cooler   2,Heater
	_uint32 PM25;
	_uint32 floor_ctrl_mode;  //0,auto, 1,manu
	_uint32 dev_type;
	_uint32 device_id;
	_uint32 dev_cnt;
	_uint32 vaild;
	QPushButton* button_on;
	QLabel *label_name;
	QLabel *label_img;
	//QToolButton* button_up;
	//QToolButton* button_down;
}ST_AC_PARAM;

class BIL_SHARE HavcWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HavcWidget(int room_id, TreeModel *model, QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	~HavcWidget();
	void translateLanguage();
	void set_button_content(int uid);
	
protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
/*		void keyPressEvent(QKeyEvent *event);
	
private:
		void moveCurrentPage(bool);
		void setLabelMove(bool);
private slots:
			void changeCurrentPage();
*/
public:
    void paintEvent(QPaintEvent *);
	void create_off_widget(QWidget *widget);
	void create_ctrl_widget(QWidget *widget,int index);
	void create_purify_widget(QWidget *widget,int index);
	void create_floor_widget(QWidget *widget,int index);
	void set_widget(Direction dir);
	void set_ac_on_status(int index, uint32 state, QPushButton *button_on);
	void set_fan_speed_status(int index);
	void set_ap_mode_status(int index);
	void set_ac_mode_status(int index);
	void ac_temp_set_status(int index);
	void ap_pm25_set_status(int index);
	enno_result set_dev_update(char *buf_in, uint32 len_buf);
	enno_result set_dev_refresh_ui();
	void switch_to_page(int new_page, QImage *new_image);
	void snap_shot(QImage **w_image);
	void paintPrevious(QPainter &, QImage *);
    void paintNext(QPainter &, QImage *);
	void setDuration(int d);
	void showContent();
	void set_on_animation(int uid,uint32 switch_on);
	void set_on_but_no_animation(int uid,uint32 switch_on);
	void set_floor_auto_man_switch(int uid);

protected:
    void scene_off();
	void ac_init();
private:
	QVariant    currentValue;    
	QPropertyAnimation *animation;
	int duration; 
    bool isAnimation;
	Direction   direction;         
	QImage *switch_new_image;
	QImage *current_image;
    //MainWindow *ui;
    QLabel *label_no_device;
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
	QLabel *function_label;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	int button_state[12];
	//QToolButton *tool_button;
	QToolButton *ac_button;
    QLabel *total_label;
    //QLabel *label_ac;
    int i_dev_cnt;
	int i_dev_num;
	ST_DeviceItem ast_dev[MAX_AC_NUM*3];
	
    //QLabel *label_temp;
	QWidget *wid_get[MAX_AC_NUM*3];
	QWidget *content;

	QWidget *wid_temp_window[MAX_AC_NUM*3];//add by iovine
	QParallelAnimationGroup *group_bits;
	QParallelAnimationGroup *group_move_widget;

	QTimer *m_timer;
	QGraphicsOpacityEffect *effect_ac_mode[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_fan[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_up[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_down[MAX_AC_NUM*3];

	QGraphicsOpacityEffect *effect_ac_d[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_b[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_u[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_n[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_pm[MAX_AC_NUM*3];
	QGraphicsOpacityEffect *effect_ac_hum[MAX_AC_NUM*3];
	QParallelAnimationGroup *group_ac;
	QParallelAnimationGroup *group_anima[MAX_AC_NUM*3];
	
	QWidget *dot_wid;

	QGridLayout *grid_layout;
	QGridLayout *grid_layout1;
	QGridLayout *grid_layout2;

	int startx;
	int starty;

	QWidget *prev_Widget;

	int left_mouse_press;
	int right_mouse_press;

	int light_brightness;
#if 0
    QPoint m_mouseSrcPos;
    QPoint m_mouseDstPos;
	bool mouse_press;
    bool mouse_move;
    bool label_move;
    int current_index;   
    QLabel *total_label;
	//QVector<PixLabel*>label_array;
    int current_pos_x;
#endif
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();
	void turnIcon(QString current_icon);
	void font_move_end();
	void havc_move_end();
	void valueChanged_slot(QVariant );
    void animationFinished();
	void next();
    void forward();

private slots:
	void buttonClicked();
	void changeLanguage();
	void ac_on_show();
	void txtButton();
	void ac_off_show();
	void ac_temp_down();
	void ac_temp_up();
	void set_ac_mode();
	void set_ap_mode();
	void set_ac_mode_off(int uid);
	void set_ac_mode_init(int uid);
	void set_floor_auto();
	void set_floor_man();
	void set_ap_mode_off(int uid);
	void set_ap_mode_init(int uid);
	void set_fan_speed();
	void set_fan_speed_off(int uid);
	void set_fan_speed_init(int uid);
	void ac_off_timer_callback();
	void ac_on_timer_callback();
	//void changeSkin(QString skin_name);
//private:
//	QList<ToolButton *> button_list;
private:
	QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	QPushButton *ac_button_on;
	QToolButton *button_down[MAX_AC_NUM*3];
	QToolButton *button_up[MAX_AC_NUM*3];
	QToolButton *tool_button[MAX_AC_NUM*3];
	int ac_uid;
	int current_uid;
	int next_uid;
	int ac_num;
	int purify_num;
	int floor_num;
	int device_num;
	ST_AC_PARAM stAc[MAX_AC_NUM*3];
	QLabel *pLabel_ac[MAX_AC_NUM*3];

	QLabel *pLabel_temp_decade[MAX_AC_NUM*3];
	QLabel *pLabel_temp_decade_brother[MAX_AC_NUM*3];

	QLabel *pLabel_temp_bits[MAX_AC_NUM*3];
	QLabel *pLabel_temp_bits_brother[MAX_AC_NUM*3];

	QLabel *pLabel_temp_unit[MAX_AC_NUM*3];

	QLabel *pLabel_ac_id[MAX_AC_NUM*3];
	QLabel *pLabel_purify[MAX_AC_NUM*3];
	QLabel *pLabel_PM25[MAX_AC_NUM*3];
	//QLabel *pLabel_humidity[MAX_AC_NUM*3];
	QToolButton *tool_button_fan[MAX_AC_NUM*3];
	QToolButton *tool_button_mode[MAX_AC_NUM*3];

public:
	void light_on_paint();
	int isAllAcClose(void);
	int isAllFloorClose(void);
    //void paintEvent(QPaintEvent *);
};

#endif // MAINWINDOW_H
