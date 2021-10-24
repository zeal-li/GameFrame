#ifndef SCRIPTOPERATEDIALOG_H
#define SCRIPTOPERATEDIALOG_H

#include "basedialog.h"
#include <QScrollArea>
#include <QPushButton>

class ScriptOperateDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ScriptOperateDialog(QWidget *parent = nullptr);
    virtual ~ScriptOperateDialog();

    virtual bool init();

    void initScritpItem();

private slots:

    void slotbtnCallScript();

private:

    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    std::map<QString, QPushButton*> ScriptItemMap;
};

#endif // SCRIPTOPERATEDIALOG_H
