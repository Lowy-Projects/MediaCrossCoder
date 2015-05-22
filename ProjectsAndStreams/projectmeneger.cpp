#include "projectmeneger.h"
#include "mcc.h"
#include <QInputDialog>
#include <QTime>
#include "mccproject.h"
#include "mccmedia.h"
#include "mccstream.h"

ProjectMeneger::ProjectMeneger(QWidget *parent) : QWidget(parent)
{
    setHidden(true);
    InitProjectsModel();

    Retranslate();
}

ProjectMeneger::~ProjectMeneger()
{
    // Tervek megsemmisítése
    foreach (mccProject* p, m_projects) {
        m_projects.removeOne(p);
        delete p;
    }
}

///
/// \brief ProjectMeneger::ProjectsModel
/// Model megadása a projectView nevű treeView elemhez.
/// \return A model mutatója
///
QStandardItemModel *ProjectMeneger::ProjectsModel()
{
    return m_projectsModel;
}

///
/// \brief ProjectMeneger::ProjectsSelectionModel
/// Kiválasztást vezérlő model a projectView nevű treeNiew elemhez.
/// \return A model mutatója.
///
QItemSelectionModel *ProjectMeneger::ProjectsSelectionModel()
{
    return m_projectsSelectionModel;
}

///
/// \brief ProjectMeneger::Retranslate
/// Nyelvváltás után, evvel a tagfügvénnyel lehet a statikus szövegeket lecserélni.
void ProjectMeneger::Retranslate()
{
    if (m_projectsModel)
    {
        m_projectsModel->setHorizontalHeaderLabels(QStringList() << "Elem" << "Leírás");
    }
}

lError &ProjectMeneger::Error()
{
    return m_Error;
}

///
/// \brief ProjectMeneger::CreateProject
/// [SLOT] Egy új project-et hoz létre. Megkérdezi a project nevét egy dialógusablakban.
void ProjectMeneger::CreateProject()
{
    m_Error.clear();

    QString name = QInputDialog::getText(this, tr("Új terv"), tr("Az új terv megszületéséhez kellene egy név.\n\nKérem, adjon neki egyet:"), QLineEdit::Normal, tr("Névtelen"));

    if (name.length())
    {
        QString projectId = CreateProjectId(name);
        mccProject* newProject = new mccProject(projectId, this);

        if (newProject)
        {
            newProject->setName(name);
            m_projects.append(newProject);
            BuildProjectTree(newProject, NULL);
        } else
        {
            m_Error.setError(Error_NotEnoughMemory);
            m_Error.show(this);
        }
    }
}

///
/// \brief ProjectMeneger::AddMedia
/// [SLOT] Média hozzáadása az aktuális project-hez. Ha nincs aktuális project, létrehoz egyet.
void ProjectMeneger::AddMedia()
{
    mccProject  * project = NULL;
    mccMedia    * media   = NULL;

    m_Error.clear();

    if (SelectedMedia)
        project = SelectedMedia->Project();

    //
    // Demuxer létrehozása
    //
    ffDemuxer *demuxer = qFFMpeg.OpenMedia("/home/lowy/Videók/pr.mkv");

    if(!demuxer)
    {
        m_Error.setError(Error_NotEnoughMemory);
        goto Error;
    }
    if(demuxer->Error().isError())
    goto Error;

    //
    // Project létrehozása, ha nincs aktuális kiválasztva
    //
    if (!project)
    {
        QString projectId = CreateProjectId(demuxer->SourceInfo().baseName());
        project = new mccProject(projectId, this);

        if (!project)
        {
            m_Error.setError(Error_NotEnoughMemory);
            goto Error;
        }
        if (project->Error().isError())
        {
            m_Error.copyFrom(project->Error());
            goto Error;
        }
        project->setName(demuxer->SourceInfo().baseName());
        m_projects.append(project);
    }

    //
    // Hozzáadás, megjelenítés
    //
    media = project->AddMedia(demuxer);
    if (project->Error().isError())
    {
        m_Error.copyFrom(project->Error());
        goto Error;
    }

    if (SelectedMedia && SelectedMedia->Project() == project)
        project = NULL;

    BuildProjectTree(project, media);

    return;

    Error:
        if (project && project->Medias().count() == 0)
            delete project;
        if (media)   delete media;
        if (demuxer) delete demuxer;

        m_Error.show();
}

