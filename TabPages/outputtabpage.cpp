#include "outputtabpage.h"
#include "ui_outputtabpage.h"
#include <LowyLib.h>
#include "mcc.h"
#include <QAbstractItemModel>
#include "ProjectsAndStreams/mccmedia.h"

OutputTabPage::OutputTabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputTabPage)
{
    ui->setupUi(this);
    ui->MetadataView->horizontalHeader()->setStretchLastSection(true);
    ui->MetadataView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

OutputTabPage::~OutputTabPage()
{
    delete ui;
}

void OutputTabPage::SelectedItemChange()
{
    QAbstractItemModel * model = NULL;

    if (SelectedMedia->Media())
        model = SelectedMedia->Media()->MetadataModel();

    ui->MetadataView->setModel(model);
}
