#include "newsvnpathdialog.h"
#include "configclass.h"
#include "mainwindow.h"
#include <QFileDialog>
#include "dialogcommondef.h"


NewSvnPathDialog::NewSvnPathDialog(QWidget *parent):BaseDialog (parent),
    pathNamelabel(nullptr), pathNameEdit(nullptr), pathEdit(nullptr), btnAddPath(nullptr), btnOk(nullptr),
    m_project_name("")
{
    init();
}

NewSvnPathDialog::~NewSvnPathDialog()
{
    if(nullptr != btnOk)
        delete btnOk;

    if(nullptr != btnAddPath)
        delete btnAddPath;

    if(nullptr != pathEdit)
        delete pathEdit;

    if(nullptr != pathNameEdit)
        delete pathNameEdit;

    if(nullptr != pathNamelabel)
        delete pathNamelabel;
}

bool NewSvnPathDialog::init()
{
    setTitle("NewSvnPath");
    resize(300, 150);
    setFixedSize(300, 150);

    pathNamelabel = new QLabel("Path Name:", this);
    pathNamelabel->setGeometry(10, 25, 60, 23);

    pathNameEdit = new QLineEdit(this);
    pathNameEdit->setGeometry(70, 25, 210, 23);
    pathNameEdit->setObjectName("pathNameEdit");

    btnAddPath = new QPushButton("Add..", this);
    btnAddPath->setGeometry(10, 55, 60, 23);
    btnAddPath->setObjectName("btnAddPath");

    pathEdit = new QLineEdit(this);
    pathEdit->setGeometry(70, 55, 210, 23);

    btnOk = new QPushButton("OK", this);
    btnOk->setGeometry(120, 100, 60, 23);
    btnOk->setObjectName("btnOk");

    QMetaObject::connectSlotsByName(this);

    return true;
}

void NewSvnPathDialog::on_pathNameEdit_textChanged(const QString &arg1)
{
    QString inputStr = arg1;
    int curlen = inputStr.count();
    if(curlen > SVN_PATH_NAME_LEGTH_MAX)
    {
        inputStr.remove(SVN_PATH_NAME_LEGTH_MAX, curlen - SVN_PATH_NAME_LEGTH_MAX);
        pathNameEdit->setText(inputStr);
    }
}

void NewSvnPathDialog::on_btnAddPath_clicked()
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
            QString path = pathEdit->text();
            if("" != path)
                path += "*";
            path += fileName[0];
            pathEdit->setText(path);
        }
     }
}

void NewSvnPathDialog::on_btnOk_clicked()
{
    QString pathName = pathNameEdit->text();
    if("" == pathName)return;

    if(!configClass::getInstance().hasProject(m_project_name))
        return;

    QString path = pathEdit->text();

    if(configClass::getInstance().addProjectSvnPath(m_project_name, pathName, path))
    {
        MainWindow *parent = getParentPtr<MainWindow>();
        if(nullptr != parent)
        {
            QString outputStr = m_project_name + " add svn path success " + pathName + ":" + path;
            parent->appendOutputInfo(outputStr);
            parent->addFlushSvnOperateWidget(m_project_name);
        }
    }

    this->close();
}
