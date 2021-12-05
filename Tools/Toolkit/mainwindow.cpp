#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdafx.h"
#include "configclass.h"
#include "timeclass.h"
#include "invokeclass.h"
#include "LogClass.h"

#include "dialog/newprojectdialog.h"
#include "dialog/projectmanagerdialog.h"
#include "dialog/scriptoperatedialog.h"
#include "dialog/showpathdialog.h"
#include "dialog/globalscrtipmanagerdialog.h"
#include "dialog/dialogcommondef.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    destroyGlobalScriptOperateWidget();
    destroyScriptOperateWidget();
    destroySvnOperateWidget();
    destroyExportOperateWidget();
    delete ui;
}


bool MainWindow::init()
{
    QFile file(":/qss/mainwindow.qss");
    file.open(QFile::ReadOnly);
    if(!file.isOpen())
    {
        LogClass::getInstance().printLog(LogClass::LOG_TYPE_ERROR, "MainWindow::init, qss/mainwindow.qss open fail!");
        return false;
    }

    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();

    initProjectCombo();
    initSvnOperateWidget();
    initScriptOperateWidget();
    initGlobalScriptOperateWidget();
    initExportOperateWidget();

    ui->scriptWidget->setVisible(false);
    ui->exopotwidget->setVisible(false);
    return true;
}

void MainWindow::initProjectCombo()
{
    QString defaultProject = configClass::getInstance().getDefaultProject();
    if("" != defaultProject)
    {
        ui->cmbPorjectSelect->addItem(defaultProject);
    }

    QStringList &projectList = configClass::getInstance().getProjectList();
    for(int i = 0; i < projectList.count(); ++i)
    {
        if(projectList[i] != defaultProject)
        {
            ui->cmbPorjectSelect->addItem(projectList[i]);
        }
    }
}

void MainWindow::changeProjectComboItem(QString &oldName, QString &newName)
{
    int itemCount = ui->cmbPorjectSelect->count();
    for(int i = 0; i < itemCount; ++i)
    {
        QString itemText = ui->cmbPorjectSelect->itemText(i);
        if(oldName == itemText)
        {
            ui->cmbPorjectSelect->setItemText(i, newName);
            break;
        }
     }
}

void MainWindow::addProjectComboItem(QString &textName)
{
    if("" == textName) return;

    ui->cmbPorjectSelect->addItem(textName);
}

void MainWindow::delProjectComboItem(QString &textName)
{
    if("" == textName) return;

    int itemCount = ui->cmbPorjectSelect->count();
    for(int i = 0; i < itemCount; ++i)
    {
        QString itemText = ui->cmbPorjectSelect->itemText(i);
        if(textName == itemText)
        {
            ui->cmbPorjectSelect->removeItem(i);
            break;
        }
     }
}

void MainWindow::appendOutputInfo(const QString &infoStr)
{
    QString time = timeClass::getInstance().getCurrentDateTimeString();
    QString str = time + " " + infoStr;
    ui->outputInfoText->append(str);
}

void MainWindow::initSvnOperateWidget(bool isforce)
{
    QString defaultProject = configClass::getInstance().getDefaultProject();
    if("" == defaultProject && !isforce) return;

    destroySvnOperateWidget();

    QStringList svnNameList = configClass::getInstance().getProjectSvnNameList(defaultProject);
    for(int i = 0; i < svnNameList.count(); ++i)
    {
        QString pathName = svnNameList[i];
        SvnPathItem *item = new SvnPathItem();
        item->label = new QLabel(pathName, ui->svn_scrollAreaWidgetContents);
        item->label->setGeometry(SVN_STRAT_X_1, SVN_START_Y_1 + i * SVN_ITEM_INTERVAL_Y, 260, 23);

        item->btnOpenPath= new QPushButton("...", ui->svn_scrollAreaWidgetContents);
        item->btnOpenPath->setObjectName(pathName);
        item->btnOpenPath->setGeometry(SVN_STRAT_X_1, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 40, 23);
        connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnOpendir);

        QString path = configClass::getInstance().getProjectSvnPath(defaultProject, pathName);
        item->lineEdit = new QLineEdit(path, ui->svn_scrollAreaWidgetContents);
        item->lineEdit->setGeometry(SVN_STRAT_X_2, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 470, 23);
        item->lineEdit->setReadOnly(true);

        item->btnUpdate = new QPushButton("update", ui->svn_scrollAreaWidgetContents);
        item->btnUpdate->setObjectName(pathName);
        item->btnUpdate->setGeometry(SVN_STRAT_X_3, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnUpdate, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnUpdate);

        item->btnCommit = new QPushButton("commit", ui->svn_scrollAreaWidgetContents);
        item->btnCommit->setObjectName(pathName);
        item->btnCommit->setGeometry(SVN_STRAT_X_4, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnCommit, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnCommit);

        item->btnRevert = new QPushButton("revert", ui->svn_scrollAreaWidgetContents);
        item->btnRevert->setObjectName(pathName);
        item->btnRevert->setGeometry(SVN_STRAT_X_5, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnRevert, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnRevert);

        item->btnCleanup= new QPushButton("cleanup", ui->svn_scrollAreaWidgetContents);
        item->btnCleanup->setObjectName(pathName);
        item->btnCleanup->setGeometry(SVN_STRAT_X_6, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnCleanup, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnCleanup);

        item->label->show();
        item->btnOpenPath->show();
        item->lineEdit->show();
        item->btnUpdate->show();
        item->btnCommit->show();
        item->btnRevert->show();
        item->btnCleanup->show();

        m_svnPathItemMap[pathName] = item;
    }

    ui->svn_scrollAreaWidgetContents->setMinimumHeight(SVN_START_Y_2 + svnNameList.count() * SVN_ITEM_INTERVAL_Y);
}

