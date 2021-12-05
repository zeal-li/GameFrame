#include "invokeclass.h"
#include "LogClass.h"
#include <QDesktopServices>
#include <QUrl>
#include <QTextCodec>

invokeclass::invokeclass():m_qprocess(nullptr)
{
    m_qprocess_param.clear();
}

invokeclass::~invokeclass()
{
    if(nullptr != m_qprocess)
    {
        delete m_qprocess;
    }
}

invokeclass &invokeclass::getInstance()
{
    static invokeclass invoke;
    return invoke;
}

bool invokeclass::init()
{
    m_qprocess = new QProcess;
    if(nullptr == m_qprocess)
    {
        LogClass::getInstance().printLog(LogClass::LOG_TYPE_ERROR, "invokeclass::init error, new QProcess error!");
        return false;
    }

    return true;
}

int invokeclass::svnUpdate(QString path)
{
    m_qprocess_param.clear();
    m_qprocess_param.push_back("/command:update");
    m_qprocess_param.push_back("/path:" + path);
    m_qprocess_param.push_back("/closeonend:2");
    return m_qprocess->startDetached("TortoiseProc.exe", m_qprocess_param);
}

int invokeclass::svnCommit(QString path)
{
    m_qprocess_param.clear();
    m_qprocess_param.push_back("/command:commit");
    m_qprocess_param.push_back("/path:" + path);
    m_qprocess_param.push_back("/closeonend:2");
    return m_qprocess->startDetached("TortoiseProc.exe", m_qprocess_param);
}

int invokeclass::svnCleanUp(QString path)
{
    m_qprocess_param.clear();
    m_qprocess_param.push_back("/command:cleanup");
    m_qprocess_param.push_back("/path:" + path);
    m_qprocess_param.push_back("/closeonend:2");
    return m_qprocess->startDetached("TortoiseProc.exe", m_qprocess_param);
}

int invokeclass::svnRevert(QString path)
{
    m_qprocess_param.clear();
    m_qprocess_param.push_back("/command:revert");
    m_qprocess_param.push_back("/path:" + path);
    m_qprocess_param.push_back("/closeonend:2");
    return m_qprocess->startDetached("TortoiseProc.exe", m_qprocess_param);
}

int invokeclass::svnShowLog(QString path)
{
    m_qprocess_param.clear();
    m_qprocess_param.push_back("/command:log");
    m_qprocess_param.push_back("/path:" + path);
    m_qprocess_param.push_back("/closeonend:2");
    return m_qprocess->startDetached("TortoiseProc.exe", m_qprocess_param);
}

bool invokeclass::openDirectory(QString path)
{
    return QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

int invokeclass::callCMDexecuteCommond(QString commond)
{
    if("" == commond) return -100;

    m_qprocess_param.clear();
    m_qprocess_param.push_back("/c");
    m_qprocess_param.push_back(commond);
    return m_qprocess->startDetached("cmd", m_qprocess_param);
}

int invokeclass::callCMDexecuteProgram(QString &programName, QString param)
{
    if("" == programName) return -100;

    m_qprocess_param.clear();

    QStringList nameList = programName.split("/");
    QString propath = "";
    QString proName = "";
    for(int i = 0; i < nameList.count() - 1; ++i)
    {
        propath += nameList[i];
        propath += "/";
    }
    proName = nameList[nameList.count() - 1];

    QString comand = "";
    if("" != propath)
    {
        comand = "/c cd /d " + propath + "&&" + "start " + proName;
    }
    else
    {
        comand = "/c start " + proName;
    }

    if("" != param)
    {
        comand += " ";
        comand += param;
    }

    m_qprocess_param.push_back(comand);
    return m_qprocess->startDetached("cmd", m_qprocess_param);
}
