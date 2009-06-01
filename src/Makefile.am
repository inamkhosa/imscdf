SUBDIRS= ASNFiles rf_app db_module cdr_filewriter cgf

bin_PROGRAMS = offlinecharging

offlinecharging_SOURCES = offline_charging_server.h offline_charging_server.c 

INCLUDES = -I. -I$(top_srcdir)/src/rf_app $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) -I$(top_srcdir)/include/amps -I$(top_srcdir)/include/libxml2

LDADD = $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) -L/usr/local/lib -lxml2 -lamps -lASNModule -L$(top_srcdir)/src/ASNFiles/

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

offlinechargingextradir = $(datadir)/@PACKAGE@
offlinechargingextra_DATA = $(top_srcdir)/config/*
