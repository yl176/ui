#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QDebug>
#include "sleep_widget.h"
#include "qtimer.h"
#include "mainwinget.h"

class Application : public QApplication
{
public:
    Application(int & argc, char ** argv);
    bool notify(QObject *, QEvent *);
    void setWindowInstance(MainWidget* wnd);
	void timer_stop();
	void TimerRestart();

private:
    MainWidget* window;
    QTimer *timer;
    QTimer *timer_off;
    QTimer *timer_set;
};

#endif // APPLICATION_H
