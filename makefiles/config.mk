include $(ROOTDIR)/makefiles/platforms.mk

VERSION = $(shell cat $(ROOTDIR)/fbreader/VERSION)
MAKE = make ROOTDIR=$(ROOTDIR)
LIBMAKE = $(MAKE) ZLSHARED=$(ZLSHARED)

include $(ROOTDIR)/makefiles/arch/$(TARGET_ARCH).mk

BINDIR ?= $(INSTALLDIR)/bin
LIBDIR ?= $(INSTALLDIR)/lib
INCDIR ?= $(INSTALLDIR)/include
SHAREDIR ?= $(INSTALLDIR)/share

INSTALLDIR_MACRO ?= $(INSTALLDIR)
LIBDIR_MACRO ?= $(LIBDIR)
SHAREDIR_MACRO ?= $(SHAREDIR)
IMAGEDIR_MACRO ?= $(IMAGEDIR)
APPIMAGEDIR_MACRO ?= $(APPIMAGEDIR)

XML_LIBS ?= -lexpat
ARCHIVER_LIBS ?= -lz -lbz2
NETWORK_LIBS ?= -lcurl
DB_LIBS ?= -lsqlite3

CFLAGS += -DINSTALLDIR=\"$(INSTALLDIR_MACRO)\" -DBASEDIR=\"$(SHAREDIR_MACRO)\" -DLIBDIR=\"$(LIBDIR_MACRO)\" -DIMAGEDIR=\"$(IMAGEDIR_MACRO)\" -DAPPIMAGEDIR=\"$(APPIMAGEDIR_MACRO)\" -DVERSION=\"$(VERSION)\" -DOSNAME=\"$(OSNAME)\"
ifeq "$(ZLSHARED)" "yes"
  CFLAGS += -fPIC -DZLSHARED
endif


ifeq "$(TARGET_STATUS)" "release"
	CFLAGS += -O3
	LDFLAGS += -s
endif
ifeq "$(TARGET_STATUS)" "debug"
	CFLAGS += -O0 -g
endif
ifeq "$(TARGET_STATUS)" "profile"
	CFLAGS += -O3 -g -pg
	LDFLAGS += -pg
endif

ZINCLUDE = -I $(ROOTDIR)/zlibrary/core/include -I $(ROOTDIR)/zlibrary/text/include

ZLSHARED ?= yes

ifeq "$(ZLSHARED)" "yes"
  CORE_LIBS = -lm -L$(ROOTDIR)/zlibrary/core -lzlcore -ldl $(DB_LIBS)
  TEXT_LIBS = -lzltext
else
  CORE_LIBS = -lm -L$(ROOTDIR)/zlibrary/ui -L$(ROOTDIR)/zlibrary/core -lzlcore -lzlui-$(UI_TYPE) -lzlcore $(UILIBS) $(XML_LIBS) $(ARCHIVER_LIBS) $(NETWORK_LIBS) $(DB_LIBS)
  TEXT_LIBS = -lzltext $(EXTERNAL_LIBS) -llinebreak -lfribidi
endif

ifeq "$(TARGET_ARCH)" "symbian"
  FBREADER_LIBS =  -l:sqlite3.dso 
  EXTERNAL_LIBS = -l:libc.dso -l:libdl.dso -l:euser.dso -l:eexe.lib -l:usrt2_2.lib -l:qtmain.lib -l:dfpaeabi.dso -l:drtaeabi.dso -l:scppnwdl.dso -lsupc++ -lgcc -l:remconcoreapi.dso -l:remconinterfacebase.dso -l:libstdcpp.dso -l:libpthread.dso
#TODO: in S60, there's no libzcore.dso, ezlib.dso should be used instead
  CORE_LIBS = -l:libm.dso -L$(ROOTDIR)/zlibrary/ui -L$(ROOTDIR)/zlibrary/core -lzlcore -lzlui-$(UI_TYPE) -lzlcore  -l:QtGui.dso -l:QtCore.dso -l:QtNetwork.dso -l:QtScroller.lib -l:expat.lib -l:libzcore.dso -l:bz2.lib $(FBREADER_LIBS) $(EXTERNAL_LIBS)
  TEXT_LIBS =  -lzltext -l:linebreak.lib  -l:fribidi.lib
endif

ifneq "$(BUILD_RESOURCE_OBJECT)" "yes"
.resources:
endif
