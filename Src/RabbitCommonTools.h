#ifndef RABBITCOMMON_TOOLS_H
#define RABBITCOMMON_TOOLS_H

#include <QTranslator>
#include "rabbitcommon_export.h"

namespace RabbitCommon {

class RABBITCOMMON_EXPORT CTools
{
public:
    static CTools* Instance();
    
    void Init();
    void InitTranslator();
    void CleanTranslator();
    void InitResource();
    void CleanResource();
    
    /**
     * @brief execute: Run with administrator privileges
     * @param program
     * @param arguments
     * @return 
     */
    static bool execute(const QString &program, const QStringList &arguments);

    /**
     * @brief InstallStartRun: auto run when startup
     * @param szName
     * @param szPath
     * @return 
     */
    static int InstallStartRun(const QString &szName = QString(),
                               const QString &szPath = QString(),
                               bool bRoot = false);
    static int RemoveStartRun(const QString &szName = QString(),
                              bool bRoot = false);
    static bool IsStartRun(const QString &szName = QString(),
                           bool bRoot = false);
    
    /**
     * @brief GenerateDesktopFile: Generate desktop file
     * @param szPath
     * @param szAppName
     * @param szUrl
     * @return 
     */
    static int GenerateDesktopFile(const QString &szPath = QString(),
                                   const QString &szAppName = QString());
      
private:
    CTools();
    virtual ~CTools();
    
    QTranslator m_Translator;
};

} //namespace RabbitCommon

#endif // RABBITCOMMON_TOOLS_H
