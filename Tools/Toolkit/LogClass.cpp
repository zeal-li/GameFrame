#include "LogClass.h"
#include "timeclass.h"

#include <QDir>

static const QString LogDirectoryDefautPath = "./Log/";
static const QString LogFileDefautSuffix = ".txt";
static const int  MAX_LOG_LENGTH = 30720;


LogClass::LogClass():m_file(nullptr),m_buff(nullptr)
{
    m_buff = (char*)malloc(MAX_LOG_LENGTH);
}

LogClass::~LogClass()
{
    if(m_file){
        m_file->close();
        m_file = nullptr;
    }

    if(m_buff){
         free(m_buff);
         m_buff = nullptr;
    }

}

LogClass &LogClass::getInstance()
{
    static LogClass log;
    return log;
}

bool LogClass::init()
{
    QDir dir;
    if(!dir.exists(LogDirectoryDefautPath)){
        bool ret = dir.mkdir(LogDirectoryDefautPath);
        if(!ret)
            return ret;
    }

    QString fileName = LogDirectoryDefautPath + timeClass::getInstance().getCurrentDateString() + LogFileDefautSuffix;
    m_file = new QFile(fileName);
    if(!m_file)
        return false;

    bool ret = m_file->open(QIODevice::ReadWrite|QIODevice::Append);
    if(!ret)
        return ret;

    return true;
}

void LogClass::printLog(LOG_TYPE logtype,const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vprintLog(logtype, fmt, argp);
    va_end(argp);
}

void LogClass::vprintLog(LogClass::LOG_TYPE logtype, const char *fmt, va_list argp)
{
    if(!m_file || !m_buff)
        return;

    QString timeStr = timeClass::getInstance().getCurrentDateTimeString();
    int cur_length = 0;
    cur_length = ::sprintf(m_buff, "[%s %s] ", getLogTypeName(logtype), timeStr.toStdString().c_str());
    cur_length += ::vsprintf(m_buff + cur_length, fmt, argp);
    *(m_buff + cur_length++) = '\n';
    m_file->write(m_buff, cur_length);
    m_file->flush();
}

const char* LogClass::getLogTypeName(LogClass::LOG_TYPE logtype)
{
    switch(logtype){
    case LOG_TYPE_INFO:{
        return "INFO";
    }
        break;
    case LOG_TYPE_WARNING:{
        return "WARNING";
    }
        break;
    case LOG_TYPE_ERROR:{
        return "ERROR";
    }
        break;
    case LOG_TYPE_Debug:{
        return "DEBUG";
    }
        break;
    default:
        return "NON";
    }
}
