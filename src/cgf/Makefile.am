

h_sources = CGF.h

c_sources = CGF.c

INCLUDES = -I. -I$(top_srcdir)/src -I$(top_srcdir)/src/rf_app -I$(top_srcdir)/src/db_module -I$(top_srcdir)/src/cdr_filewriter -I$(top_srcdir)/src/ASNFiles $(LIBXML_DEPS_CFLAGS) $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) $(LIBAMPS_DEPS_CFLAGS)

LDADD = -lASNModule -L$(top_srcdir)/src/ASNFiles/ $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) $(LIBXML_DEPS_LIBS) 

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

lib_LTLIBRARIES= libCGF.la
libCGF_la_SOURCES= $(h_sources) $(c_sources)
libCGF_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(RELEASE)
