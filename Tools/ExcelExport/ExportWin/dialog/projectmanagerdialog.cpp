#include "projectmanagerdialog.h"
#include "configclass.h"
#include "dialog/newprojectdialog.h"
#include "dialog/projectdetaildialog.h"
#include "mainwindow.h"
#include "dialogcommondef.h"


ProjectManagerDialog::ProjectManagerDialog(QWidget *parent):BaseDialog (parent),
    btnAddProject(nullptr), scrollArea(nullptr), scrollAreaWidgetContents(nullptr)
{
    init();
}

ProjectManagerDialog::~ProjectManagerDialog()
{
    destroyProjectItem();

    if(nullptr != scrollAreaWidgetContents)
        delete scrollAreaWidgetContents;

    if(nullptr != scrollArea)
        delete scrollArea;

    if(nullptr != btnAddProject)
        delete btnAddProject;
}

bool ProjectManagerDialog::init()
{
    setTitle("ProjectManager");
    resize(450, 620);
    setFixedSize(450, 620);

    btnAddProject = new QPushButton("Add..", this);
    btnAddProject->setGeometry(5, 10, 65, 25);
    connect(btnAddProject, &QPushButton::clicked, this, &ProjectManagerDialog::slotBtnAddProject);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(0, 40, 450, 580));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 448, 618));
    scrollArea->setWidget(scrollAreaWidgetContents);

    initProjectItem();

    QMetaObject::connectSlotsByName(this);
    return true;
}

void ProjectManagerDialog::initProjectItem()
{
    QStringList &projectNameList = configClass::getInstance().getProjectList();
    for(int i = 0; i < projectNameList.count(); ++i)
    {
        QString projectName = projectNameList[i];
        ProjectItem *item = new ProjectItem();
        item->lineEdit = new QLineEdit(projectName, scrollAreaWidgetContents);
        item->lineEdit->setGeometry(PROJECT_STAR_X_1, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 280, 23);
        item->lineEdit->setReadOnly(true);

        item->btnEdit = new QPushButton("Edit", scrollAreaWidgetContents);
        item->btnEdit->setObjectName(projectName);
        item->btnEdit->setGeometry(PROJECT_STAR_X_2, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 60, 23);
        connect(item->btnEdit, &QPushButton::clicked, this, &ProjectManagerDialog::slotBtnEditProjet);

        item->btnDelete = new QPushButton("Delete", scrollAreaWidgetContents);
        item->btnDelete->setObjectName(projectName);
        item->btnDelete->setGeometry(PROJECT_STAR_X_3, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 60, 23);
        connect(item->btnDelete, &QPushButton::clicked, this, &ProjectManagerDialog::slotBtnDeleteProjet);

        projectItemMap[projectName]= item;
    }

    scrollAreaWidgetContents->setMinimumHeight(PROJECT_STAR_Y + (projectNameList.count() + 1) * PROJECT_ITEM_INTERVAL_Y);
}

void ProjectManagerDialog::destroyProjectItem()
{
    std::map<QString, ProjectItem*>::iterator iter = projectItemMap.begin();
    for(; iter != projectItemMap.end();iter++)
    {
       if(nullptr != iter->second)
       {
           delete iter->second;
       }
    }

    projectItemMap.clear();
}

void ProjectManagerDialog::flushProjectItem()
{
    destroyProjectItem();

    QStringList &projectNameList = configClass::getInstance().getProjectList();
    for(int i = 0; i < projectNameList.count(); ++i)
    {
        QString projectName = projectNameList[i];
        ProjectItem *item = getProjectItemByName(projectName);
        item = new ProjectItem();
        item->lineEdit = new QLineEdit(projectName, scrollAreaWidgetContents);
        item->lineEdit->setGeometry(PROJECT_STAR_X_1, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 280, 23);
        item->lineEdit->setReadOnly(true);

        item->btnEdit = new QPushButton("Edit", scrollAreaWidgetContents);
        item->btnEdit->setObjectName(projectName);
        item->btnEdit->setGeometry(PROJECT_STAR_X_2, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 60, 23);
        connect(item->btnEdit, &QPushButton::clicked, this, &ProjectManagerDialog::slotBtnEditProjet);

        item->btnDelete = new QPushButton("Delete", scrollAreaWidgetContents);
        item->btnDelete->setObjectName(projectName);
        item->btnDelete->setGeometry(PROJECT_STAR_X_3, PROJECT_STAR_Y + i * PROJECT_ITEM_INTERVAL_Y, 60, 23);
        connect(item->btnDelete, &QPushButton::clicked, this, &ProjectManagerDialog::slotBtnDeleteProjet);

        item->lineEdit->show();
        item->btnEdit->show();
        item->btnDelete->show();

        projectItemMap[projectName] = item;
    }

    scrollAreaWidgetContents->setMinimumHeight(PROJECT_STAR_Y + (projectNameList.count() + 1) * PROJECT_ITEM_INTERVAL_Y);
}

ProjectManagerDialog::ProjectItem *ProjectManagerDialog::getProjectItemByName(QString &projectName)
{
    std::map<QString, ProjectItem*>::iterator iter =  projectItemMap.find(projectName);
    if(iter != projectItemMap.end())
    {
        return iter->second;
    }

    return nullptr;
}

void ProjectManagerDialog::slotBtnAddProject()
{
    MainWindow *parent = getParentPtr<MainWindow>();
    NewProjectDialog w(parent);
    w.showDialog(true);

    flushProjectItem();
}

void ProjectManagerDialog::slotBtnDeleteProjet()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString projectName = act->objectName();

    std::map<QString, ProjectItem*>::iterator iter = projectItemMap.find(projectName);
    if(iter == projectItemMap.end()) return;

    std::map<QString, ProjectItem*>::iterator next = iter;
    ++next;

    int pos_new_y = iter->second->lineEdit->geometry().y();
    int pos_old_y = 0;
    for(; next != projectItemMap.end(); next++)
    {
        pos_old_y = next->second->lineEdit->geometry().y();
        next->second->lineEdit->move(PROJECT_STAR_X_1, pos_new_y);
        next->second->btnEdit->move(PROJECT_STAR_X_2, pos_new_y);
        next->second->btnDelete->move(PROJECT_STAR_X_3, pos_new_y);
        pos_new_y = pos_old_y;
    }

    delete iter->second;
    projectItemMap.erase(iter);
    configClass::getInstance().delProject(projectName);

    scrollAreaWidgetContents->setMinimumHeight(PROJECT_STAR_Y + (static_cast<int>(projectItemMap.size()) + 1) * PROJECT_ITEM_INTERVAL_Y);

    MainWindow *parent = getParentPtr<MainWindow>();
    if(nullptr != parent)
    {
        QString outputStr = "delete project " + projectName;
        parent->appendOutputInfo(outputStr);
        parent->delProjectComboItem(projectName);
    }
}

void ProjectManagerDialog::slotBtnEditProjet()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString projectName = act->objectName();

    MainWindow *parent = getParentPtr<MainWindow>();
    ProjectDetailDialog w(parent);
    w.createDetail(projectName);
    w.showDialog(true);

    flushProjectItem();
}
