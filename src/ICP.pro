QT       += core gui qml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
TARGET = icp_fsm_interpreter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/model
INCLUDEPATH += $$PWD/interpreter
INCLUDEPATH += $$PWD/exceptions
INCLUDEPATH += $$PWD/view

SOURCES += $$files($$PWD/*.cpp, true)

HEADERS += $$files($$PWD/*.h, true)

FORMS += $$files($$PWD/*.ui, true)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += $$files($$PWD/*.qrc, true)
