#include "projectdetaildialog.h"
#include "configclass.h"
#include "newsvnpathdialog.h"
#include "newscriptdialog.h"
#include "mainwindow.h"
#include "dialogcommondef.h"
#include <QFileDialog>


#include "stdafx.h"
ProjectDetailDialog::ProjectDetailDialog(QWidget *parent):BaseDialog (parent),
    scrollArea(nullptr), scrollAreaWidgetContents(nullptr),
    btnAddSvnPath(nullptr), btnAddScript(nullptr), btnApply(nullptr),
    lineEdit(nullptr), m_project_name(""),
    svnPathTitle(nullptr), scriptTitle(nullptr)
{
    init();
}

ProjectDetailDialog::~ProjectDetailDialog()
{
    destoryDetail();

    if(nullptr != scriptTitle)
        delete scriptTitle;

    if(nullptr != svnPathTitle)
        delete svnPathTitle;

    if(nullptr != lineEdit)
        delete lineEdit;

    if(nullptr != btnApply)
        delete btnApply;

    if(nullptr != btnAddScript)
        delete btnAddScript;

    if(nullptr != btnAddSvnPath)
        delete btnAddSvnPath;

    if(nullptr != scrollAreaWidgetContents)
        delete scrollAreaWidgetContents;

    if(nullptr != scrollArea)
        delete scrollArea;
}

bool ProjectDetailDialog::init()
{
    setTitle("ProjectEdit");
    resize(450, 620);
    setFixedSize(450, 620);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(0, 40, 450, 540));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 448, 538));
    scrollArea->setWidget(scrollAreaWidgetContents);

    btnAddSvnPath = new QPushButton("addSvn", this);
    btnAddSvnPath->setObjectName("btnAddSvnPath");
    btnAddSvnPath->setGeometry(310, 10, 60, 23);
    connect(btnAddSvnPath, &QPushButton::clicked, this, &ProjectDetailDialog::slotBtnAddSvnPath);

    btnAddScript = new QPushButton("addScript", this);
    btnAddScript->setObjectName("btnAddScript");
    btnAddScript->setGeometry(370, 10, 60, 23);
    connect(btnAddScript, &QPushButton::clicked, this, &ProjectDetailDialog::slotBtnAddScript);

    btnApply = new QPushButton("Apply", this);
    btnApply->setObjectName("btnApply");
    btnApply->setGeometry(360, 590, 60, 23);
    connect(btnApply, &QPushButton::clicked, this, &ProjectDetailDialog::slotBtnApply);

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(5, 10, 300, 23);
    lineEdit->setObjectName("lineEdit");

    QMetaObject::connectSlotsByName(this);
    return true;
}

