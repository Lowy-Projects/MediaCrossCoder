#include "mcc.h"
#include "mediainfobox.h"


const QString MertekEgysegek[] = {
    "byte",
    "Kbyte",
    "Mbyte",
    "Gbyte",
    "Tbyte"
};

MediaInfoBox::MediaInfoBox(QTextBrowser *browser, QObject *parent) : QObject(parent)
{
    m_InfoBrowser = browser;
    ChangeItemSlot();
}

void MediaInfoBox::ChangeItemSlot()
{
    mccMedia              *media = NULL;
    QList<ffStream*> Streams;
    QDomElement        body;

    body = NewDocument();

    if (!SelectedMedia)
        body.appendChild(NoMediaMessage(m_Doc));
     else
    {
        media = SelectedMedia->Media();
        if (!media) body.appendChild(NoMediaMessage(m_Doc, 1));
        else
        {
            Streams = media->FFDemuxer()->Streams();

            body.appendChild(ShowMediaInfo(media));
            foreach (ffStream * stream, Streams) {
                body.appendChild(ShowStreamInfo(stream));
            }
        }
    }

    m_InfoBrowser->setHtml(m_Doc.toString());
}

QDomElement MediaInfoBox::NewDocument()
{
    m_Doc.clear();

    QDomElement root   = m_Doc.createElement("html");
    QDomElement head  = m_Doc.createElement("head");
    QDomElement body  = m_Doc.createElement("body");
    QDomElement style   = m_Doc.createElement("style");

    m_Doc.appendChild(m_Doc.createComment( "DOCTYPE html"));
    m_Doc.appendChild(root);
    m_Doc.setPrefix("!DOCTYPE HTML PUBLIC");

    style.setAttribute("type", "text/css");
    style.appendChild(m_Doc.createTextNode("td {padding: 15pt 5pt; text-align: left;} "));
    style.appendChild(m_Doc.createTextNode("headerText { font-size: 11pt; font-weight:800; }"));

    head.appendChild(style);
    root.appendChild(head);

    body.setAttribute("style", "font-family:'Sans Serif'; font-size:10pt; font-weight:550; font-style:normal;");
    body.appendChild(style);
    root.appendChild(body);

    return body;
}

QDomElement MediaInfoBox::NoMediaMessage(QDomDocument &doc, int messageId)
{
    QDomElement p = doc.createElement("d");
    QDomText        t  = doc.createTextNode("t");
    QDomElement b = doc.createElement("span");
    QString            msg = tr("Nyisson meg egy média fájlt, vagy hozzon létre egy új tervet.");
    QString            msgAttribute = "font-size:10pt; color:#%1";
    QString            attrColor = "e81837";


    if (messageId == 1)
    {
        msg = tr("Nincs média fájl kijelőlve.");
        attrColor = "20c526";
    }

    p.setAttribute("style", QString(msgAttribute).arg(attrColor));
    p.appendChild(b);
    t.setNodeValue(msg);
    b.setAttribute("style", "font-weight:700;");
    b.appendChild(t);

    return p;
}

QDomElement MediaInfoBox::ShowMediaInfo(mccMedia *media)
{
    QDomElement span = m_Doc.createElement("span");
    QDomElement table = CreateCategory();
    QList<ffStream*> streams;
    ffDictionaryModel* metadata = media->FFDemuxer()->Metadata();
    QString                   t;

    int                      index, indexSi;
    float                   meret, meretSi;

    streams.clear();
    span.appendChild(TitleLine(tr("Forrás média:")));
    span.appendChild(table);

    index = indexSi = 0;
    meret = meretSi = media->FFDemuxer()->SourceInfo().size();
    while(meret > 1024)
        { meret /= 1024; index++;}
    while(meretSi > 1000)
    { meretSi /= 1000; indexSi++; }

    table.appendChild(CategoryEntry(tr("Fájlnév:"), media->FFDemuxer()->SourceInfo().fileName()));
    table.appendChild(CategoryEntry(tr("Méret:"), QString("%1 %2 (%3 %4)").arg(meret).arg(MertekEgysegek[index]).arg(meretSi).arg(MertekEgysegek[indexSi])));
    if (metadata && metadata->Count())
    {
        t = metadata->Value("title", false);
        if (! t.isEmpty())
            table.appendChild(CategoryEntry(tr("Cím:"), t));
        t = metadata->Value("creation_time", false);
        if (! t.isEmpty())
            table.appendChild(CategoryEntry(tr("Létrehozva:"), t));
    }
    table.appendChild(CategoryEntry(tr("Konténer:"), media->FFDemuxer()->ContainerName()));
    if (media->isOk())
    {
        table.appendChild(CategoryEntry(tr("Kezdés:"), media->FFDemuxer()->Start().toString()));
        table.appendChild(CategoryEntry(tr("Hossz:"), media->FFDemuxer()->Duration().toString(true) ));
        meret = media->FFDemuxer()->BitRate() / 1024;
        table.appendChild(CategoryEntry(tr("Bitsebesség:"), QString("%1 Kbit/sec").arg(meret)));
        streams = media->FFDemuxer()->Streams();
        table.appendChild(CategoryEntry(tr("Stream-ek:"), QString("%1").arg(streams.count())));
    }

    if (metadata)
        delete metadata;

    return span;
}

