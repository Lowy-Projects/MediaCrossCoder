#ifndef MCC_H
#define MCC_H

#include <QObject>
#include <QString>
#include <mccconfig.h>
#include <qffmpeg>
#include "ProjectsAndStreams/mccobject.h"

#define  lm_Destroy(obj)    \
    if (obj)                \
    {                       \
        delete obj;         \
        obj = NULL;         \
    }

#define  mccObjectIdSeparator "::"

#define  mccInfoBox_BackgroundColor0 "#eaeaea"
#define  mccInfoBox_BackgroundColor1 "#dadada"
#define  mccInfoBox_NameColor              "#4e4e0e"
#define  mccInfoBox_ValueColor               "#2288aa"

extern mccConfig* Config;
extern QFfmpeg    qFFMpeg;
extern mccObject* SelectedMedia;
#endif // MCC_H
