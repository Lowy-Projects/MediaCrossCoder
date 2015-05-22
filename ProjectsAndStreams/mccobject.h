#ifndef MCCOBJECT_H
#define MCCOBJECT_H

#include <QObject>
#include <QString>
#include <LowyLib.h>
#include <avUtility/ffDictionary.h>

class mccProject;
class mccMedia;
class mccStream;

#define setObjectType setObjectName

class mccObject : public QObject
{
    Q_OBJECT
public:
    explicit mccObject(QObject *parent = 0);
    ~mccObject();

    QString          ObjectId()     {return m_ObjectId;}
    QString          ObjectType()   {return objectName();}
    mccProject      *Project()      {return m_Project;}
    mccMedia        *Media()        {return m_Media;}
    mccStream       *Stream()       {return m_Stream;}
    bool            isEnabled()     {return m_Enable;}
    bool            isOk()          {return m_Ok;}

//    void            setProject(mccProject* project) {m_Project = project;}
//    void            setMedia(mccMedia* media) {m_Media = media;}
//    void            setStream(mccStream* stream) {m_Stream = stream;}
    void            setEnable(bool enable) {m_Enable = enable;}
    void            setOk(bool ok) { m_Ok = ok;}
    lError&         Error() {return m_Error;}

protected:
    mccProject*     m_Project;                      // A Project, melyhez tartozik ez az objektum.
    mccMedia*       m_Media;                        // A Media, melyhez tartozik ez az objektum.
    mccStream*      m_Stream;                       // A Stream, melyhez tartozik ez az objektum
    lError          m_Error;

private:
    QString CreateProjectId(void *parent);

    bool            m_Ok;                           // Elem rendben, használható
    bool            m_Enable;                       // Használat engedélyezése
    QString         m_ObjectId;                     // Egyedi objektumazonosító
};

#endif // MCCOBJECT_H
