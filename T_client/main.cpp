#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    printf("hello\n");
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
