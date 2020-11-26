#include "mainwindow.h"
#include <QApplication>
#include "mywidgetvoicetotext.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWidgetVoiceToText w;
    w.show();

    return a.exec();
}