void ProjectMeneger::AppendMedia()
{
    Log << "Append media.";
}

void ProjectMeneger::DeleteMedia()
{
    Log << "Delete media.";
}

///
/// \brief ProjectMeneger::SelectedChange
/// [SOLT] tagfügvény. A projectView-n történt kijelölés változásainak kezelése.
/// A SelectedMedia nevű mccObjet tipusú mutató globális mutató beállítása, az újonnan kijelőlt elemre.
/// \param selected [in] QItenSelection tipusú mutató, mely az ujonnan kijelőlt elemet írja le.
/// \param deselected [in] QItenSelection tipusú mutató, mely azt az elemet írja le, ami elveszíti a kijelölést. (nem használt)
///
void ProjectMeneger::SelectedChange(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(deselected);
    QModelIndex index;
    QString selObjectId;

    if (selected.count() == 0)
        return;

    index  = selected.indexes()[0];

    if (index.column()) return;

    selObjectId = m_projectsModel->itemFromIndex(index)->data().toString();
    SelectedMedia = SearchProjectsItem(selObjectId);

    emit MediaItemSelectionChanged();                                               // Signal küldése az aktív elem változásáról.
#ifdef QT_DEBUG_
    QString str("Nincs média kijelőlve.");
    if (SelectedMedia)
        str = QString("Aktív project: %0, kijelőlt elem száma %1").arg(SelectedMedia->Project()->Name()).arg(index.row());

    Log << str;
#endif
}

///
/// \brief ProjectMeneger::ItemChange
/// [SOLT] tagfügvény. A projectView-n történt változások feljegyzése, az mmcObject tipusú elemben.
/// \param item [in] QStandardItem tipusú mutató arra a projectView elemre, ahól a változás történt.
///
void ProjectMeneger::ItemChange(QStandardItem *item)
{
    mccObject*      elem = SearchProjectsItem(item->data().toString()); // A kijelőlt elemet tartalmazó osztály megkeresése
    QStandardItem*  parent = item->parent();                            // A kijelőlt elem treeView szülője.
    if (!parent)
        parent = m_projectsModel->invisibleRootItem();                  // Ha nincs, a főág a szülője

    // Sór kijelölése, ha még nincs kijelőlve
    if (m_projectsSelectionModel->selectedRows().contains(parent->child(item->row())->index()) == false)
    {
        m_projectsSelectionModel->clearSelection();
        m_projectsSelectionModel->setCurrentIndex(parent->child(item->row())->index() , QItemSelectionModel::SelectCurrent);
        m_projectsSelectionModel->setCurrentIndex(parent->child(item->row(), 1)->index(), QItemSelectionModel::Select);
    }

    // Változások rögzításe az osztályban.
    elem->setEnable(item->checkState() == Qt::Checked);
    if (elem->ObjectType().compare("mccProject") == 0)
        ((mccProject*)elem)->setName(item->data(0).toString());

#ifdef QT_DEBUG
    QString str("Az elem: ");

    switch (elem->isEnabled()) {
    case true:
        str.append("engedélyezve");
        break;
    case false:
        str.append("tíltva");
        break;
    }

    QString str2 = "Nincs";
    if (elem->ObjectType().compare("mccProject") == 0)
        str2 = ((mccProject*)elem)->Name();
    str.append( QString(", neve:'%0', azonosítója: '%1'").arg(item->data(0).toString(), item->data().toString()));

    Log << str;
#endif
}

