

h_sources = db_cdrs.h db_cmd_id.h DBModule.h offCS_db_debug.h

c_sources = DBModule.c offCS_db_debug.c

INCLUDES = -I. -I$(top_srcdir)/src -I$(top_srcdir)/src/rf_app -I$(top_srcdir)/src/cdr_filewriter $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) -I$(top_srcdir)/include/amps -I$(top_srcdir)/include/libxml2

LDADD = $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) -L/usr/local/lib -lxml2 -lamps

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

lib_LTLIBRARIES= libDBModule.la
libDBModule_la_SOURCES= $(h_sources) $(c_sources)
libDBModule_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(RELEASE)
