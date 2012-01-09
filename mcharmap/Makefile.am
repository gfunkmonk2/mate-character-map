## Copyright © 2003  Noah Levitt
## Copyright © 2008  Christian Persch
##
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation; either version 2 of the License, or (at your
## option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02110-1301  USA
##

NULL =

lib_LTLIBRARIES = libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@.la

BUILT_SOURCES = \
	mcharmap-marshal.c \
	mcharmap-marshal.h \
	mcharmap-type-builtins.c \
	mcharmap-type-builtins.h \
	$(NULL)

libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_SOURCES = \
	mcharmap-block-chapters-model.c \
	mcharmap-block-chapters-model.h \
	mcharmap-block-codepoint-list.c \
	mcharmap-block-codepoint-list.h \
	mcharmap-chapters-model.c \
	mcharmap-chapters-model.h \
	mcharmap-chapters-view.c \
	mcharmap-chapters-view.h \
	mcharmap-charmap.c \
	mcharmap-charmap.h \
	mcharmap-chartable-accessible.c \
	mcharmap-chartable-accessible.h \
	mcharmap-chartable-cell-accessible.c \
	mcharmap-chartable-cell-accessible.h \
	mcharmap-chartable.c \
	mcharmap-chartable.h \
	mcharmap-codepoint-list.c \
	mcharmap-codepoint-list.h \
	mcharmap-init.c \
	mcharmap-private.h \
	mcharmap-script-chapters-model.c \
	mcharmap-script-chapters-model.h \
	mcharmap-script-codepoint-list.c \
	mcharmap-script-codepoint-list.h \
	mcharmap-unicode-info.c \
	mcharmap-unicode-info.h \
	unicode-blocks.h \
	unicode-categories.h \
	unicode-names.h \
	unicode-nameslist.h \
	unicode-scripts.h \
	unicode-unihan.h \
	unicode-versions.h \
	$(NULL)

nodist_libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_SOURCES = $(BUILT_SOURCES)

libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_CPPFLAGS = \
	-I$(top_srcdir) \
	-DMCHARMAP_COMPILATION \
	-DDATADIR=\"$(datadir)\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DHELPDIR=\"$(HELP_DIR)\" \
	-DG_DISABLE_SINGLE_INCLUDES \
	-DPANGO_DISABLE_SINGLE_INCLUDES \
	-DATK_DISABLE_SINGLE_INCLUDES \
	-DGDK_DISABLE_SINGLE_INCLUDES \
	-DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES \
	-DGTK_DISABLE_SINGLE_INCLUDES \
	$(DISABLE_DEPRECATED) \
	$(AM_CPPFLAGS)

libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_CFLAGS = \
	$(GTK_CFLAGS)	\
	$(WARN_CFLAGS)	\
	$(AM_CFLAGS)

libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_LDFLAGS = \
	-version-info $(LIBMCHARMAP_LT_VERSION)	\
	-export-symbols-regex "^mcharmap_*"		\
	$(AM_LDFLAGS)

libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@_la_LIBADD = \
	$(GTK_LIBS)	\
	$(INTLLIBS)

libmcharmapincludedir = $(includedir)/mcharmap-$(MCHARMAP_API_VERSION)/mcharmap
libmcharmapinclude_HEADERS = \
	mcharmap-block-chapters-model.h \
	mcharmap-block-codepoint-list.h \
	mcharmap-chapters-model.h \
	mcharmap-chapters-view.h \
	mcharmap-charmap.h \
	mcharmap-chartable.h \
	mcharmap-codepoint-list.h \
	mcharmap.h \
	mcharmap-script-chapters-model.h \
	mcharmap-script-codepoint-list.h \
	mcharmap-unicode-info.h \
	$(NULL)

nodist_libmcharmapinclude_HEADERS = \
	mcharmap-type-builtins.h \
	$(NULL)

if ENABLE_CHARMAP
bin_PROGRAMS = mcharmap
else
noinst_PROGRAMS = mcharmap
endif

mcharmap_SOURCES = \
	mcharmap-mini-fontsel.c \
	mcharmap-mini-fontsel.h \
	mcharmap-print-operation.c \
	mcharmap-print-operation.h \
	mcharmap-search-dialog.c \
	mcharmap-search-dialog.h \
	mcharmap-settings.c \
	mcharmap-settings.h \
	mcharmap-window.c \
 	mcharmap-window.h \
	main.c \
	$(NULL)

mcharmap_CPPFLAGS = \
	-I$(top_srcdir) \
	-DDATADIR=\"$(datadir)\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DHELPDIR=\"$(HELP_DIR)\" \
	-DG_DISABLE_SINGLE_INCLUDES \
	-DPANGO_DISABLE_SINGLE_INCLUDES \
	-DATK_DISABLE_SINGLE_INCLUDES \
	-DGDK_DISABLE_SINGLE_INCLUDES \
	-DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES \
	-DGTK_DISABLE_SINGLE_INCLUDES \
	$(DISABLE_DEPRECATED) \
	$(AM_CPPFLAGS)

mcharmap_CFLAGS = \
	$(GTK_CFLAGS)	\
	$(MATECONF_CFLAGS)	\
	$(WARN_CFLAGS)	\
	$(AM_CFLAGS)

mcharmap_LDFLAGS = \
	$(AM_LDFLAGS)

mcharmap_LDADD = \
	libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@.la	\
	$(GTK_LIBS)	\
	$(MATECONF_LIBS)	\
	$(INTL_LIBS)

if OS_WIN32
mcharmap_LDADD += mcharmaprc.o
mcharmaprc.o: mcharmaprc.rc
	$(AM_V_GEN) windres -i $< -o $@
endif

mcharmap-marshal.h: $(srcdir)/mcharmap-marshal.list
	$(AM_V_GEN) ( $(GLIB_GENMARSHAL) --prefix=_mcharmap_marshal $(srcdir)/mcharmap-marshal.list \
                            --header \
                            --internal > mcharmap-marshal.h.tmp \
	  && mv mcharmap-marshal.h.tmp mcharmap-marshal.h ) \
        || ( rm -f mcharmap-marshal.h.tmp && exit 1 )

mcharmap-marshal.c: $(srcdir)/mcharmap-marshal.list
	$(AM_V_GEN) ( $(GLIB_GENMARSHAL) --prefix=_mcharmap_marshal $(srcdir)/mcharmap-marshal.list \
			    --header \
                            --body \
                            --internal > mcharmap-marshal.c.tmp \
          && mv mcharmap-marshal.c.tmp mcharmap-marshal.c ) \
        || ( rm -f mcharmap-marshal.c.tmp && exit 1 )


mcharmap-type-builtins.h: stamp-mcharmap-type-builtins.h
	@true

stamp-mcharmap-type-builtins.h: mcharmap-type-builtins.h.template $(libmcharmapinclude_HEADERS)
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $< $(filter-out $<,$^) > xgen-gtbh \
	&& (cmp -s xgen-gtbh mcharmap-type-builtins.h || cp xgen-gtbh mcharmap-type-builtins.h ) \
	&& rm -f xgen-gtbh \
	&& echo timestamp > $(@F)

mcharmap-type-builtins.c: mcharmap-type-builtins.c.template $(libmcharmapinclude_HEADERS)
	$(AM_V_GEN) $(GLIB_MKENUMS) --template $< $(filter-out $<,$^) > xgen-gtbc \
	&& (cmp -s xgen-gtbc mcharmap-type-builtins.c || cp xgen-gtbc mcharmap-type-builtins.c ) \
	&& rm -f xgen-gtbc

# printoptions.h: print.glade
# 	$(AM_V_GEN) $(GTK_BUILDER_CONVERT) --root custom_tab_container $< - | \
# 		$(XMLLINT) --noblanks - | \
# 		$(HEXDUMP) -v -e '"" 1/1 "0x%02x,""\n"' > xgen-$(@F) \
# 	&& (cmp -s xgen-$(@F) $@ || cp -f xgen-$(@F) $@) \
# 	&& rm -f xgen-$(@F)

if ENABLE_INTROSPECTION

girdir = $(GIRDIR)
gir_DATA = Mcharmap-$(MCHARMAP_API_VERSION).gir

typelibdir = $(GIRTYPELIBDIR)
typelib_DATA = Mcharmap-$(MCHARMAP_API_VERSION).typelib

Mcharmap-$(MCHARMAP_API_VERSION).gir: $(G_IR_SCANNER) libmcharmap@MCHARMAP_LIBRARY_SUFFIX_U@.la $(libmcharmapinclude_HEADERS) Makefile
	$(AM_V_GEN) PKG_CONFIG_PATH=$(top_builddir):$$PKG_CONFIG_PATH \
	$(G_IR_SCANNER) \
		--verbose \
		--namespace=Mcharmap \
		--nsversion=$(MCHARMAP_API_VERSION) \
		--include=GLib-2.0 \
		--include=GObject-2.0 \
		--include=Gtk-$(GTK_API_VERSION) \
		--library=mcharmap@MCHARMAP_LIBRARY_SUFFIX_U@ \
		--libtool="$(LIBTOOL)" \
		--output $@ \
		--pkg mcharmap-$(MCHARMAP_API_PC_VERSION) \
		-I$(top_srcdir) \
		-I$(top_builddir) \
		-DMCHARMAP_COMPILATION \
		$(filter %.h,$^)

%.typelib: %.gir $(G_IR_COMPILER)
	$(AM_V_GEN) $(G_IR_COMPILER) --includedir=$(srcdir) --includedir=. $(G_IR_COMPILER_OPTS) $< -o $@

endif # ENABLE_INTROSPECTION

EXTRA_DIST = \
	mcharmap-marshal.list \
	mcharmap-type-builtins.c.template \
	mcharmap-type-builtins.h.template \
	mcharmaprc.rc \
	$(NULL)

CLEANFILES = \
	stamp-mcharmap-type-builtins.h \
	$(BUILT_SOURCES) \
	$(gir_DATA) \
	$(typelib_DATA) \
	$(NULL)

if ENABLE_CHARMAP

# don't do the symlink stuff if this is windows
if !OS_WIN32
# install a symlink to the binary called "charmap"
#install-exec-hook:
#	test -e "$(DESTDIR)$(bindir)/charmap" \
#        || (cd "$(DESTDIR)$(bindir)" && $(LN_S) mcharmap charmap) ; \
#	test -e "$(DESTDIR)$(bindir)/mate-character-map" \
#        || (cd "$(DESTDIR)$(bindir)" && $(LN_S) mcharmap mate-character-map)

# remove bin/charmap if it's a symlink (it's a pain to test what it's
# a symlink to)
uninstall-local:
	test -h "$(DESTDIR)$(bindir)/charmap" \
	&& rm -f "$(DESTDIR)$(bindir)/charmap" ; \
	test -h "$(DESTDIR)$(bindir)/mate-character-map" \
	&& rm -f "$(DESTDIR)$(bindir)/mate-character-map"

endif # !OS_WIN32

endif # ENABLE_CHARMAP

-include $(top_srcdir)/git.mk