void MainWindow::destroySvnOperateWidget()
{
    std::map<QString, SvnPathItem*>::iterator iter =  m_svnPathItemMap.begin();
    for(;iter != m_svnPathItemMap.end(); ++iter)
    {
       delete iter->second;
    }

    m_svnPathItemMap.clear();
}

void MainWindow::addFlushSvnOperateWidget(QString &projectName)
{
    QString selectProject = ui->cmbPorjectSelect->currentText();
    if(projectName != selectProject) return;

    QStringList &pathNameList = configClass::getInstance().getProjectSvnNameList(projectName);
    for(int i = 0; i < pathNameList.count(); ++i)
    {
        QString pathName = pathNameList[i];
        SvnPathItem *item = getSvnPathItemByName(pathName);
        if(nullptr == item)
        {
            item = new SvnPathItem();
            item->label = new QLabel(pathName, ui->svn_scrollAreaWidgetContents);
            item->label->setGeometry(0, 0, 260, 23);

            item->btnOpenPath= new QPushButton("...", ui->svn_scrollAreaWidgetContents);
            item->btnOpenPath->setObjectName(pathName);
            item->btnOpenPath->setGeometry(0, 0, 40, 23);
            connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnOpendir);

            QString path = configClass::getInstance().getProjectSvnPath(projectName, pathName);
            item->lineEdit = new QLineEdit(path, ui->svn_scrollAreaWidgetContents);
            item->lineEdit->setGeometry(0, 0, 470, 23);
            item->lineEdit->setReadOnly(true);

            item->btnUpdate = new QPushButton("update", ui->svn_scrollAreaWidgetContents);
            item->btnUpdate->setObjectName(pathName);
            item->btnUpdate->setGeometry(0, 0, 60, 21);
            connect(item->btnUpdate, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnUpdate);

            item->btnCommit = new QPushButton("commit", ui->svn_scrollAreaWidgetContents);
            item->btnCommit->setObjectName(pathName);
            item->btnCommit->setGeometry(0, 0, 60, 21);
            connect(item->btnCommit, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnCommit);

            item->btnRevert = new QPushButton("revert", ui->svn_scrollAreaWidgetContents);
            item->btnRevert->setObjectName(pathName);
            item->btnRevert->setGeometry(0, 0, 60, 21);
            connect(item->btnRevert, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnRevert);

            item->btnCleanup= new QPushButton("cleanup", ui->svn_scrollAreaWidgetContents);
            item->btnCleanup->setObjectName(pathName);
            item->btnCleanup->setGeometry(0, 0, 60, 21);
            connect(item->btnCleanup, &QPushButton::clicked, this, &MainWindow::slotsvn_BtnCleanup);

            item->label->show();
            item->btnOpenPath->show();
            item->lineEdit->show();
            item->btnUpdate->show();
            item->btnCommit->show();
            item->btnRevert->show();
            item->btnCleanup->show();

            m_svnPathItemMap[pathName] = item;
        }
    }

    flushSvnOperateWidget();
}

