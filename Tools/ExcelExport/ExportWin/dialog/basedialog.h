#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

namespace Ui {
class BaseDialog;
}

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr);
    virtual ~BaseDialog();

    virtual bool init();

    void showDialog(bool modal = true);

    void setTitle(QString title = "BaseDialog");

    template <typename T>
    inline T * getParentPtr ()
    {
        return static_cast<T*>(parentWidget());
    }


private:
    Ui::BaseDialog *ui;
};

#endif // BASEDIALOG_H
