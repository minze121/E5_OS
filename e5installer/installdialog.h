#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include "CInstallThread.h"
#include <QTextEdit>
#include <QComboBox>
#include <QTimer>
#include <QLabel>

namespace Ui {
class InstallDialog;
}

class InstallDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InstallDialog(QWidget *parent = 0);
    ~InstallDialog();
    
private:
    Ui::InstallDialog *ui;

private slots:
    void StartInstall();
    void StopInstall();
    void OnNotify(int i);
    void outputSlot(QByteArray);
    void updateTime();
    void mountInfo(QString);

signals:
    void updateLinuxVersion(int);
    void updateDiskCapacity(int);
    void updateOSVersion(int);

private:
    QLabel *m_logo;
    QLabel *m_ProgressHint;
    QTextEdit * m_output;
    QProgressBar *m_ProgressBar;
    QPushButton *m_StartBtn;
    QPushButton *m_StopBtn;
    CInstallThread m_Thread;
    QComboBox * m_linuxVersionCombo;
    QComboBox * m_diskCapacityCombo;
    QComboBox * m_OSVersion;
    QTimer timer;
    QLabel * m_minuteLabel;
    QLabel * m_secondLabel;
    QLabel * m_middle;

    QLabel * m_mountLabel;
    QLabel * m_objectLabel;

    static int minute;
    static int second;

    QString fontTemplate;
};

#endif // INSTALLDIALOG_H