void MainWindow::deleteFlushSvnOperateWidget(QString &projectName)
{
    QString selectProject = ui->cmbPorjectSelect->currentText();
    if(projectName != selectProject) return;

     QStringList &pathNameList = configClass::getInstance().getProjectSvnNameList(projectName);
     std::map<QString,int> pathNameMap;
     for(int i = 0; i < pathNameList.count(); ++i)
     {
         QString tempName = pathNameList[i];
         pathNameMap[tempName] = i;
     }

    std::map<QString, SvnPathItem*>::iterator iter = m_svnPathItemMap.begin();
    for(; iter != m_svnPathItemMap.end();)
    {
        QString pathName = iter->second->label->text();
        if(pathNameMap.find(pathName) == pathNameMap.end())
        {
            delete iter->second;
            m_svnPathItemMap.erase(iter++);
            continue;
        }

        ++iter;
    }

    flushSvnOperateWidget();
}

void MainWindow::flushSvnOperateWidget()
{
    std::map<QString, SvnPathItem*>::iterator iter = m_svnPathItemMap.begin();
    int i = 0;
    for(; iter != m_svnPathItemMap.end(); ++iter)
    {
        SvnPathItem *item = iter->second;
        item->label->move(SVN_STRAT_X_1, SVN_START_Y_1 + i * SVN_ITEM_INTERVAL_Y);
        item->btnOpenPath->move(SVN_STRAT_X_1, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        item->lineEdit->move(SVN_STRAT_X_2, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        item->btnUpdate->move(SVN_STRAT_X_3, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        item->btnCommit->move(SVN_STRAT_X_4, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        item->btnRevert->move(SVN_STRAT_X_5, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        item->btnCleanup->move(SVN_STRAT_X_6, SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
        ++i;
    }

    ui->svn_scrollAreaWidgetContents->setMinimumHeight(SVN_START_Y_2 + i * SVN_ITEM_INTERVAL_Y);
}

MainWindow::SvnPathItem *MainWindow::getSvnPathItemByName(const QString &name)
{
    std::map<QString, SvnPathItem*>::iterator iter = m_svnPathItemMap.find(name);
    if(iter != m_svnPathItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void MainWindow::initScriptOperateWidget(bool isforce)
{
    QString defaultProject = configClass::getInstance().getDefaultProject();
    if("" == defaultProject && !isforce) return;

    destroyScriptOperateWidget();

    QStringList scriptNameList = configClass::getInstance().getProjectScriptNameList(defaultProject);
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scriptName = scriptNameList[i];
        ScriptItem *item = new ScriptItem();
        item->label = new QLabel(scriptName, ui->script_scrollAreaWidgetContents);
        item->label->setGeometry(SCRIPT_STRAT_X_1, SCRIPT_START_Y_1 + i * SCRIPT_ITEM_INTERVAL_Y, 260, 23);

        item->btnOpenPath= new QPushButton("...", ui->script_scrollAreaWidgetContents);
        item->btnOpenPath->setObjectName(scriptName);
        item->btnOpenPath->setGeometry(SCRIPT_STRAT_X_1, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 40, 23);
        connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotscript_BtnOpendir);

        QString script = configClass::getInstance().getProjectScript(defaultProject, scriptName);
        item->scriptEdit = new QLineEdit(script, ui->script_scrollAreaWidgetContents);
        item->scriptEdit->setGeometry(SCRIPT_STRAT_X_2, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 220, 23);
        item->scriptEdit->setReadOnly(true);

        QString scriptParam = configClass::getInstance().getProjectScriptParam(defaultProject, scriptName);
        item->scriptParamtEdit = new QLineEdit(scriptParam, ui->script_scrollAreaWidgetContents);
        item->scriptParamtEdit->setGeometry(SCRIPT_STRAT_X_3, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 130, 23);

        item->btnExecut = new QPushButton("excute", ui->script_scrollAreaWidgetContents);
        item->btnExecut->setObjectName(scriptName);
        item->btnExecut->setGeometry(SCRIPT_STRAT_X_4, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnExecut, &QPushButton::clicked, this, &MainWindow::slotscript_BtnExcute);

        item->label->show();
        item->btnOpenPath->show();
        item->scriptEdit->show();
        item->scriptParamtEdit->show();
        item->btnExecut->show();

        m_scirptItemMap[scriptName] = item;
    }

    ui->script_scrollAreaWidgetContents->setMinimumHeight(SVN_START_Y_2 + scriptNameList.count() * SVN_ITEM_INTERVAL_Y);
}

void MainWindow::destroyScriptOperateWidget()
{
    std::map<QString, ScriptItem*>::iterator iter =  m_scirptItemMap.begin();
    for(;iter != m_scirptItemMap.end(); ++iter)
    {
       delete iter->second;
    }

    m_scirptItemMap.clear();
}

void MainWindow::addFlushScriptOperateWidget(QString &projectName)
{
    QString selectProject = ui->cmbPorjectSelect->currentText();
    if(projectName != selectProject) return;

    QStringList &scriptNameList = configClass::getInstance().getProjectScriptNameList(projectName);
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scirptName = scriptNameList[i];
        ScriptItem *item = getScriptItemByName(scirptName);
        if(nullptr == item)
        {
            item = new ScriptItem();
            item->label = new QLabel(scirptName, ui->script_scrollAreaWidgetContents);
            item->label->setGeometry(0, 0, 260, 23);

            item->btnOpenPath= new QPushButton("...", ui->script_scrollAreaWidgetContents);
            item->btnOpenPath->setObjectName(scirptName);
            item->btnOpenPath->setGeometry(0, 0, 40, 23);
            connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotscript_BtnOpendir);

            QString script = configClass::getInstance().getProjectScript(projectName, scirptName);
            item->scriptEdit = new QLineEdit(script, ui->script_scrollAreaWidgetContents);
            item->scriptEdit->setGeometry(0, 0, 220, 23);
            item->scriptEdit->setReadOnly(true);

            QString scriptParam = configClass::getInstance().getProjectScriptParam(projectName, scirptName);
            item->scriptParamtEdit = new QLineEdit(scriptParam, ui->script_scrollAreaWidgetContents);
            item->scriptParamtEdit->setGeometry(0, 0, 130, 23);

            item->btnExecut = new QPushButton("excute", ui->script_scrollAreaWidgetContents);
            item->btnExecut->setObjectName(scirptName);
            item->btnExecut->setGeometry(0, 0, 60, 21);
            connect(item->btnExecut, &QPushButton::clicked, this, &MainWindow::slotscript_BtnExcute);

            item->label->show();
            item->btnOpenPath->show();
            item->scriptEdit->show();
            item->scriptParamtEdit->show();
            item->btnExecut->show();

            m_scirptItemMap[scirptName] = item;
        }
    }

    flushScriptOperateWidget();
}

void MainWindow::deleteFlushScriptOperateWidget(QString &projectName)
{
    QString selectProject = ui->cmbPorjectSelect->currentText();
    if(projectName != selectProject) return;

     QStringList &scriptNameList = configClass::getInstance().getProjectScriptNameList(projectName);
     std::map<QString,int> scriptNameMap;
     for(int i = 0; i < scriptNameList.count(); ++i)
     {
         QString tempName = scriptNameList[i];
         scriptNameMap[tempName] = i;
     }

    std::map<QString, ScriptItem*>::iterator iter = m_scirptItemMap.begin();
    for(; iter != m_scirptItemMap.end();)
    {
        QString scriptName = iter->second->label->text();
        if(scriptNameMap.find(scriptName) == scriptNameMap.end())
        {
            delete iter->second;
            m_scirptItemMap.erase(iter++);
            continue;
        }

        ++iter;
    }

    flushScriptOperateWidget();
}

void MainWindow::flushScriptOperateWidget()
{
    std::map<QString, ScriptItem*>::iterator iter = m_scirptItemMap.begin();
    int i = 0;
    for(; iter != m_scirptItemMap.end(); ++iter)
    {
        ScriptItem *item = iter->second;
        item->label->move(SCRIPT_STRAT_X_1, SCRIPT_START_Y_1 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->btnOpenPath->move(SCRIPT_STRAT_X_1, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->scriptEdit->move(SCRIPT_STRAT_X_2, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->scriptParamtEdit->move(SCRIPT_STRAT_X_3, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->btnExecut->move(SCRIPT_STRAT_X_4, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        ++i;
    }

    ui->script_scrollAreaWidgetContents->setMinimumHeight(SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
}

MainWindow::ScriptItem *MainWindow::getScriptItemByName(const QString &name)
{
    std::map<QString, ScriptItem*>::iterator iter = m_scirptItemMap.find(name);
    if(iter != m_scirptItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void MainWindow::initGlobalScriptOperateWidget()
{
    destroyGlobalScriptOperateWidget();

    QStringList scriptNameList = configClass::getInstance().getGlobalScriptNameList();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scriptName = scriptNameList[i];
        ScriptItem *item = new ScriptItem();
        item->label = new QLabel(scriptName, ui->g_script_scrollAreaWidgetContents);
        item->label->setGeometry(SCRIPT_STRAT_X_1, SCRIPT_START_Y_1 + i * SCRIPT_ITEM_INTERVAL_Y, 260, 23);

        item->btnOpenPath= new QPushButton("...", ui->g_script_scrollAreaWidgetContents);
        item->btnOpenPath->setObjectName(scriptName);
        item->btnOpenPath->setGeometry(SCRIPT_STRAT_X_1, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 40, 23);
        connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotglobalScript_BtnOpendir);

        QString script = configClass::getInstance().getGlobalScript(scriptName);
        item->scriptEdit = new QLineEdit(script, ui->g_script_scrollAreaWidgetContents);
        item->scriptEdit->setGeometry(SCRIPT_STRAT_X_2, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 220, 23);
        item->scriptEdit->setReadOnly(true);

        QString scriptParam = configClass::getInstance().getGlobalScriptParam(scriptName);
        item->scriptParamtEdit = new QLineEdit(scriptParam, ui->g_script_scrollAreaWidgetContents);
        item->scriptParamtEdit->setGeometry(SCRIPT_STRAT_X_3, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 130, 23);

        item->btnExecut = new QPushButton("excute", ui->g_script_scrollAreaWidgetContents);
        item->btnExecut->setObjectName(scriptName);
        item->btnExecut->setGeometry(SCRIPT_STRAT_X_4, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y, 60, 21);
        connect(item->btnExecut, &QPushButton::clicked, this, &MainWindow::slotglobalScript_BtnExcute);

        item->label->show();
        item->btnOpenPath->show();
        item->scriptEdit->show();
        item->scriptParamtEdit->show();
        item->btnExecut->show();

        m_global_scirptItemMap[scriptName] = item;
    }

    ui->g_script_scrollAreaWidgetContents->setMinimumHeight(SVN_START_Y_2 + scriptNameList.count() * SVN_ITEM_INTERVAL_Y);
}

void MainWindow::destroyGlobalScriptOperateWidget()
{
    std::map<QString, ScriptItem*>::iterator iter =  m_global_scirptItemMap.begin();
    for(;iter != m_global_scirptItemMap.end(); ++iter)
    {
       delete iter->second;
    }

    m_global_scirptItemMap.clear();
}

void MainWindow::addFlushGlobalScriptOperateWidget()
{
    QStringList &scriptNameList = configClass::getInstance().getGlobalScriptNameList();
    for(int i = 0; i < scriptNameList.count(); ++i)
    {
        QString scirptName = scriptNameList[i];
        ScriptItem *item = getGlobalScriptItemByName(scirptName);
        if(nullptr == item)
        {
            item = new ScriptItem();
            item->label = new QLabel(scirptName, ui->g_script_scrollAreaWidgetContents);
            item->label->setGeometry(0, 0, 260, 23);

            item->btnOpenPath= new QPushButton("...", ui->g_script_scrollAreaWidgetContents);
            item->btnOpenPath->setObjectName(scirptName);
            item->btnOpenPath->setGeometry(0, 0, 40, 23);
            connect(item->btnOpenPath, &QPushButton::clicked, this, &MainWindow::slotglobalScript_BtnOpendir);

            QString script = configClass::getInstance().getGlobalScript(scirptName);
            item->scriptEdit = new QLineEdit(script, ui->g_script_scrollAreaWidgetContents);
            item->scriptEdit->setGeometry(0, 0, 220, 23);
            item->scriptEdit->setReadOnly(true);

            QString scriptParam = configClass::getInstance().getGlobalScriptParam(scirptName);
            item->scriptParamtEdit = new QLineEdit(scriptParam, ui->g_script_scrollAreaWidgetContents);
            item->scriptParamtEdit->setGeometry(0, 0, 130, 23);

            item->btnExecut = new QPushButton("excute", ui->g_script_scrollAreaWidgetContents);
            item->btnExecut->setObjectName(scirptName);
            item->btnExecut->setGeometry(0, 0, 60, 21);
            connect(item->btnExecut, &QPushButton::clicked, this, &MainWindow::slotglobalScript_BtnExcute);

            item->label->show();
            item->btnOpenPath->show();
            item->scriptEdit->show();
            item->scriptParamtEdit->show();
            item->btnExecut->show();

            m_global_scirptItemMap[scirptName] = item;
        }
    }

    flushGlobalScriptOperateWidget();
}

void MainWindow::deleteFlushGlobalScriptOperateWidget()
{
     QStringList &scriptNameList = configClass::getInstance().getGlobalScriptNameList();
     std::map<QString,int> scriptNameMap;
     for(int i = 0; i < scriptNameList.count(); ++i)
     {
         QString tempName = scriptNameList[i];
         scriptNameMap[tempName] = i;
     }

    std::map<QString, ScriptItem*>::iterator iter = m_global_scirptItemMap.begin();
    for(; iter != m_global_scirptItemMap.end();)
    {
        QString scriptName = iter->second->label->text();
        if(scriptNameMap.find(scriptName) == scriptNameMap.end())
        {
            delete iter->second;
            m_global_scirptItemMap.erase(iter++);
            continue;
        }

        ++iter;
    }

    flushGlobalScriptOperateWidget();
}

void MainWindow::flushGlobalScriptOperateWidget()
{
    std::map<QString, ScriptItem*>::iterator iter = m_global_scirptItemMap.begin();
    int i = 0;
    for(; iter != m_global_scirptItemMap.end(); ++iter)
    {
        ScriptItem *item = iter->second;
        item->label->move(SCRIPT_STRAT_X_1, SCRIPT_START_Y_1 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->btnOpenPath->move(SCRIPT_STRAT_X_1, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->scriptEdit->move(SCRIPT_STRAT_X_2, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->scriptParamtEdit->move(SCRIPT_STRAT_X_3, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        item->btnExecut->move(SCRIPT_STRAT_X_4, SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
        ++i;
    }

    ui->g_script_scrollAreaWidgetContents->setMinimumHeight(SCRIPT_START_Y_2 + i * SCRIPT_ITEM_INTERVAL_Y);
}

MainWindow::ScriptItem *MainWindow::getGlobalScriptItemByName(const QString &name)
{
    std::map<QString, ScriptItem*>::iterator iter = m_global_scirptItemMap.find(name);
    if(iter != m_global_scirptItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void MainWindow::initExportOperateWidget(bool isforce)
{
    QString defaultProject = configClass::getInstance().getDefaultProject();
    if("" == defaultProject && !isforce) return;

    destroyExportOperateWidget();


    flushExportOperateWidget();
}

void MainWindow::destroyExportOperateWidget()
{

}

void MainWindow::flushExportOperateWidget()
{
    QString curProject = configClass::getInstance().getDefaultProject();
    QString inputDirPath = configClass::getInstance().getProjectExportInputPath(curProject);
    QString outputDirPath = configClass::getInstance().getProjectExportOutputPath(curProject);
    ui->lineEditInput->setText(inputDirPath);
    ui->lineEditOutput->setText(outputDirPath);

    QDir inputDir(inputDirPath);
    QString name = "*.xls"; //选择要筛选的文件类型
    QStringList filters; //筛选器
    filters<<name;
    QFileInfoList list = inputDir.entryInfoList(filters);

    for(int i=0; i<list.count() ;i++)
    {
        QFileInfo tmpFileInfo = list.at(i);
        if(tmpFileInfo.isDir())
        {
            QIcon icon("dir.png"); //在显示的目录前加图片
            QString fileName = tmpFileInfo.fileName();
            //QListWidgetItem *tmp = new QListWidgetItem(fileName);
            QListWidgetItem *tmp = new QListWidgetItem(icon, fileName); //添加图片
            ui->listWidget->addItem(tmp);
        }
        else if(tmpFileInfo.isFile())
        {
            QIcon icon("../history/jinmao_1.jpg");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(fileName);
            ui->listWidget->addItem(tmp);
        }
    }

}

void MainWindow::on_cmbPorjectSelect_currentTextChanged(const QString &arg1)
{ 
    QString defaultProject = configClass::getInstance().getDefaultProject();
    qDebug() << arg1 <<endl;
    qDebug() << defaultProject <<endl;
    if(arg1 != defaultProject)
    {
        QString str = "select project " + arg1;
        appendOutputInfo(str);
        configClass::getInstance().setDefaultProject(arg1);

        if("" != arg1)
        {
            initSvnOperateWidget();
            initScriptOperateWidget();
            initExportOperateWidget();
        }
        else
        {
            destroySvnOperateWidget();
            destroyScriptOperateWidget();
            destroyExportOperateWidget();
        }
    }
}

void MainWindow::on_action_N_triggered()
{
    NewProjectDialog w(this);
    w.showDialog();
}

void MainWindow::on_action_C_triggered()
{
    ProjectManagerDialog w(this);
    w.showDialog(true);
}

void MainWindow::on_action_G_triggered()
{
    GlobalScrtipManagerdialog w(this);
    w.showDialog(true);
}

void MainWindow::on_btnSvn_clicked()
{
    ui->scriptWidget->setVisible(false);
    ui->exopotwidget->setVisible(false);
    ui->svnWidget->setVisible(true);
}

void MainWindow::on_btnScript_clicked()
{
    ui->scriptWidget->setVisible(true);
    ui->exopotwidget->setVisible(false);
    ui->svnWidget->setVisible(false);
}

void MainWindow::on_btnExport_clicked()
{
    ui->scriptWidget->setVisible(false);
    ui->exopotwidget->setVisible(true);
    ui->svnWidget->setVisible(false);
}

void MainWindow::on_btnClear_clicked()
{
    ui->outputInfoText->clear();
}

void MainWindow::on_btnUpdateAll_clicked()
{
    QString curProject = configClass::getInstance().getDefaultProject();
    QStringList svnPathList = configClass::getInstance().getProjectSvnPathList(curProject);
    QString path = "";
    for(int i = 0; i < svnPathList.count(); ++i)
    {
        path +=svnPathList[i];
        path +="*";
    }

    if("" != path && "*" != path)
    {
        invokeclass::getInstance().svnUpdate(path);
    }
}

void MainWindow::on_btnCommitAll_clicked()
{
    QString curProject = configClass::getInstance().getDefaultProject();
    QStringList svnPathList = configClass::getInstance().getProjectSvnPathList(curProject);
    for(int i = 0; i < svnPathList.count(); ++i)
    {
        invokeclass::getInstance().svnCommit(svnPathList[i]);
    }
}

void MainWindow::on_btnCleanupAll_clicked()
{
    QString curProject = configClass::getInstance().getDefaultProject();
    QStringList svnPathList = configClass::getInstance().getProjectSvnPathList(curProject);
    for(int i = 0; i < svnPathList.count(); ++i)
    {
        invokeclass::getInstance().svnCleanUp(svnPathList[i]);
    }
}

void MainWindow::on_btnRevertAll_clicked()
{
    QString curProject = configClass::getInstance().getDefaultProject();
    QStringList svnPathList = configClass::getInstance().getProjectSvnPathList(curProject);
    for(int i = 0; i < svnPathList.count(); ++i)
    {
        invokeclass::getInstance().svnRevert(svnPathList[i]);
    }
}

void MainWindow::slotsvn_BtnUpdate()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    SvnPathItem* pathItem = getSvnPathItemByName(pathName);
    if(nullptr != pathItem)
    {
        QString curProject = configClass::getInstance().getDefaultProject();
        QString path = pathItem->lineEdit->text();
        invokeclass::getInstance().svnUpdate(path);
        QString outputStr = curProject + " update " + pathName+ " svn path:" + path;
        appendOutputInfo(outputStr);
    }
}

void MainWindow::slotsvn_BtnCommit()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    SvnPathItem* pathItem = getSvnPathItemByName(pathName);
    if(nullptr != pathItem)
    {
        QString curProject = configClass::getInstance().getDefaultProject();
        QString path = pathItem->lineEdit->text();
        invokeclass::getInstance().svnCommit(path);
        QString outputStr = curProject + " commit " + pathName+ " svn path:" + path;
        appendOutputInfo(outputStr);
    }
}

void MainWindow::slotsvn_BtnRevert()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    SvnPathItem* pathItem = getSvnPathItemByName(pathName);
    if(nullptr != pathItem)
    {
        QString curProject = configClass::getInstance().getDefaultProject();
        QString path = pathItem->lineEdit->text();
        invokeclass::getInstance().svnRevert(path);
        QString outputStr = curProject + " revert " + pathName+ " svn path:" + path;
        appendOutputInfo(outputStr);
    }
}

void MainWindow::slotsvn_BtnCleanup()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    SvnPathItem* pathItem = getSvnPathItemByName(pathName);
    if(nullptr != pathItem)
    {
        QString curProject = configClass::getInstance().getDefaultProject();
        QString path = pathItem->lineEdit->text();
        invokeclass::getInstance().svnCleanUp(path);
        QString outputStr = curProject + " cleanup " + pathName+ " svn path:" + path;
        appendOutputInfo(outputStr);
    }
}

void MainWindow::slotsvn_BtnOpendir()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    QStringList pathList;
    pathList.clear();
    SvnPathItem* pathItem = getSvnPathItemByName(pathName);
    if(nullptr == pathItem) return;

    QString path = pathItem->lineEdit->text();
    if("" == path) return;

    pathList = path.split("*");

    if(1 == pathList.count())
    {
        invokeclass::getInstance().openDirectory(pathList[0]);
    }
    else if(pathList.count() > 1)
    {
        ShowPathDialog w(this);
        w.createPathItem(pathList);
        w.showDialog(true);
    }
}

void MainWindow::slotscript_BtnOpendir()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    QStringList pathList;
    pathList.clear();
    ScriptItem* scriptItem = getScriptItemByName(pathName);
    if(nullptr == scriptItem) return;

    QString path = scriptItem->scriptEdit->text();
    if("" == path) return;

    pathList = path.split(";");

    if(1 == pathList.count())
    {
        QString temp = pathList[0];
        int i = temp.count();
        for(; i >= 0; --i)
        {
            if(temp[i] == "/")
            {
                break;
            }
        }

        temp.remove(i, temp.count());
        invokeclass::getInstance().openDirectory(temp);
    }
    else if(pathList.count() > 1)
    {
        ShowPathDialog w(this);
        w.createPathItem(pathList, false);
        w.showDialog(true);
    }
}

void MainWindow::slotscript_BtnExcute()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString scriptName = act->objectName();

    ScriptItem *item = getScriptItemByName(scriptName);
    if(nullptr == item) return;

    QString script = item->scriptEdit->text();
    QString scriptParam = item->scriptParamtEdit->text();

    QStringList scriptList = script.split(";");
    QStringList scriptParamList = scriptParam.split(";");

    for(int i = 0; i < scriptList.count(); ++i)
    {
        QString param = "";
        if(i < scriptParamList.count() && ";" != scriptParamList[i])
        {
            param = scriptParamList[i];
        }
        invokeclass::getInstance().callCMDexecuteProgram(scriptList[i], param);
    }

    QString curProject = configClass::getInstance().getDefaultProject();
    QString outputStr = curProject + " execute " + scriptName+ ", param:" + scriptParam;
    appendOutputInfo(outputStr);
}

void MainWindow::slotglobalScript_BtnOpendir()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    QStringList pathList;
    pathList.clear();
    ScriptItem* scriptItem = getGlobalScriptItemByName(pathName);
    if(nullptr == scriptItem) return;

    QString path = scriptItem->scriptEdit->text();
    if("" == path) return;

    pathList = path.split(";");

    if(1 == pathList.count())
    {
        QString temp = pathList[0];
        int i = temp.count();
        for(; i >= 0; --i)
        {
            if(temp[i] == "/")
            {
                break;
            }
        }

        temp.remove(i, temp.count());
        invokeclass::getInstance().openDirectory(temp);
    }
    else if(pathList.count() > 1)
    {
        ShowPathDialog w(this);
        w.createPathItem(pathList, false);
        w.showDialog(true);
    }
}

void MainWindow::slotglobalScript_BtnExcute()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString scriptName = act->objectName();

    ScriptItem *item = getGlobalScriptItemByName(scriptName);
    if(nullptr == item) return;

    QString script = item->scriptEdit->text();
    QString scriptParam = item->scriptParamtEdit->text();

    QStringList scriptList = script.split(";");
    QStringList scriptParamList = scriptParam.split(";");

    for(int i = 0; i < scriptList.count(); ++i)
    {
        QString param = "";
        if(i < scriptParamList.count() && ";" != scriptParamList[i])
        {
            param = scriptParamList[i];
        }
        invokeclass::getInstance().callCMDexecuteProgram(scriptList[i], param);
    }

    QString curProject = configClass::getInstance().getDefaultProject();
    QString outputStr = "execute global script " + scriptName+ ", param:" + scriptParam;
    appendOutputInfo(outputStr);
}

void MainWindow::on_btnBuild_clicked()
{
    QString outputStr = "on_btnBuild_clicked";
    appendOutputInfo(outputStr);
}

void MainWindow::on_btnBuildAll_clicked()
{
    QString outputStr = "on_btnBuildAll_cliced";
    appendOutputInfo(outputStr);
}

void MainWindow::on_btnSetting_clicked()
{
    QString outputStr = "on_btnSetting_clicked";
    appendOutputInfo(outputStr);
}

void MainWindow::on_btnInputBrowse_clicked()
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
            QString curProject = configClass::getInstance().getDefaultProject();
            configClass::getInstance().setProjectExportInputPath(curProject, fileName[0]);
            flushExportOperateWidget();

            QString outputStr = curProject + " update export input dir" + " path:" + fileName[0];
            appendOutputInfo(outputStr);
        }
     }
}

void MainWindow::on_btnInputOpen_clicked()
{
    invokeclass::getInstance().openDirectory(configClass::getInstance().getProjectExportInputPath(configClass::getInstance().getDefaultProject()));
}

void MainWindow::on_btnOutputBrowse_clicked()
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
            QString curProject = configClass::getInstance().getDefaultProject();
            configClass::getInstance().setProjectExportOutputPath(curProject, fileName[0]);
            flushExportOperateWidget();

            QString outputStr = curProject + " update export output dir" + " path:" + fileName[0];
            appendOutputInfo(outputStr);
        }
     }
}

void MainWindow::on_btnOutputOpen_clicked()
{
    invokeclass::getInstance().openDirectory(configClass::getInstance().getProjectExportOutputPath(configClass::getInstance().getDefaultProject()));
}