void ProjectDetailDialog::createDetail(QString &projectName)
{
    if(!configClass::getInstance().hasProject(projectName)) return;

    m_project_name = projectName;
    lineEdit->setText(projectName);

    svnPathTitle = new QLabel("Svn Path:", scrollAreaWidgetContents);
    svnPathTitle->setGeometry(5, 5, 100, 23);
    QStringList &pathNameList = configClass::getInstance().getProjectSvnNameList(projectName);
    for(int i = 0; i < pathNameList.count(); ++i)
    {
        QString pathName = pathNameList[i];
        PathItem *item = new PathItem(pathName);
        item->pathNameEdit = new QLineEdit(pathName, scrollAreaWidgetContents);
        item->pathNameEdit->setGeometry(PROJECT_DETAIL_START_X_1, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1, 250, 23);
        item->pathNameEdit->setObjectName(pathName);
        connect(item->pathNameEdit, &QLineEdit::textChanged, this, &ProjectDetailDialog::slot_PathItem_SvnPathNameEdit_textChanged);

        item->btnAddPath = new QPushButton("addPath", scrollAreaWidgetContents);
        item->btnAddPath->setGeometry(PROJECT_DETAIL_START_X_2, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1, 60, 23);
        item->btnAddPath->setObjectName(pathName);
        connect(item->btnAddPath, &QPushButton::clicked, this, &ProjectDetailDialog::slot_PathIem_BtnAddSvnPath);

        item->btnDeletePath = new QPushButton("Delete", scrollAreaWidgetContents);
        item->btnDeletePath->setGeometry(PROJECT_DETAIL_START_X_3, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1, 60, 23);
        item->btnDeletePath->setObjectName(pathName);
        connect(item->btnDeletePath, &QPushButton::clicked, this, &ProjectDetailDialog::slot_PathIem_BtnDeletePath);

        QString path = configClass::getInstance().getProjectSvnPath(m_project_name, pathName);
        item->pathEdit = new QLineEdit(path, scrollAreaWidgetContents);
        item->pathEdit->setGeometry(PROJECT_DETAIL_START_X_1, PROJECT_DETAIL_START_Y_2 + i * PROJECT_DETAIL_INTERVAL_Y_1, 250, 23);

        pathItemMap[pathName] = item;
    }

    scriptTitle = new QLabel("Script:", scrollAreaWidgetContents);
    scriptTitle->setGeometry(5, PROJECT_DETAIL_START_Y_1 + pathNameList.count() * PROJECT_DETAIL_INTERVAL_Y_1, 100, 23);
    int start_y_1 = PROJECT_DETAIL_START_Y_1 + pathNameList.count() * PROJECT_DETAIL_INTERVAL_Y_1 + 23;
    int start_y_2 = start_y_1 + 23;
    int start_y_3 = start_y_2 +23;
    QStringList &scriptNameList = configClass::getInstance().getProjectScriptNameList(projectName);
    for(int j = 0; j < scriptNameList.count(); ++j)
    {
        QString scriptName = scriptNameList[j];
        ScriptItem *item = new ScriptItem(scriptName);
        item->scriptNameEdit = new QLineEdit(scriptName, scrollAreaWidgetContents);
        item->scriptNameEdit->setGeometry(PROJECT_DETAIL_START_X_1, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2, 250, 23);
        item->scriptNameEdit->setObjectName(scriptName);
        connect(item->scriptNameEdit, &QLineEdit::textChanged, this, &ProjectDetailDialog::slot_ScriptItem_scriptNameEdit_textChanged);

        item->btnAddScript = new QPushButton("addScript", scrollAreaWidgetContents);
        item->btnAddScript->setGeometry(PROJECT_DETAIL_START_X_2, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2, 60, 23);
        item->btnAddScript->setObjectName(scriptName);
        connect(item->btnAddScript, &QPushButton::clicked, this, &ProjectDetailDialog::slot_ScriptItem_BtnAddScript);

        item->btnDeleteScript = new QPushButton("Delete", scrollAreaWidgetContents);
        item->btnDeleteScript->setGeometry(PROJECT_DETAIL_START_X_3, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2, 60, 23);
        item->btnDeleteScript->setObjectName(scriptName);
        connect(item->btnDeleteScript, &QPushButton::clicked, this, &ProjectDetailDialog::slot_ScriptItem_BtnDeleteScript);

        QString script = configClass::getInstance().getProjectScript(m_project_name, scriptName);
        item->scriptEdit = new QLineEdit(script, scrollAreaWidgetContents);
        item->scriptEdit->setGeometry(PROJECT_DETAIL_START_X_1, start_y_2 + j * PROJECT_DETAIL_INTERVAL_Y_2, 250, 23);

        QString scriptParam = configClass::getInstance().getProjectScriptParam(m_project_name, scriptName);
        item->scriptParamEdit = new QLineEdit(scriptParam, scrollAreaWidgetContents);
        item->scriptParamEdit->setGeometry(PROJECT_DETAIL_START_X_1, start_y_3 + j * PROJECT_DETAIL_INTERVAL_Y_2, 250, 23);

        scriptItemMap[scriptName] = item;
    }

    scrollAreaWidgetContents->setMinimumHeight(start_y_2 + (scriptNameList.count()) * PROJECT_DETAIL_INTERVAL_Y_2);
}

