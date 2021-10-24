#include "scriptoperatedialog.h"
#include "dialogcommondef.h"
#include "invokeclass.h"


#include "stdafx.h"
ScriptOperateDialog::ScriptOperateDialog(QWidget *parent):BaseDialog (parent),
    scrollArea(nullptr), scrollAreaWidgetContents(nullptr)
{
    init();
}

ScriptOperateDialog::~ScriptOperateDialog()
{
    if(nullptr != scrollAreaWidgetContents)
        delete scrollAreaWidgetContents;

    if(nullptr != scrollArea)
        delete scrollArea;
}

bool ScriptOperateDialog::init()
{
    setTitle("SvnOperate");
    resize(420, 500);
    setFixedSize(420, 500);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(0, 0, 420, 500));
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 418, 498));
    scrollArea->setWidget(scrollAreaWidgetContents);

    initScritpItem();

    return true;
}

void ScriptOperateDialog::initScritpItem()
{
    int a = 200;
    int y = 0;
    for(int i = 0; i < a; ++i)
    {
        QPushButton *btn = new QPushButton("1111", scrollAreaWidgetContents);
        connect(btn, &QPushButton::clicked, this, &ScriptOperateDialog::slotbtnCallScript);
        int line = i / SCRIPT_HORIZONTAL_ITEM_COUNT;
        int column = i % SCRIPT_HORIZONTAL_ITEM_COUNT;
        int x = SCRIPT_START_X + SCRIPT_INTERVAL_X * column;
        y = SCRIPT_START_Y + SCRIPT_INTERVAL_Y * line;

        qDebug() << "x:" << x << endl;
        qDebug() << "y:" << y << endl;
        btn->setGeometry(x, y, 60, 23);
    }

    scrollAreaWidgetContents->setMinimumHeight(y);
}

void ScriptOperateDialog::slotbtnCallScript()
{
    QString program = "ping www.baidu.com";
    QString param = "";
    invokeclass::getInstance().callCMDexecuteCommond(program);
}
