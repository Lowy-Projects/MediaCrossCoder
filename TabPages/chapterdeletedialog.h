#ifndef CHAPTERDELETEDIALOG_H
#define CHAPTERDELETEDIALOG_H

#include <QDialog>

namespace Ui {
class chapterDeleteDialog;
}

class chapterDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    enum {
        chRetCancel = 0,
        chRetPrev,
        chRetNext,
        chRetDelete
    };

    enum {
        ButtonDelete = 1,
        ButtonNext    = 2,
        ButtonPrev    = 4
    };

    explicit chapterDeleteDialog(int chapterNum, int flag, QWidget *parent = 0);
    ~chapterDeleteDialog();

private slots:
    void on_bCancel_clicked();
    void on_bNext_clicked();
    void on_bPrev_clicked();
    void on_bDelete_clicked();

private:
    Ui::chapterDeleteDialog *ui;
};

#endif // CHAPTERDELETEDIALOG_H
