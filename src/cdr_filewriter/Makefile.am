

h_sources = CdrFileWriterModule.h cdrfw_file_writer.h offcs_com_cdr_contents.h

c_sources = CdrFileWriterModule.c cdrfw_file_writer.c offcs_com_cdr_contents.c

INCLUDES = -I. -I$(top_srcdir)/src -I$(top_srcdir)/src/rf_app -I$(top_srcdir)/src/db_module -I$(top_srcdir)/src/ASNFiles -I$(top_srcdir)/src/cdr_filewriter -I$(top_srcdir)/include/amps -I$(top_srcdir)/include/libxml2 $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS)

LDADD = $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) -L/usr/local/lib -lxml2 -lamps -L$(top_srcdir)/src/ASNFiles -lASNModule

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

lib_LTLIBRARIES= libCdrFileWriterModule.la
libCdrFileWriterModule_la_SOURCES= $(h_sources) $(c_sources)
libCdrFileWriterModule_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(RELEASE)
