#include <QApplication>

#include "errornum.h"
#include "logclass.h"
#include "timeclass.h"
#include "configclass.h"
#include "invokeclass.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!logClass::getInstance().init())
    {
        return START_ERROR_INIT_LOG;
    }

    if(!configClass::getInstance().init())
    {
        return START_ERROR_INIT_CONFIG;
    }

    if(!invokeclass::getInstance().init())
    {
        return START_ERROR_INVOKE;
    }

    MainWindow w;
    if(!w.init())
    {
        return START_ERRPR_MAIN_WINDOW;
    }
    w.show();

    return a.exec();
}
