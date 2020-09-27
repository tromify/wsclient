#include "wsclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WSClient w;
    w.show();
    return a.exec();
}
