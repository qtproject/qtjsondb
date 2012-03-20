TEMPLATE = app
TARGET = tst_jsondblistmodel
DEPENDPATH += .
INCLUDEPATH += . ../../shared/

QT = core network testlib gui qml jsondb
CONFIG -= app_bundle
CONFIG += testcase

include($$PWD/../../shared/shared.pri)

DEFINES += JSONDB_DAEMON_BASE=\\\"$$QT.jsondb.bins\\\"
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += testjsondblistmodel.h \
           $$PWD/../../shared/requestwrapper.h
SOURCES += testjsondblistmodel.cpp
