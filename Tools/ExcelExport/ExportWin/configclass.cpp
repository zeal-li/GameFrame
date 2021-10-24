#include "configclass.h"
#include "logclass.h"

static const char *projectRoot = "ProjectList";
static const char *defaultProjectKey = "DefaultProject";
static const QString SVN_PATH_SUFFIX = "_SVN";
static const QString SCRIPT_SUFFIX = "_SCRIPT";
static const QString EXPORT_SUFFIX = "_EXPORT";
static const QString SCRIPT_PARAM_SUFFIX = "_SCRIPT_PARAM";
static const QString GLOBAL_SCRIPT_ROOT = "GLOBAL_SCRIPT";
static const QString GLOBAL_SCRIPT_PARAM_ROOT = "GLOBAL_SCRIPT_PARAM";

static const QString EXPORT_INPUT_PATH_NAME = "INPUT_PATH";
static const QString EXPORT_OUTPUT_PATH_NAME = "OUTPUT_PATH";
static const QString EXPORT_OUTPUT_TYPE = "OUTPUT_TYPE";

configClass::configClass():m_qsettings(nullptr)
{
    m_string_list.clear();
}

configClass::~configClass()
{
    if(nullptr != m_qsettings)
    {
        delete m_qsettings;
    }
}

configClass &configClass::getInstance()
{
    static configClass config;
    return config;
}

bool configClass::init()
{
    m_qsettings = new QSettings("./config/projectCfg.ini", QSettings::IniFormat);
    if(nullptr == m_qsettings)
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_ERROR, "configClass::init error, new QSettings error!");
        return false;
    }

    return true;
}

void configClass::setValue(const QString &key, const QVariant &value)
{
    m_qsettings->setValue(key, value);
}

QVariant configClass::getValue(const QString &key, const QVariant &defaultValue)
{
    return m_qsettings->value(key, defaultValue);
}

bool configClass::hasProject(const QString &projectName)
{
   m_qsettings->beginGroup(projectRoot);
   QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
   m_qsettings->endGroup();

   if(QVariant::Invalid != project.type())
   {
       return true;
   }

   return false;
}

bool configClass::addProject(const QString &projectName)
{
    QString defaultProject = m_qsettings->value(defaultProjectKey).toString();
    if(defaultProject == "")
    {
        m_qsettings->setValue(defaultProjectKey, projectName);
    }

    m_qsettings->beginGroup(projectRoot);
    m_qsettings->setValue(projectName, projectName);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

bool configClass::delProject(const QString &projectName)
{
    m_qsettings->beginGroup(projectRoot);
    m_qsettings->remove(projectName);
    m_qsettings->endGroup();

    clearProjectSvnPath(projectName);
    clearProjectScript(projectName);

    return true;
}

void configClass::setDefaultProject(const QString &projectName)
{
    m_qsettings->setValue(defaultProjectKey, projectName);
    m_qsettings->sync();
}

bool configClass::addProjectSvnPath(const QString &projectName, const QString &pathName, const QString &path)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addProjectSvnPath error, Attempt to add svn path for not exit project!");
        return false;
    }

    QString svnName = project.toString() + SVN_PATH_SUFFIX;
    m_qsettings->beginGroup(svnName);
    m_qsettings->setValue(pathName, path);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

bool configClass::deleteProjectSvnPath(const QString &projectName, const QString &pathName)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClas::deleteProjectSvnPath erro, Attempt to delete svn path for not exit project!");
        return false;
    }

    QString svnName = project.toString() + SVN_PATH_SUFFIX;
    m_qsettings->beginGroup(svnName);
    m_qsettings->remove(pathName);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

void configClass::clearProjectSvnPath(const QString &projectName)
{
    QString svnRoot = projectName + SVN_PATH_SUFFIX;
    m_qsettings->remove(svnRoot);
}

QString configClass::getDefaultProject()
{
    QString defaultProject = "";
    return m_qsettings->value(defaultProjectKey, defaultProject).toString();
}

