#ifndef CINSTALLTHREAD_H
#define CINSTALLTHREAD_H

#include <QThread>
#include <QByteArray>
#include <QProcess>

#define LINUX_3_8_13 0
#define LINUX_3_10_18 1

#define DISK_CAPACITY_500G 0
#define DISK_CAPACITY_1T 1

#define DEVELOP_OS 0
#define USER_OS 1

enum {
    INSTALL_PROCESS_PARTITION=0,
    INSTALL_PROCESS_FORMAT,
    INSTALL_PROCESS_BOOTLODER,
    INSTALL_PROCESS_IMAGE_SYSTEM,
    INSTALL_PROCESS_IMAGE_HOME,
    INSTALL_PROCESS_FINISH
};
#include <QDebug>

class CInstallThread : public QThread
{
Q_OBJECT
public:
    CInstallThread()
    {
        linuxversion = LINUX_3_8_13;
        diskCapacity = DISK_CAPACITY_500G;
        OSVersion = DEVELOP_OS;
    }

    void init()
    {
        qDebug()<<"init";
        QProcess process;
        QString command("echo 'abc123' |sudo -S fsck | grep 'sd'");
        process.start("sh",QStringList()<<"-c"<<command);

        process.waitForFinished();
        QByteArray output = process.readAllStandardOutput();

        mountedDisk=QString(output);

        emit notifyMountInfo(mountedDisk);
    }

signals:
   void notify(int state);
   void notifyOutput(QByteArray content);
   void notifyMountInfo(QString);

public slots:
    void   updateLinuxVersion(int value)
    {
        linuxversion = value;
    }

    void   updateDiskCapacity(int value)
    {
        diskCapacity = value;
    }

    void updateOSVersion(int value)
    {
        OSVersion = value;
    }

protected:
  void run();

  void execute(QString cmd);

private:
  int linuxversion;
  int diskCapacity;
  int OSVersion;
  QString mountedDisk;
};
#endif // CINSTALLTHREAD_H
