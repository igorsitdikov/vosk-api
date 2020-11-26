#include "mainwindow.h"
#include "mywidgetvoicetotext.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    myWidgetVoiceToText w;
    w.show();
    return a.exec();
}
