#include "showpathdialog.h"
#include "invokeclass.h"

ShowPathDialog::ShowPathDialog(QWidget *parent):BaseDialog (parent),
    scrollArea(nullptr), scrollAreaWidgetContents(nullptr), m_is_dir(true)
{
    init();
}

ShowPathDialog::~ShowPathDialog()
{
    destoryPathItem();

    if(nullptr != scrollAreaWidgetContents)
        delete scrollAreaWidgetContents;

    if(nullptr != scrollArea)
        delete scrollArea;
}

bool ShowPathDialog::init()
{
    setTitle("Path");
    resize(400, 350);
    setFixedSize(400, 350);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(0, 0, 400, 350));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 498, 348));
    scrollArea->setWidget(scrollAreaWidgetContents);

    return true;
}

void ShowPathDialog::createPathItem(QStringList &pathlist, bool is_dir)
{
    static int start_x = 5;
    static int start_y = 10;
    static int interval_y = 23;
    for(int i = 0; i < pathlist.count(); ++i)
    {
        QString path = pathlist[i];
        ShowItem *item = new ShowItem();
        item->btn_open_dir = new QPushButton(path, this);
        item->btn_open_dir->setGeometry(start_x, start_y + i * interval_y, 320, 23);
        connect(item->btn_open_dir, &QPushButton::clicked, this, &ShowPathDialog::slotBtnOpenDir);

        item->btn_show_log = new QPushButton("show log", this);
        item->btn_show_log->setGeometry(325, start_y + i * interval_y, 60, 23);
        item->btn_show_log->setObjectName(path);
        connect(item->btn_show_log, &QPushButton::clicked, this, &ShowPathDialog::slotBtnShowLog);

        svnPathItemMap[path] = item;
    }

    scrollAreaWidgetContents->setMinimumHeight(start_y + pathlist.count() * interval_y);

    m_is_dir = is_dir;
}

void ShowPathDialog::destoryPathItem()
{
    std::map<QString, ShowItem*>::iterator iter =  svnPathItemMap.begin();
    for(; iter != svnPathItemMap.end(); ++iter)
    {
        delete iter->second;
    }

    svnPathItemMap.clear();
}

void ShowPathDialog::slotBtnOpenDir()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->text();

    if(m_is_dir)
    {
        invokeclass::getInstance().openDirectory(pathName);
    }
    else
    {
        QString temp = pathName;
        int i = temp.count();
        for(; i >= 0; --i)
        {
            if(temp[i] == "/")
            {
                break;
            }
        }

        temp.remove(i, temp.count());
        invokeclass::getInstance().openDirectory(temp);
    }
}

void ShowPathDialog::slotBtnShowLog()
{
    QPushButton *act=qobject_cast<QPushButton*>(sender());
    QString pathName = act->objectName();

    invokeclass::getInstance().svnShowLog(pathName);
}
