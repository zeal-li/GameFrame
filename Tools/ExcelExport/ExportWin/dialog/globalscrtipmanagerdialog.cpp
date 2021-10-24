#include "globalscrtipmanagerdialog.h"
#include "newscriptdialog.h"
#include "mainwindow.h"
#include "configclass.h"
#include "dialogcommondef.h"
#include <QFileDialog>

GlobalScrtipManagerdialog::GlobalScrtipManagerdialog(QWidget *parent):BaseDialog (parent),
    scrollArea(nullptr), scrollAreaWidgetContents(nullptr), btnAddScript(nullptr), btnApply(nullptr)
{
    init();
}

GlobalScrtipManagerdialog::~GlobalScrtipManagerdialog()
{
    destoryScriptItemMap();

    if(nullptr != btnApply)
        delete btnApply;

    if(nullptr != btnAddScript)
        delete btnAddScript;

    if(nullptr != scrollAreaWidgetContents)
        delete scrollAreaWidgetContents;

    if(nullptr != scrollArea)
        delete scrollArea;
}

bool GlobalScrtipManagerdialog::init()
{
    setTitle("GlobalScriptEdit");
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

    btnAddScript = new QPushButton("add...", this);
    btnAddScript->setObjectName("btnAddScript");
    btnAddScript->setGeometry(5, 10, 65, 25);
    connect(btnAddScript, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slotBtnAddScript);

    btnApply = new QPushButton("Apply", this);
    btnApply->setObjectName("btnApply");
    btnApply->setGeometry(360, 590, 60, 23);
    connect(btnApply, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slotBtnApply);

    initScriptItemMap();

    QMetaObject::connectSlotsByName(this);
    return true;
}

void GlobalScrtipManagerdialog::initScriptItemMap()
{
    QStringList &scriptNameList = configClass::getInstance().getGlobalScriptNameList();
    for(int j = 0; j < scriptNameList.count(); ++j)
    {
        QString scriptName = scriptNameList[j];
        ScriptItem *item = new ScriptItem(scriptName);
        item->scriptNameEdit = new QLineEdit(scriptName, scrollAreaWidgetContents);
        item->scriptNameEdit->setGeometry(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1, 250, 23);
        item->scriptNameEdit->setObjectName(scriptName);
        connect(item->scriptNameEdit, &QLineEdit::textChanged, this, &GlobalScrtipManagerdialog::slot_ScriptItem_scriptNameEdit_textChanged);

        item->btnAddScript = new QPushButton("addScript", scrollAreaWidgetContents);
        item->btnAddScript->setGeometry(G_SCIRPT_DETAIL_START_X_2, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1, 60, 23);
        item->btnAddScript->setObjectName(scriptName);
        connect(item->btnAddScript, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slot_ScriptItem_BtnAddScript);

        item->btnDeleteScript = new QPushButton("Delete", scrollAreaWidgetContents);
        item->btnDeleteScript->setGeometry(G_SCIRPT_DETAIL_START_X_3, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1, 60, 23);
        item->btnDeleteScript->setObjectName(scriptName);
        connect(item->btnDeleteScript, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slot_ScriptItem_BtnDeleteScript);

        QString script = configClass::getInstance().getGlobalScript(scriptName);
        item->scriptEdit = new QLineEdit(script, scrollAreaWidgetContents);
        item->scriptEdit->setGeometry(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_2 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1, 250, 23);

        QString scriptParam = configClass::getInstance().getGlobalScriptParam(scriptName);
        item->scriptParamEdit = new QLineEdit(scriptParam, scrollAreaWidgetContents);
        item->scriptParamEdit->setGeometry(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_3 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1, 250, 23);

        scriptItemMap[scriptName] = item;
    }

    scrollAreaWidgetContents->setMinimumHeight(G_SCIRPT_DETAIL_START_Y_2 + scriptNameList.count() * G_SCIRPT_DETAIL_INTERVAL_Y_1);
}

