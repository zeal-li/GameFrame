#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    struct SvnPathItem
    {
        SvnPathItem():label(nullptr), lineEdit(nullptr), btnUpdate(nullptr), btnCommit(nullptr),
            btnRevert(nullptr), btnCleanup(nullptr), btnDelete(nullptr), btnOpenPath(nullptr)
        {
        }
        ~SvnPathItem()
        {
            if(nullptr != label)
                delete label;

            if(nullptr != lineEdit)
                delete lineEdit;

            if(nullptr != btnUpdate)
                delete btnUpdate;

            if(nullptr != btnCommit)
                delete btnCommit;

            if(nullptr != btnRevert)
                delete btnRevert;

            if(nullptr != btnCleanup)
                delete btnCleanup;

            if(nullptr != btnDelete)
                delete btnDelete;

            if(nullptr != btnOpenPath)
                delete btnOpenPath;
        }

        QLabel *label;
        QLineEdit *lineEdit;
        QPushButton *btnUpdate;
        QPushButton *btnCommit;
        QPushButton *btnRevert;
        QPushButton *btnCleanup;
        QPushButton *btnDelete;
        QPushButton *btnOpenPath;
    };

    struct ScriptItem
    {
        ScriptItem():label(nullptr), scriptEdit(nullptr), scriptParamtEdit(nullptr), btnExecut(nullptr), btnOpenPath(nullptr)
        {
        }
        ~ScriptItem()
        {
            if(nullptr != label)
                delete label;

            if(nullptr != scriptEdit)
                delete scriptEdit;

            if(nullptr != scriptParamtEdit)
                delete scriptParamtEdit;

            if(nullptr != btnExecut)
                delete btnExecut;

            if(nullptr != btnOpenPath)
                delete btnOpenPath;
        }

        QLabel *label;
        QLineEdit *scriptEdit;
        QLineEdit *scriptParamtEdit;
        QPushButton *btnExecut;
        QPushButton *btnOpenPath;
    };

    bool init();

    void initProjectCombo();
    void addProjectComboItem(QString &textName);
    void delProjectComboItem(QString &textName);
    void changeProjectComboItem(QString &oldName, QString &newName);
    void appendOutputInfo(const QString &infoStr);

    void initSvnOperateWidget(bool isforce = false);
    void destroySvnOperateWidget();
    void addFlushSvnOperateWidget(QString &projectName);
    void deleteFlushSvnOperateWidget(QString &projectName);
    void flushSvnOperateWidget();
    SvnPathItem * getSvnPathItemByName(const QString &name);

    void initScriptOperateWidget(bool isforce = false);
    void destroyScriptOperateWidget();
    void addFlushScriptOperateWidget(QString &projectName);
    void deleteFlushScriptOperateWidget(QString &projectName);
    void flushScriptOperateWidget();
    ScriptItem * getScriptItemByName(const QString &name);

    void initGlobalScriptOperateWidget();
    void destroyGlobalScriptOperateWidget();
    void addFlushGlobalScriptOperateWidget();
    void deleteFlushGlobalScriptOperateWidget();
    void flushGlobalScriptOperateWidget();
    ScriptItem * getGlobalScriptItemByName(const QString &name);

    void initExportOperateWidget(bool isforce = false);
    void destroyExportOperateWidget();
    void flushExportOperateWidget();

private slots:

    void on_cmbPorjectSelect_currentTextChanged(const QString &arg1);
    void on_action_N_triggered();
    void on_action_C_triggered();
    void on_action_G_triggered();
    void on_btnSvn_clicked();
    void on_btnScript_clicked();
    void on_btnExport_clicked();
    void on_btnClear_clicked();

    void on_btnUpdateAll_clicked();
    void on_btnCommitAll_clicked();
    void on_btnCleanupAll_clicked();
    void on_btnRevertAll_clicked();
    void slotsvn_BtnUpdate();
    void slotsvn_BtnCommit();
    void slotsvn_BtnRevert();
    void slotsvn_BtnCleanup();
    void slotsvn_BtnOpendir();

    void slotscript_BtnOpendir();
    void slotscript_BtnExcute();
    void slotglobalScript_BtnOpendir();
    void slotglobalScript_BtnExcute();

    void on_btnBuild_clicked();
    void on_btnBuildAll_clicked();
    void on_btnSetting_clicked();
    void on_btnInputBrowse_clicked();
    void on_btnInputOpen_clicked();
    void on_btnOutputBrowse_clicked();
    void on_btnOutputOpen_clicked();

private:
    Ui::MainWindow *ui;

    std::map<QString, SvnPathItem*> m_svnPathItemMap;
    std::map<QString, ScriptItem*> m_scirptItemMap;
    std::map<QString, ScriptItem*> m_global_scirptItemMap;

};

#endif // MAINWINDOW_H
