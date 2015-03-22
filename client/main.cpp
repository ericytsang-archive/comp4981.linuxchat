#include "clientwindow.h"
#include <QApplication>
#define CLIENT

int main(int argc, char *argv[])
{
#ifdef CLIENT
    QApplication a(argc, argv);
    ClientWindow w;
    w.show();
    return a.exec();
#endif
}