///
/// \brief ProjectMeneger::InitProjectsModel
/// A projectView nevő treeView elem model-jeinek beállítása,
/// es az üzeneteinek elkapása.
/// A visszatérési érték igaz, ha nem történt hiba.
/// Hiba esetén hamis értékkel tér vissza, és at osztály Error() tagfügvénye szoltáltatja a hiba okát.
/// Lehetséges hibakódok:
///     Error_NotEnoughMemory
///  \return Igaz értékkel tér vissza, ha a model-ek létrehozása sikeres volt. Hamis érték esetén, az osztály Error() tagfügvénye
/// szólgáltat további információt.
///
bool ProjectMeneger::InitProjectsModel()
{
    m_Error.clear();

    m_projectsModel = new QStandardItemModel(0, 2,  this);
    if (!m_projectsModel)
    {
        m_Error.setError(Error_NotEnoughMemory);
        return false;
    }

    m_projectsSelectionModel = new QItemSelectionModel(m_projectsModel, this);
    if (!m_projectsSelectionModel)
    {
        m_Error.setError(Error_NotEnoughMemory);
        return false;
    }

    connect(m_projectsModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(ItemChange(QStandardItem*)));
    connect(m_projectsSelectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(SelectedChange(QItemSelection,QItemSelection)));

    return true;
}

///
/// \brief ProjectMeneger::BuildProjectTree
/// Elem felfűzése a projectView nevű treeView fára.
/// A visszatérési érték igaz, ha nem történt hiba.
/// Hiba esetén hamis értékkel tér vissza, és at osztály Error() tagfügvénye szoltáltatja a hiba okát.
/// Lehetséges hibák:
///     Error_NotEnoughMemory
/// \param project [in] Az project mccProject tipusú osztálya, melyet fell kell fűzni. Lehet NULL is.
/// \param media [in] A media mccMedia tipusú osztálya, amit fel kell fűzni. Lehet NULL is.
///  Ha a project elem NULL, a media osztálypéldány, Project() tagfügvénye által meghatározott osztálypéldányra fűzi fel.
/// \return Hamis értékkel tér vissza, ha hiba történt. Az osztály Error() tagfügvénye szolgál további információkkal

bool ProjectMeneger::BuildProjectTree(mccProject *project, mccMedia *media)
{
    QStandardItem*          projectRoot = NULL;
    QStandardItem*          mediaRoot = NULL;
    QList<QStandardItem*>   row;

    m_Error.clear();
    if (project)
    {
        row = BuildProjectRow(project->Name(), tr("MCC Terv"), project);
        if (Error().isError())
            return false;

        m_projectsModel->appendRow(row);
        projectRoot = row[0];
    }

    if (media)
    {
        if (!projectRoot)
            projectRoot = SearchStandardItem(media->Project()->ObjectId());

        row = BuildProjectRow(media->SourceInfo().fileName(), tr("Media fájl"), media);
        projectRoot->appendRow(row);

        mediaRoot = row[0];
        foreach (mccStream* cStream, media->Streams()) {
            row = BuildProjectRow(cStream->objectName(), "Még csak statikus szöveg", cStream);
            mediaRoot->appendRow(row);
        }
    }

    //Beszúrt elem kijelölése
    int rows = projectRoot->row();
    m_projectsSelectionModel->clearSelection();
    m_projectsSelectionModel->setCurrentIndex(m_projectsModel->index(rows, 0), QItemSelectionModel::SelectCurrent);
    m_projectsSelectionModel->setCurrentIndex(m_projectsModel->index(rows, 1), QItemSelectionModel::Select);

    return true;
}

///
/// \brief ProjectMeneger::BuildProjectRow
/// Egy sór létrehozása a projectView nemű treeView osztály számára.
/// Hiba esetén a lista üres, és az osztály Error() tabfügvénye szolgáltat további információt rolla.
/// Lehetséges hibák:
///     Error_NotEnoughMemory
/// \param s1 [in] QString tipusú szöveg, mely az első oszlopban fog megjelleni.
/// \param s2 [in] QString tipusú szöveg, mely az első oszlopban fog megjelleni.
/// \param object [in] mccObject tipusú mutató, megyet a sór reprezentál.
/// \return QList<QStandardItem *> tipusú osztálypédány, megy a létrejött sórt írja le.
///
QList<QStandardItem *> ProjectMeneger::BuildProjectRow(QString s1, QString s2, mccObject *object)
{
    QStandardItem * col1, *col2;
    QList<QStandardItem*> row;

    m_Error.clear();
    col1 = new QStandardItem(s1);
    col2 = new QStandardItem(s2);

    if (col1 == NULL || col2 == NULL)
    {
        if (col1) delete col1;
        if (col2) delete col2;

        m_Error.setError(Error_NotEnoughMemory);
        return row;
    }

    col1->setCheckable(true);
    col1->setCheckState(Qt::Checked);
    col1->setData(object->ObjectId());
    if (object->ObjectType().compare("mccProject") == 0)
        col1->setEditable(true);

    col2->setCheckable(false);
    col2->setEditable(false);

    row.append(col1);
    row.append(col2);

    return row;
}

