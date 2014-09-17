#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget * nextWidget = 0,QWidget *parent = 0);
    ~Dialog();
    
signals:
    void turnToYou();

public slots:
    void moveLogo();
    void startTimer();

private:
    Ui::Dialog *ui;

    QTimer timer;
    QWidget * nextWidget;
    QPushButton * logoButton;
};

#endif // DIALOG_H
