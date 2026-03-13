#ifndef _QRGEN_H_
#define _QRGEN_H_
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>  
#include <time.h>
#include <cstring>  
#include <math.h> 
#include <fstream> 
#include <streambuf> 
#include <unistd.h>
#include "common.h"   

#define SETUP_ID_LEN 4+1
#define SETUP_CODE_LEN 8+1

#define INIT_URI "http://www.enno.com/cn/cozy.html"
#define MAX_LEN_URI 128

extern char g_init_uri[MAX_LEN_URI];

using namespace std;  

typedef enum
{
    lock_text,
	pair_text
}text_type;

typedef enum
{
   qr_lock,
   qr_pair
}qrType;

class BIL_SHARE QqrWidget : public QWidget
{
    Q_OBJECT

public:
    QqrWidget();
	void doQrgen();
	void doflutterQrgen();
	void GetSetupId(char *str);
	void GetSetupCode(char *str);

	void doQrgenInit();

private:	
	char getStrFromInt(int value);
	int getIntFromStr(char value);
	char qiuyu(string &num, int M, int N);
	string baseConvert(string num, int M, int N);  
	void DToB(long n,int len);	
	char *decToBin(long num,int* len);
	int make_rand_setup_code(long* setupcode);
    QLabel *pair_label;
	QToolButton *tool1_button;
	QToolButton *tool_button_qr;
	char setup_id[SETUP_ID_LEN];
	char setup_code[SETUP_CODE_LEN];
	int qrFlag;
	
protected:
	void mousePressEvent(QMouseEvent *event); 

signals:
	void clearPairCode();
private slots:
	void qr_clicked(void);
	
};

class BIL_SHARE StartWordWidget : public QWidget
{
    Q_OBJECT

public:
    StartWordWidget();
	virtual ~StartWordWidget();
	void translateLanguage(void);
	void setText(void);
	void setTextType(int textType);

private:	
    QLabel *wordFirstLine;
	QLabel *wordSecondLine;
	QLabel *wordThirdLine;

	int m_enTextType;
	
protected:

signals:
private slots:
	
};


#endif

