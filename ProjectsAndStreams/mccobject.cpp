#include "mccobject.h"
#include <QTime>
#include "mcc.h"

QStringList mccObjectTypes;

mccObject::mccObject(QObject *parent) : QObject(parent)
{
    m_Error.clear();

    mccObjectTypes  << "mccProject" << "mccMedia" << "mccStream";
    m_ObjectId = CreateProjectId(parent);

    setObjectName("mccObject");

    m_Project   = NULL;
    m_Media     = NULL;
    m_Stream    = NULL;

    m_Ok = m_Enable = true;

}

mccObject::~mccObject()
{

}

QString mccObject::CreateProjectId(void* parent)
{
    QString sId;
    void * num = this;

    sId = "";
    if (parent && mccObjectTypes.contains(((QObject*)parent)->objectName()))
        sId = ((mccObject*)parent)->ObjectId().append(mccObjectIdSeparator);

    sId.append(QString("%0").arg((qint64)num));

    return sId;
}

