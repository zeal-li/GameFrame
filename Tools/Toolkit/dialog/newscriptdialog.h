#ifndef NEWSCRIPTDIALOG_H
#define NEWSCRIPTDIALOG_H

#include "basedialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewScriptDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit NewScriptDialog(QWidget *parent = nullptr);
    virtual ~NewScriptDialog();

    virtual bool init();

    void setProjectName(QString &projectName){m_project_name = projectName;}
    void setIsNewGlobalScript(bool is_global){m_is_new_global_script = is_global;}

private slots:
    void on_scriptNameEdit_textChanged(const QString &arg1);
    void on_btnAddScript_clicked();
    void on_btnOk_clicked();

private:

    QLabel * scriptNamelabel;
    QLineEdit * scriptNameEdit;
    QLineEdit * scriptEdit;
    QPushButton *btnAddScript;
    QLabel * scriptParamlabel;
    QLineEdit * scriptParamEdit;
    QPushButton *btnOk;

    QString m_project_name;
    bool m_is_new_global_script;
};

#endif // NEWSCRIPTDIALOG_H
