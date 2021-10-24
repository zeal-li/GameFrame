#ifndef PROJECTDETAILDIALOG_H
#define PROJECTDETAILDIALOG_H

#include "basedialog.h"
#include <QScrollArea>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class ProjectDetailDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ProjectDetailDialog(QWidget *parent = nullptr);
    virtual ~ProjectDetailDialog();

    struct PathItem
    {
        PathItem(QString &ItemName):pathName(ItemName), pathNameEdit(nullptr), pathEdit(nullptr),
            btnAddPath(nullptr), btnDeletePath(nullptr)
        {

        }

        ~PathItem()
        {
            if(nullptr != btnDeletePath)
                delete btnDeletePath;

            if(nullptr != btnAddPath)
                delete btnAddPath;

            if(nullptr != pathEdit)
                delete pathEdit;

            if(nullptr != pathNameEdit)
                delete pathNameEdit;
        }

        QString pathName;
        QLineEdit *pathNameEdit;
        QLineEdit *pathEdit;
        QPushButton *btnAddPath;
        QPushButton *btnDeletePath;
    };

    struct ScriptItem
    {
        ScriptItem(QString &ItemName):scriptName(ItemName), scriptNameEdit(nullptr), scriptEdit(nullptr), scriptParamEdit(nullptr),
            btnAddScript(nullptr), btnDeleteScript(nullptr)
        {}

        ~ScriptItem()
        {
            if(nullptr != btnDeleteScript)
                delete btnDeleteScript;

            if(nullptr != btnAddScript)
                delete btnAddScript;

            if(nullptr != scriptParamEdit)
                delete scriptParamEdit;

            if(nullptr != scriptEdit)
                delete scriptEdit;

            if(nullptr != scriptNameEdit)
                delete scriptNameEdit;
        }

        QString scriptName;
        QLineEdit *scriptNameEdit;
        QLineEdit *scriptEdit;
        QLineEdit *scriptParamEdit;
        QPushButton *btnAddScript;
        QPushButton *btnDeleteScript;

    };

    virtual bool init();

    void createDetail(QString &projectName);
    void destoryDetail();

    void flushDetailAddPathItem();
    void flushDetailPathItem();

    void flushDetailAddScriptItem();
    void flushDetailScriptItem();

    PathItem * getPathItemByName(QString &name);
    ScriptItem * getScriptItemByName(QString &name);

private slots:

    void slotBtnAddSvnPath();
    void slotBtnAddScript();
    void slotBtnApply();
    void on_lineEdit_textChanged(const QString &arg1);

    void slot_PathItem_SvnPathNameEdit_textChanged(const QString &arg1);
    void slot_PathIem_BtnAddSvnPath();
    void slot_PathIem_BtnDeletePath();

    void slot_ScriptItem_scriptNameEdit_textChanged(const QString &arg1);
    void slot_ScriptItem_BtnAddScript();
    void slot_ScriptItem_BtnDeleteScript();

private:

    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    QPushButton *btnAddSvnPath;
    QPushButton *btnAddScript;
    QPushButton *btnApply;

    QLineEdit *lineEdit;

    QString m_project_name;

    QLabel *svnPathTitle;
    QLabel *scriptTitle;

    std::map<QString, PathItem*> pathItemMap;
    std::map<QString, ScriptItem*> scriptItemMap;

};

#endif // PROJECTDETAILDIALOG_H
