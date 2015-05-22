/*****************************************************************
 * Media Cross-Coder By: Lowy
 * 2015.03.25
 *
 * Ez az osztály tartalmazza az egy project-hez tartozó fájlokat,
 * és beállításokat.
 *****************************************************************/

#ifndef MCCPROJECT_H
#define MCCPROJECT_H

#include "mcc.h"
#include <QObject>
#include <QString>
#include "mccobject.h"

class mccProject : public mccObject
{
    Q_OBJECT
public:
    explicit mccProject(QString name, QObject *parent = 0);
    ~mccProject();

    mccMedia *AddMedia(ffDemuxer* media);

    QString             Name()      {return m_Name;}
    void                setName(QString name) {m_Name = name;}
    QList<mccMedia*>    Medias() {return m_Medias;}
signals:

public slots:

private:
    QString             m_Name;                 // Project neve
    QList<mccMedia*>    m_Medias;
};

#endif // MCCPROJECT_H
