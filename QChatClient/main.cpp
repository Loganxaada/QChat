#include "mainwindow.h"
#include"loginwidget.h"
#include <QApplication>
#include"ComAPI/global.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Global::init(a.applicationDirPath());


    LoginWidget w;
    w.show();
    return a.exec();
}
