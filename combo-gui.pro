#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T18:24:54
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = combo-gui
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += main.cpp\
		mainwindow.cpp

HEADERS  += mainwindow.h \
	weapon.h

FORMS    += mainwindow.ui

TRANSLATIONS = combo_en.ts combo_ru.ts
