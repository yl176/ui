#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QVBoxLayout>
#include "push_button.h"
#include <QToolButton>
#include <QDebug>
#include <QScrollArea>

#include "common.h"

#include "tool_button.h"
#include "treemodel.h"
#include "treeitem.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsView>

//#include "main_menu.h"

#define MAX_ROOM 32
#define TITLE_MENU_WIDTH OLED_PIX_X
#define TITLE_MENU_HEIGHT 90  //108

extern int g_current_room;
extern int g_room_num;
extern int g_current_title;

typedef enum EN_MOVE_DIRtag
{
    EN_NO_MOVE,
	EN_MOVE_UP,
	EN_MOVE_DOWN,
}EN_MOVE_DIR;


class BIL_SHARE HouseButton : public QPushButton
{
	Q_OBJECT
public:

	explicit HouseButton(QWidget *parent = 0);
	~HouseButton()
	{
	}
	void setIndex(int index);
	void setLevel(int level);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	
signals:
	void pressButton();
	
public:
	QPoint button_press_pos;	
	QPoint button_move_pos;
	int mouse_is_move;
	bool button_on;
	bool mouse_over; 
	bool mouse_press; 
	int page_id;
	int item_id;
	int m_index;
	int m_level;
};

class BIL_SHARE TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(TreeModel *model, QWidget *parent = 0);
	~TitleWidget();
	void translateLanguage(int flag);
	void setButtonLevel(HouseButton *button, int level);
	void setButtonStytle(HouseButton *button, int level);
	void setButtonStytleOffset(HouseButton *button, int level, int offset);
	void paintEvent(QPaintEvent *);
	void set_aqi();

public slots:	
	void press_button_check();
	void switch_to_scene();
	void switch_to_scene_first();
	void scene_show_delay();
	void bottomAniFinished();

signals:

	//void showSkin();
    //void showMin();
	//void showMainMenu();
	//void closeWidget();
	void turnPage(int current_page, int last_page);
	void switch_to_house(int current_room, QString current_name);

public slots:
    void light_button_slot();
	void curtain_button_slot();
	void content_button_slot();	
	void switch_button_slot();
	void press_button_slot();
	void switch_button1_slot();
	void turnPage(QString current_page, int last_page);
	void turnIcon(QString current_icon);
	void scene_button_on();
	void scene_button_off();
	void hvac2scene_button_on();
	void scene2curtain_button_on();
	void hvac2curtain_button_on();
	void curtain_button_off();
	void hvac_button_on();
	void scene2hvac_button_on();
	void hvac_button_off();
	void group_winsFinished();
	void TimerTimeOut();
	void set_room_name();

public:
	void light_button_select();
	void curtain_button_select();
	void havc_button_select();
	void turnRoom(int current_room);
	void return_room_func();
	void selectRoom(int index,int flg);
	void control_lihght_point(int room_id,int status);

private:
	ST_DeviceItem *stRoom;
    ST_DeviceItem astRoom[MAX_ROOM];
    int light_status[MAX_ROOM];

    QPoint press_point;
    bool is_move;
	QParallelAnimationGroup *group_anima;
	
	QParallelAnimationGroup *anim_group;

	QLabel *version_title; 
	QLabel *lable_aqi;
	QLabel *label_scene;
	QLabel *label_house;
	QLabel* label_icon_light;
	int aqi_value;

	QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	int m_mouseMoveCnt;
#if 0
	PushButton* light_button;
	PushButton* curtans_button;
	PushButton* hvac_button;
#endif

	QPushButton* scene_button;
	QPushButton* curtans_button;
	QPushButton* hvac_button;
    QPushButton* switch_button;
	QToolButton* switch_icon;
	QToolButton* switch_button_1;

	QPushButton* current_button;

    QWidget* scene_wid;
	QWidget* curtain_wid;
	QWidget* hvac_wid;
	QWidget* aqi_wid;

	QWidget* wid_scene;
	QWidget* wid_house;
	QWidget* wid_right_icon;

	QPushButton* label_icon_scene;
	QPushButton* label_icon_curtain;
	QPushButton* label_icon_hvac;

	QPushButton* label_icon_scene_big;
	QPushButton* label_icon_curtain_big;
	QPushButton* label_icon_hvac_big;
	
	QLabel *label_bar;
	QLabel *label_bar_s;


    QGridLayout *grid_layout;
	QGridLayout *grid_layout1;
	
	QParallelAnimationGroup *group1;
	QParallelAnimationGroup *group_buttom_up;
	QParallelAnimationGroup *group_buttom_down;
	QParallelAnimationGroup *group_bar;
	QParallelAnimationGroup *group_wins;
	QWidget* wid_get[MAX_ROOM];
	//QWidget* room_widget[MAX_ROOM];
	HouseButton *room_button[MAX_ROOM];

	QTimer *m_timer;
	QGraphicsOpacityEffect *effect_bar;
	QGraphicsOpacityEffect *effect_bars;

    int house_height;
	int room_offset;
	int room_pos_y;
	int room_pos_y_end;
	int room_top;//add by iovine
	EN_MOVE_DIR move_direct;  //0,up 1,down;

	int startx;
	int starty;
	int page_first_room;
	int move_first_room;
	int current_room;
	int room_num;

	QWidget *current_Widget;
	QWidget *next_Widget;
	QWidget *prev_Widget;

	int left_mouse_press;
	int right_mouse_press;

	int light_brightness;

	QList<ToolButton *> button_list;
	
	QList<HouseButton *> button_list_house;

	//QScrollArea *scrollArea;


	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void create_house_widget(TreeModel *model);	
	void create_title_widget();
	void setIconLevel(QPushButton *button, int level, QWidget* wid);
};

#endif // TITLEWIDGET_H


