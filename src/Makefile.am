
AM_CPPFLAGS = -I$(top_srcdir)/include $(CXXFLAGS_VISIBILITY)

if WITH_XSLT
lib_LTLIBRARIES = libxmlwrapp.la libxsltwrapp.la
else
lib_LTLIBRARIES = libxmlwrapp.la
endif

libxmlwrapp_la_CPPFLAGS = -DXMLWRAPP_BUILD $(AM_CPPFLAGS) $(LIBXML_CFLAGS)
libxmlwrapp_la_LIBADD = $(LIBXML_LIBS)
libxmlwrapp_la_LDFLAGS = -version-info 6:0:0 -no-undefined

libxmlwrapp_la_SOURCES = \
		libxml/ait_impl.cxx \
		libxml/ait_impl.h \
		libxml/attributes.cxx \
		libxml/document.cxx \
		libxml/dtd_impl.cxx \
		libxml/dtd_impl.h \
		libxml/event_parser.cxx \
		libxml/errors.cxx \
		libxml/errors_impl.h \
		libxml/init.cxx \
		libxml/node.cxx \
		libxml/nodes_view.cxx \
		libxml/node_iterator.cxx \
		libxml/node_iterator.h \
		libxml/node_manip.cxx \
		libxml/node_manip.h \
		libxml/relaxng.cxx \
		libxml/schema.cxx \
		libxml/tree_parser.cxx \
		libxml/utility.cxx \
		libxml/utility.h \
		libxml/version.cxx \
		libxml/xpath.cxx


if WITH_XSLT

libxsltwrapp_la_CPPFLAGS = -DXSLTWRAPP_BUILD -DXMLWRAPP_USE_DLL $(AM_CPPFLAGS) $(LIBEXSLT_CFLAGS) $(LIBXSLT_CFLAGS)
libxsltwrapp_la_LIBADD = libxmlwrapp.la $(LIBEXSLT_LIBS) $(LIBXSLT_LIBS)
libxsltwrapp_la_LDFLAGS = -version-info 4:0:0 -no-undefined

libxsltwrapp_la_SOURCES = \
		libxslt/init.cxx \
		libxslt/result.h \
		libxslt/stylesheet.cxx

endif