///
/// \brief ProjectMeneger::CreateProjectId
/// A project-hez hoz létre egyedi azanosítót, a neve alapján.
/// \param name [in] QString objektum, megy a projekt nevét tartalmazza.
/// \return QString tipusú objektum, megy az egyedi azonosítót tartalmazza.
///
QString ProjectMeneger::CreateProjectId(QString name)
{
    QTime time = QTime::currentTime();
    long  id = (((((time.hour() * 60) + time.minute()) * 60) + time.second()) * 1000) + time.msec();

    QString objectId = QString("%0_%1").arg(name).arg(id);

    return objectId;
}

///
/// \brief ProjectMeneger::SearchProjectsItem
/// Egy mmcObject tipusú elem keresése a project-ek között.
/// Visszatérési érték a megtalált elem, vagy NULL, ha nincs ilyen.
/// \param mccObjectId [in] QString, az azonosító string
/// \return mmcObject tipusú osztálypédány mutatója, vagy NULL, ha nincs talált egyezőt.
///
mccObject *ProjectMeneger::SearchProjectsItem(QString mccObjectId)
{
    mccObject*  item = NULL;
    QStringList Ids = mccObjectId.split(mccObjectIdSeparator);

    foreach (mccProject* p, m_projects) {
        if (p->ObjectId().compare(Ids[0]) == 0)
        {
            item = p;
            break;
        }
    }

    if (!item || Ids.count() == 1)
        return item;

    Ids[1] = Ids[0].append(mccObjectIdSeparator).append(Ids[1]);
    mccProject* Project = (mccProject*)item;
    item = NULL;

    foreach (mccMedia* p, Project->Medias()) {
        if (p->ObjectId().compare(Ids[1]) == 0)
        {
            item = p;
            break;
        }
    }

    if (!item || Ids.count() == 2)
        return item;


    mccMedia* Media = (mccMedia*)item;
    item = NULL;

    foreach (mccStream* p, Media->Streams()) {
        if (p->ObjectId().compare(mccObjectId) == 0)
        {
            item = p;
            break;
        }
    }

    return item;
}

///
/// \brief ProjectMeneger::SearchStandardItem
/// A projectModel által leírt, treeView fá 'parent' paraméter által azonosított ágában keresi meg egy elemet mutatóját.
/// Visszatérési érték a megtalált elem QStandardItem tipusú mutatója, vagy NULL, ha nem találta.
/// A visszaadott mutató minden esetben, a treeView sor első elemére mutat.
/// \param mccObjectId [in] QString Az azonosító string
/// \param parent [in]  A QStandatdItem tipusú szülő elem mutatója. Ebben az ágban keres.
///  Ha értéke NULL, vagy nem adjuk meg, az egész fa szerkezetben keres.
/// \return QStandardItem tipusú mutató az elemre, vagy NULL, ha nem taálta meg.
///
QStandardItem *ProjectMeneger::SearchStandardItem(QString mccObjectId, QStandardItem *parent)
{
    int  i;
    QStandardItem *item;

    if (!parent)
        parent = m_projectsModel->invisibleRootItem();

    if (parent->data().toString().compare(mccObjectId) == 0)
        return parent;

    for (i = 0; i < parent->rowCount(); i++)
    {
        item = SearchStandardItem(mccObjectId, parent->child(i, 0));
        if (item)
            return item;

    }

    return NULL;
}
