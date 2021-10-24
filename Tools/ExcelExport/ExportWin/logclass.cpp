#include "logclass.h"

logClass::logClass()
{

}

logClass::~logClass()
{

}

logClass &logClass::getInstance()
{
    static logClass log;
    return log;
}

bool logClass::init()
{
    return true;
}

void logClass::printlog(int logtype, QString log)
{

}
