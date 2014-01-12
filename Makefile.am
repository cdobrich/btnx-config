
## Main Makefile.am

if BUILD_HELP_TEST
docsubdir=doc
endif

SUBDIRS = intl src $(docsubdir) po

DATADIR = $(top_srcdir)/data
ICONDIR = $(top_srcdir)/data/icons

$(DATADIR)/$(PACKAGE).desktop:
	@sed -e 's:<ROOT_EXEC>:$(rootexec):' -e 's:<BTNX_EXEC_PATH>:$(sbindir)/$(PACKAGE):' \
		<$(DATADIR)/$(PACKAGE).desktop.template >$(DATADIR)/$(PACKAGE).desktop
$(DATADIR)/btnx.png:

gladedir = $(pkgdatadir)
glade_DATA = $(DATADIR)/btnx-config.glade
pixmapdir = /usr/share/pixmaps
desktopdir = /usr/share/applications
pixmap_DATA = $(DATADIR)/btnx.png
desktop_DATA = $(DATADIR)/$(PACKAGE).desktop
EXTRA_DIST = $(DATADIR)/$(PACKAGE).desktop.template \
	m4 \
	COPYING-DOCS \
	ABOUT-NLS \
	COPYING \
	AUTHORS \
	README \
	INSTALL

uninstall-local:
	rm -rf $(pkgdatadir)
clean-local:
	-$(RM) -f $(DATADIR)/$(PACKAGE).desktop


ACLOCAL_AMFLAGS = -I m4
