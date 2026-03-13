#ifndef LIGHT_WINGET_H
#define LIGHT_WINGET_H

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

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"
#include "common.h"

class BIL_SHARE LightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LightWidget(QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
    void get_dim_value(char *value, uint32 len_buf);
	void paintEvent(QPaintEvent *e);

private:
    //MainWindow *ui;
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;
	
	QWidget *widget_light;

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
	int light_value;
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
	
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:

	//void showWidget();

private slots:
	
	void changeLanguage();
	//void changeSkin(QString skin_name);

};

#endif // MAINWINDOW_H
