#ifndef CHAPTERTABPAGE_H
#define CHAPTERTABPAGE_H

#include <QWidget>
#include <QItemSelectionModel>
#include <avFormat/ffChapter.h>

namespace Ui {
class chapterTabPage;
}

class chapterTabPage : public QWidget
{
    Q_OBJECT

public:
    explicit chapterTabPage(QWidget *parent = 0);
    ~chapterTabPage();

public slots:
    void    SelectedItemChange();

private slots:
    void    SelectedChaperChanced(const QItemSelection &item, const QItemSelection &oldItem);

    void on_addButton_pressed();
    void on_changeButton_pressed();
    void on_delButton_pressed();

private:
    void    SetButtonVisibility();

    Ui::chapterTabPage *ui;
};

#endif // CHAPTERTABPAGE_H
