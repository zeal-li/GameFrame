#ifndef LOGCLASS_H
#define LOGCLASS_H

#include "stdafx.h"
#include <QFile>

class LogClass
{
public:
    LogClass();
    virtual ~LogClass();

    enum LOG_TYPE
    {
        LOG_TYPE_INFO,
        LOG_TYPE_WARNING,
        LOG_TYPE_ERROR,
        LOG_TYPE_Debug,
    };

    static LogClass& getInstance();

    bool init();

    void printLog(LOG_TYPE logtype, const char *fmt, ...);
    void vprintLog(LOG_TYPE logtype, const char *fmt, va_list argp);
    const char* getLogTypeName(LOG_TYPE logtype);

private:
    QFile *m_file;
    char *m_buff;
};

#endif // LOGCLASS_H
