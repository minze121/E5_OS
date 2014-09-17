#include <QApplication>
#include "dialog.h"

#include <QDebug>
#include <QDesktopWidget>

#define SCREEN0 1920
#define SCREEN1 1280
#define SCREEN2 800

int main(int argc,char * argv[])
{
    QApplication app(argc,argv);

    QDesktopWidget * desktopWidget = QApplication::desktop();
    int screenNum = desktopWidget->screenCount();

    if (screenNum == 3)
    {
        QRect rect;
        int i;

        Dialog window0;
        Dialog window1(&window0);
        Dialog window2(&window1);

        qDebug()<<"window0 "<<window0.winId();
        qDebug()<<"window1 "<<window1.winId();
        qDebug()<<"window2 "<<window2.winId();

        for (i=0;i<screenNum;i++)
        {
            rect = desktopWidget->screenGeometry(i);
            qDebug()<<i<<" "<<rect;

            switch(rect.width())
            {
            case SCREEN0:
            {
                window0.move(rect.topLeft());
                window0.resize(rect.width(),rect.height());

                break;
            }
            case SCREEN1:
            {
                window1.move(rect.topLeft());
                window1.resize(rect.width(),rect.height());
                break;
            }
            case SCREEN2:
            {
                window2.move(rect.topLeft());
                window2.resize(rect.width(),rect.height());
                break;
            }
            }
        }

        window0.show();
        window1.show();
        window2.show();

        window2.startTimer();

        return app.exec();
    }
}
