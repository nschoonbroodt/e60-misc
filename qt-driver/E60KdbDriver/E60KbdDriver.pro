#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T19:40:23
#
#-------------------------------------------------

QT       += core gui

TARGET = E60KbdDriver
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/kbddrivers

SOURCES += \
    e60kbdhandler.cpp \
    e60kbdplugin.cpp

HEADERS += \
    e60kbdplugin.h \
    e60kbdhandler.h
