QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = icp_fsm_interpreter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    menu.cpp \
    model.cpp \
    view.cpp \

HEADERS += \
    menu.h \
    model.h \
    view.h \
    mvc_interface.h

FORMS += \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Custom targets
#DOCS = $$OUT_PWD/doc
#DOXYFILE = $$PWD/Doxyfile

#QMAKE_EXTRA_TARGETS += doxygen
#doc.depends = Doxyfile $(SOURCES)
#doc.commands ()
