#ifndef CURTAIN_WINGET_H
#define CURTAIN_WINGET_H

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
#include "treemodel.h"
#include "treeitem.h"

#include "content_widget.h"
#include "src/slider_button.h"

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
#define MAX_CURTAIN 128
#define MAX_CURTAIN_PAGE 16
extern int shade_dim_press;

#define MIN_CHINESE_NAME_SIZE 2
#define MID_CHINESE_NAME_SIZE 3
#define MAX_CHINESE_NAME_SIZE 4

typedef enum
{
    RS485_SHADES_DOOYA_ALL = 1,
    RS485_SHADES_DOOYA_DM45Q_JUAN,
    RS485_SHADES_DOOYA_DM45Q_KAIHE,
    RS485_SHADES_BINSHENG,
    RS485_SHADES_PROWELL,
    RS485_SHADES_SILENT_AM95,
    RS485_SHADES_ZONGLIANB,
    RS485_SHADES_SMF,
    RS485_SHADES_WISTAR,
    RS485_SHADES_AOKE,
    RS485_SHADES_ENJOY,
    RS485_SHADES_MENGHUAN,
    RS485_SHADES_MENGHUAN_GENERAL,
    RS485_SHADES_VELUX_KLE100,
}rs485_shades_support_model;

class BIL_SHARE CurtainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurtainWidget(int room_id, TreeModel *model, QWidget *parent = 0);
	~CurtainWidget();
	void translateLanguage();
	void paintEvent(QPaintEvent *);
	enno_result set_dev_update(char *buf_in, uint32 len_buf);
	void snap_shot(QImage **w_image);
	void switch_to_page(int new_page, QImage *new_image);
	void setDuration(int d);
	void next();
    void forward();
	void showContent();
	void lightdim_to_shade_start();
	void set_progressbar_value(int value);
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
    //MainWindow *ui;
    QParallelAnimationGroup *group_light_anima;
	QParallelAnimationGroup *group_shade_anima;
    QPropertyAnimation *animation;
    QParallelAnimationGroup *group_move_widget;
	int duration; 
    bool isAnimation;
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;
	int button_state[MAX_CURTAIN];

	QLineEdit *lineEdit;
	QTextEdit *textEdit;
	QPlainTextEdit *plainTextEdit;
	QTextCodec *codec;
	QTimer *timer_shade_out;

	QHBoxLayout *hboxLayout;
	QVBoxLayout *vboxLayout;
	//QWidget *widget;
	QCalendarWidget *calendarWidget;

	QLineEdit *filename;
	QPushButton *button_file;

	SliderButton *bar;
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
	QList<SceneButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;

	QWidget *content;
	QWidget *widget_tmp;
	QWidget *wid_get[MAX_CURTAIN_PAGE];
	QWidget *wid_get2;
	QLabel *label_scene_onoff[MAX_CURTAIN_PAGE];
	QLabel *label_scene_name[MAX_CURTAIN_PAGE];

    QLabel *label_curtain;
	int last_index;
	int current_index;
	
	int left_mouse_press;
	int right_mouse_press;

	int i_curtain_num;
	int i_curtain_cnt;
	int page_num;

	int startx;
	int starty;

	int current_page;
	int next_page;
	SceneButton *button_select;
	QLabel *label_mask_L;
	QLabel *label_mask_R;

	QLabel *ProgressBar_L;
	QLabel *ProgressBar_R;
	QLabel *ProgressBar_C;

    QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	
	QWidget *current_Widget;
	QWidget *next_Widget;
	QWidget *prev_Widget;
	
	QString cur_button;
    int cur_curtain;
	int target_pos;

	ST_DeviceItem ast_dev[MAX_CURTAIN];
	int last_pos;
	int set_pos;
	int current_pos;
	void paintPrevious(QPainter &, QImage *);
    void paintNext(QPainter &, QImage *);
	QVariant    currentValue;    
    int         widgetCount;        
    int         nextIndex;         
    Direction   direction;         
	QImage *switch_new_image;
	QImage *current_image;
	
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:
	void turnIcon(QString current_icon);
	void onTimerOut();
	void PressShade();
	void shade_pos_check();
    void setLightValue(int);
	void lightdim_to_shade();
	void shade_to_lightdim();
	void shade_turn_onoff();
	void shade_main_back();
	void curtaion_move_end();

private slots:	
	//void changeSkin(QString skin_name);
    void valueChanged_slot(QVariant );
    void animationFinished();

};

#endif // MAINWINDOW_H
