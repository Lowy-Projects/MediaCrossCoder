#include "mccconfig.h"

#include <QApplication>
#include <QString>
#include <QSettings>

QString     m_FileName;
QSettings*  m_Settings = NULL;

mccConfig::mccConfig()
{
    QString appPath(QApplication::applicationFilePath().append(".ini"));
    m_Settings = new QSettings(appPath, QSettings::IniFormat);

    m_Settings->setValue("Próba", "Sikerűlt");
}

mccConfig::~mccConfig()
{
    if (m_Settings)
        delete m_Settings;
}
