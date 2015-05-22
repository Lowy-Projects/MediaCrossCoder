#include "chaptertimesettingdialog.h"
#include "ui_chaptertimesettingdialog.h"

#include <QDebug>

chapterTimeSettingDialog::chapterTimeSettingDialog(const ffRational &timeBase, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chapterTimeSettingDialog)
{
    ui->setupUi(this);

    m_TimeBase  = timeBase;
    m_LockStart =
    m_LockEnd   = true;
    m_isRunung = false;
}

chapterTimeSettingDialog::~chapterTimeSettingDialog()
{
    delete ui;
}

void chapterTimeSettingDialog::setChapter(ffTimeStamp start, ffTimeStamp end, QString name)
{
    QString chTitle = tr("Aktuális fejezet");

    m_Start = start;
    m_End   = end;
    m_Start.rescaleTimeStamp();
    m_End.rescaleTimeStamp();

    if (!name.isEmpty())
    {
        chTitle = tr("Fejezet:");
        chTitle.append("\n").append(name);
    }

    ui->labelActualChapter1->setText(chTitle);
    ui->labelActualChapter2->setText(chTitle);
}

void chapterTimeSettingDialog::setPrevChapter(ffTimeStamp start, ffTimeStamp end, QString name)
{
    QString chTitle = tr("Előző fejezet");

    m_Min = start;
    m_Min.rescaleTimeStamp();

    if (!m_Max.isValid() )
    {
        m_Max = end;
        m_Max.rescaleTimeStamp();
    }

    if (!name.isEmpty())
    {
        chTitle = tr("Fejezet:");
        chTitle.append("\n").append(name);
    }

    m_PrewName  = chTitle;
    m_LockStart = false;
}

void chapterTimeSettingDialog::setNextChapter(ffTimeStamp start, ffTimeStamp end, QString name)
{
    QString chTitle = tr("Következő fejezet");

    m_Max = end;
    m_Max.rescaleTimeStamp();

    if (!m_Min.isValid() || (m_Min.Value() > start.Value()))
    {
        m_Min = start;
        m_Min.rescaleTimeStamp();
    }

    if (!name.isEmpty())
    {
        chTitle = tr("Fejezet:");
        chTitle.append("\n").append(name);
    }

    m_NextName = chTitle;
    m_LockEnd = false;
}

int chapterTimeSettingDialog::exec()
{
    int         ret;

    if (!m_Start.isValid() || !m_End.isValid())
    {
        if (!m_Min.isValid() || ! m_Max.isValid())
        {
            qDebug() << "ERROR: Not setting the chapterTimeSettingDialog minimal settings.";
            qDebug() << "       Please call setChapter() function.";
            qDebug() << "       Failed exec()";

            return QDialog::Rejected;
        }

        m_Start = m_Min;
        m_Start.add((m_Max.Value() - m_Min.Value()) / 2);
        m_End = m_LockEnd ? m_Max : m_Start;

        ui->labelActualChapter1->setText(tr("Új fejezet"));
        ui->labelActualChapter2->setText("Új fejezet");
    } else
    {
        if (!m_Min.isValid() || m_Min.Value() > m_Start.Value())
            m_Min = m_Start;

        if (!m_Max.isValid() || m_Max.Value() < m_End.Value())
            m_Max = m_End;
    }

    initSliders();

    m_isRunung = true;
    ret = QDialog::exec();
    m_isRunung = false;

    return ret;
}

void chapterTimeSettingDialog::initSliders()
{
    int        tick;
    double step;

    if (!m_Min.isValid())
        m_Min = m_Start;

    if (!m_Max.isValid())
        m_Max = m_End;

    if (m_PrewName.isEmpty())
        ui->labelPrevChapter->setText("");
    else
        ui->labelPrevChapter->setText(m_PrewName);

    if (m_NextName.isEmpty())
        ui->labelNextChapter->setText("");
    else
        ui->labelNextChapter->setText(m_NextName);

    ui->labelStartMin->setText(m_Min.toString(true));
    ui->labelStartMax->setText(m_Max.toString(true));
    ui->labelEndMin->setText(m_Min.toString(true));
    ui->labelEndMax->setText(m_Max.toString(true));
    ui->editStartActual->setText(m_Start.toString(true));
    ui->editEndActual->setText(m_End.toString(true));

    ui->sliderStart->setMaximum(m_Max.Value());
    ui->sliderStart->setMinimum(m_Min.Value());
    ui->sliderStart->setValue(m_Start.Value());

    ui->sliderEnd->setMaximum(m_Max.Value());
    ui->sliderEnd->setMinimum(m_Min.Value());
    ui->sliderEnd->setValue(m_End.Value());

    tick  = m_Max.Value();
    tick -= m_Min.Value();
    tick /= 10;
    ui->sliderStart->setTickInterval(tick);
    ui->sliderEnd->setTickInterval(tick);

    step = (1 / ffRational().value()) / m_TimeBase.value();
    ui->sliderStart->setSingleStep(step);
    ui->sliderStart->setPageStep(step * m_TimeBase.value());
    ui->sliderEnd->setSingleStep(step);
    ui->sliderEnd->setPageStep(step * m_TimeBase.value());
}

void chapterTimeSettingDialog::on_sliderStart_valueChanged(int position)
{
    if (!m_isRunung)                            // Hogy az InitSliders() eljárás ne változtasson meg semmit sem.
        return;

    if (m_LockStart)
    {
        if (ui->sliderStart->value() != m_Min.Value())
        {
            m_Start = m_Min;
            ui->sliderStart->setValue(m_Min.Value());
            ui->editStartActual->setText(m_Min.toString(true));
        }
        return;
    }

    m_Start.setValue(position);

    ui->editStartActual->setText(m_Start.toString(true));

    if (ui->sliderEnd->value() < position)
        ui->sliderEnd->setValue(position);

}

void chapterTimeSettingDialog::on_sliderEnd_valueChanged(int position)
{
    if (!m_isRunung)                            // Hogy az InitSliders() eljárás ne változtasson meg semmit sem.
        return;

    if (m_LockEnd)
    {
        if (ui->sliderEnd->value() != m_Max.Value())
        {
            m_End = m_Max;
            ui->sliderEnd->setValue(m_Max.Value());
            ui->editEndActual->setText(m_Max.toString(true));
        }
        return;
    }

    m_End.setValue(position);

    ui->editEndActual->setText(m_End.toString(true));

    if (ui->sliderStart->value() > position)
        ui->sliderStart->setValue(position);
}
