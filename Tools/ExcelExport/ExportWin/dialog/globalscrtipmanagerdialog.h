#ifndef GLOBALSCRTIPMANAGERDIALOG_H
#define GLOBALSCRTIPMANAGERDIALOG_H

#include "basedialog.h"
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class GlobalScrtipManagerdialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit GlobalScrtipManagerdialog(QWidget *parent = nullptr);
    virtual ~GlobalScrtipManagerdialog();

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

    void initScriptItemMap();
    void destoryScriptItemMap();

    ScriptItem * getScriptItemByName(QString &name);

    void flushAddGlobalScriptItem();
    void flushGlobalScriptItem();

private slots:

    void slotBtnAddScript();
    void slotBtnApply();

    void slot_ScriptItem_scriptNameEdit_textChanged(const QString &arg1);
    void slot_ScriptItem_BtnAddScript();
    void slot_ScriptItem_BtnDeleteScript();

private:

    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPushButton *btnAddScript;
    QPushButton *btnApply;

    std::map<QString, ScriptItem*> scriptItemMap;
};

#endif // GLOBALSCRTIPMANAGERDIALOG_H
