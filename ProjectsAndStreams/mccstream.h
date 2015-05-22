#ifndef MCCSTREAM_H
#define MCCSTREAM_H

#include <QObject>
#include "mcc.h"

class mccStream : public mccObject
{
    Q_OBJECT
public:
    mccStream(ffStream* stream, mccMedia *parent);
    ~mccStream();

    ffStream*       FFStream() {return m_ffStream;}
private:
    ffStream        *m_ffStream;
};

#endif // MCCSTREAM_H
