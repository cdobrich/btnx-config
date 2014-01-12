
figdir = figures
docname = btnx-manual
##$(helpname)
lang = C
omffile = btnx-manual-C.omf
entities = legal.xml
include $(top_srcdir)/doc/xmldocs.make
dist-hook: app-dist-hook

$(omffile):


if BUILD_HELP_HTML_TEST
htmldocsdir = $(docrootdir)/$(docname)/$(lang)
htmldocs_DATA = *.html
endif

##$(top_srcdir)/doc/$(docname)/$(lang)/%.html:
%.html:
	@echo ""
	@echo "Please wait while HTML documentation is generated from DocBook SGML..."
	@meinproc $(docname).xml
	@echo "Done"

clean-local:
	-$(RM) -f *.html