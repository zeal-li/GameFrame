#ifndef INVOKECLASS_H
#define INVOKECLASS_H

#include <QProcess>

class invokeclass
{
public:
    invokeclass();
    virtual ~invokeclass();

    static invokeclass& getInstance();

    bool init();

    int svnUpdate(QString path);
    int svnCommit(QString path);
    int svnCleanUp(QString path);
    int svnRevert(QString path);
    int svnShowLog(QString path);

    bool openDirectory(QString path);

    int callCMDexecuteCommond(QString commond);
    int callCMDexecuteProgram(QString &programName, QString param ="");

private:

    QProcess *m_qprocess;
    QStringList m_qprocess_param;
};

#endif // INVOKECLASS_H
