QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++latest
QMAKE_CXXFLAGS += \
            -std=c++20 \
            -fcoroutines

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = \
    x_pch.h

SOURCES += \
    appcontroller.cpp \
    appoptions.cpp \
    config.cpp \
    directories.cpp \
    error.cpp \
    filetransfer.cpp \
    format.cpp \
    main.cpp \
    mainwindow.cpp \
    random.cpp

HEADERS += \
    appcontroller.h \
    appoptions.h \
    benchmark.h \
    config.h \
    directories.h \
    error.h \
    filetransfer.h \
    format.h \
    mainwindow.h \
    random.h \
    serialize.h \
    x_pch.h

FORMS += \
    appoptions.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
