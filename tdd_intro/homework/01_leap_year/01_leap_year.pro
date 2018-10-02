include(../../gtest.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    test.cpp \
    LeapYear.cpp

HEADERS += \
    LeapYear.h
