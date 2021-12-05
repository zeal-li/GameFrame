#include <QApplication>

#include "errornum.h"
#include "LogClass.h"
#include "timeclass.h"
#include "configclass.h"
#include "invokeclass.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!timeClass::getInstance().init())
    {
        return START_ERROR_INIT_TIME_MGR;
    }

    if(!LogClass::getInstance().init())
    {
        return START_ERROR_INIT_LOG_MGR;
    }
    LogClass::getInstance().printLog(LogClass::LOG_TYPE_INFO, "Init LogClass Success");

    if(!configClass::getInstance().init())
    {
        return START_ERROR_INIT_CONFIG_MGR;
    }
    LogClass::getInstance().printLog(LogClass::LOG_TYPE_INFO, "Init configClass Success");

    if(!invokeclass::getInstance().init())
    {
        return START_ERROR_INVOKE;
    }
    LogClass::getInstance().printLog(LogClass::LOG_TYPE_INFO, "Init invokeclass Success");

    MainWindow w;
    if(!w.init())
    {
        return START_ERRPR_MAIN_WINDOW;
    }
    w.show();

    return a.exec();
}
