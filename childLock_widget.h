#ifndef CHILDLOCK_WIDGET_H
#define CHILDLOCK_WIDGET_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class childLock_widget;
class childLockBlack_widget;
}

class childLock_widget : public QWidget
{
    Q_OBJECT

public:
    explicit childLock_widget(QWidget *parent = 0);
    ~childLock_widget();
	void translateLanguage();


private:
	QLabel *label_1;

};


//-----------
class childLockBlack_widget : public QWidget
{
    Q_OBJECT

public:
    explicit childLockBlack_widget(QWidget *parent = 0);
    ~childLockBlack_widget();
	

private:
    QLabel *label_1;

};



#endif // MAINWINDOW_H
