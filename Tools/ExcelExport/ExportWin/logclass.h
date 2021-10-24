#ifndef LOGCLASS_H
#define LOGCLASS_H

#include "stdafx.h"

class logClass
{
public:
    logClass();
    virtual ~logClass();

    enum LOG_TYPE
    {
        LOG_TYPE_ERROR,
        LOG_TYPE_WARNING,
        LOG_TYPE_TIPS,
    };

    static logClass& getInstance();

    bool init();

    void printlog(int logtype, QString log);

private:

};

#endif // LOGCLASS_H
