#ifndef HOUSE_ROOM_WINGET_H
#define HOUSE_ROOM_WINGET_H

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

#include "tool_button.h"
#include "treemodel.h"
#include "treeitem.h"
#define MAX_ROOM 32

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
class HouseRoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HouseRoomWidget(TreeModel *model, QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
	void show_move();
	void create_houseflag_file();

private:
	ST_DeviceItem *stRoom;
    ST_DeviceItem astRoom[MAX_ROOM];
	QPushButton *room_button[MAX_ROOM];
	int room_num;
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
	QWidget* wid_room;
	QLabel *function_label;
	QLabel *hrlabel_house;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	
    QParallelAnimationGroup *group_buttom_scale;
	QList< QPushButton *> button_list_room;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:
	void turnIcon(QString current_icon);
	//void showWidget();

private slots:
	void changeLanguage();
	void show_scene();
	//void changeSkin(QString skin_name);
		
signals:
	void changeRoom(int current_index,int flg);

};

#endif // MAINWINDOW_H
