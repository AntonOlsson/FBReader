ROOTDIR = $(CURDIR)/../..
MAKEFILESDIR = $(ROOTDIR)/makefiles
include $(MAKEFILESDIR)/config.mk

VERSION = $(shell cat ../VERSION)
SOVERSION = $(shell cat SOVERSION)

ifeq "$(ZLSHARED)" "yes"
  ifeq "$(TARGET_ARCH)" "macosx"
    TARGET = libzlcore.$(VERSION).dylib
    TARGET_SONAME = libzlcore.$(SOVERSION).dylib
    TARGET_SHORTNAME = libzlcore.dylib
  else
    TARGET = libzlcore.so.$(VERSION)
    TARGET_SONAME = libzlcore.so.$(SOVERSION)
    TARGET_SHORTNAME = libzlcore.so
  endif
else
  TARGET = libzlcore.a
endif

SUBDIRS_ALL = src/library src/typeId src/util src/constants src/logger src/filesystem src/filesystem/zip src/filesystem/bzip2 src/filesystem/tar src/dialogs src/optionEntries src/application src/view src/encoding src/options src/message src/resources src/time src/xml src/xml/expat src/image src/language src/runnable src/network src/network/requests src/blockTreeView src/tree src/desktop/application src/desktop/dialogs src/unix/time src/unix/xmlconfig src/unix/filesystem src/unix/iconv src/unix/library src/unix/curl src/win32/encoding src/win32/filesystem src/win32/config \
src/tree

SUBDIRS = src/library src/typeId src/util src/constants src/logger src/filesystem src/filesystem/zip src/filesystem/bzip2 src/filesystem/tar src/dialogs src/optionEntries src/application src/view src/encoding src/options src/message src/resources src/time src/xml src/xml/expat src/image src/language src/unix/time src/runnable src/network src/network/requests src/blockTreeView src/tree

ifeq "<$(TARGET_ARCH)>" "$(findstring <$(TARGET_ARCH)>, <desktop> <pepperpad3>)"
  SUBDIRS += src/desktop/application src/desktop/dialogs
endif

ifneq "<$(UI_TYPE)>" "$(findstring <$(UI_TYPE)>, <qt4> <qml>)"
  SUBDIRS += src/unix/curl
endif

# TODO: Temporary, remove it
ifeq "$(UI_TYPE)" "qml"
  SUBDIRS += src/desktop/application src/desktop/dialogs
endif

ifeq "$(TARGET_ARCH)" "pdaxrom"
  SUBDIRS += src/desktop/dialogs
endif

ifeq "$(TARGET_ARCH)" "win32"
  SUBDIRS += src/desktop/application src/desktop/dialogs src/win32/encoding src/win32/filesystem src/win32/config
else
  SUBDIRS += src/unix/xmlconfig src/unix/filesystem src/unix/iconv src/unix/library
endif

# TODO: Temporary, remove it
ifeq "$(TARGET_ARCH)" "symbian"
  SUBDIRS = src/library src/typeId src/util src/constants src/logger src/filesystem src/filesystem/zip src/filesystem/bzip2 src/filesystem/tar src/dialogs src/optionEntries src/application src/view src/encoding src/options src/message src/resources src/time src/xml src/xml/expat src/image src/language  src/runnable src/network src/network/requests src/blockTreeView
  SUBDIRS += src/unix/xmlconfig src/unix/filesystem src/tree
endif 
# TODO: Temporary, remove it
ifeq "$(TARGET_ARCH)" "desktop"
	ifeq "$(UI_TYPE)" "qtwidgets"
	SUBDIRS = src/library src/typeId src/util src/constants src/logger src/filesystem src/filesystem/zip src/filesystem/bzip2 src/filesystem/tar src/dialogs src/optionEntries src/application src/view src/encoding src/options src/message src/resources src/time src/xml src/xml/expat src/image src/language  src/runnable src/network src/network/requests src/blockTreeView
	SUBDIRS += src/unix/xmlconfig src/unix/filesystem src/tree
	endif
endif

