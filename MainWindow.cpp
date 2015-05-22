#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "mcc.h"
#include <QWidget>
#include <QDialog>

#include "LowyLib.h"
#include <avUtility/avUtility>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QApplication::setApplicationName("MCC");
    m_Error.clear();

    ui->setupUi(this);

    ui->progressBarCurrent->setValue(0);
    ui->progressBarTotal->setValue(0);

    ui->splitter->setStretchFactor(0, 55);
    ui->splitter->setStretchFactor(1, 45);

#ifdef QT_DEBUG
    Log << "Debug fodítás.";
#endif
    if (!InitProjectMeneger())
        goto FatalError;

    m_TabManager = new TabManager(ui->tabWidget, this);
    m_TabManager->setSelectedChangeSender(m_ProjectMeneger);

    m_MediaInfoBox = new MediaInfoBox(ui->textBrowser, this);
    connect(m_ProjectMeneger, SIGNAL(MediaItemSelectionChanged()), m_MediaInfoBox, SLOT(ChangeItemSlot()));

    if (!InitLog())
        goto FatalError;

    if (!qFFMpeg.Init(Log.MainWidget()))                  // QFFMpeg log ablak beállítása, mivel shared könyvtár.
        goto FatalError;

    Log << tr("Inicializálás kész.");

    return;

FatalError:
    m_Error.show(this);

    exit(-1);
}

MainWindow::~MainWindow()
{
    if (m_ProjectMeneger)
        delete m_ProjectMeneger;

    if (m_TabManager)
        delete m_TabManager;

    delete ui;
}

void MainWindow::on_actionQTAbout_triggered()
{
    QApplication::aboutQt();
}

// Log ablak létrehozása és inicializálása
bool MainWindow::InitLog()
{
#ifdef QT_DEBUG
    QDialog * logWin = new QDialog(this);
    if (logWin)
    {
        logWin->setWindowTitle("MCC - Log");
        logWin->move(0, 0);
        Log.setMainWidget(logWin);
        logWin->show();
    }
#else
    QWidget * logWin = new QWidget(ui->tabWidget);
    if (logWin)
    {
        logWin->setObjectName("Log");
        m_TabManager->AddExternPage(logTab, "Log");
//        ui->tabWidget->addTab(logWin, "Log");
        Log.setMainWidget(logWin);
    }
#endif
    else
    {
        m_Error.setError(Error_NotEnoughMemory);
        return false;
    }
    return true;
}

// Project meneger létrehozása és inicializálása
bool MainWindow::InitProjectMeneger()
{
    // Project meneger létrehozása
    m_ProjectMeneger = new ProjectMeneger(this);

    if (!m_ProjectMeneger)
        { m_Error.setError(Error_NotEnoughMemory);      return false; }

    if (m_ProjectMeneger->Error().isError())
        { m_Error.copyFrom(m_ProjectMeneger->Error()); return false; }

    // View összekötése a Model-el, fejléctulajdonságok beállítása
    ui->projectView->setModel(m_ProjectMeneger->ProjectsModel());
    ui->projectView->setSelectionModel(m_ProjectMeneger->ProjectsSelectionModel());
    ui->projectView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->projectView->header()->setStretchLastSection(true);

    // Gombok összekötése a Project Meneger-el
    connect(ui->buttonNewProject, SIGNAL(pressed()), m_ProjectMeneger, SLOT(CreateProject()));
    connect(ui->buttonAddMedia, SIGNAL(pressed()), m_ProjectMeneger, SLOT(AddMedia()));
    connect(ui->buttonAppendMedia, SIGNAL(pressed()), m_ProjectMeneger, SLOT(AppendMedia()));
    connect(ui->buttonDeleteMedia, SIGNAL(pressed()), m_ProjectMeneger, SLOT(DeleteMedia()));

    // Iconok és menük összekötése a gombokkal
    connect(ui->actionAddProject, SIGNAL(triggered()), ui->buttonNewProject, SIGNAL(pressed()));
    connect(ui->actionAddMedia, SIGNAL(triggered()), ui->buttonAddMedia, SIGNAL(pressed()));
    connect(ui->actionAppendMedia, SIGNAL(triggered()), ui->buttonAppendMedia, SIGNAL(pressed()));
    connect(ui->actionMediaDelete, SIGNAL(triggered()), ui->buttonDeleteMedia, SIGNAL(pressed()));

    return true;
}

void MainWindow::Retranslate()
{
    ui->retranslateUi(this);
    m_ProjectMeneger->Retranslate();
}

