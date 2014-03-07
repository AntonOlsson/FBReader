ROOTDIR = $(CURDIR)/../..
include $(ROOTDIR)/makefiles/config.mk

FBSHARE_DEST := $(DESTDIR)$(SHAREDIR)/$(FBREADER_NAME)
install:
#	@mv $(DESTDIR)/$(BINDIR)/FBReader $(DESTDIR)/$(BINDIR)/$(FBREADER_NAME)
	@install -d $(FBSHARE_DEST)/default
	@install -m 0644 ../data/default/keymap.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/keymap.xml
	@install -m 0644 ../data/default/styles.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/styles.xml
	@install -m 0644 ../data/default/menubar.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/menubar.xml
	@install -m 0644 ../data/default/config.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/config.xml
	@install -m 0644 ../data/default/toolbar.$(TARGET_ARCH).xml $(FBSHARE_DEST)/default/toolbar.xml
	@install -d $(DESTDIR)$(SHAREDIR)/icons/hicolor/86x86/apps
	@install -m 0644 ../data/icons/application/$(TARGET_ARCH).png $(DESTDIR)$(SHAREDIR)/icons/hicolor/86x86/apps/$(FBREADER_NAME).png
	@install -d $(DESTDIR)$(SHAREDIR)/applications/
	@install -m 0644 $(FBREADER_NAME).desktop $(DESTDIR)$(SHAREDIR)/applications/$(FBREADER_NAME).desktop

clean:
