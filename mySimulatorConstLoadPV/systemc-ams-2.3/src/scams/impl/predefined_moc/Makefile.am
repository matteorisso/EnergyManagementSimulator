
include $(top_srcdir)/config/Make-rules

SUBDIRS = \
	conservative \
	eln \
	lsf \
	tdf

INCLUDE_DIR = $(prefix)/include/scams/impl/predefined_moc

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(INCLUDE_DIR)

uninstall-local:
	@$(NORMAL_UNINSTALL)
	-rmdir $(DESTDIR)$(INCLUDE_DIR)

## Taf!
