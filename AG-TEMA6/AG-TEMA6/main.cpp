#include "GraphDisplay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphDisplay w;
    w.show();
    return a.exec();
}
