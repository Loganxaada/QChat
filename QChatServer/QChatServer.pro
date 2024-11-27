QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    public.h \
    widget.h

include($$PWD/custom/custom.pri)
include($$PWD/db/db.pri)
include($$PWD/model/model.pri)
include($$PWD/thirdparty/thirdparty.pri)
include($$PWD/server/server.pri)

# 增加 MySQL 和 Muduo 库的头文件路径
INCLUDEPATH += /usr/include/mysql \
               /usr/local/include

# 增加库路径
LIBS += -L/usr/lib/x86_64-linux-gnu \
        -L/usr/local/lib

# 链接 MySQL 和 Muduo 库
LIBS += -lmysqlclient \
        -lmuduo_base \
        -lmuduo_net \
        -lpthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
