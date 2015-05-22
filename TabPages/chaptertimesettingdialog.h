#ifndef CHAPTERTIMESETTINGDIALOG_H
#define CHAPTERTIMESETTINGDIALOG_H

#include <QDialog>
#include <avUtility/ffTimeStamp.h>

namespace Ui {
class chapterTimeSettingDialog;
}

class chapterTimeSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit chapterTimeSettingDialog(const ffRational &timeBase = ffRational(), QWidget *parent = 0);
    ~chapterTimeSettingDialog();

    void                setChapter(ffTimeStamp start, ffTimeStamp end, QString name = QString());
    void                setPrevChapter(ffTimeStamp start, ffTimeStamp end, QString name = QString());
    void                setNextChapter(ffTimeStamp start, ffTimeStamp end, QString name = QString());

    ffTimeStamp&        valueStart() {return m_Start;}
    ffTimeStamp&        valueEnd() {return m_End;}

    int                 exec() Q_DECL_OVERRIDE;

private slots:
    void on_sliderStart_valueChanged(int position);
    void on_sliderEnd_valueChanged(int position);

private:
    Ui::chapterTimeSettingDialog *ui;
    void    initSliders();

    bool                m_LockStart;            // Fix az eleje időpont? (nincs előző fejeet)
    bool                m_LockEnd;              // Fix a vége időpont? (nincs következő fejezet)
    bool                    m_isRunung;             // Fut a kijelzés? Változás kezelő slot-ok vezérlésére
    QString             m_PrewName;             // Előző fejezet neve
    QString             m_NextName;             // Következő fejezet neve
    ffRational          m_TimeBase;             // Lépésközt adja meg, plusz a belő időalap
    ffTimeStamp         m_Min;                  // Legkorábbi időpont
    ffTimeStamp         m_Max;                  // Maximális időpont
    ffTimeStamp         m_Start;                // Aktuális fejezet kezdete
    ffTimeStamp         m_End;                  // Aktuális fejezet vége
};

#endif // CHAPTERTIMESETTINGDIALOG_H
