#include "installdialog.h"
#include "ui_installdialog.h"
#include <qdebug.h>
#include <QProcess>
#include <QMessageBox>
#include <QTime>
#include <QCoreApplication>

#define WEIGHT 1200
#define HEIGHT 800

#define LINUX_3_8_13 "linux 3.8.13 + Xenomai 2.6.3"
#define LINUX_3_10_18 "linux 3.10.18 + Xenomai 2.6.3"

#define DISK_CAPACITY_500G "CAPACITY : 500G"
#define DISK_CAPACITY_1T "CAPACITY : 1T"

#define DEVELOP_OS "Developer OS"
#define USER_OS "User OS"

int InstallDialog::minute = 0;
int InstallDialog::second = 0;

InstallDialog::InstallDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstallDialog)
{
    ui->setupUi(this);

    //set background color
    QPalette palette(this->palette());
//    palette.setColor(QPalette::Background,QColor(50,120,150));
    palette.setColor(QPalette::Background,QColor(0,0,0));
    this->setPalette(palette);

    fontTemplate = QString("<font color=white>%1</font>");
#if 0
    //To adjust the current resolution
    QDesktopWidget * desktop = QApplication::desktop();

    desktop->screenNumber();
    QRect rect = desktop->screenGeometry(0);
    this->setFixedSize(rect.width(),rect.height());
#else
    this->setFixedSize(WEIGHT, HEIGHT);
#endif

    //logo

    m_logo = this->ui->m_logo;
    m_logo->resize(960,520);
    m_logo->setPixmap(QPixmap(":/res/edan_logo.png"));
    m_logo->setPixmap(QPixmap(":/res/ultra.png").scaled(m_logo->size()));
    qDebug()<<"size "<<m_logo->size();

    // hint text
    m_ProgressHint= this->ui->m_ProgressHint;
    m_ProgressHint->setText(fontTemplate.arg("Install software system to E5 device:"));

    //OS Version
    m_OSVersion = this->ui->m_OSVersion;
    m_OSVersion->addItem(DEVELOP_OS);
    m_OSVersion->addItem(USER_OS);

    //disk capacity
    m_diskCapacityCombo = this->ui->m_diskCapacityCombo;
    m_diskCapacityCombo->addItem(DISK_CAPACITY_500G);
    m_diskCapacityCombo->addItem(DISK_CAPACITY_1T);

    //linux version
    m_linuxVersionCombo = this->ui->m_linuxVersionCombo;
    m_linuxVersionCombo->addItem(LINUX_3_8_13);
  //  m_linuxVersionCombo->addItem(LINUX_3_10_18);

    //output textedit
//    m_output = this->ui->m_output;

    // progerss bar
    m_ProgressBar = this->ui->m_ProgressBar;
    m_ProgressBar->setRange(0, 10);
    m_ProgressBar->setTextVisible(true);
    m_ProgressBar->setValue(0);

    m_minuteLabel= this->ui->m_minuteLabel;
    m_secondLabel= this->ui->m_secondLabel;
    m_middle = this->ui->m_middle;
    m_minuteLabel->setText(fontTemplate.arg("00"));
    m_secondLabel->setText(fontTemplate.arg("00"));
    m_middle->setText(fontTemplate.arg(":"));

    // start button
    m_StartBtn=this->ui->m_StartBtn;
    m_StartBtn->setText(tr("Start"));

    // stop button
    m_StopBtn=this->ui->m_StopBtn;
    m_StopBtn->setText(tr("Stop"));

    // mount info
    m_mountLabel = this->ui->m_mountLabel;
    m_objectLabel = this->ui->m_objectLabel;


    // connect slot to button
    connect(m_StartBtn,SIGNAL(clicked()),this,SLOT(StartInstall()));
    connect(m_StopBtn,SIGNAL(clicked()),this, SLOT(StopInstall()));

    connect(this,SIGNAL(updateLinuxVersion(int)),&m_Thread,SLOT(updateLinuxVersion(int)));
    connect(this,SIGNAL(updateDiskCapacity(int)),&m_Thread,SLOT(updateDiskCapacity(int)));
    connect(this,SIGNAL(updateOSVersion(int)),&m_Thread,SLOT(updateOSVersion(int)));

    connect(&m_Thread,SIGNAL(notifyMountInfo(QString)),this,SLOT(mountInfo(QString)));
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTime()));

#if 0
    QTime dieTime = QTime::currentTime().addSecs(10);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
