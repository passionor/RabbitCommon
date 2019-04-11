#include <QApplication>
#include "DlgAbout/DlgAbout.h"
#include "FrmUpdater/FrmUpdater.h"
#include <QTranslator>
#include <QDir>
#include "Tools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QString szPre;    
#if defined(Q_OS_ANDROID) || _DEBUG
    szPre = ":/Translations";
#else
    szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
#endif
    
    QTranslator tApp, tTasks;
    tApp.load(szPre + "/RabbitCommonApp_" + QLocale::system().name() + ".qm");
    a.installTranslator(&tApp);

    CTools::InitTranslator();
    
    CFrmUpdater update;
    update.SetTitle(qApp->applicationDisplayName(), QPixmap(":/icon/RabbitCommon/App"));
    update.show();
    CDlgAbout dlg;
    return dlg.exec();
    
    return a.exec();
}
