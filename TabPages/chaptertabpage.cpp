#include <QStyledItemDelegate>
#include "chaptertabpage.h"
#include "ui_chaptertabpage.h"
#include "mcc.h"
#include "ProjectsAndStreams/mccmedia.h"
#include "chapterdeletedialog.h"
#include "chaptertimesettingdialog.h"

chapterTabPage::chapterTabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chapterTabPage)
{
    ui->setupUi(this);

    ui->chapterView->horizontalHeader()->setStretchLastSection(true);
    ui->chapterView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    SetButtonVisibility();
}

chapterTabPage::~chapterTabPage()
{
    delete ui;
}

void chapterTabPage::SelectedItemChange()
{
    QStyledItemDelegate* delegat = NULL;
    ffChapterModel*      model   = NULL;

    if (SelectedMedia->Media())
    {
        model   = SelectedMedia->Media()->ChapterModel();
        delegat = model->delegat();
    }

    ui->chapterView->setModel(model);
    ui->chapterView->setItemDelegate(delegat);
    if (model)
    {
        QItemSelectionModel *selModel = ui->chapterView->selectionModel();
        if (selModel)
        {
            connect(selModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    this,     SLOT(SelectedChaperChanced(QItemSelection,QItemSelection)));

        }
    }

    SetButtonVisibility();
}

void chapterTabPage::SelectedChaperChanced(const QItemSelection &item, const QItemSelection &oldItem)
{
    Q_UNUSED(item)
    Q_UNUSED(oldItem)

    SetButtonVisibility();
}

void chapterTabPage::on_addButton_pressed()
{
    ffStream                                *stream = NULL;
    ffChapter                               *chapter = NULL;
    ffDemuxer                             *demuxer = NULL;
    ffTimeStamp                           stamp;
    chapterTimeSettingDialog *dialog;
    ffChapterModel                    *model   = (ffChapterModel*)ui->chapterView->model();
    int                                            chNum   = model->rowCount() - 1;

    demuxer = SelectedMedia->Media()->FFDemuxer();
    stream    = demuxer->VideoStreams()[0];
    if (!stream)
        return;                                                                                                                                         // Ha nincs video stream, nincsenek fejezetek

    if (chNum == -1)                                                                                                                            // Ha nincs még fejezet, beszúrunk egyet
    {
        chapter = new ffChapter(model);
        chapter->setStart(demuxer->Start());
        chapter->setLength(demuxer->Duration());
        chapter->setValue(tr("Új fejezet"));
        model->appendChapter(chapter);
        return;
    }

    chapter = model->chapter(chNum);

    dialog = new chapterTimeSettingDialog(stream->FrameRate(), this);
    dialog->setPrevChapter(chapter->Start(), chapter->End(),chapter->Value());
    if (dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return;
    }

    chapter->setEnd(dialog->valueStart());
    chapter = new ffChapter();
    stamp = dialog->valueStart();
    stamp.addFrameNum(1, stream->FrameRate());
    chapter->setStart(stamp);
    chapter->setEnd(dialog->valueEnd());
    chapter->setValue(tr("Új fejezet"));
    model->appendChapter(chapter);

    delete dialog;
}

void chapterTabPage::on_changeButton_pressed()
{
    ffRational timeBase(1001, 24000);
    ffTimeStamp start(0, timeBase);
    ffTimeStamp end(200, timeBase);

    chapterTimeSettingDialog *dialog = NULL;
    ffStream                                *stream = NULL;
    ffChapter                              *prev   = NULL,
                                                   *cur     = NULL,
                                                   *next   = NULL;
    ffChapterModel                    *model        = (ffChapterModel*)ui->chapterView->model();
    int                                            curChap    = ui->chapterView->selectionModel()->currentIndex().row();
    int                                            maxChap   = model->rowCount();

    stream =  SelectedMedia->Media()->FFDemuxer()->VideoStreams()[0];
    if (!stream)
        return;

    if (curChap > 0)
        prev = model->chapter(curChap - 1);
    if (curChap < maxChap - 1)
        next = model->chapter(curChap + 1);
    cur = model->chapter(curChap);

    dialog = new chapterTimeSettingDialog(stream->FrameRate(), this);
    if (prev)
        dialog->setPrevChapter(prev->Start(), prev->End(), prev->Value());
    if (next)
        dialog->setNextChapter(next->Start(), next->End(), next->Value());
    dialog->setChapter(cur->Start(), cur->End(), cur->Value());

    if (dialog->exec() == QDialog::Rejected)
    {
        delete dialog;
        return;
    }

    if (prev &&  (cur->Start() != dialog->valueStart()))
    {
        model->changeChapterStart(cur, dialog->valueStart());
        dialog->valueStart().subFrameNum(1, stream->FrameRate());
        model->changeChapterEnd(prev, dialog->valueStart());
    }

    if (next &&  (cur->End() != dialog->valueEnd()))
    {
        model->changeChapterEnd(cur, dialog->valueEnd());
        dialog->valueEnd().addFrameNum(1, stream->FrameRate());
        model->changeChapterStart(next, dialog->valueEnd());
    }

    delete dialog;
    Log << "Change chapter button";
}

void chapterTabPage::on_delButton_pressed()
{
    int ret;
    int flag = 0;
    chapterDeleteDialog *dialog;
    ffChapterModel         *model = (ffChapterModel*)ui->chapterView->model();
    ffChapter *deleteChapter;
    int row = ui->chapterView->selectionModel()->currentIndex().row();

    if (model->chapters().count() == 1)
        flag |= chapterDeleteDialog::ButtonDelete;

    if (row > 0)
        flag |= chapterDeleteDialog::ButtonPrev;

    if (row < (model->chapters().count() - 1))
        flag |= chapterDeleteDialog::ButtonNext;

    dialog = new chapterDeleteDialog(row, flag, this);
    ret = dialog->exec();
    delete dialog;

    if (ret == chapterDeleteDialog::chRetCancel)
        return;

    if (ret == chapterDeleteDialog::chRetDelete)
    {
        model->removeChapter(row);
        return;
    }

    deleteChapter = model->chapter(row);
    if (ret == chapterDeleteDialog::chRetNext)
        model->changeChapterStart(row+1, deleteChapter->Start());

    if (ret == chapterDeleteDialog::chRetPrev)
        model->changeChapterEnd(row-1, deleteChapter->End());

    model->removeChapter(row);

    delete deleteChapter;
}

void chapterTabPage::SetButtonVisibility()
{
    bool            enable = false,
                    enableAdd = false;

    ffChapterModel *model = (ffChapterModel*)ui->chapterView->model();

    if (model)
    {
        enableAdd = true;
        if (ui->chapterView->selectionModel()->currentIndex().isValid())
            enable = true;
    }

    ui->addButton->setEnabled(enableAdd);
    ui->changeButton->setEnabled(enable);
    ui->delButton->setEnabled(enable);
}
