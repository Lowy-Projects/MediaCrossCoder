#include "MainWindow.h"
#include <QApplication>
#include <qffmpeg>

#include "mccconfig.h"
#include "ProjectsAndStreams/mccobject.h"

mccConfig* Config;
mccObject* SelectedMedia;
QFfmpeg    qFFMpeg;

int main(int argc, char *argv[])
{
    int ret;

    QApplication a(argc, argv);

    Config = new mccConfig();
    SelectedMedia = NULL;

    MainWindow w;
    w.show();

    ret = a.exec();

    delete Config;

    return ret;
}
