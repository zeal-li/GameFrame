#ifndef SHOWPATHDIALOG_H
#define SHOWPATHDIALOG_H

#include "basedialog.h"
#include <QString>
#include <QScrollArea>
#include <QPushButton>

class ShowPathDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ShowPathDialog(QWidget *parent = nullptr);
    virtual ~ShowPathDialog();

    struct ShowItem
    {
        ShowItem():btn_open_dir(nullptr), btn_show_log(nullptr)
        {}

        ~ShowItem()
        {
            if(nullptr != btn_show_log)
                delete btn_show_log;

            if(nullptr != btn_open_dir)
                delete btn_open_dir;
        }

        QPushButton *btn_open_dir;
        QPushButton *btn_show_log;
    };
    virtual bool init();

    void createPathItem(QStringList &pathlist, bool is_dir = true);
    void destoryPathItem();

private slots:

    void slotBtnOpenDir();
    void slotBtnShowLog();

private:

    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    std::map<QString, ShowItem*> svnPathItemMap;
    bool m_is_dir;
};

#endif // SHOWPATHDIALOG_H
