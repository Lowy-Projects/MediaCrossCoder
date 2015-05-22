#include "mccstream.h"
#include "mccmedia.h"

mccStream::mccStream(ffStream *stream, mccMedia *parent) :
    mccObject(parent)
{
    m_Error.clear();

    setObjectType("mccStream");
    m_Project = parent->Project();
    m_Media   = parent->Media();
    m_Stream  = this;

    m_ffStream = stream;
}

mccStream::~mccStream()
{

}

