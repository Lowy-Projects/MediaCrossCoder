/*****************************************************************
 * Media Cross-Coder By: Lowy
 * 2015.03.25
 *
 * Ez az osztály tartalmazza az egy mediához tartozó adatokat
 * és beállításokat.
 *****************************************************************/

#ifndef MCCMEDIA_H
#define MCCMEDIA_H

#include "mcc.h"
#include <LowyLib.h>
#include <QObject>
#include <QList>
#include <avUtility/avUtility>
#include <avFormat/avFormat>
#include "mccobject.h"

class mccMedia: public mccObject
{
    Q_OBJECT

public:
    mccMedia(ffDemuxer* demuxer, mccProject* parent);
    ~mccMedia();

    QFileInfo           SourceInfo() {return m_Demuxer->SourceInfo();}
    ffDemuxer*          FFDemuxer() {return m_Demuxer;}
    ffChapterModel*     ChapterModel() {return m_ChapterModel;}
    QList<mccStream*>   Streams() {return m_Streams;}
    QStandardItemModel* MetadataModel();

private:
    ffDemuxer*              m_Demuxer;          // Forrás objectum
    ffChapterModel*         m_ChapterModel;     // Fejezeteket tartalmazó model
    QList<mccStream*>       m_Streams;          // Stream-ek listálya
    ffDictionaryModel*      m_Metadata;         // Forrás meta adatai

};

#endif // MCCMEDIA_H
