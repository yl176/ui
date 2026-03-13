#ifndef CONTENT_WINGET_H
#define CONTENT_WINGET_H

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
#include <QProgressBar>
#include "common.h"

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"
#include "treemodel.h"
#include "treeitem.h"
#include "enno_encode.h"
#include "src/slider_button.h"
#include <QGraphicsBlurEffect>

//#include "sliderpicture.h"

#define DEFAULT_SKIN            "skin/17_big"
#define MAX_SCENE_PAGE 16

#define WIDGET_LIGHT_X 10
#define WIDGET_LIGHT_Y 260

#define BUTTON_MOVE_THD 10

#define SCENE_BUTTON_X 100 //110
#define SCENE_BUTTON_Y 100

#define SCENE_DO_TIME 1500

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
//extern class ContentWidget;
extern int light_dim_press;
class BIL_SHARE SceneButton : public QToolButton
{
	Q_OBJECT
public:

	explicit SceneButton(QWidget *parent = 0);
	~SceneButton();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

signals:
	void pressIcon();
	
public:
	QPoint button_press_pos;	
	QPoint button_move_pos;
	int mouse_is_move;
	bool button_on;
	bool mouse_over; 
	bool mouse_press; 
	ST_DeviceItem *stScene;
	ST_DeviceItem *stCurtain;
	int page_id;
	int item_id;
	int m_index;
	int m_pos_x;
	int m_pos_y;
	void sceneCtrl(int on);
	void devCtrl(int set_pos);
	void setImg(QString str_img);
	void setWnd(QWidget *wnd);	
	void setIndex(int index);
	void setLocalPos(int x, int y);
	void getLocalPos(int *p_x, int *p_y);
	QString m_img;
};
class sceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sceneWidget(QWidget *parent = 0);
	
public:
	void paintEvent(QPaintEvent *);
	void setButton(SceneButton *scene_button);
    bool eventFilter(QObject *watched, QEvent  *event);
    void magicTime();
private:
	SceneButton *cur_button;
	//QWidget *img_widget;
	//QStackedWidget *ani_statk_widget;
};

class BIL_SHARE ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(int room_id, TreeModel *model, QWidget *parent = 0);
	~ContentWidget();
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
	enno_result set_dim_value(char *buf_in, uint32 len_buf);
	enno_result get_scene_status(char *buf_in, uint32 len_buf);
	enno_result get_scene_status_byname(char *buf_in, uint32 len_buf);
	void paintPrevious(QPainter &, QImage *);
    void paintNext(QPainter &, QImage *);

protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		//void TouchUpdate(QMouseEvent *event);
/*		void keyPressEvent(QKeyEvent *event);
	
private:
		void moveCurrentPage(bool);
		void setLabelMove(bool);
private slots:
			void changeCurrentPage();
*/
public:
    void paintEvent(QPaintEvent *);
	void switch_to_scene(int last_page);
	void snap_shot(QImage **w_image);
	void switch_to_page(int new_page, QImage *new_image);
	void setDuration(int d);
	void showContent();
	void lightdim_to_scene_start();
	void set_progressbar_value(int value);

protected:
    void scene_off();
	
private:
	QVariant    currentValue;    
	QPropertyAnimation *animation;
	int duration; 
    bool isAnimation;
	Direction   direction;         
	QImage *switch_new_image;
	QImage *current_image;
	ST_DeviceItem ast_dev[MAX_SCENE];
	int i_scene_num;
	int i_scene_cnt;
	TreeModel *model;	
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

	SliderButton *bar;
	QTimer *timer_light_out;
	QLabel *label;
    QImage *img;
    QTimer *timer_exe;

	//QAxWidget *axWidget;
	QToolButton *openButton;
	QToolButton *playPauseButton;
	QToolButton *stopButton;
	QSlider *seekSlider;
	QString fileFilters;
	int updateTimer;
	QWidget *widget;
	
	QWidget *widget_tmp;
	QWidget *right_center_function_widget;
	QLabel *function_label;
	QPushButton *more_button;
	QList<SceneButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	int button_state[16+16];
	int last_state[16+16];
	
    QLabel *total_label;
	sceneWidget *wid_get[MAX_SCENE_PAGE];
	sceneWidget *wid_get1;
	sceneWidget *widget_light;
	sceneWidget *wid_get_wait;
	QSignalMapper *signal_mapper;
	QLabel *label_scene_onoff[MAX_SCENE_PAGE];
	QLabel *label_scene_name[MAX_SCENE_PAGE];
	QLabel *label_scene_first_name[MAX_SCENE_PAGE];
	//QSignalMapper *signal_mapper_light;
	QWidget *content;
	int wait_for_status;
	int do_scene_flag;
	int page_num;

	QLabel *label_light;
//	QLabel *label_mask_L;
//	QLabel *label_mask_R;

	QLabel *ProgressBar_L;
	QLabel *ProgressBar_R;
	QLabel *ProgressBar_C;

	QGridLayout *grid_layout;
	QGridLayout *grid_layout1;
	QGridLayout *grid_layout2;

	int startx;
	int starty;
	int startA;
	int startB;
	int startBak;

	QWidget *current_Widget;
	QWidget *next_Widget;
	QWidget *prev_Widget;

	int current_page;
	int next_page;
	
	QString cur_button;

	int left_mouse_press;
	int right_mouse_press;
	int light_flag;

	int light_brightness;
	int last_dim;
	int dim_value;
	int last_index;
	int current_index;
	QImage *new_image;	
	//QGraphicsBlurEffect *img_effect;

	SceneButton *button_select;
	QParallelAnimationGroup *group_scene_anima;
	QParallelAnimationGroup *group_light_anima;
	QParallelAnimationGroup *group_move_widget;

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
    void onTimerOut();
	//void showWidget();
	void turnIcon(QString current_icon);
	void PressIcon();
	void onTimerExe();
	void light_dim_check();
	void scene_main_back();
	void scene_turn_onoff();
	void valueChanged_slot(QVariant );
    void animationFinished();
	void next();
    void forward();
	void scene_to_lightdim();
	void lightdim_to_scene();
	void scene_move_end();

private slots:
	void buttonClicked();
	void changeLanguage();
	void setLightValue(int);
	//void changeSkin(QString skin_name);
//private:
//	QList<ToolButton *> button_list;
private:
	QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	
public:
	QImage *blur(QImage * origin);
	void light_on_paint();
	void scene_back();
    //void paintEvent(QPaintEvent *);
};



#endif // MAINWINDOW_H
