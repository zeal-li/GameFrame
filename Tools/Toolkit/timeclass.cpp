#include <QTime>

#include "timeclass.h"

timeClass::timeClass()
{

}

timeClass::~timeClass()
{

}

timeClass &timeClass::getInstance()
{
    static timeClass time;
    return time;
}

bool timeClass::init()
{
    return true;
}

QString timeClass::getCurrentDateString()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd");
    return current_date;
}

QString timeClass::getCurrentDateTimeString()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    return current_date;
}
