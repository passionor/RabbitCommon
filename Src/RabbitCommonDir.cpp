#include "RabbitCommonDir.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>
#include <QDebug>

namespace RabbitCommon {

CDir::CDir()
{  
    //注意这个必须的在最前  
    m_szDocumentPath =  QStandardPaths::writableLocation(
             QStandardPaths::DocumentsLocation)
             + QDir::separator() + "Rabbit"
             + QDir::separator() + QApplication::applicationName();
    qDebug() << "Document path:" << m_szDocumentPath;
    QDir d;
    if(!d.exists(m_szDocumentPath))
        if(!d.mkpath(m_szDocumentPath))
            qCritical() << "mkdir doucments dir fail:" << m_szDocumentPath;
    
    m_szApplicationDir =  qApp->applicationDirPath();
#if defined (Q_OS_ANDROID)
    m_szApplicationRootDir = "assets:";
#else
    m_szApplicationRootDir = m_szApplicationDir + QDir::separator() + "..";
#endif
}

CDir* CDir::Instance()
{
    static CDir* p = nullptr;
    if(!p)
        p = new CDir;
    return p;
}

QString CDir::GetDirApplication()
{
    qDebug() << "GetDirApplication:" << qApp->applicationDirPath();
    return m_szApplicationDir;
}

int CDir::SetDirApplication(const QString &szPath)
{
    m_szApplicationDir = szPath;
    return 0;
}

QString CDir::GetDirApplicationInstallRoot()
{
    return m_szApplicationRootDir;
}

int CDir::SetDirApplicationInstallRoot(const QString &szPath)
{
    m_szApplicationRootDir = szPath;
    return 0;
}

QString CDir::GetDirConfig(bool bReadOnly)
{
    QString szPath;
#if defined (Q_OS_ANDROID)
    if(bReadOnly)
        return "assets:/etc";

    szPath = GetDirUserDocument() + QDir::separator() + "root" + QDir::separator() + "etc";
    QDir d;
    if(!d.exists(szPath))
    {
        d.mkpath(szPath);
        //Todo: Copy assets:/etc to here.
        //CopyDirectory("assets:/etc", szPath);
    }
#else
    szPath = GetDirApplicationInstallRoot() + QDir::separator() + "etc";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
#endif
    return szPath;
}

QString CDir::GetDirData(bool bReadOnly)
{
    QString szPath;
#if defined (Q_OS_ANDROID)
    if(bReadOnly)
        return "assets:/data";
    szPath = GetDirUserDocument() + QDir::separator() + "root" + QDir::separator() + "data";
    QDir d;
    if(!d.exists(szPath))
    {
        d.mkpath(szPath);
        //TODO: Copy assets:/etc to here.
        //CopyDirectory("assets:/data", szPath);
    }
#else
    szPath = GetDirApplicationInstallRoot() + QDir::separator() + "data";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
#endif
    return szPath;
}

QString CDir::GetDirDatabase(bool bReadOnly)
{
    QString szPath = GetDirData(bReadOnly) + QDir::separator() + "db";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return szPath;
}

QString CDir::GetDirDatabaseFile(const QString &szFile, bool bReadOnly)
{
    if(szFile.isEmpty())
        return GetDirDatabase(bReadOnly) + QDir::separator() + "database.db";
    return GetDirDatabase(bReadOnly) + QDir::separator() + szFile;
}

QString CDir::GetDirApplicationXml(bool bReadOnly)
{
    QString szPath = GetDirConfig(bReadOnly) + QDir::separator() + "xml";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return szPath;
}

QString CDir::GetDirPlugins()
{
    QString szPath;
#if defined (Q_OS_ANDROID)
    szPath = GetDirApplication();
#else
    szPath = GetDirApplicationInstallRoot() + QDir::separator() + "plugins";
#endif
    return szPath;
}

QString CDir::GetDirUserDocument()
{
    return m_szDocumentPath;
}

int CDir::SetDirUserDocument(QString szPath)
{
    m_szDocumentPath = szPath;
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return 0;
}

QString CDir::GetDirUserData()
{
    QString szPath = GetDirUserDocument() + QDir::separator() + "data";
    
    QDir d;
    if(!d.exists(szPath))
    {
        d.mkpath(szPath);
    }
    return szPath;
}

QString CDir::GetDirUserDatabase()
{
    QString szPath = GetDirUserData() + QDir::separator() + "db";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return szPath;
}

QString CDir::GetDirUserDatabaseFile(const QString &szFile)
{
    if(szFile.isEmpty())
        return GetDirUserDatabase() + QDir::separator() + "database.db";
    return GetDirUserDatabase() + QDir::separator() + szFile;
}

QString CDir::GetDirUserXml()
{
    QString szPath = GetDirUserData() + QDir::separator() + "xml";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return szPath;
}

QString CDir::GetDirUserImage()
{
    QString szPath = GetDirUserData() + QDir::separator() + "image";
    QDir d;
    if(!d.exists(szPath))
        d.mkpath(szPath);
    return szPath;
}

QString CDir::GetDirTranslations()
{
#if _DEBUG
    return ":/translations";
#elif defined (Q_OS_ANDROID)
    return "assets:/translations";
#endif
    return GetDirApplicationInstallRoot() + QDir::separator() + "translations";
}

QString CDir::GetFileApplicationConfigure(bool bReadOnly)
{
    return GetDirConfig(bReadOnly) + QDir::separator() + QApplication::applicationName() + ".conf";
}

QString CDir::GetFileUserConfigure()
{
    QString szName = GetDirUserDocument() + QDir::separator() + QApplication::applicationName() + ".conf";
    return szName;
}

int CDir::CopyDirectory(const QString &fromDir, const QString &toDir, bool bCoverIfFileExists)
{
    QDir formDir_(fromDir);
    QDir toDir_(toDir);
 
    if(!toDir_.exists())
    {
        if(!toDir_.mkpath(toDir_.absolutePath()))
            return false;
    }
 
    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "."|| fileInfo.fileName() == "..")
            continue;
 
