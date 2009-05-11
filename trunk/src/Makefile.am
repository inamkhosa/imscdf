SUBDIRS= ASNFiles rf_app db_module cdr_filewriter cgf

bin_PROGRAMS = offlinecharging

offlinecharging_SOURCES = offline_charging_server.h offline_charging_server.c 

INCLUDES = -I. -I$(top_srcdir)/src/rf_app $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) $(LIBAMPS_DEPS_CFLAGS)

LDADD = $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) $(LIBXML_DEPS_LIBS) $(LIBAMPS_DEPS_LIBS) -lASNModule -L$(top_srcdir)/src/ASNFiles/

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

offlinechargingextradir = $(datadir)/@PACKAGE@
offlinechargingextra_DATA = $(top_srcdir)/config/*
