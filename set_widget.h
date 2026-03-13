#ifndef SET_WINGET_H
#define SET_WINGET_H

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
#include "common.h"

//#include "util.h"
#include "tool_button.h"
#include "title_widget.h"
#include "mainwinget.h"

#define UP_SET_BTN_HEIGHT 70

class BIL_SHARE SetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetWidget(QWidget *parent = 0);
	void translateLanguage();
	void setWidSlot(MainWidget *wnd);

	

signals:
	void changeLanguage(int flag);

private slots:
	void setfunc(QString current_icon);
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
private:
	QTranslator* translator;
	int language_flag = 0;
	QList<QPushButton *> button_list;
	QPushButton *return_button;
	int button_num;
	QWidget *wid_page;
	int left_mouse_press;
	QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	QPoint last_mouseDstPos;
	int y_pos;
    MainWidget* window;
	QScrollArea *scrollArea;
};

class SetAboutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetAboutWidget(QWidget *parent = 0);
	void translateLanguage();
	void setWidSlot(MainWidget *wnd);
	void setQrwid(QqrWidget *qrwid);
	void doSetMsg(int icon_id);
	void quitSet();
	void get_dev_info(char *value, uint32 len_buf);
	void get_ssid_info(char *value, uint32 len_buf);
	void get_dali_info(char *value, uint32 len_buf);
	void get_net_state(char *value, uint32 len_buf);
	void show_net_state();
	void show_net_wac_state();
    virtual ~SetAboutWidget();
signals:
	void changeLanguage(int flag);
	void retMainwid();	
	void showSetupCode();
	
private slots:
	//void changeLanguage();
	void setfunc(QString current_icon);
	void chineseClicked();
	void englishClicked();
	void trcnClicked();
	void wacClicked();
	void netrebootClicked();
	void netfixClicked();
	void dali_1_all_Clicked();
	void dali_1_new_Clicked();
	void dali_2_all_Clicked();
	void dali_2_new_Clicked();
	void identify_start_Clicked();
	void identify_stop_Clicked();
	void identify_pre_Clicked();
	void identify_next_Clicked();
	void dali_reconfiguration_Clicked();
	void dali_1_addr_reconfiguration_sure_Clicked();
	void dali_1_addr_reconfiguration_cancel_Clicked();
	void dali_2_addr_reconfiguration_sure_Clicked();
	void dali_2_addr_reconfiguration_cancel_Clicked();
	void facreset_sure_Clicked();
	void facreset_cancel_Clicked();
	void programClicked();
	void logClicked();
	void iosClicked();
	void andClicked();
	void setcodeClicked();
	void facClicked();
	void retClicked();
	void onTimerTaskSet();
    void on_pbtn_1_clicked();
    void on_pbtn_2_clicked();

private:
	QqrWidget *qr_widget;
	QLabel *netfix_status;
	QLabel *dali_addr_set_status;
	QLabel *fac_status;
	//QLabel *set_status;
	QLabel *model_status;
	QLabel *version_status;
	QTranslator* translator;
	int language_flag = 0;
	QList<QPushButton *> button_list;
	int button_num;
	QStackedWidget *info_widget;
	EN_NET_STATE net_state;

	QLabel *net_wac_status;

	QTimer *timer_tick_one;
	int timer_count;
	int timer_countfix_net;
	
	QLabel *netfix_title;
	QPushButton *netfix_yes;
	QLabel *dali_addr_set_title;
	QPushButton *dali_1_all;
	QPushButton *dali_1_new;
	QPushButton *dali_2_all;
	QPushButton *dali_2_new;
	QLabel *fac_title;
	QPushButton *fac_yes;
	QPushButton *program_button;
	QPushButton *log_button;
	QPushButton *ios_button;
	QPushButton *and_button;
	//QLabel *setcode_title;
	QLabel *bootmode_title;
	QLabel *ets_title;
	QPushButton *setcode_yes;
	QPushButton *chinese_button;
	QPushButton *english_button;
	QPushButton *tr_cn_button;
	QLabel *language_title;
	QLabel *about_title;

	QPushButton *wac_button;
	QPushButton *reboot_button;
	QLabel *net_wac_title;
	//QLabel *about_title;

	QLabel *identify_title;
	QLabel *identify_status;

	QLabel *dali_reconfiguration_title;
	QLabel *reconfiguration_status;

	QLabel *dali_1_addr_reconfiguration_title;
	QLabel *dali_2_addr_reconfiguration_title;

	QLabel *facreset_sure_title;


    //
	QPushButton *statrt_identify_button;
	QPushButton *stop_identify_button;
	QPushButton *next_light_button;
	QPushButton *previous_light_button;

	QPushButton *dali_reconfiguration_button;

	
	QPushButton *dali_1_addr_reconfiguration_sure_button;
	QPushButton *dali_1_addr_reconfiguration_cancel_button;
	QPushButton *dali_2_addr_reconfiguration_sure_button;
	QPushButton *dali_2_addr_reconfiguration_cancel_button;

	QPushButton *facreset_sure_button;
	QPushButton *facreset_cancel_button;


	
	QLabel *label_devid;
	char ssid_name[SSID_LEN];
	char ip_name[IP_LEN];
	char db_name[16];
	char device_id[DEVICE_ID_LEN];
    char device_version[DEVICE_VERSION_LEN];
    QLabel *labelNetworkName;
    QLabel *labelNetipName;
    QLabel *labelNetdbName;

    QString strNetworkName;
    QString strNetipName;
    QString strNetdbName;
	
    QToolButton* ret_button;

    QLabel *labelNetStatusHead;

    QLabel *labelWacStatusHead;


    QWidget *wid_netfix;
    QWidget *wid_daliset;
    QWidget *wid_facreset;
    QWidget *wid_etsset;
    QWidget *wid_bootmode;
    QWidget *wid_setcode;
    QWidget *wid_language;
    QWidget *wid_about;
	QWidget *wid_netreset;
	QWidget *wid_lights_identify;
	QWidget *wid_dali_reconfiguration;    
	QWidget *wid_dali_1_addr_reconfiguration_sure;
	QWidget *wid_dali_2_addr_reconfiguration_sure;
	QWidget *wid_facreset_sure;

    //
    QWidget *wid_child_lock;
    QPushButton *pbtn_1;
    QPushButton *pbtn_2;
    QLabel *label_1;
    QLabel *label_2;
public:	
	bool b_child_lock_on;

};

extern uint32 g_dali_set_flag;

#endif // MAINWINDOW_H
