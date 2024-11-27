#include "widget.h"

#include <QApplication>

void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();


    signal(SIGINT, resetHandler);

    return a.exec();
}
