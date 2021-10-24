#ifndef NEWSVNPATHDIALOG_H
#define NEWSVNPATHDIALOG_H

#include "basedialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewSvnPathDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit NewSvnPathDialog(QWidget *parent = nullptr);
    virtual ~NewSvnPathDialog();

    virtual bool init();

    void setProjectName(QString &projectName){m_project_name = projectName;}

private slots:
    void on_pathNameEdit_textChanged(const QString &arg1);
    void on_btnAddPath_clicked();
    void on_btnOk_clicked();

private:

    QLabel * pathNamelabel;
    QLineEdit * pathNameEdit;
    QLineEdit * pathEdit;
    QPushButton *btnAddPath;
    QPushButton *btnOk;

    QString m_project_name;
};

#endif // NEWSVNPATHDIALOG_H
