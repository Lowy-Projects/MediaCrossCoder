#ifndef OUTPUTTABPAGE
#define OUTPUTTABPAGE

#include <QWidget>

namespace Ui {
class OutputTabPage;
}

class OutputTabPage : public QWidget
{
    Q_OBJECT

public:
    explicit OutputTabPage(QWidget *parent = 0);
    ~OutputTabPage();

public slots:
    void SelectedItemChange();

private:
    Ui::OutputTabPage *ui;
};

#endif // OUTPUTTABPAGE

