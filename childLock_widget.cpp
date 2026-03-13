#include "childLock_widget.h"


childLock_widget::childLock_widget(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	label_1 = new QLabel(tr("The system protection function has been enabled."),this);
	label_1->setObjectName("label_1");
	label_1->setStyleSheet("color: white; font-size: 30px;");
    label_1->setGeometry(QRect(0, 100, OLED_PIX_X, 150));
    label_1->setWordWrap(true);

}

childLock_widget::~childLock_widget()
{
    delete label_1;
}

void childLock_widget::translateLanguage()
{
	label_1->setText(tr("The system protection function has been enabled."));
}



//--------------
childLockBlack_widget::childLockBlack_widget(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);
}

childLockBlack_widget::~childLockBlack_widget()
{

}
