#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QObject>
#include <QTabWidget>
#include "mcc.h"
#include "outputtabpage.h"
#include "chaptertabpage.h"

class TabManager : public QObject
{
    Q_OBJECT
public:
    explicit TabManager(QTabWidget* wTab ,QObject *parent = 0);
    ~TabManager();

    void    AddExternPage(QWidget *externPage, QString pageName);
    void    setSelectedChangeSender(QWidget* sender);
signals:

public slots:
    void    ChangeItemSlot();

private:
    void                ChangeItemConnect();
    void                ChangeItemDisConnect();

    QTabWidget*         m_TabWidget;
    QWidget*            m_ChangeSender;

    OutputTabPage*      m_OutputTabPage;
    chapterTabPage*     m_ChapterTabPage;
};

#endif // TABMANAGER_H
