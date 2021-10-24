#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "basedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewProjectDialog : public BaseDialog
{
     Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);
    virtual ~NewProjectDialog();

    virtual bool init();

private slots:
    void on_btnOk_clicked();
    void on_lineEdit_textChanged(const QString &arg1);

private:

    QLabel * label;
    QLineEdit * lineEdit;
    QPushButton *btnOk;

};

#endif // NEWPROJECTDIALOG_H