QDomElement MediaInfoBox::ShowStreamInfo(ffStream *stream)
{
    QDomElement span  = m_Doc.createElement("span");
    QDomElement table = CreateCategory();
    QString             s, t;
    ffDictionaryModel*   metadata = stream->Metadata();
    bool                    closeStream = ! stream->isOpen();

    stream->Open();
    span.appendChild(m_Doc.createElement("br"));
    span.appendChild(m_Doc.createElement("br"));
    span.appendChild(TitleLine(QString("%1. stream").arg(stream->Index())));

    table.appendChild(CategoryEntry(tr("ID"), QString("%1").arg(stream->Id())));
    table.appendChild(CategoryEntry(tr("Tipus:"), stream->TypeString()));

    t = stream->CodecName();
    s = ffUtil::getProfileName(stream->Codec(), stream->Profile());
    if (! s.isEmpty())
        t.append(QString(" %1 L%2").arg(s).arg(stream->Level()));
    table.appendChild(CategoryEntry(tr("Kódolás:"), t));

    s = metadata->Value("language", false);
    if (! s.isEmpty() )
        table.appendChild(CategoryEntry(tr("Nyelv:"), s));
    table.appendChild(CategoryEntry(tr("Bitsebesség:"), QString("%1 Kbit/sec").arg( stream->BitRate().value()/1024)));

    switch (stream->Type())
    {
    case ffStream::MediaType_Video:
        ShowVideoInfo(stream, table);
        break;
    case ffStream::MediaType_Audio:
        ShowAudioStream(stream, table);
        break;
    case ffStream::MediaType_Unknown:
        break;
    }
    table.appendChild(CategoryEntry(tr("Kezdés:"), stream->Start().toString(true)));
    table.appendChild(CategoryEntry(tr("Hossz:"),   stream->Duration().toString(true)));

    if (closeStream)
        stream->Close();

    span.appendChild(table);
    return span;
}

QDomElement MediaInfoBox::ShowVideoInfo(ffStream *stream, QDomElement& table)
{
    QSize size = stream->FrameSize();

    table.appendChild(CategoryEntry(tr("Képméret:"),  QString("%1x%2").arg(size.width()).arg(size.height()) ));
    table.appendChild(CategoryEntry(tr("Képarány:"),  QString("%1:1").arg(stream->PictureRatio().value()) ));
    table.appendChild(CategoryEntry(tr("Képpont arány:"),  QString("%1:1").arg(stream->PixelRatio().value()) ));
    table.appendChild(CategoryEntry(tr("Képkockasebesség:"),  QString("%1 FPS (Kép/Másodperc)").arg(stream->FrameRate().value(), 10, ' ', 2)));

    return table;
}

QDomElement MediaInfoBox::ShowAudioStream(ffStream *stream, QDomElement &table)
{
    table.appendChild(CategoryEntry(tr("Hangzás:"),  QString(tr("%1 csatorna")).arg(stream->Channels()) ));
    table.appendChild(CategoryEntry(tr("Bitmélység:"),  stream->SampleFormat() )); // QString(tr("%1 bit/minta")).arg(stream->BitDepth()) ));
    table.appendChild(CategoryEntry(tr("Minta sebesség:"),  QString("%1 minta/másodperc").arg(stream->SampleRate()) ));

    return table;
}

QDomElement MediaInfoBox::TitleLine(QString title)
{
    QDomElement  p = m_Doc.createElement("headerText");
    QDomText          t = m_Doc.createTextNode(title);

    p.appendChild(t);

    m_Line = -1;
    return p;
}

QDomElement MediaInfoBox::CreateCategory()
{
    QDomElement table = m_Doc.createElement("table");
    table.setAttribute("width", "99%");
    table.setAttribute("style","border-spacing: 0;");

    return table;
}

QDomElement MediaInfoBox::CategoryEntry(QString name, QString value)
{
    QDomElement line   = m_Doc.createElement("tr");
    QDomElement iName= m_Doc.createElement("td");
    QDomElement iValue = m_Doc.createElement("td");

    m_Line++;
    line.setAttribute("bgcolor",((m_Line & 1) == 0) ? mccInfoBox_BackgroundColor0 : mccInfoBox_BackgroundColor1);

    iName.appendChild(m_Doc.createTextNode(name));
    iValue.appendChild(m_Doc.createTextNode(value));
    iName.setAttribute("style", QString("color:%1").arg(mccInfoBox_NameColor));
    iValue.setAttribute("style", QString("color:%1").arg(mccInfoBox_ValueColor));

    line.appendChild(iName);
    line.appendChild(iValue);

    return line;
}

/*
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
    <html>
        <head>
            <meta name="qrichtext" content="1" />
            <style type="text/css">p, li { white-space: pre-wrap; }</style>
        </head>

        <body style=" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;">
            <p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
                <span style=" font-family:'Ubuntu'; font-size:12pt; font-weight:600;">Szia!</span>
            </p>
            <p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
                <span style=" font-family:'Ubuntu'; color:#e81837;">Ez egy </span>
                <span style=" font-family:'Ubuntu'; color:#20c526;">próba</span>
                <span style=" font-family:'Ubuntu'; color:#e81837;"> szöveg.</span>
            </p>
        </body>
    </html>
 */
