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
#include <QtGui>

#include "installdialog.h"

int main(int argc, char **argv)
{
   QApplication app(argc, argv);
//   CInstallDialog *dialog = new CInstallDialog;

   InstallDialog * dialog = new InstallDialog;

   dialog->setWindowTitle("E5 install system. Edan R&D@Xi'an");
   dialog->show();

   return app.exec();
}