#endif
    QProcess process;
    process.start("sh",QStringList()<<"-c"<<"echo 'abc123' | sudo -S fdisk -l");
    process.waitForFinished();
    QByteArray output;
    output = process.readAllStandardOutput();
    outputSlot(output);

    //layout
    int startX = (this->width()-m_logo->width())/2;
    int startY = 20;
    int interval = 10;
    m_logo->move(startX,startY);
    startY += m_logo->height() + interval;
    m_ProgressHint->move(startX,startY);

    m_OSVersion->resize(940/3,m_OSVersion->height());
    m_diskCapacityCombo->resize(940/3,m_diskCapacityCombo->height());
    m_linuxVersionCombo->resize(940/3,m_linuxVersionCombo->height());
    startY += m_ProgressHint->height() + interval;
    m_OSVersion->move(startX,startY);
    m_diskCapacityCombo->move(startX+m_OSVersion->width()+10,startY);
    m_linuxVersionCombo->move(startX+m_OSVersion->width()*2+20,startY);

    startY += m_OSVersion->height() + interval;
    m_ProgressBar->resize(960,m_ProgressBar->height());
    m_ProgressBar->move(startX,startY);

    startY += m_ProgressBar->height() + interval;

    m_minuteLabel->resize(30,m_minuteLabel->height());
    m_secondLabel->resize(30,m_secondLabel->height());
    m_middle->resize(20,m_middle->height());
    m_minuteLabel->move((this->width()-100)/2,startY);
    m_middle->move(m_minuteLabel->pos().x()+interval+m_minuteLabel->width(),startY);
    m_secondLabel->move(m_middle->pos().x()+interval+m_middle->width(),startY);

    startY += m_secondLabel->height() + interval;

    m_StartBtn->resize(950/2,m_StartBtn->height());
    m_StopBtn->resize(950/2,m_StopBtn->height());
    m_StartBtn->move(startX,startY);
    m_StopBtn->move(startX+m_StartBtn->width()+interval,startY);

    startY += m_StartBtn->height() + interval;
    m_mountLabel->resize(150,m_mountLabel->height());
    m_objectLabel->resize(150,m_objectLabel->height());
    m_mountLabel->setAlignment(Qt::AlignLeft);
    m_objectLabel->setAlignment(Qt::AlignLeft);
    qDebug()<<(this->width()-300-10)/2;
    m_mountLabel->move((this->width()-300-10)/2,startY);
    m_objectLabel->move(m_mountLabel->pos().x()+m_mountLabel->width()+10,startY);

    //thread
    m_Thread.init();
}

InstallDialog::~InstallDialog()
{
    delete ui;
}

void InstallDialog::updateTime()
{
    second++;
    if (second == 60)
    {
        minute++;
        second=0;
    }
    m_minuteLabel->setText(fontTemplate.arg(minute));
    m_secondLabel->setText(fontTemplate.arg(second));
}

void InstallDialog::mountInfo(QString info)
{
//    qDebug()<<"info "<<info;
    outputSlot(info.toAscii());

    if (info.contains("sda"))
    {
        m_mountLabel->setText(fontTemplate.arg("/dev/sda is mounted"));
        m_objectLabel->setText(fontTemplate.arg("/dev/sdb is object"));
    }
    else if(info.contains("sdb"))
    {
        m_mountLabel->setText(fontTemplate.arg("/dev/sdb is mounted"));
        m_objectLabel->setText(fontTemplate.arg("/dev/sda is object"));
    }
    else
    {
        m_mountLabel->setText(info);
        m_objectLabel->setText(info);
    }
}

void InstallDialog::StartInstall()
{

    emit updateLinuxVersion(m_linuxVersionCombo->currentIndex());
    emit updateDiskCapacity(m_diskCapacityCombo->currentIndex());
    emit updateOSVersion(m_OSVersion->currentIndex());

    timer.start(1000);
    m_ProgressHint->setText(fontTemplate.arg("Start to install..."));
    m_StartBtn->setDisabled(true);
    m_ProgressBar->setValue(1);

    connect(&m_Thread,SIGNAL(notify(int)),this,SLOT(OnNotify(int)));
    connect(&m_Thread,SIGNAL(notifyOutput(QByteArray)),this,SLOT(outputSlot(QByteArray)));
    m_Thread.start();
}

void InstallDialog::StopInstall()
{
    m_ProgressBar->setValue(0);
    m_StartBtn->setEnabled(true);
    m_StartBtn->setFocus();

    QProcess shutdownProcess;

    QString command="echo 'abc123' | ";
    command.append("sudo -S init 0");
    shutdownProcess.execute("sh",QStringList()<<"-c"<<command);
}

void InstallDialog::outputSlot(QByteArray content)
{
//    m_output->append(QString(content));
}

void InstallDialog::OnNotify(int state)
{
    switch (state)
    {
        case INSTALL_PROCESS_PARTITION:
            m_ProgressHint->setText(fontTemplate.arg("Partition the destination disk..."));
            m_ProgressBar->setValue(1);
            break;
        case INSTALL_PROCESS_FORMAT:
            m_ProgressHint->setText(fontTemplate.arg("Format filesystem partitions..."));
            m_ProgressBar->setValue(2);
            break;
        case INSTALL_PROCESS_BOOTLODER:
            m_ProgressHint->setText(fontTemplate.arg("Install bootloader(grub2)..."));
            m_ProgressBar->setValue(4);
            break;
        case INSTALL_PROCESS_IMAGE_SYSTEM:
            m_ProgressHint->setText(fontTemplate.arg("Programing system images to destination disk..."));
            m_ProgressBar->setValue(6);
            break;
        case INSTALL_PROCESS_IMAGE_HOME:
            m_ProgressHint->setText(fontTemplate.arg("Programing system images to destination disk..."));
            m_ProgressBar->setValue(8);
            break;
        case INSTALL_PROCESS_FINISH:
            timer.stop();
            m_ProgressHint->setText(fontTemplate.arg("Install Finished."));
            m_ProgressBar->setValue(10);
            m_StartBtn->setEnabled(true);
            m_StartBtn->setFocus();
            break;
        default:
            break;
    }
}

