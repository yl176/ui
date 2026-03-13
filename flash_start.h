#ifndef FLASH_START_H
#define FLASH_START_H

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
#define FLASH_FILE "/home/user/ui/qwid/other/start_gif"

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
class FlashStartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlashStartWidget(QWidget *parent = 0);
	void create_flash_file();
	
public slots:
	void moveon_close();
	
signals:
	void flash_singal();
	
private:
    QTimer *timer;
	int img_num;
	int img_turn;
	
	QMovie *movie_on;
	
	QLabel *label_gif;
	QLabel *label_txt;

};

class FlashTextWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FlashTextWidget(QWidget *parent = 0);
	
private:
    QTimer *timer;	
	QMovie *movie_on;
	
	QLabel *label_gif;
	QLabel *label_txt;
};


#endif // MAINWINDOW_H
