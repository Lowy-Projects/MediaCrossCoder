/*****************************************************************
 * Media Cross-Coder By: Lowy
 * 2015.03.25
 *
 * Ez az osztály tartalmazza az egy project-hez tartozó fájlokat,
 * és beállításokat.
 *****************************************************************/

#include "mccproject.h"
#include "mccmedia.h"

mccProject::mccProject(QString name, QObject *parent) : mccObject(parent)
{
    m_Name = name;
    setObjectType("mccProject");
    m_Project = this;
}

mccProject::~mccProject()
{
    foreach (mccMedia* media, m_Medias) {
        m_Medias.removeOne(media);
        delete media;
    }
}

mccMedia* mccProject::AddMedia(ffDemuxer *media)
{
    m_Error.clear();

    mccMedia *newMedia = new mccMedia(media, this);
    if (!media)
    {
        media->Close();
        delete media;
        m_Error.setError(Error_NotEnoughMemory);
        return NULL;
    }

    if (newMedia->Error().isError())
    {
        m_Error.copyFrom(newMedia->Error());
        delete newMedia;

        return NULL;
    }

    m_Medias.append(newMedia);
    return newMedia;
}

