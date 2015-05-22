#include <LowyLib.h>
#include "chapterdeletedialog.h"
#include "ui_chapterdeletedialog.h"

chapterDeleteDialog::chapterDeleteDialog(int chapterNum, int flag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chapterDeleteDialog)
{
    QString s = QString(tr("Ön a(z) %1. fejezet törlésére készűl!\nMit kíván tenni?")).arg(chapterNum + 1);
    ui->setupUi(this);
    ui->textLabel->setText(s);

    if (flag & ButtonDelete)
    {
        ui->bNext->setVisible(false);
        ui->bPrev->setVisible(false);
    } else
    {
        ui->bDelete->setVisible(false);

        if (!(flag & ButtonNext))
            ui->bNext->setEnabled(false);

        if (!(flag & ButtonPrev))
            ui->bPrev->setEnabled(false);
    }
}

chapterDeleteDialog::~chapterDeleteDialog()
{
    delete ui;
}

void chapterDeleteDialog::on_bCancel_clicked()
{
    done(chRetCancel);
}

void chapterDeleteDialog::on_bNext_clicked()
{
    done(chRetNext);
}

void chapterDeleteDialog::on_bPrev_clicked()
{
    done(chRetPrev);
}

void chapterDeleteDialog::on_bDelete_clicked()
{
    done(chRetDelete);
}
