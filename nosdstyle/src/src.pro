#Configure
TEMPLATE    = lib
CONFIG     += plugin warn_on
TARGET      = nosdstyle
DESTDIR     = ../build

#Temporary Directory
MOC_DIR =       ../tmp
UI_DIR =        ../tmp
OBJECTS_DIR =   ../tmp
RCC_DIR =       ../tmp
INCLUDEPATH +=  ../tmp

#exteranl include path, and library search path
CONFIG(designer) {
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/neux
LIBS += -L$$[QT_INSTALL_PLUGINS]/designer/
} else {
INCLUDEPATH += /${TOOLCHAIN_USR_INSTALL}/include/neux
LIBS += -L${TOOLCHAIN_USR_INSTALL}/lib
}

#Library
LIBS += -lneux


#Input
HEADERS     = nosdstyle.h \
              nosdstyleplugin.h

SOURCES     = nosdstyle.cpp \
              nosdstyleplugin.cpp

#Install
CONFIG(designer) {
style_plugin.path = $$[QT_INSTALL_PLUGINS]/styles
style_plugin.files = ../build/*
style_plugin.extra = cp -a ../build/* $$[QT_INSTALL_PLUGINS]/styles

INSTALLS += style_plugin
} else {
rootfs.path = /${INSTALL_MOD_PATH}/opt/Qt/plugins/styles
rootfs.files = ../build/*
rootfs.extra = cp -a ../build/* /${INSTALL_MOD_PATH}/opt/Qt/plugins/styles

INSTALLS += rootfs
}

#Clean
QMAKE_CLEAN = ../build/* ../tmp/* ./*~ ./Makefile