void ProjectDetailDialog::destoryDetail()
{
    std::map<QString, PathItem*>::iterator iter = pathItemMap.begin();
    for(; iter != pathItemMap.end(); iter++)
    {
       if(nullptr != iter->second)
       {
           delete iter->second;
       }
    }

    pathItemMap.clear();

    std::map<QString, ScriptItem*>::iterator s_iter = scriptItemMap.begin();
    for(; s_iter != scriptItemMap.end(); s_iter++)
    {
       if(nullptr != s_iter->second)
       {
           delete s_iter->second;
       }
    }

    scriptItemMap.clear();
}

void ProjectDetailDialog::flushDetailAddPathItem()
{
    if(!configClass::getInstance().hasProject(m_project_name)) return;

    QStringList &pathNameList = configClass::getInstance().getProjectSvnNameList(m_project_name);
    for(int i = 0; i < pathNameList.count(); ++i)
    {
        QString pathName = pathNameList[i];
        PathItem *item = getPathItemByName(pathName);
        if(nullptr == item)
        {
            item = new PathItem(pathName);
            item->pathNameEdit = new QLineEdit(pathName, scrollAreaWidgetContents);
            item->pathNameEdit->setObjectName(pathName);
            item->pathNameEdit->setGeometry(0, 0, 250, 23);
            connect(item->pathNameEdit, &QLineEdit::textChanged, this, &ProjectDetailDialog::slot_PathItem_SvnPathNameEdit_textChanged);

            item->btnAddPath = new QPushButton("addPath", scrollAreaWidgetContents);
            item->btnAddPath->setObjectName(pathName);
            item->btnAddPath->setGeometry(0, 0, 60, 23);
            connect(item->btnAddPath, &QPushButton::clicked, this, &ProjectDetailDialog::slot_PathIem_BtnAddSvnPath);

            item->btnDeletePath = new QPushButton("Delete", scrollAreaWidgetContents);
            item->btnDeletePath->setObjectName(pathName);
            item->btnDeletePath->setGeometry(0, 0, 60, 23);
            connect(item->btnDeletePath, &QPushButton::clicked, this, &ProjectDetailDialog::slot_PathIem_BtnDeletePath);

            QString path = configClass::getInstance().getProjectSvnPath(m_project_name, pathName);
            item->pathEdit = new QLineEdit(path, scrollAreaWidgetContents);
            item->pathEdit->setGeometry(0, 0, 250, 23);

            item->pathNameEdit->show();
            item->btnAddPath->show();
            item->btnDeletePath->show();
            item->pathEdit->show();

            pathItemMap[pathName] = item;
        }
    }

    flushDetailPathItem();
}

