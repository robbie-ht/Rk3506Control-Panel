QT       += core gui widgets

TARGET = LyraShell
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sidebar/sidebar.cpp \
    sidebar/sidebaritem.cpp \
    content/contentarea.cpp \
    widgets/powerbutton.cpp \
    apps/appmanager.cpp

HEADERS += \
    mainwindow.h \
    sidebar/sidebar.h \
    sidebar/sidebaritem.h \
    content/contentarea.h \
    widgets/powerbutton.h \
    apps/appmanager.h \
    apps/appinterface.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
