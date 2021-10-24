#ifndef PROJECTMANAGERDIALOG_H
#define PROJECTMANAGERDIALOG_H

#include "basedialog.h"
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>

class ProjectManagerDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ProjectManagerDialog(QWidget *parent = nullptr);
    virtual ~ProjectManagerDialog();

    struct ProjectItem
    {
        ProjectItem():lineEdit(nullptr), btnEdit(nullptr), btnDelete(nullptr)
        {
        }
        ~ProjectItem()
        {
            if(nullptr != lineEdit)
            {
                delete lineEdit;
            }

            if(nullptr != btnEdit)
                delete btnEdit;

            if(nullptr != btnDelete)
                delete btnDelete;
        }

        QLineEdit *lineEdit;
        QPushButton *btnEdit;
        QPushButton *btnDelete;
    };

    virtual bool init();

    void initProjectItem();
    void destroyProjectItem();

    void flushProjectItem();

    ProjectItem * getProjectItemByName(QString &projectName);

private slots:

    void slotBtnAddProject();
    void slotBtnDeleteProjet();
    void slotBtnEditProjet();

private:

    QPushButton *btnAddProject;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    std::map<QString, ProjectItem*> projectItemMap;
};

#endif // PROJECTMANAGERDIALOG_H