void ProjectDetailDialog::flushDetailPathItem()
{
    if(!configClass::getInstance().hasProject(m_project_name)) return;

    std::map<QString, PathItem*>::iterator iter = pathItemMap.begin();
    int i = 0;
    for(; iter != pathItemMap.end(); ++iter)
    {
        PathItem *item = iter->second;
        item->pathNameEdit->move(PROJECT_DETAIL_START_X_1, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1);
        item->btnAddPath->move(PROJECT_DETAIL_START_X_2, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1);
        item->btnDeletePath->move(PROJECT_DETAIL_START_X_3, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1);
        item->pathEdit->move(PROJECT_DETAIL_START_X_1, PROJECT_DETAIL_START_Y_2 + i * PROJECT_DETAIL_INTERVAL_Y_1);
        ++i;
    }

    scriptTitle->move(5, PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1);

    int start_y_1 = PROJECT_DETAIL_START_Y_1 + i * PROJECT_DETAIL_INTERVAL_Y_1 + 23;
    int start_y_2 = start_y_1 + 23;
    int start_y_3 = start_y_2 +23;
    std::map<QString, ScriptItem*>::iterator s_iter = scriptItemMap.begin();
    int j = 0;
    for(; s_iter != scriptItemMap.end(); ++s_iter)
    {
        ScriptItem *item = s_iter->second;
        item->scriptNameEdit->move(PROJECT_DETAIL_START_X_1, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->btnAddScript->move(PROJECT_DETAIL_START_X_2, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->btnDeleteScript->move(PROJECT_DETAIL_START_X_3, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->scriptEdit->move(PROJECT_DETAIL_START_X_1, start_y_2 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->scriptParamEdit->move(PROJECT_DETAIL_START_X_1, start_y_3 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        ++j;
    }

    scrollAreaWidgetContents->setMinimumHeight(start_y_2 + j * PROJECT_DETAIL_INTERVAL_Y_2);
}

void ProjectDetailDialog::flushDetailAddScriptItem()
{
    if(!configClass::getInstance().hasProject(m_project_name)) return;

    QStringList &scriptNameList = configClass::getInstance().getProjectScriptNameList(m_project_name);
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scriptName = scriptNameList[i];
        ScriptItem *item = getScriptItemByName(scriptName);
        if(nullptr == item)
        {
            item = new ScriptItem(scriptName);
            item->scriptNameEdit = new QLineEdit(scriptName, scrollAreaWidgetContents);
            item->scriptNameEdit->setObjectName(scriptName);
            item->scriptNameEdit->setGeometry(0, 0, 250, 23);
            connect(item->scriptNameEdit, &QLineEdit::textChanged, this, &ProjectDetailDialog::slot_ScriptItem_scriptNameEdit_textChanged);

            item->btnAddScript = new QPushButton("addScript", scrollAreaWidgetContents);
            item->btnAddScript->setObjectName(scriptName);
            item->btnAddScript->setGeometry(0, 0, 60, 23);
            connect(item->btnAddScript, &QPushButton::clicked, this, &ProjectDetailDialog::slot_ScriptItem_BtnAddScript);

            item->btnDeleteScript = new QPushButton("Delete", scrollAreaWidgetContents);
            item->btnDeleteScript->setObjectName(scriptName);
            item->btnDeleteScript->setGeometry(0, 0, 60, 23);
            connect(item->btnDeleteScript, &QPushButton::clicked, this, &ProjectDetailDialog::slot_ScriptItem_BtnDeleteScript);

            QString script = configClass::getInstance().getProjectScript(m_project_name, scriptName);
            item->scriptEdit = new QLineEdit(script, scrollAreaWidgetContents);
            item->scriptEdit->setGeometry(0, 0, 250, 23);

            QString scriptParam = configClass::getInstance().getProjectScriptParam(m_project_name, scriptName);
            item->scriptParamEdit = new QLineEdit(scriptParam, scrollAreaWidgetContents);
            item->scriptParamEdit->setGeometry(0, 0, 250, 23);

            item->scriptNameEdit->show();
            item->btnAddScript->show();
            item->btnDeleteScript->show();
            item->scriptEdit->show();
            item->scriptParamEdit->show();

            scriptItemMap[scriptName] = item;
        }
    }

    flushDetailScriptItem();
}

void ProjectDetailDialog::flushDetailScriptItem()
{
    int start_y_1 = scriptTitle->geometry().y() + 23;
    int start_y_2 = start_y_1 + 23;
    int start_y_3 = start_y_2 +23;
    std::map<QString, ScriptItem*>::iterator s_iter = scriptItemMap.begin();
    int j = 0;
    for(; s_iter != scriptItemMap.end(); ++s_iter)
    {
        ScriptItem *item = s_iter->second;
        item->scriptNameEdit->move(PROJECT_DETAIL_START_X_1, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->btnAddScript->move(PROJECT_DETAIL_START_X_2, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->btnDeleteScript->move(PROJECT_DETAIL_START_X_3, start_y_1 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->scriptEdit->move(PROJECT_DETAIL_START_X_1, start_y_2 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        item->scriptParamEdit->move(PROJECT_DETAIL_START_X_1, start_y_3 + j * PROJECT_DETAIL_INTERVAL_Y_2);
        ++j;
    }

    scrollAreaWidgetContents->setMinimumHeight(start_y_2 + j * PROJECT_DETAIL_INTERVAL_Y_2);
}

ProjectDetailDialog::PathItem *ProjectDetailDialog::getPathItemByName(QString &name)
{
    std::map<QString, PathItem*>::iterator iter = pathItemMap.find(name);
    if(iter != pathItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

ProjectDetailDialog::ScriptItem *ProjectDetailDialog::getScriptItemByName(QString &name)
{
    std::map<QString, ScriptItem*>::iterator iter = scriptItemMap.find(name);
    if(iter != scriptItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void ProjectDetailDialog::slotBtnAddSvnPath()
{
    MainWindow *parent = getParentPtr<MainWindow>();
    NewSvnPathDialog w(parent);
    w.setProjectName(m_project_name);
    w.showDialog(true);

    flushDetailAddPathItem();
}

void ProjectDetailDialog::slotBtnAddScript()
{
    MainWindow *parent = getParentPtr<MainWindow>();
    NewScriptDialog w(parent);
    w.setProjectName(m_project_name);
    w.showDialog(true);

    flushDetailAddScriptItem();
}

void ProjectDetailDialog::slotBtnApply()
{
    QString newProjectName = lineEdit->text();
    if("" == newProjectName) return;

    if(newProjectName != m_project_name)
    {
        configClass::getInstance().delProject(m_project_name);
    }

    bool is_has = configClass::getInstance().hasProject(newProjectName);
    if(is_has)
    {
       configClass::getInstance().clearProjectSvnPath(newProjectName);
       configClass::getInstance().clearProjectScript(newProjectName);
    }
    else
    {
        configClass::getInstance().addProject(newProjectName);
    }

    std::map<QString, PathItem*>::iterator iter = pathItemMap.begin();
    for(; iter != pathItemMap.end();)
    {
        QString pathName = iter->second->pathNameEdit->text();
        if("" == pathName)
        {
            delete iter->second;
            pathItemMap.erase(iter++);
            continue;
        }
        else
        {
            QString path = iter->second->pathEdit->text();
            configClass::getInstance().addProjectSvnPath(newProjectName, pathName, path);
        }

         ++iter;
    }

    std::map<QString, ScriptItem*>::iterator s_iter = scriptItemMap.begin();
    for(; s_iter != scriptItemMap.end();)
    {
        QString scirptName = s_iter->second->scriptNameEdit->text();
        if("" == scirptName)
        {
            delete s_iter->second;
            scriptItemMap.erase(s_iter++);
            continue;
        }
        else
        {
            QString scirpt = s_iter->second->scriptEdit->text();
            QString scirptParam = s_iter->second->scriptParamEdit->text();
            configClass::getInstance().addProjectScript(newProjectName, scirptName, scirpt, scirptParam);
        }

         ++s_iter;
    }


    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        if(newProjectName != m_project_name)
        {
            parent->delProjectComboItem(m_project_name);
            if(!is_has)
            {
                parent->addProjectComboItem(newProjectName);
            }
        }

        parent->initSvnOperateWidget(true);
        parent->initScriptOperateWidget(true);
        parent->initExportOperateWidget(true);
    }


    this->close();
}

void ProjectDetailDialog::on_lineEdit_textChanged(const QString &arg1)
{
    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > PROJECT_NAME_LEGTH_MAX)
    {
        inputStr.remove(PROJECT_NAME_LEGTH_MAX, curlen - PROJECT_NAME_LEGTH_MAX);
        lineEdit->setText(inputStr);
    }
}

void ProjectDetailDialog::slot_PathItem_SvnPathNameEdit_textChanged(const QString &arg1)
{
    QLineEdit *act=qobject_cast<QLineEdit*>(sender());
    QString itemName = act->objectName();

    PathItem *item = getPathItemByName(itemName);
    if(nullptr == item) return;

    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > SVN_PATH_NAME_LEGTH_MAX)
    {
        inputStr.remove(SVN_PATH_NAME_LEGTH_MAX, curlen - SVN_PATH_NAME_LEGTH_MAX);
        item->pathNameEdit->setText(inputStr);
    }
}

void ProjectDetailDialog::slot_PathIem_BtnAddSvnPath()
{
    QFileDialog f_dlg(this);
    f_dlg.setFileMode(QFileDialog::DirectoryOnly);
    f_dlg.show();

    if (f_dlg.exec() == QDialog::Accepted )
    {
        QStringList fileName;
        fileName = f_dlg.selectedFiles();

        if(fileName.count() > 0)
        {
            QPushButton *act=qobject_cast<QPushButton*>(sender());
            QString itemName = act->objectName();

            PathItem *item = getPathItemByName(itemName);
            if(nullptr != item)
            {
                QString path = item->pathEdit->text();
                if("" != path)
                    path += "*";
                path += fileName[0];
                item->pathEdit->setText(path);
            }
        }
    }
}

void ProjectDetailDialog::slot_ScriptItem_scriptNameEdit_textChanged(const QString &arg1)
{
    QLineEdit *act=qobject_cast<QLineEdit*>(sender());
    QString itemName = act->objectName();

    ScriptItem *item = getScriptItemByName(itemName);
    if(nullptr == item) return;

    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > SCRIPT_NAME_LEGTH_MAX)
    {
        inputStr.remove(SCRIPT_NAME_LEGTH_MAX, curlen - SCRIPT_NAME_LEGTH_MAX);
        item->scriptNameEdit->setText(inputStr);
    }
}

void ProjectDetailDialog::slot_ScriptItem_BtnAddScript()
{
    QFileDialog f_dlg(this);
    f_dlg.setFileMode(QFileDialog::ExistingFiles);
    f_dlg.show();

    if (f_dlg.exec() == QDialog::Accepted )
    {
        QStringList fileName;
        fileName = f_dlg.selectedFiles();

        if(fileName.count() > 0)
        {
            QPushButton *act=qobject_cast<QPushButton*>(sender());
            QString itemName = act->objectName();

            ScriptItem *item = getScriptItemByName(itemName);
            if(nullptr != item)
            {
                QString script = item->scriptEdit->text();
                if("" != script)
                    script += ";";
                script += fileName[0];
                item->scriptEdit->setText(script);

                QString scriptparam = item->scriptParamEdit->text();
                scriptparam += ";";
                item->scriptParamEdit->setText(scriptparam);
            }
        }
    }
}

void ProjectDetailDialog::slot_ScriptItem_BtnDeleteScript()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString itemName = act->objectName();

    std::map<QString, ScriptItem*>::iterator iter = scriptItemMap.find(itemName);
    if(iter == scriptItemMap.end()) return;

    delete iter->second;
    scriptItemMap.erase(iter);

    if(configClass::getInstance().deleteProjectScript(m_project_name, itemName))
    {
        flushDetailScriptItem();
    }

    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        QString script = configClass::getInstance().getProjectScript(m_project_name, itemName);
        QString outputStr = m_project_name + " delete script success, " + itemName + ":" + script;
        parent->appendOutputInfo(outputStr);
        parent->deleteFlushScriptOperateWidget(m_project_name);
    }
}

void ProjectDetailDialog::slot_PathIem_BtnDeletePath()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString itemName = act->objectName();

    std::map<QString, PathItem*>::iterator iter = pathItemMap.find(itemName);
    if(iter == pathItemMap.end()) return;

    delete iter->second;
    pathItemMap.erase(iter);

    if(configClass::getInstance().deleteProjectSvnPath(m_project_name, itemName))
    {
       flushDetailPathItem();
    }

    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        QString path = configClass::getInstance().getProjectSvnPath(m_project_name, itemName);
        QString outputStr = m_project_name + " delete svn path success, " + itemName + ":" + path;
        parent->appendOutputInfo(outputStr);
        parent->deleteFlushSvnOperateWidget(m_project_name);
    }

}