.objects:
	@for subdir in $(SUBDIRS); do \
		if [ -d $$subdir ]; then \
			if ! $(LIBMAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk; then \
				exit 1; \
			fi; \
		fi; \
	done;

$(TARGET): .objects
	@echo -n 'Creating $@ ...'
	@$(RM_QUIET) $(TARGET)
ifeq "$(ZLSHARED)" "yes"
	@$(LD) $(LDFLAGS) -shared -Wl,-soname,$(TARGET_SONAME) -o $(TARGET) -lc $(patsubst %, %/*.o, $(SUBDIRS)) $(XML_LIBS) $(ARCHIVER_LIBS) $(NETWORK_LIBS) -ldl
	#@$(LD) $(LDFLAGS) -dynamiclib -dylinker_install_name $(TARGET_SONAME) -o $(TARGET) -lc $(patsubst %, %/*.o, $(SUBDIRS)) $(XML_LIBS) $(ARCHIVER_LIBS) $(NETWORK_LIBS) -ldl -liconv
	@ln -sf $(TARGET) $(TARGET_SONAME)
	@ln -sf $(TARGET) $(TARGET_SHORTNAME)
else
	@$(AR) $(TARGET) $(patsubst %, %/*.o, $(SUBDIRS))
endif
	@echo ' OK'

ZLSHAREDIR ?= $(SHAREDIR)/zlibrary
ZLSHARE_DEST := $(DESTDIR)/$(ZLSHAREDIR)

ENCODING_FILES = $(wildcard data/encodings/*)
ifeq "$(TARGET_ARCH)" "maemo"
  ENCODING_FILES = $(filter-out %/Big5,$(wildcard data/encodings/*))
endif

do_install:
	@install -d $(ZLSHARE_DEST)
	@install -d $(ZLSHARE_DEST)/encodings
	@install -m 0644 $(ENCODING_FILES) $(ZLSHARE_DEST)/encodings
	@install -d $(ZLSHARE_DEST)/resources
	@install -m 0644 $(wildcard data/resources/*.xml) $(ZLSHARE_DEST)/resources
	@install -m 0644 data/languagePatterns.zip $(ZLSHARE_DEST)
	@install -m 0644 data/unicode.xml.gz $(ZLSHARE_DEST)
	@install -d $(ZLSHARE_DEST)/default
	@if [ -e data/default/config.$(TARGET_ARCH).xml ]; then \
		install -m 0644 data/default/config.$(TARGET_ARCH).xml $(ZLSHARE_DEST)/default/config.xml; \
	fi
	@if [ -e data/default/config.$(TARGET_ARCH)-$(UI_TYPE).xml ]; then \
		install -m 0644 data/default/config.$(TARGET_ARCH)-$(UI_TYPE).xml $(ZLSHARE_DEST)/default/config.xml; \
	fi
ifeq "$(ZLSHARED)" "yes"
	@install -d $(DESTDIR)$(LIBDIR)
	@install $(TARGET) $(DESTDIR)$(LIBDIR)
	@ln -sf $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET_SONAME)
endif

do_install_dev:
	@install -d $(DESTDIR)$(LIBDIR)
ifeq "$(ZLSHARED)" "yes"
	@ln -sf $(TARGET) $(DESTDIR)$(LIBDIR)/$(TARGET_SHORTNAME)
else
	@install $(TARGET) $(DESTDIR)$(LIBDIR)
endif
	@install -d $(DESTDIR)$(INCDIR)/zlibrary/core
	@install -m 0644 $(wildcard include/*.h) $(DESTDIR)$(INCDIR)/zlibrary/core
	@install -d $(DESTDIR)$(INCDIR)/zlibrary/core/optionEntries
	@install -m 0644 $(wildcard include/optionEntries/*.h) $(DESTDIR)$(INCDIR)/zlibrary/core/optionEntries

clean:
	@for subdir in $(SUBDIRS_ALL); do \
		if [ -d $$subdir ]; then \
			$(MAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk clean; \
		fi; \
	done;
	@$(RM) *.so *.so.* *.dylib *.a
