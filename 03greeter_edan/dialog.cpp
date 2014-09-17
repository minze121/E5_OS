#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget * nextWidget,QWidget *parent) :
    nextWidget(nextWidget),
    QDialog(parent),
    ui(new Ui::Dialog)
{

    logoButton = new QPushButton(this);

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,Qt::darkGray);
    this->setPalette(palette);

//    QPixmap pixmap(":/logo.bmp");
    QPixmap pixmap(":/logo.jpg");

    this->logoButton->resize(QSize(pixmap.width(),pixmap.height()));

    this->logoButton->setIconSize(QSize(pixmap.width(),pixmap.height()));

     this->logoButton->setIcon(QIcon(pixmap));

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint);
    timer.setInterval(40);

    if (nextWidget)
    {
        connect(this,SIGNAL(turnToYou()),nextWidget,SLOT(startTimer()));
    }

    connect(&timer,SIGNAL(timeout()),this,SLOT(moveLogo()));

    this->logoButton->setVisible(false);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::moveLogo()
{
//    qDebug()<<"LOGO POS:"<<this->logoButton->pos();
    int y = this->logoButton->pos().y()-5;
    if (y<=0)
    {
        y=0;
        if (nextWidget)
        {
            emit turnToYou();
            this->logoButton->setVisible(false);
        }
        else
            QApplication::exit();
        timer.stop();
    }
    this->logoButton->move(this->logoButton->pos().x(),y);
}

void Dialog::startTimer()
{
    qDebug()<<this->objectName()<<" startTimer "<<this->winId()<<" winid "<<this->height()-this->logoButton->height();
    this->logoButton->move((this->width()-this->logoButton->width())/2,
                               this->height()-this->logoButton->height());
    this->logoButton->setVisible(true);
    timer.start();
}
