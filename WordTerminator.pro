#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T17:05:05
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WordTerminator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

OTHER_FILES += myapp.rc
RC_FILE += myapp.rc

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

INCLUDEPATH += "./src/utils"
INCLUDEPATH += "./src/view"
INCLUDEPATH += "./src/widget"
SOURCES += main.cpp\
    src/utils/scenenode.cpp \
    src/widget/wordautomatedmemorizescenegraphview.cpp \
    src/widget/wordautomatedmemorizewidget.cpp \
    src/widget/wordterminator.cpp \
    src/widget/wordlibrarywidget.cpp \
    src/widget/wordcreatewidget.cpp \
    src/widget/wordshowwidget.cpp \
    src/widget/wordmemorizewidget.cpp \
    src/widget/wordfunctionwidget.cpp \
    src/widget/wordsettingwidget.cpp \
    src/utils/wtool.cpp \
    src/utils/wordadmin.cpp \
    src/utils/wmessage.cpp \
    src/utils/forgetthread.cpp \
    src/utils/memorythread.cpp \
    src/utils/json.cpp \
    src/utils/global.cpp \
    src/utils/version.cpp \
    src/utils/dispatcher.cpp \
    src/view/copylabel.cpp \
    src/view/linklabel.cpp \
    src/view/dtcp_tool_button.cpp \
    src/view/dtcp_input_dialog.cpp \
    src/view/dtcp_tool_tip.cpp

HEADERS += src/widget/wordterminator.h \
    src/utils/scenenode.h \
    src/widget/wordautomatedmemorizescenegraphview.h \
    src/widget/wordautomatedmemorizewidget.h \
    src/widget/wordlibrarywidget.h \
    src/widget/wordcreatewidget.h \
    src/widget/wordshowwidget.h \
    src/widget/wordmemorizewidget.h \
    src/widget/wordfunctionwidget.h \
    src/widget/wordsettingwidget.h \
    src/utils/wtool.h \
    src/utils/wordadmin.h \
    src/utils/wmessage.h \
    src/utils/forgetthread.h \
    src/utils/memorythread.h \
    src/utils/json.h \
    src/utils/global.h \
    src/utils/version.h \
    src/utils/dispatcher.h \
    src/view/copylabel.h \
    src/view/linklabel.h \
    src/view/dtcp_tool_button.h \
    src/view/dtcp_input_dialog.h \
    src/view/dtcp_tool_tip.h

FORMS += src/widget/wordterminator.ui \
    src/widget/wordautomatedmemorizewidget.ui \
    src/widget/wordlibrarywidget.ui \
    src/widget/wordcreatewidget.ui \
    src/widget/wordshowwidget.ui \
    src/widget/wordmemorizewidget.ui \
    src/widget/wordfunctionwidget.ui \
    src/widget/wordsettingwidget.ui

RESOURCES += \
    main.qrc
