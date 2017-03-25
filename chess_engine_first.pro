QT += core
QT -= gui

CONFIG += c++11

TARGET = chess_engine_first
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    board.cpp \
    movegenerator.cpp \
    communicationbus.cpp \
    manager.cpp

HEADERS += \
    board.h \
    pieces.h \
    defines.h \
    safelist.h \
    pack.h \
    movegenerator.h \
    gloabalvariables.h \
    communicationbus.h \
    manager.h
