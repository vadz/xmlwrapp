dnl configure.ac script for xmlwrapp, process with autoconf to create configure
dnl
dnl Copyright (C) 2008 Vaclav Slavik <vslavik@gmail.com>,
dnl               2008 Vadim Zeitlin <vz-xmlwrapp@zeitlins.org>
dnl All Rights Reserved
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in
dnl    the documentation and/or other materials provided with the
dnl    distribution.
dnl 3. Neither the name of the Author nor the names of its contributors
dnl    may be used to endorse or promote products derived from this software
dnl    without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
dnl AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
dnl TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
dnl PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
dnl OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
dnl USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
dnl AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
dnl OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
dnl OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
dnl SUCH DAMAGE.

dnl === Prologue ===

AC_PREREQ(2.61)
AC_INIT(xmlwrapp, 0.10.0, [xmlwrapp@googlegroups.com])

AC_CONFIG_SRCDIR([xmlwrapp.pc.in])
AC_CONFIG_AUX_DIR([admin])
AC_CONFIG_MACRO_DIR([admin])

AM_INIT_AUTOMAKE([foreign subdir-objects tar-ustar dist-xz])
AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

dnl remember, "build" is where we compile, "host" is where the resulting
dnl program runs (which may be different from "build" for cross-compilation)
AC_CANONICAL_HOST

dnl Give error for unknown options instead of just warning about them.
m4_divert_text([DEFAULTS],[enable_option_checking=fatal])

dnl === Command line options ===

AC_ARG_ENABLE(docs,
             [AS_HELP_STRING([--disable-docs],[don't build the documentation (requires Doxygen)])],
             [case "x${enableval}" in
                   x) build_docs=no ;;
                xyes) build_docs=yes ;;
                 xno) build_docs=no ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-docs) ;;
             esac],
             [build_docs=yes])

AC_ARG_ENABLE(examples,
             [AS_HELP_STRING([--disable-examples],[don't build the examples])],
             [case "x${enableval}" in
                   x) build_examples=no ;;
                xyes) build_examples=yes ;;
                 xno) build_examples=no ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-examples) ;;
             esac],
             [build_examples=yes])

AC_ARG_ENABLE(tests,
             [AS_HELP_STRING([--disable-tests],[don't build the tests])],
             [case "x${enableval}" in
                   x) build_tests=no ;;
                xyes) build_tests=yes ;;
                 xno) build_tests=no ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-tests) ;;
             esac],
             [build_tests=yes])

AC_ARG_ENABLE(xslt,
             [AS_HELP_STRING([--disable-xslt],[don't build libxsltwrapp library])],
             [case "x${enableval}" in
                   x) build_xslt=no ;;
                xyes) build_xslt=yes ;;
                 xno) build_xslt=no ;;
                   *) AC_MSG_ERROR(bad value ${enableval} for --enable-xslt) ;;
             esac],
             [build_xslt=yes])


dnl === Program checks ===

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX(11)
LT_PATH_LD

dnl This is a workaround for the harmless but annoying warning
dnl
dnl ar: `u' modifier ignored since `D' is the default (see `U')
dnl
dnl given by Linux systems using recent binutils with the switch to building
dnl deterministic archives (that can't include the timestamps) when building
dnl all static libraries with default "cru" flags used by Libtool up to 2.4.6.
AR_FLAGS=cr

LT_INIT([win32-dll])

if test "x$build_docs" = "xyes" ; then
    AC_CHECK_PROGS(HAVE_DOXYGEN, doxygen, yes)
    if test "x$HAVE_DOXYGEN" != "xyes" ; then
        AC_MSG_WARN([Doxygen not available, documentation generation disabled])
        build_docs=no
    fi
fi

dnl === Library checks ===

dnl use C++ compiler as we're checking for C++ libraries/headers from now on
dnl (we couldn't do it before as libtool tests must be done with C compiler)
AC_LANG(C++)

PKG_CHECK_MODULES(LIBXML, [libxml-2.0 >= 2.4.28])

if test "x$build_xslt" = "xyes" ; then
    PKG_CHECK_MODULES(LIBXSLT, [libxslt >= 1.1.6])
    PKG_CHECK_MODULES(LIBEXSLT, [libexslt])
fi


AC_HEADER_ASSERT

dnl === Compiler-specific stuff ===

AC_CACHE_CHECK([if using clang],
    xmlwrapp_cv_prog_clang,
    [
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
                [],
                [[
                #ifndef __clang__
                    not clang
                #endif
                ]],
            )],
            xmlwrapp_cv_prog_clang=yes,
            xmlwrapp_cv_prog_clang=no
        )
    ]
)

CLANG=$xmlwrapp_cv_prog_clang

dnl Enable more warnings for the compilers we know about.
if test "x$CLANG" == "xyes"; then
    dnl Enable clang-specific warnings.
    CXXFLAGS="$CXXFLAGS -Wextra-semi"
elif test "x$GCC" == "xyes"; then
    dnl Enable gcc-only warnings here.
    CXXFLAGS="$CXXFLAGS -Wlogical-op -Wuseless-cast"

    dnl These warnings are not supported by the old gcc versions.
    AX_APPEND_COMPILE_FLAGS([-Wduplicated-cond -Wnull-dereference])
fi

if test "x$GCC" == "xyes"; then
    dnl Enable some useful warnings that both clang and gcc support that are
    dnl not already enabled by -Wall
    CXXFLAGS="$CXXFLAGS -W -Wall -Wcast-align -Wcast-qual -Wconversion\
 -Wdouble-promotion -Woverloaded-virtual -Wsign-conversion -Wshadow\
 -Wtype-limits -Wswitch-enum -Wundef -Wwrite-strings"
fi

dnl Check if the compiler supports symbols visibility:
XMLWRAPP_VISIBILITY

dnl === Generate output files ===

dnl This is support for the legacy xmlwrapp-config script, which has to behave
dnl as it did in 0.5.x, i.e. output flags for both libxmlwrapp and
dnl libxsltwrapp and also include dependency libs' flags for static linking
LEGACY_LINK_FLAGS="-lxmlwrapp $LIBXML_LIBS"
if test "x$build_xslt" = "xyes" ; then
    LEGACY_LINK_FLAGS="-lxsltwrapp $LIBEXSLT_LIBS $LIBXSLT_LIBS $LEGACY_LINK_FLAGS"
fi
AC_SUBST(LEGACY_LINK_FLAGS)

AM_CONDITIONAL(WITH_DOCS, [ test "x$build_docs" = "xyes" ])
AM_CONDITIONAL(WITH_EXAMPLES, [ test "x$build_examples" = "xyes" ])
AM_CONDITIONAL(WITH_TESTS, [ test "x$build_tests" = "xyes" ])
AM_CONDITIONAL(WITH_XSLT, [ test "x$build_xslt" = "xyes" ])

AC_CONFIG_FILES([
    xmlwrapp.pc
    xsltwrapp.pc
    xmlwrapp-config
    Makefile
    include/Makefile
    src/Makefile
    docs/Makefile
    examples/Makefile
    examples/01-tree_parsing/Makefile
    examples/02-event_parsing/Makefile
    examples/03-xml_generation/Makefile
    examples/04-xslt/Makefile
    examples/05-xpath/Makefile
    tests/Makefile
])
AC_OUTPUT
