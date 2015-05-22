#include "tabmanager.h"

TabManager::TabManager(QTabWidget* wTab ,QObject *parent) : QObject(parent)
{
    m_TabWidget    = wTab;
    m_ChangeSender = NULL;

    m_OutputTabPage  = new OutputTabPage(wTab);
    m_ChapterTabPage = new chapterTabPage(wTab);

    wTab->addTab(m_OutputTabPage,  tr("Kimenet"));
    wTab->addTab(m_ChapterTabPage, tr("Fejezetek"));
}

TabManager::~TabManager()
{
//    ChangeItemDisConnect();
//    lm_Destroy(m_ChangeSender);
}

void TabManager::AddExternPage(QWidget *externPage, QString pageName)
{
    if (m_TabWidget)
        m_TabWidget->addTab(externPage, tr(pageName.toUtf8()));
}

void TabManager::setSelectedChangeSender(QWidget *sender)
{
    ChangeItemDisConnect();

    m_ChangeSender = sender;

    if (sender)
        ChangeItemConnect();
}

void TabManager::ChangeItemSlot()
{
    Log << "Tab meneger: Item change";
}

void TabManager::ChangeItemConnect()
{
    connect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), this, SLOT(ChangeItemSlot()));

    if (m_OutputTabPage)
        connect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), m_OutputTabPage, SLOT(SelectedItemChange()));

    if (m_ChapterTabPage)
        connect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), m_ChapterTabPage, SLOT(SelectedItemChange()));
}

void TabManager::ChangeItemDisConnect()
{
    if (!m_ChangeSender)
        return;

    disconnect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), this, SLOT(ChangeItemSlot()));

    if (m_OutputTabPage)
        disconnect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), m_OutputTabPage, SLOT(SelectedItemChange()));

    if (m_ChapterTabPage)
        disconnect(m_ChangeSender, SIGNAL(MediaItemSelectionChanged()), m_ChapterTabPage, SLOT(SelectedItemChange()));
}

