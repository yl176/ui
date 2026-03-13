#ifndef START_WINGET_H
#define START_WINGET_H

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
class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
    void end_show();
	void translateLanguage();

private:
    //MainWindow *ui;
	QTranslator* translator;
	int language_flag = 0;

	QTimer *m_timer;
	 QGraphicsOpacityEffect *effect_label;
	QLabel *please_label;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();

private slots:
	void TimerTimeOut();
	void changeLanguage();
	//void changeSkin(QString skin_name);

};
class StartTitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartTitleWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();

private:
    //MainWindow *ui;
	QTranslator* translator;
	int language_flag = 0;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();

private slots:
	
	void changeLanguage();
	//void changeSkin(QString skin_name);

};

#endif // MAINWINDOW_H
