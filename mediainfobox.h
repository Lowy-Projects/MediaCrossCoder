#ifndef MEDIAINFOBOX_H
#define MEDIAINFOBOX_H

#include <QObject>
#include <QTextBrowser>
#include <QDomDocument>
#include "ProjectsAndStreams/mccmedia.h"
class MediaInfoBox : public QObject
{
    Q_OBJECT
public:
    explicit MediaInfoBox(QTextBrowser *browser, QObject *parent = 0);

signals:

public slots:
    void        ChangeItemSlot();

private:
QDomElement        NewDocument();                                                  // Fejléc
QDomElement        NoMediaMessage(QDomDocument &doc, int messageId = 0);           // Üzenet, ha nincs kiválasztva média fájl
QDomElement        ShowMediaInfo(mccMedia *media);
QDomElement        ShowStreamInfo(ffStream * stream);
QDomElement        ShowVideoInfo(ffStream* stream, QDomElement &table);
QDomElement        ShowAudioStream(ffStream* stream, QDomElement &table);

QDomElement        TitleLine(QString title);
QDomElement        CreateCategory();
QDomElement        CategoryEntry(QString name, QString value);

    int                           m_Line;                                   // Categórián belüli sorszám
    QTextBrowser      *m_InfoBrowser;                     // Megjelenítő szöveg-böngésző
    QDomDocument  m_Doc;
    QDomElement      m_Body;
};

#endif // MEDIAINFOBOX_H
