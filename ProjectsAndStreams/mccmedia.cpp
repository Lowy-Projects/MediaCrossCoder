/*****************************************************************
 * Media Cross-Coder By: Lowy
 * 2015.03.25
 *
 * Ez az osztály tartalmazza az egy médiához tartozó
 * adatokat és beállításokat.
 *****************************************************************/

#include "mccmedia.h"
#include "mccproject.h"
#include "mccstream.h"

mccMedia::mccMedia(ffDemuxer *demuxer, mccProject *parent) :
    mccObject(parent)
{
    m_Error.clear();

    setObjectType("mccMedia");
    m_Project = parent;
    m_Media   = this;

    m_Demuxer       = demuxer;
    m_Metadata      = demuxer->Metadata();
    m_ChapterModel  = demuxer->Chapter();

    foreach (ffStream *stream, demuxer->Streams()) {
        mccStream* str = new mccStream(stream, this);
        if (!str)
            {m_Error.setError(Error_NotEnoughMemory); return; }
        if (str->Error().isError())
            {m_Error.copyFrom(str->Error()); return; }
        m_Streams.append(str);
    }

}

mccMedia::~mccMedia()
{
    foreach (mccStream* stream, m_Streams) {
        m_Streams.removeOne(stream);
        delete stream;
    }

    if (m_Demuxer)
    {
        m_Demuxer->Close();
        delete m_Demuxer;
        m_Demuxer = NULL;
    }
}

QStandardItemModel *mccMedia::MetadataModel()
{
    if (m_Metadata)
        return m_Metadata->Model();

    return NULL;
}

