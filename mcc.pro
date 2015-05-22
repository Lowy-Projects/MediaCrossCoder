#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T15:36:45
#
#-------------------------------------------------

QT += core gui xml
QT += widgets

TARGET = mcc
TEMPLATE = app

SOURCES +=\
    main.cpp \
    MainWindow.cpp \
    mccconfig.cpp \
    ProjectsAndStreams/projectmeneger.cpp \
    ProjectsAndStreams/mccproject.cpp \
    ProjectsAndStreams/mccmedia.cpp \
    ProjectsAndStreams/mccobject.cpp \
    ProjectsAndStreams/mccstream.cpp \
    TabPages/outputtabpage.cpp \
    TabPages/tabmanager.cpp \
    TabPages/chaptertabpage.cpp \
    TabPages/chapterdeletedialog.cpp \
    TabPages/chaptertimesettingdialog.cpp \
    mediainfobox.cpp

HEADERS  += \
    MainWindow.h \
    mccconfig.h \
    mcc.h \
    ProjectsAndStreams/projectmeneger.h \
    ProjectsAndStreams/mccproject.h \
    ProjectsAndStreams/mccmedia.h \
    ProjectsAndStreams/mccobject.h \
    ProjectsAndStreams/mccstream.h \
    TabPages/tabmanager.h \
    TabPages/chaptertabpage.h \
    TabPages/outputtabpage.h \
    TabPages/chapterdeletedialog.h \
    TabPages/chaptertimesettingdialog.h \
    mediainfobox.h

FORMS    += \
    MainWindow.ui \
    TabPages/outputtabpage.ui \
    TabPages/chaptertabpage.ui \
    TabPages/chapterdeletedialog.ui \
    TabPages/chaptertimesettingdialog.ui

RESOURCES += \
    mcc.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/LowyLib/Debug/release/ -lLowyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/LowyLib/Debug/debug/ -lLowyLib
else:unix: LIBS += -L$$PWD/../build/LowyLib/Debug/ -lLowyLib

INCLUDEPATH += $$PWD/../LowyLib
DEPENDPATH += $$PWD/../LowyLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build/LowyLib/Debug/release/libLowyLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build/LowyLib/Debug/debug/libLowyLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build/LowyLib/Debug/release/LowyLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build/LowyLib/Debug/debug/LowyLib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../build/LowyLib/Debug/libLowyLib.a

INCLUDEPATH += $$PWD/../QFFMpeg
DEPENDPATH += $$PWD/../build/QFfmpeg/Debug

unix|win32: LIBS += -L$$PWD/../build/QFfmpeg/Debug/
unix|win32: LIBS += -lQFfmpeg
