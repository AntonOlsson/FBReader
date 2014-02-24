# Sailfish OS configuration

OSNAME = Sailfish
INSTALLDIR = /usr
SHAREDIR = $(INSTALLDIR)/share
IMAGEDIR = $(SHAREDIR)/$(FBREADER_NAME)/icons
APPIMAGEDIR = $(IMAGEDIR)

FBREADER_NAME = harbour-fbreader

## override application paths, we move share/FBReader share/zlibrary to here on install
#SHAREDIR_MACRO = /usr/share/harbour-fbreader

UNAME_MACHINE := $(shell uname -m)

ZLSHARED = no

CC = gcc
AR = ar rsu
LD = g++
MOC = moc -DQT5
DEPGEN = $(CC) -MM
RM_QUIET = rm -rf
RM = rm -rvf
CFLAGS += -DSAILFISH -DQT5
CFLAGS += -DQT_DISABLE_DEPRECATED_BEFORE=4
CFLAGS += -pipe -fno-exceptions -Wall -Wno-ctor-dtor-privacy -W -DLIBICONV_PLUG -fPIC
LDFLAGS += -pie
QTINCLUDE += -I/usr/include/qt5 -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtQuick -I/usr/include/qt5/QtQml -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtNetwork
UILIBS = -lQt5Core -lQt5Quick -lQt5Qml -lQt5Gui -lQt5Network -lsailfishapp
EXTERNAL_LIBS += -L$(ROOTDIR)/libs/sailfish/$(UNAME_MACHINE)
EXTERNAL_INCLUDE += -I$(ROOTDIR)/libs/sailfish/include