        if(fileInfo.isDir())
        {
            if(!CopyDirectory(fileInfo.filePath(),
                              toDir_.filePath(fileInfo.fileName()),
                              true))
                return false;
        }
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}

QString CDir::GetOpenDirectory(QWidget *parent,
                               const QString &caption,
                               const QString &dir,
                               QFileDialog::Options options)
{
    QString szFile;
#if defined (Q_OS_ANDROID)
    QString szCaption = caption;
    if(caption.isEmpty())
        szCaption = QObject::tr("Open");
    QFileDialog fd(parent, szCaption, dir);
    fd.setOptions(options);
    fd.setFileMode(QFileDialog::Directory);
    fd.showMaximized();
    if(QFileDialog::Reject == fd.exec())
    {
        return QString();
    }
    szFile = fd.selectedFiles().at(0);
#else
    szFile = QFileDialog::getExistingDirectory(parent, 
                                          caption,
                                          dir,
                                          options);
#endif
    return szFile;
}

QString CDir::GetOpenFileName(QWidget *parent,
                             const QString &caption,
                             const QString &dir,
                             const QString &filter,
                             QFileDialog::Options options)
{
    QString szFile;
#if defined (Q_OS_ANDROID)
    QString szCaption = caption;
    if(caption.isEmpty())
        szCaption = QObject::tr("Open");
    QFileDialog fd(parent, szCaption, dir, filter);
    fd.setOptions(options);
    fd.showMaximized();
    if(QFileDialog::Reject == fd.exec())
    {
        return QString();
    }
    szFile = fd.selectedFiles().at(0);
#else
    szFile = QFileDialog::getOpenFileName(parent, 
                                          caption,
                                          dir,
                                          filter,
                                          nullptr,
                                          options);
#endif
    return szFile;
}

QString CDir::GetSaveFileName(QWidget *parent,
                              const QString &caption,
                              const QString &dir,
                              const QString &filter,
                              QFileDialog::Options options)
{
    QString szFile;
#if defined (Q_OS_ANDROID)
    QString szCaption = caption;
    if(caption.isEmpty())
        szCaption = QObject::tr("Save");
    QFileDialog fd(parent, szCaption, dir, filter);
    fd.setOptions(options);
    fd.showMaximized();
    if(QFileDialog::Reject == fd.exec())
    {
        return QString();
    }
    szFile = fd.selectedFiles().at(0);
#else
    szFile = QFileDialog::getSaveFileName(parent, 
                                          caption,
                                          dir,
                                          filter,
                                          nullptr,
                                          options);
#endif
    return szFile;
}

} //namespace RabbitCommon
