#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ProjectsAndStreams/projectmeneger.h"
#include "TabPages/tabmanager.h"
#include "mediainfobox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    lError  *Error() {return &m_Error;}

private slots:
    void on_actionQTAbout_triggered();

private:

    bool    InitLog();
    bool    InitProjectMeneger();
    void    Retranslate();

    Ui::MainWindow *ui;

    ProjectMeneger*     m_ProjectMeneger;           // Terveket kezelő, és a megjelenítésért felelős modelleket kezelő osztály
    TabManager*         m_TabManager;               // Tabulátorok kezelése
    MediaInfoBox*       m_MediaInfoBox;             // Infórmációt jelenít meg az aktuális médiáról
    lError              m_Error;
};

#endif // MAINWINDOW_H
