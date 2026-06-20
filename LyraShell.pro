QT       += core gui widgets

TARGET = LyraShell
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main.cpp \
    lyramainwindow.cpp \
    sidebar/sidebar.cpp \
    sidebar/sidebaritem.cpp \
    content/contentarea.cpp \
    widgets/powerbutton.cpp \
    apps/appmanager.cpp \
    filemanager/filemanager.cpp \
    filemanager/filelistview.cpp \
    filemanager/filemanagerwidget.cpp \
    filemanager/filemanagerapp.cpp \
    filemanager/toolbar.cpp \
    filemanager/statusbar.cpp \
    filemanager/dialogs.cpp \
    sysinfo/sysinfocard.cpp \
    sysinfo/sysinfomanager.cpp \
    sysinfo/homepage.cpp \
    utils/thememanager.cpp

HEADERS += \
    lyramainwindow.h \
    sidebar/sidebar.h \
    sidebar/sidebaritem.h \
    content/contentarea.h \
    widgets/powerbutton.h \
    apps/appmanager.h \
    apps/appinterface.h \
    filemanager/filemanager.h \
    filemanager/filelistview.h \
    filemanager/filemanagerwidget.h \
    filemanager/filemanagerapp.h \
    filemanager/toolbar.h \
    filemanager/statusbar.h \
    filemanager/dialogs.h \
    sysinfo/sysinfocard.h \
    sysinfo/sysinfomanager.h \
    sysinfo/homepage.h \
    utils/thememanager.h

INCLUDEPATH += filemanager sysinfo

RESOURCES += \
    resources.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
