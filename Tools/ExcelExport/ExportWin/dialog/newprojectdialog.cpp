#include "newprojectdialog.h"

#include "configclass.h"
#include "mainwindow.h"
#include "dialogcommondef.h"

NewProjectDialog::NewProjectDialog(QWidget *parent):BaseDialog (parent),
    label(nullptr), lineEdit(nullptr), btnOk(nullptr)
{
    init();
}

NewProjectDialog::~NewProjectDialog()
{
    if(nullptr != btnOk)
        delete btnOk;

    if(nullptr != lineEdit)
        delete lineEdit;

    if(nullptr != label)
        delete label;
}

bool NewProjectDialog::init()
{
    setTitle("NewProject");
    resize(300, 150);
    setFixedSize(300, 150);

    label = new QLabel("Input Project Name", this);
    label->setGeometry(80, 10, 150, 21);

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(10, 50, 280, 30);
    lineEdit->setObjectName("lineEdit");

    btnOk = new QPushButton("OK", this);
    btnOk->setGeometry(110, 100, 60, 21);
    btnOk->setObjectName("btnOk");

    QMetaObject::connectSlotsByName(this);

    return true;
}

void NewProjectDialog::on_btnOk_clicked()
{
    QString inputStr = lineEdit->text();
    QString outputStr = "";
    MainWindow *parent = getParentPtr<MainWindow>();

    if(configClass::getInstance().hasProject(inputStr))
    {
        outputStr = "project " + inputStr + " is exist!";
    }
    else if("" == inputStr)
    {
        outputStr = "Project name is null!";
    }
    else
    {
        outputStr = "new project " + inputStr;
        configClass::getInstance().addProject(inputStr);
        parent->addProjectComboItem(inputStr);
    }

    parent->appendOutputInfo(outputStr);

    this->close();
}

void NewProjectDialog::on_lineEdit_textChanged(const QString &arg1)
{
    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > PROJECT_NAME_LEGTH_MAX)
    {
        inputStr.remove(PROJECT_NAME_LEGTH_MAX, curlen - PROJECT_NAME_LEGTH_MAX);
        lineEdit->setText(inputStr);
    }
}