QStringList& configClass::getProjectList()
{
    m_string_list.clear();

    m_qsettings->beginGroup(projectRoot);
    m_string_list = m_qsettings->childKeys();
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getProjectSvnNameList(const QString &projectName)
{
    m_string_list.clear();
    QString curProject = projectName + SVN_PATH_SUFFIX;
    m_qsettings->beginGroup(curProject);
    m_string_list = m_qsettings->childKeys();
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getProjectSvnPathList(const QString &projectName)
{
    m_string_list.clear();

    QString curProject = projectName + SVN_PATH_SUFFIX;
    m_qsettings->beginGroup(curProject);
    QStringList pathNameList = m_qsettings->childKeys();
    for(int i = 0; i < pathNameList.count(); ++i)
    {
        QString svnPath = "";
        svnPath = m_qsettings->value(pathNameList[i], svnPath).toString();
        if("" != svnPath)
        {
            m_string_list.push_back(svnPath);
        }
    }
    m_qsettings->endGroup();

    return m_string_list;
}

QString configClass::getProjectSvnPath(const QString &projectName, const QString &svnName)
{
    QString svnPath = "";
    QString curProject = projectName + SVN_PATH_SUFFIX;
    m_qsettings->beginGroup(curProject);
    svnPath = m_qsettings->value(svnName, svnPath).toString();
    m_qsettings->endGroup();

    return svnPath;
}

bool configClass::addProjectScript(const QString &projectName, const QString &scriptName, const QString &script, const QString &scriptParam)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addProjectScript error, Attempt to add script for not exit project!");
        return false;
    }

    if("" == scriptName)
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addProjectScript error, script name is null!");
        return false;
    }

    QString projectScript = project.toString() + SCRIPT_SUFFIX;
    m_qsettings->beginGroup(projectScript);
    m_qsettings->setValue(scriptName, script);
    m_qsettings->endGroup();

    QString projectScriptParam = project.toString() + SCRIPT_PARAM_SUFFIX;
    m_qsettings->beginGroup(projectScriptParam);
    m_qsettings->setValue(scriptName, scriptParam);
    m_qsettings->endGroup();

    m_qsettings->sync();

    return true;
}

bool configClass::deleteProjectScript(const QString &projectName, const QString &scriptName)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClas::deleteProjectScript erro, Attempt to delete Script for not exit project!");
        return false;
    }

    QString script = project.toString() + SCRIPT_SUFFIX;
    m_qsettings->beginGroup(script);
    m_qsettings->remove(scriptName);
    m_qsettings->endGroup();

    QString scriptParam = project.toString() + SCRIPT_PARAM_SUFFIX;
    m_qsettings->beginGroup(scriptParam);
    m_qsettings->remove(scriptName);
    m_qsettings->endGroup();

    m_qsettings->sync();

    return true;
}

void configClass::clearProjectScript(const QString &projectName)
{
    QString scriptRoot = projectName + SCRIPT_SUFFIX;
    m_qsettings->remove(scriptRoot);

    QString scriptParamRoot = projectName + SCRIPT_PARAM_SUFFIX;
    m_qsettings->remove(scriptParamRoot);
}

QStringList &configClass::getProjectScriptNameList(const QString &projectName)
{
    m_string_list.clear();
    QString curProject = projectName + SCRIPT_SUFFIX;
    m_qsettings->beginGroup(curProject);
    m_string_list = m_qsettings->childKeys();
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getProjectScriptList(const QString &projectName)
{
    m_string_list.clear();

    QString curProject = projectName + SCRIPT_SUFFIX;
    m_qsettings->beginGroup(curProject);
    QStringList scriptNameList = m_qsettings->childKeys();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString script = "";
        script = m_qsettings->value(scriptNameList[i], script).toString();
        if("" != script)
        {
            m_string_list.push_back(script);
        }
    }
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getProjectScriptParamList(const QString &projectName)
{
    m_string_list.clear();

    QString curProject = projectName + SCRIPT_PARAM_SUFFIX;
    m_qsettings->beginGroup(curProject);
    QStringList scriptNameList = m_qsettings->childKeys();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scriptParam = "";
        scriptParam = m_qsettings->value(scriptNameList[i], scriptParam).toString();
        if("" != scriptParam)
        {
            m_string_list.push_back(scriptParam);
        }
    }
    m_qsettings->endGroup();

    return m_string_list;
}

QString configClass::getProjectScript(const QString &projectName, const QString &scriptName)
{
    QString script = "";
    QString curProject = projectName + SCRIPT_SUFFIX;
    m_qsettings->beginGroup(curProject);
    script = m_qsettings->value(scriptName, script).toString();
    m_qsettings->endGroup();

    return script;
}

QString configClass::getProjectScriptParam(const QString &projectName, const QString &scriptName)
{
    QString scriptParam = "";
    QString curProject = projectName + SCRIPT_PARAM_SUFFIX;
    m_qsettings->beginGroup(curProject);
    scriptParam = m_qsettings->value(scriptName, scriptParam).toString();
    m_qsettings->endGroup();

    return scriptParam;
}

bool configClass::addGlobalScript(const QString &scriptName, const QString &script, const QString &scriptParam)
{
    if("" == scriptName)
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addGlobalScript error, script name is null!");
        return false;
    }

    m_qsettings->beginGroup(GLOBAL_SCRIPT_ROOT);
    m_qsettings->setValue(scriptName, script);
    m_qsettings->endGroup();

    m_qsettings->beginGroup(GLOBAL_SCRIPT_PARAM_ROOT);
    m_qsettings->setValue(scriptName, scriptParam);
    m_qsettings->endGroup();

    m_qsettings->sync();

    return true;
}

bool configClass::deleteGlobalScript(const QString &scriptName)
{
    m_qsettings->beginGroup(GLOBAL_SCRIPT_ROOT);
    m_qsettings->remove(scriptName);
    m_qsettings->endGroup();

    m_qsettings->beginGroup(GLOBAL_SCRIPT_PARAM_ROOT);
    m_qsettings->remove(scriptName);
    m_qsettings->endGroup();

    m_qsettings->sync();

    return true;
}

