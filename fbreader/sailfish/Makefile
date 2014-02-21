ROOTDIR = $(CURDIR)/../..
include $(ROOTDIR)/makefiles/config.mk

FBSHAREDIR = $(DESTDIR)$(SHAREDIR)/FBReader

install:
	@install -d $(FBSHAREDIR)/default
	@install -m 0644 ../data/default/keymap.$(TARGET_ARCH).xml $(FBSHAREDIR)/default/keymap.xml
	@install -m 0644 ../data/default/styles.$(TARGET_ARCH).xml $(FBSHAREDIR)/default/styles.xml
	@install -m 0644 ../data/default/menubar.$(TARGET_ARCH).xml $(FBSHAREDIR)/default/menubar.xml
	@install -m 0644 ../data/default/config.$(TARGET_ARCH).xml $(FBSHAREDIR)/default/config.xml
	@install -d $(DESTDIR)$(SHAREDIR)/icons/hicolor/80x80/apps
	@install -m 0644 ../data/icons/application/$(TARGET_ARCH).png $(DESTDIR)$(SHAREDIR)/icons/hicolor/80x80/apps/FBReader.png
	@install -d $(DESTDIR)$(SHAREDIR)/applications/$(DESKTOP_DIR)
	@install -m 0644 FBReader.desktop $(DESTDIR)$(SHAREDIR)/applications/FBReader.desktop

clean:
