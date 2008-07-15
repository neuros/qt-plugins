TEMPLATE = lib
TARGET = ../build/osdir-plugin
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt plugin

QT -= network

# Input
HEADERS += osdirkeyboardhandler.h  osdirkeyboarddriverplugin.h
SOURCES += osdirkeyboardhandler.cpp osdirkeyboarddriverplugin.cpp

rootfs.path = /${INSTALL_MOD_PATH}/opt/Qt/plugins/kbddrivers
rootfs.files = ../build/*
INSTALLS += rootfs

QMAKE_CLEAN += ../build/* ./Makefile ../Makefile