void configClass::clearGlobalScript()
{
    m_qsettings->remove(GLOBAL_SCRIPT_ROOT);
    m_qsettings->remove(GLOBAL_SCRIPT_PARAM_ROOT);
}

QStringList &configClass::getGlobalScriptNameList()
{
    m_string_list.clear();
    m_qsettings->beginGroup(GLOBAL_SCRIPT_ROOT);
    m_string_list = m_qsettings->childKeys();
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getGlobalScriptList()
{
    m_string_list.clear();

    m_qsettings->beginGroup(GLOBAL_SCRIPT_ROOT);
    QStringList scriptNameList = m_qsettings->childKeys();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString script = "";
        script = m_qsettings->value(scriptNameList[i], script).toString();
        if("" != script)
        {
            m_string_list.push_back(script);
        }
    }
    m_qsettings->endGroup();

    return m_string_list;
}

QStringList &configClass::getGlobalScriptParamList()
{
    m_string_list.clear();

    m_qsettings->beginGroup(GLOBAL_SCRIPT_PARAM_ROOT);
    QStringList scriptNameList = m_qsettings->childKeys();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scriptParam = "";
        scriptParam = m_qsettings->value(scriptNameList[i], scriptParam).toString();
        if("" != scriptParam)
        {
            m_string_list.push_back(scriptParam);
        }
    }
    m_qsettings->endGroup();

    return m_string_list;
}

QString configClass::getGlobalScript(const QString &scriptName)
{
    QString script = "";
    m_qsettings->beginGroup(GLOBAL_SCRIPT_ROOT);
    script = m_qsettings->value(scriptName, script).toString();
    m_qsettings->endGroup();

    return script;
}

QString configClass::getGlobalScriptParam(const QString &scriptName)
{
    QString scriptParam = "";
    m_qsettings->beginGroup(GLOBAL_SCRIPT_PARAM_ROOT);
    scriptParam = m_qsettings->value(scriptName, scriptParam).toString();
    m_qsettings->endGroup();

    return scriptParam;
}

bool configClass::setProjectExportInputPath(const QString &projectName, const QString &path)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addProjectExportInputPath error, Attempt to add path for not exit project!");
        return false;
    }

    QString pathName = project.toString() + EXPORT_SUFFIX;
    m_qsettings->beginGroup(pathName);
    m_qsettings->setValue(EXPORT_INPUT_PATH_NAME, path);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

bool configClass::deleteProjectExportInputPath(const QString &projectName)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClas::deleteProjectExportInputPath erro, Attempt to delete path for not exit project!");
        return false;
    }

    QString pathName = project.toString() + EXPORT_SUFFIX;
    m_qsettings->beginGroup(pathName);
    m_qsettings->remove(EXPORT_INPUT_PATH_NAME);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

bool configClass::setProjectExportOutputPath(const QString &projectName, const QString &path)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClass::addProjectExportOutputPath error, Attempt to add path for not exit project!");
        return false;
    }

    QString pathName = project.toString() + EXPORT_SUFFIX;
    m_qsettings->beginGroup(pathName);
    m_qsettings->setValue(EXPORT_OUTPUT_PATH_NAME, path);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

bool configClass::deleteProjectExportOutputPath(const QString &projectName)
{
    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid == project.type())
    {
        logClass::getInstance().printlog(logClass::LOG_TYPE_WARNING, "configClas::deleteProjectExportOutputPath erro, Attempt to delete path for not exit project!");
        return false;
    }

    QString pathName = project.toString() + EXPORT_SUFFIX;
    m_qsettings->beginGroup(pathName);
    m_qsettings->remove(EXPORT_OUTPUT_PATH_NAME);
    m_qsettings->endGroup();
    m_qsettings->sync();

    return true;
}

void configClass::clearProjectExportPath(const QString &projectName)
{
    QString exportRoot = projectName + EXPORT_SUFFIX;
    m_qsettings->remove(exportRoot);
}

QString configClass::getProjectExportInputPath(const QString &projectName)
{
    QString path = "";

    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid != project.type())
    {
        QString exportRoot = project.toString() + EXPORT_SUFFIX;
        m_qsettings->beginGroup(exportRoot);
        path = m_qsettings->value(EXPORT_INPUT_PATH_NAME, "").toString();
        m_qsettings->endGroup();
    }

    return path;
}

QString configClass::getProjectExportOutputPath(const QString &projectName)
{
    QString path = "";

    m_qsettings->beginGroup(projectRoot);
    QVariant project = m_qsettings->value(projectName, QVariant(QVariant::Invalid));
    m_qsettings->endGroup();

    if(QVariant::Invalid != project.type())
    {
        QString exportRoot = project.toString() + EXPORT_SUFFIX;
        m_qsettings->beginGroup(exportRoot);
        path = m_qsettings->value(EXPORT_OUTPUT_PATH_NAME, "").toString();
        m_qsettings->endGroup();
    }

    return path;
}
