/****************************************************************************
**
** Copyright (C) EDAN Instruments, INC.  2013, 1014, 2015
** All rights reserved.
**
** Project: E5
** Date:    2014-5-10
** Coder:   Xia Aijun
** Description: E5 application installer
**
** Modify History:
**    2014-5-10   Xiaaijun   Create.
**
****************************************************************************/
#include <QtCore>
#include "CInstallThread.h"
#include <QProcess>
#include <QDebug>

#define LINUX_3_8_13 0
#define LINUX_3_10_18 1

void  CInstallThread::run()
{
    if (mountedDisk.contains("sda")) // /dev/sda is mounted
    {
        emit notify(INSTALL_PROCESS_PARTITION);
        execute(QString(" dd if=/dev/zero of=/dev/sdb bs=512 count=1"));
        execute(QString(" sfdisk --force /dev/sdb < /opt/e5-partition-hd.sfdisk"));

        emit notify(INSTALL_PROCESS_FORMAT);

        //boot partition
        execute(QString(" umount /dev/sdb1"));
        execute(QString(" mkfs.ext4 /dev/sdb1"));

        //system partition
        execute(QString(" umount /dev/sdb2"));
        execute(QString(" mkfs.ext4 /dev/sdb2"));

        //home partition
        execute(QString(" umount /dev/sdb5"));
        execute(QString(" mkfs.ext4 /dev/sdb5"));

        //data partition
        execute(QString(" umount /dev/sdb6"));
        execute(QString(" mkfs.ext4 /dev/sdb6"));

        emit notify(INSTALL_PROCESS_BOOTLODER);

        execute(QString(" mount /dev/sdb1 /mnt"));
        execute(QString(" grub-install --no-floppy --force --boot-directory=/mnt /dev/sdb"));

        if (linuxversion == LINUX_3_8_13)
        {
            execute(QString(" cp -rf /opt/3.8.13/boot/* /mnt/"));
	   
            if (OSVersion == DEVELOP_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.8.13/system.img of=/dev/sdb2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.8.13/home.img of=/dev/sdb5 bs=5M"));
            }
            else if (OSVersion == USER_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.8.13/system.img.user of=/dev/sdb2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.8.13/home.img.user of=/dev/sdb5 bs=5M"));
            }

            emit notify(INSTALL_PROCESS_FINISH);
        }
        else if (linuxversion == LINUX_3_10_18)
        {
            execute(QString(" cp -rf /opt/3.10.18/boot/* /mnt/"));

            if (OSVersion == DEVELOP_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.10.18/system.img of=/dev/sdb2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.10.18/home.img of=/dev/sdb5 bs=5M"));
            }
            else if (OSVersion == USER_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.10.18/system.img.user of=/dev/sdb2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.10.18/home.img.user of=/dev/sdb5 bs=5M"));
            }

            emit notify(INSTALL_PROCESS_FINISH);
        }
    }
    else if (mountedDisk.contains("sdb")) // /dev/sdb is mounted
    {
        emit notify(INSTALL_PROCESS_PARTITION);
        execute(QString(" dd if=/dev/zero of=/dev/sda bs=512 count=1"));
        execute(QString("  sfdisk --force /dev/sda < /opt/e5-partition-hd.sfdisk"));

        emit notify(INSTALL_PROCESS_FORMAT);

        //boot partition
        execute(QString(" umount /dev/sda1"));
        execute(QString(" mkfs.ext4 /dev/sda1"));

        //system partition
        execute(QString(" umount /dev/sda2"));
        execute(QString(" mkfs.ext4 /dev/sda2"));

        //home partition
        execute(QString(" umount /dev/sda5"));
        execute(QString(" mkfs.ext4 /dev/sda5"));

        //data partition
        execute(QString(" umount /dev/sda6"));
        execute(QString(" mkfs.ext4 /dev/sda6"));

        emit notify(INSTALL_PROCESS_BOOTLODER);

        execute(QString(" mount /dev/sda1 /mnt"));
        execute(QString(" grub-install --no-floppy --force --boot-directory=/mnt /dev/sda"));

        if (linuxversion == LINUX_3_8_13)
        {
            execute(QString(" cp -rf /opt/3.8.13/boot/* /mnt/"));

            if (OSVersion == DEVELOP_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.8.13/system.img of=/dev/sda2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.8.13/home.img of=/dev/sda5 bs=5M"));
            }
            else if (OSVersion == USER_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.8.13/system.img.user of=/dev/sda2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.8.13/home.img.user of=/dev/sda5 bs=5M"));
            }

            emit notify(INSTALL_PROCESS_FINISH);
        }
        else if (linuxversion == LINUX_3_10_18)
        {
            execute(QString(" cp -rf /opt/3.10.18/boot/* /mnt/"));
            if (OSVersion == DEVELOP_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.10.18/system.img of=/dev/sda2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.10.18/home.img of=/dev/sda5 bs=5M"));
            }
            else if (OSVersion == USER_OS)
            {
                emit notify(INSTALL_PROCESS_IMAGE_SYSTEM);
                execute(QString(" dd if=/opt/3.10.18/system.img.user of=/dev/sda2 bs=5M"));

                emit notify(INSTALL_PROCESS_IMAGE_HOME);
                execute(QString(" dd if=/opt/3.10.18/home.img.user of=/dev/sda5 bs=5M"));
            }

            emit notify(INSTALL_PROCESS_FINISH);
        }
    }

     emit notify(INSTALL_PROCESS_FINISH);
}

void CInstallThread::execute(QString cmd){

    QProcess passwdProcess;

    QString command="echo 'abc123' | sudo -S ";
    command.append(cmd);

    // print the command
    QString outputCommand="Command  ";
    outputCommand.append(cmd);
    emit notifyOutput(QByteArray(outputCommand.toAscii()));

    //execute the command
    passwdProcess.execute("sh",QStringList()<<"-c"<<command);

    // print the error after execute the command
    QByteArray error = passwdProcess.readAllStandardError();
    emit notifyOutput(error);
}

