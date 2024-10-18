QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Bullet.cpp \
    src/Enemy.cpp \
    src/Fire.cpp \
    src/Game.cpp \
    src/Health.cpp \
    src/Player.cpp \
    src/Score.cpp \
    src/Wall.cpp \
    src/main.cpp

HEADERS += \
    include/Bullet.h \
    include/Enemy.h \
    include/Fire.h \
    include/Game.h \
    include/Health.h \
    include/Player.h \
    include/Score.h \
    include/Wall.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
