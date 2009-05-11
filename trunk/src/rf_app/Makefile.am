

h_sources = rf_ims_info_avp_parser.h RFModule.h rf_sms_info_avp_parser.h rf_lcs_info_avp_parser.h rf_msg_api.h rf_wlan_info_avp_parser.h rf_mbms_info_avp_parser.h rf_poc_info_avp_parser.h rf_mms_info_avp_parser.h rf_ps_info_avp_parser.h

c_sources = rf_ims_info_avp_parser.c RFModule.c rf_sms_info_avp_parser.c rf_lcs_info_avp_parser.c rf_msg_api.c rf_wlan_info_avp_parser.c rf_mbms_info_avp_parser.c rf_poc_info_avp_parser.c rf_mms_info_avp_parser.c rf_ps_info_avp_parser.c

INCLUDES = -I. -I$(top_srcdir)/src -I$(top_srcdir)/src/db_module -I$(top_srcdir)/src/cdr_filewriter $(LIBXML_DEPS_CFLAGS) $(DIAMETER_DEPS_CFLAGS) $(DIAMAPPCOMMON_DEPS_CFLAGS) $(LIBAMPS_DEPS_CFLAGS)

LDADD = $(DIAMETER_DEPS_LIBS) $(DIAMAPPCOMMON_DEPS_LIBS) $(LIBXML_DEPS_LIBS)

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@/\" -DLIBDIR=\"$(libdir)/\"

lib_LTLIBRARIES= libRFModule.la
libRFModule_la_SOURCES= $(h_sources) $(c_sources)
libRFModule_la_LDFLAGS= -version-info $(LIBRARY_VERSION) -release $(RELEASE)
