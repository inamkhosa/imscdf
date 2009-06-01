

h_sources = CGF.h

c_sources = CGF.c

INCLUDES = -I. -I$(top_srcdir)/src -I$(top_srcdir)/src/rf_app -I$(top_srcdir)/src/db_module -I$(top_srcdir)/src/cdr_filewriter -I$(top_srcdir)/src/ASNFiles $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) -I$(top_srcdir)/include/amps -I$(top_srcdir)/include/libxml2

LDADD = -lASNModule -L$(top_srcdir)/src/ASNFiles/ $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) -L/usr/local/lib -lxml2 -lamps 

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

lib_LTLIBRARIES= libCGF.la
libCGF_la_SOURCES= $(h_sources) $(c_sources)
libCGF_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(RELEASE)
