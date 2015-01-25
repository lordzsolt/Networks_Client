#include <QApplication>

#include "Controllers/ApplicationController.h"

#ifndef int32_t
#define int32_t qint32
#endif

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ApplicationController::currentApplication().windowController();

    return a.exec();
}
