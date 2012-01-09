NULL =

# FIXMEchpe: keep this in sync with mcharmap/Makefile.am !
srcdirheaders = \
	mcharmap-block-chapters-model.h \
	mcharmap-block-codepoint-list.h \
	mcharmap-chapters-model.h \
	mcharmap-chapters-view.h \
	mcharmap-charmap.h \
	mcharmap-chartable.h \
	mcharmap-codepoint-list.h \
	mcharmap-script-chapters-model.h \
	mcharmap-script-codepoint-list.h \
	mcharmap-unicode-info.h \
	$(NULL)
builddirheaders = \
	mcharmap.h \
	mcharmap-type-builtins.h \
	$(NULL)

defsdir = $(datadir)/pygtk/2.0/defs
defs_DATA = mcharmap.defs

pkgpyexecdir = $(pyexecdir)/gtk-2.0
pkgpyexec_LTLIBRARIES = mcharmap.la

BUILT_SOURCES = \
	mcharmap.c \
	$(NULL)

mcharmap_la_SOURCES = \
	mcharmap.c \
	mcharmapmodule.c \
	$(NULL)

mcharmap_la_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(PYTHON_INCLUDES) \
	$(DISABLE_DEPRECATED) \
	$(AM_CPPFLAGS)

mcharmap_la_CFLAGS = \
	$(NO_STRICT_ALIASING_CFLAGS) \
	$(PYGTK_CFLAGS) \
	$(GTK_CFLAGS) \
	$(WARN_CFLAGS) \
	$(AM_CFLAGS)

mcharmap_la_LIBADD = \
	$(top_builddir)/mcharmap/libmcharmap.la \
	$(PYGTK_LIBS) \
        $(PYTHON_LIB_LOC) \
	$(PYTHON_LIBS) \
        $(PYTHON_EXTRA_LIBS)

mcharmap_la_LDFLAGS = \
	-module \
	-avoid-version	\
	-export-symbols-regex initmcharmap \
	$(AM_LDFLAGS)

CLEANFILES = \
	$(BUILT_SOURCES) \
	$(NULL)

EXTRA_DIST = \
	mcharmap.defs \
	mcharmap.override \
	$(NULL)

mcharmap.c: mcharmap.defs mcharmap.override

.defs.c:
	$(AM_V_GEN) (cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c

regenerate-python-binding:
	$(AM_V_GEN) $(PYGTK_H2DEF) \
		$(sort $(addprefix $(top_srcdir)/mcharmap/,$(srcdirheaders)) \
		       $(addprefix $(top_builddir)/mcharmap/,$(builddirheaders))) \
		> mcharmap.defs.new

-include $(top_srcdir)/git.mk
