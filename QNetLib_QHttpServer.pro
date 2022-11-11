QT += core gui network httpserver widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qflexiblejsonobject.cpp \
    qnetaccessmanager.cpp \
    qnetrequest.cpp

HEADERS += \
    mainwindow.h \
    qflexiblejsonobject.h \
    qnetaccessmanager.h \
    qnetrequest.h

FORMS += \
    mainwindow.ui

LIBS += -L"C:/Qt/Tools/OpenSSL/Win_x64/bin" -lcrypto-1_1-x64 -lssl-1_1-x64
