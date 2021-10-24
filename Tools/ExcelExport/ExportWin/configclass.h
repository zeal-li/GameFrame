#ifndef CONFIGCLASS_H
#define CONFIGCLASS_H

#include <QSettings>

class configClass
{
public:
    configClass();
    virtual ~configClass();

    static configClass& getInstance();

    bool init();

    void setValue(const QString &key, const QVariant &value);
    QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant());

    bool hasProject(const QString &projectName);
    bool addProject(const QString &projectName);
    bool delProject(const QString &projectName);

    void setDefaultProject(const QString &projectName);
    bool addProjectSvnPath(const QString &projectName, const QString &pathName, const QString &path);
    bool deleteProjectSvnPath(const QString &projectName, const QString &pathName);
    void clearProjectSvnPath(const QString &projectName);
    QString getDefaultProject();
    QStringList& getProjectList();
    QStringList& getProjectSvnNameList(const QString &projectName);
    QStringList& getProjectSvnPathList(const QString &projectName);
    QString getProjectSvnPath(const QString &projectName, const QString &svnName);

    bool addProjectScript(const QString &projectName, const QString &scriptName, const QString &script, const QString &scriptParam);
    bool deleteProjectScript(const QString &projectName, const QString &scriptName);
    void clearProjectScript(const QString &projectName);
    QStringList& getProjectScriptNameList(const QString &projectName);
    QStringList& getProjectScriptList(const QString &projectName);
    QStringList& getProjectScriptParamList(const QString &projectName);
    QString getProjectScript(const QString &projectName, const QString &scriptName);
    QString getProjectScriptParam(const QString &projectName, const QString &scriptName);

    bool addGlobalScript(const QString &scriptName, const QString &script, const QString &scriptParam);
    bool deleteGlobalScript(const QString &scriptName);
    void clearGlobalScript();
    QStringList& getGlobalScriptNameList();
    QStringList& getGlobalScriptList();
    QStringList& getGlobalScriptParamList();
    QString getGlobalScript(const QString &scriptName);
    QString getGlobalScriptParam(const QString &scriptName);

    bool setProjectExportInputPath(const QString &projectName, const QString &path);
    bool deleteProjectExportInputPath(const QString &projectName);
    bool setProjectExportOutputPath(const QString &projectName, const QString &path);
    bool deleteProjectExportOutputPath(const QString &projectName);
    void clearProjectExportPath(const QString &projectName);
    QString getProjectExportInputPath(const QString &projectName);
    QString getProjectExportOutputPath(const QString &projectName);

private:

    QSettings *m_qsettings;
    QStringList m_string_list;
};

#endif // CONFIGCLASS_H
