#ifndef PROJECTMENEGER_H
#define PROJECTMENEGER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>

#include <LowyLib.h>
#include "mccproject.h"

class ProjectMeneger : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectMeneger(QWidget *parent = 0);
    ~ProjectMeneger();

    QStandardItemModel*     ProjectsModel();
    QItemSelectionModel*    ProjectsSelectionModel();

    void                    Retranslate();
    lError&                 Error();
signals:
    void                    MediaItemSelectionChanged();

public slots:
    //  Nyomógomb események
    void                    CreateProject();
    void                    AddMedia();
    void                    AppendMedia();
    void                    DeleteMedia();

    // TreeView események
    void                    SelectedChange(QItemSelection selected, QItemSelection deselected);         // Új kiválasztva
    void                    ItemChange(QStandardItem* item);                                            // Elem átnevezve, vagy Check változás

private:
    // TreeView eljárások
    bool                    InitProjectsModel();                                                        // Inicializálás
    bool                    BuildProjectTree(mccProject* project, mccMedia* media);                     // Project és media felvétele a treeView-ra
    QList<QStandardItem*>   BuildProjectRow(QString s1, QString s2, mccObject *object);                 // Sor létrehozása
    QString                 CreateProjectId(QString name);                                              // Project egyedi azonosítót hoz létre.

    // Keresések
    mccObject*              SearchProjectsItem(QString mccObjectId);
    QStandardItem*          SearchStandardItem(QString mccObjectId, QStandardItem* parent = NULL);

    // Változók
    QStandardItemModel     *m_projectsModel;           // Terveket megjelenítő treeView elem modelje
    QItemSelectionModel     *m_projectsSelectionModel;  // Terveket megjelenítő treeView kiválasztás model
    QList<mccProject *>      m_projects;                // Tervek listája

    lError                  m_Error;
};

#endif // PROJECTMENEGER_H
