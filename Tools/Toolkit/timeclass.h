#ifndef TIMECLASS_H
#define TIMECLASS_H

#include "stdafx.h"

class timeClass
{
public:
    timeClass();
    virtual ~timeClass();

    static timeClass& getInstance();

    bool init();

    QString getCurrentDateString();
    QString getCurrentDateTimeString();

private:

};

#endif // TIMECLASS_H
