#ifndef ROOM_SELECT_WINGET_H
#define ROOM_SELECT_WINGET_H

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
#include <QGraphicsBlurEffect>
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
class RoomSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomSelectWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();

	void start_timer();
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
	QLabel *function_label;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;

    QTimer *m_timer;

    QGraphicsOpacityEffect *effect;
    QLabel *room_select_label;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:
	void TimerTimeOut();
	//void showWidget();

private slots:
	void changeLanguage();

	//void changeSkin(QString skin_name);

};

#endif // MAINWINDOW_H
