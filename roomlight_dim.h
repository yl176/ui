#ifndef ROOM_LIGHT_DIM_H
#define ROOM_LIGHT_DIM_H

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
#include <QProgressBar>

#include "common.h"

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"

#define MAX_LIGHT_OF_ROOM 128
#define MAX_STEP_INDEX 100
class BIL_SHARE RoomLightDimWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RoomLightDimWidget(int room_id, TreeModel *model, QWidget *parent = 0);
	//void setTranslator(QTranslator* translator);
	void translateLanguage();
	enno_result light_dim(char *buf_in, uint32 len_buf);
	enno_result set_light_dim_value(uint32 val);
	void paintEvent(QPaintEvent *);
	void paintValue(QPainter *painter);
	void paintSide(QPainter *painter);
	void paintOutE(QPainter *painter);
	void drawArc(QPainter *painter, int radius);

private:
    //MainWindow *ui;
    int radiusWidth;            //半径宽度
    int shadowWidth;            //光晕宽度

    QColor textColor;           //文本颜色
    QColor shadowColor;         //光晕颜色
    
	QStackedWidget *statked_widget;
	QPushButton *button;
	char ucButtonFlagA;
    uint32 last_value;

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

	QWidget *main_wid;
	
	uint32 last_average;
	uint32 average;
	uint32 current_average;
	uint32 current_value;
	uint32 paint_value;
	int slider_speed_time;

	int dim_sem;

	int side;
	int outRadis;
	//QSemaphore sem_dim;

	QLabel *ProgressBar_L;
	QLabel *ProgressBar_R;
	QLabel *ProgressBar_C;

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
	QLabel *dis_label;
	QLabel *img_label_back;
	QLabel *img_label;
	QLabel *label_lighton;
	QLabel *label_lightoff;
	QMovie *movie_on;
	QMovie *movie_off;
	QPushButton *more_button;
	QList<QToolButton *> button_list;
	QTranslator* translator;
	int language_flag = 0;
    QString skin_name;
	int i_light_cnt;
	int i_light_num;
	int i_last_val;
	QLabel *dim_text;
	QLabel *dim_percent;
	QProgressBar *dim_bar;
	ST_DeviceItem ast_dev[MAX_LIGHT_OF_ROOM];
	QTimer *timer_dim_out;
	QTimer *timer_dim_flash;
	int r_index;
	int w_index;
	int array_step_buf[MAX_STEP_INDEX];
	int dim_per;
//protected:
//	void paintEvent(QPaintEvent *event);
public slots:
    void onTimerOut();
	void onTimerDimOut();
	void single_send_light_dim();
	void single_dim_paint0();
	
	void single_dim_paint1();

	void single_dim_paint2();

	void single_dim_paint3();

	void single_dim_paint4();

	//void showWidget()
signals:
	void closeRoomlightDim();


private slots:
	//void lighton_close();
	//void lightoff_close();
	void changeLanguage();
	//void changeSkin(QString skin_name);

};



#endif // MAINWINDOW_H