void GlobalScrtipManagerdialog::destoryScriptItemMap()
{
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

GlobalScrtipManagerdialog::ScriptItem *GlobalScrtipManagerdialog::getScriptItemByName(QString &name)
{
    std::map<QString, ScriptItem*>::iterator iter = scriptItemMap.find(name);
    if(iter != scriptItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void GlobalScrtipManagerdialog::flushAddGlobalScriptItem()
{
    QStringList &scriptNameList = configClass::getInstance().getGlobalScriptNameList();
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
            connect(item->scriptNameEdit, &QLineEdit::textChanged, this, &GlobalScrtipManagerdialog::slot_ScriptItem_scriptNameEdit_textChanged);

            item->btnAddScript = new QPushButton("addScript", scrollAreaWidgetContents);
            item->btnAddScript->setObjectName(scriptName);
            item->btnAddScript->setGeometry(0, 0, 60, 23);
            connect(item->btnAddScript, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slot_ScriptItem_BtnAddScript);

            item->btnDeleteScript = new QPushButton("Delete", scrollAreaWidgetContents);
            item->btnDeleteScript->setObjectName(scriptName);
            item->btnDeleteScript->setGeometry(0, 0, 60, 23);
            connect(item->btnDeleteScript, &QPushButton::clicked, this, &GlobalScrtipManagerdialog::slot_ScriptItem_BtnDeleteScript);

            QString script = configClass::getInstance().getGlobalScript(scriptName);
            item->scriptEdit = new QLineEdit(script, scrollAreaWidgetContents);
            item->scriptEdit->setGeometry(0, 0, 250, 23);

            QString scriptParam = configClass::getInstance().getGlobalScriptParam(scriptName);
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

    flushGlobalScriptItem();
}

void GlobalScrtipManagerdialog::flushGlobalScriptItem()
{
    std::map<QString, ScriptItem*>::iterator s_iter = scriptItemMap.begin();
    int j = 0;
    for(; s_iter != scriptItemMap.end(); ++s_iter)
    {
        ScriptItem *item = s_iter->second;
        item->scriptNameEdit->move(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
        item->btnAddScript->move(G_SCIRPT_DETAIL_START_X_2, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
        item->btnDeleteScript->move(G_SCIRPT_DETAIL_START_X_3, G_SCIRPT_DETAIL_START_Y_1 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
        item->scriptEdit->move(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_2 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
        item->scriptParamEdit->move(G_SCIRPT_DETAIL_START_X_1, G_SCIRPT_DETAIL_START_Y_3 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
        ++j;
    }

    scrollAreaWidgetContents->setMinimumHeight(G_SCIRPT_DETAIL_START_Y_2 + j * G_SCIRPT_DETAIL_INTERVAL_Y_1);
}

void GlobalScrtipManagerdialog::slotBtnAddScript()
{
    MainWindow *parent = getParentPtr<MainWindow>();
    NewScriptDialog w(parent);
    w.setIsNewGlobalScript(true);
    w.showDialog(true);

    flushAddGlobalScriptItem();
}

void GlobalScrtipManagerdialog::slotBtnApply()
{
    configClass::getInstance().clearGlobalScript();
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
            configClass::getInstance().addGlobalScript(scirptName, scirpt, scirptParam);
        }

         ++s_iter;
    }

    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        parent->initGlobalScriptOperateWidget();
    }

    this->close();
}

void GlobalScrtipManagerdialog::slot_ScriptItem_scriptNameEdit_textChanged(const QString &arg1)
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

void GlobalScrtipManagerdialog::slot_ScriptItem_BtnAddScript()
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

void GlobalScrtipManagerdialog::slot_ScriptItem_BtnDeleteScript()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString itemName = act->objectName();

    std::map<QString, ScriptItem*>::iterator iter = scriptItemMap.find(itemName);
    if(iter == scriptItemMap.end()) return;

    delete iter->second;
    scriptItemMap.erase(iter);

    if(configClass::getInstance().deleteGlobalScript(itemName))
    {
        flushGlobalScriptItem();
    }

    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        QString script = configClass::getInstance().getGlobalScript(itemName);
        QString outputStr = "delete global script success, " + itemName + ":" + script;
        parent->appendOutputInfo(outputStr);
        parent->deleteFlushGlobalScriptOperateWidget();
    }
}
