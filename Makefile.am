ACLOCAL_AMFLAGS = -I admin

if WITH_DOCS
maybe_docs = docs
endif

if WITH_EXAMPLES
maybe_examples = examples
endif

if WITH_TESTS
maybe_tests = tests
endif

SUBDIRS = include src $(maybe_docs) $(maybe_examples) $(maybe_tests)

pkgconfigdir=$(libdir)/pkgconfig

if WITH_XSLT
pkgconfig_DATA = xmlwrapp.pc xsltwrapp.pc
else
pkgconfig_DATA = xmlwrapp.pc
endif

bin_SCRIPTS = xmlwrapp-config

EXTRA_DIST = \
		LICENSE \
		$(srcdir)/platform/Win32/README \
		$(srcdir)/platform/Win32/*.sln \
		$(srcdir)/platform/Win32/*.vcxproj \
		$(srcdir)/platform/Win32/*.vcxproj.filters \
		$(srcdir)/platform/Win32/libxml.props
