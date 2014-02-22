ROOTDIR = $(CURDIR)/..

MAKEFILESDIR = $(ROOTDIR)/makefiles

include $(MAKEFILESDIR)/config.mk

TARGET = FBReader
target = fbreader

ifeq "$(TARGET_ARCH)" "sailfish"
TARGET = harbour-fbreader
endif

ALL_SUBDIRS = src src/database src/database/sqldb src/database/sqldb/implsqlite src/database/booksdb src/database/booksdb/runnables src/migration src/options src/library src/bookmodel src/formats src/formats/fb2 src/formats/docbook src/formats/css src/formats/html src/formats/pdb src/formats/txt src/formats/tcr src/formats/chm src/formats/xhtml src/formats/oeb src/formats/rtf src/formats/openreader src/formats/pdf src/formats/dummy src/formats/util src/external src/fbreader src/encodingOption src/network src/network/authentication src/network/authentication/basic src/network/atom src/network/opds src/network/authentication/litres src/blockTree src/libraryActions src/libraryTree src/networkActions src/networkTree src/optionsDialog src/optionsDialog/bookInfo src/optionsDialog/library src/optionsDialog/network src/optionsDialog/system src/optionsDialog/reading src/optionsDialog/lookAndFeel src/optionsDialogMobile src/tree
ALL_ARCHSUBDIRS = desktop pdaxrom opie zaurus maemo openzaurus pma400 win32 meego sailfish

SUBDIRS = src/database src/database/sqldb src/database/sqldb/implsqlite src/database/booksdb src/database/booksdb/runnables src/migration src/options src/library src/bookmodel \
	src/formats src/formats/fb2 src/formats/css src/formats/html src/formats/pdb src/formats/txt src/formats/tcr src/formats/chm src/formats/xhtml src/formats/oeb src/formats/rtf src/formats/openreader src/formats/util \
	src/external src/fbreader src/encodingOption src/network src/network/authentication src/network/authentication/basic src/network/atom src/network/opds src/network/authentication/litres \
	src/blockTree src/libraryActions src/libraryTree src/networkActions src/networkTree src/optionsDialog src/optionsDialog/bookInfo src/optionsDialog/library src/optionsDialog/network src/optionsDialog/system src/optionsDialog/reading src/optionsDialog/lookAndFeel \
	src/optionsDialogMobile \
	src/tree

all: .resources
	@for subdir in $(SUBDIRS); do \
		if ! $(MAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk; then \
			exit 1; \
		fi; \
	done;
	@echo -n 'Linking $(TARGET) ...'
	@$(LD) $(LDFLAGS) -o $(TARGET) `find src -name *.o` -L$(ROOTDIR)/zlibrary/text $(TEXT_LIBS) $(CORE_LIBS)
	@echo ' OK'

FBSHAREDIR ?= $(SHAREDIR)/$(TARGET)
FBSHARE_DEST := $(DESTDIR)/$(FBSHAREDIR)

EXTERNAL_FILE = "external.$(TARGET_ARCH).xml"

ifeq "$(TARGET_ARCH)" "maemo"
  ifeq "$(UI_TYPE)" "qml"
	EXTERNAL_FILE = "external.maemo6.xml"
  endif
endif

VARIANT = $(TARGET_ARCH)
ifneq "$(RESOLUTION)" "" 
  VARIANT = $(TARGET_ARCH)_$(RESOLUTION)
endif

APPIMAGEDIR_REAL = $(subst %application_name%,$(target),$(subst %APPLICATION_NAME%,$(TARGET),$(APPIMAGEDIR)))
do_install:
	@install -d $(DESTDIR)$(BINDIR)
	@install $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	@install -d $(FBSHARE_DEST)
	@install -d $(FBSHARE_DEST)/help
	@./scripts/install_help.sh $(VARIANT) $(FBSHARE_DEST)/help
	@install -d $(FBSHARE_DEST)/network
	@install -d $(FBSHARE_DEST)/network/certificates
	@install -m 0644 $(wildcard data/network/certificates/*.crt) $(FBSHARE_DEST)/network/certificates
	@install -d $(FBSHARE_DEST)/formats/html
	@install -m 0644 data/formats/html/html.ent $(FBSHARE_DEST)/formats/html
	@install -d $(FBSHARE_DEST)/formats/xhtml
	@install -m 0644 $(wildcard data/formats/xhtml/*.ent) $(FBSHARE_DEST)/formats/xhtml
	@install -d $(FBSHARE_DEST)/formats/fb2
	@sed "s/VERSION/$(VERSION)/" data/formats/fb2/FBReaderVersion.ent > $(FBSHARE_DEST)/formats/fb2/FBReaderVersion.ent
	@install -m 0644 data/formats/fb2/fb2genres.xml $(FBSHARE_DEST)/formats/fb2
	@install -d $(FBSHARE_DEST)/default
	@./scripts/install_toolbar_and_menu.sh $(VARIANT) $(UI_TYPE) $(FBSHARE_DEST)/default
	@./scripts/install_config.sh $(VARIANT) $(UI_TYPE) $(FBSHARE_DEST)/default
	@install -m 0644 data/default/$(EXTERNAL_FILE) $(FBSHARE_DEST)/default/external.xml
	@if [ -f data/default/messages.$(TARGET_ARCH).xml ]; then \
		install -m 0644 data/default/messages.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/messages.xml; \
	fi
	@install -d $(FBSHARE_DEST)/resources
	@install -m 0644 $(wildcard data/resources/*.xml) $(FBSHARE_DEST)/resources
	@install -d $(DESTDIR)$(APPIMAGEDIR_REAL)
	@install -m 0644 $(wildcard data/icons/toolbar/$(VARIANT)/*.*) $(DESTDIR)$(APPIMAGEDIR_REAL)
	@install -m 0644 $(wildcard data/icons/filetree/$(VARIANT)/*.*) $(DESTDIR)$(APPIMAGEDIR_REAL)
	@install -m 0644 $(wildcard data/icons/booktree/$(VARIANT)/*.*) $(DESTDIR)$(APPIMAGEDIR_REAL)
	@make -C $(TARGET_ARCH) RESOLUTION=$(RESOLUTION) install

clean:
	@for subdir in $(ALL_SUBDIRS); do \
		$(MAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk clean; \
	done;
	@for subdir in $(ALL_ARCHSUBDIRS); do \
		cd $$subdir; make clean; cd ..; \
	done;
	@$(RM) $(TARGET) err
