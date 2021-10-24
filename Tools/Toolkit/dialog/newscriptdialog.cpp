#include "newscriptdialog.h"
#include "dialogcommondef.h"
#include "configclass.h"
#include <QFileDialog>
#include "mainwindow.h"

NewScriptDialog::NewScriptDialog(QWidget *parent):BaseDialog (parent),
    scriptNamelabel(nullptr), scriptNameEdit(nullptr), scriptEdit(nullptr), btnAddScript(nullptr), scriptParamlabel(nullptr), scriptParamEdit(nullptr), btnOk(nullptr),
    m_project_name(""),m_is_new_global_script(false)
{
    init();
}

NewScriptDialog::~NewScriptDialog()
{
    if(nullptr != btnOk)
        delete btnOk;

    if(nullptr != scriptParamEdit)
        delete scriptParamEdit;

    if(nullptr != scriptParamlabel)
        delete scriptParamlabel;

    if(nullptr != btnAddScript)
        delete btnAddScript;

    if(nullptr != scriptEdit)
        delete scriptEdit;

    if(nullptr != scriptNameEdit)
        delete scriptNameEdit;

    if(nullptr != scriptNamelabel)
        delete scriptNamelabel;
}

bool NewScriptDialog::init()
{
    setTitle("NewScript");
    resize(300, 150);
    setFixedSize(300, 150);

    scriptNamelabel = new QLabel("Script Name:", this);
    scriptNamelabel->setGeometry(5, 15, 70, 23);

    scriptNameEdit = new QLineEdit(this);
    scriptNameEdit->setGeometry(80, 15, 210, 23);
    scriptNameEdit->setObjectName("scriptNameEdit");

    btnAddScript = new QPushButton("Add..", this);
    btnAddScript->setGeometry(5, 45, 65, 23);
    btnAddScript->setObjectName("btnAddScript");

    scriptEdit = new QLineEdit(this);
    scriptEdit->setGeometry(80, 45, 210, 23);

    scriptParamlabel = new QLabel("Script Param:", this);
    scriptParamlabel->setGeometry(5, 75, 75, 23);

    scriptParamEdit = new QLineEdit(this);
    scriptParamEdit->setGeometry(80, 75, 210, 23);
    scriptParamEdit->setObjectName("scriptParamEdit");

    btnOk = new QPushButton("OK", this);
    btnOk->setGeometry(120, 110, 60, 23);
    btnOk->setObjectName("btnOk");

    QMetaObject::connectSlotsByName(this);

    return true;
}

void NewScriptDialog::on_scriptNameEdit_textChanged(const QString &arg1)
{
    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > SCRIPT_NAME_LEGTH_MAX)
    {
        inputStr.remove(SCRIPT_NAME_LEGTH_MAX, curlen - SCRIPT_NAME_LEGTH_MAX);
        scriptNameEdit->setText(inputStr);
    }
}

void NewScriptDialog::on_btnAddScript_clicked()
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
            QString script = scriptEdit->text();
            if("" != script)
                script += ";";
            script += fileName[0];
            scriptEdit->setText(script);

            QString scriptparam = scriptParamEdit->text();
            scriptparam += ";";
            scriptParamEdit->setText(scriptparam);
        }
     }
}

void NewScriptDialog::on_btnOk_clicked()
{
    QString scriptName = scriptNameEdit->text();
    if("" == scriptName) return;

    QString script = scriptEdit->text();
    QString scriptParam = scriptParamEdit->text();

    if(m_is_new_global_script)
    {
        if(configClass::getInstance().addGlobalScript(scriptName, script, scriptParam))
        {
            MainWindow *parent = getParentPtr<MainWindow>();
            if(nullptr != parent)
            {
                QString outputStr = "add " + scriptName + " gloabl script success=> " + script + ":" + scriptParam;
                parent->appendOutputInfo(outputStr);
                parent->addFlushGlobalScriptOperateWidget();
            }
        }

        this->close();
    }
    else
    {
        if(!configClass::getInstance().hasProject(m_project_name))
            return;

        if(configClass::getInstance().addProjectScript(m_project_name, scriptName, script, scriptParam))
        {
            MainWindow *parent = getParentPtr<MainWindow>();
            if(nullptr != parent)
            {
                QString outputStr = m_project_name + " add " + scriptName + " script success=> " + script + ":" + scriptParam;
                parent->appendOutputInfo(outputStr);
                parent->addFlushScriptOperateWidget(m_project_name);
            }
        }

        this->close();
    }
}
