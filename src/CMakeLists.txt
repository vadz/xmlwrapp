# Configure shared library with the given name and (full) ELF version.
#
# Set properties for using hidden ELF visibility, defining proper DLL-related
# symbols and use $ORIGIN in the runpath.
#
# Note that this does nothing when shared libraries are not used.
function(setup_shared_library target version)
  if (XMLWRAPP_SHARED)
    # Construct XMLWRAPP_USE_DLL or XSLTWRAPP_USE_DLL symbol name.
    string(TOUPPER "${target}_USE_DLL" use_dll_symbol)
    target_compile_definitions(${target}
      PRIVATE
        HAVE_VISIBILITY
      INTERFACE
        ${use_dll_symbol}
    )

    # Set SOVERSION to the major VERSION component.
    string(REPLACE "." ";" version_as_list ${version})
    list(GET version_as_list 0 soversion)

    set(output_name "${target}")
    if( XMLWRAPP_NAME_PREFIX )
      set(output_name "${XMLWRAPP_NAME_PREFIX}${output_name}")
    endif()
    if( XMLWRAPP_NAME_SUFFIX )
      set(output_name "${output_name}${XMLWRAPP_NAME_SUFFIX}")
    endif()

    set_target_properties(${target} PROPERTIES
      OUTPUT_NAME ${output_name}
      BUILD_RPATH_USE_ORIGIN ON
      DEFINE_SYMBOL "DLL_EXPORT"
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN ON
      VERSION ${version}
      SOVERSION ${soversion}
    )
  endif()
endfunction()

add_library(xmlwrapp ${XMLWRAPP_LIB_TYPE})
target_sources(xmlwrapp
  PRIVATE
    libxml/ait_impl.cxx
    libxml/ait_impl.h
    libxml/attributes.cxx
    libxml/document.cxx
    libxml/dtd_impl.cxx
    libxml/dtd_impl.h
    libxml/errors.cxx
    libxml/errors_impl.h
    libxml/event_parser.cxx
    libxml/init.cxx
    libxml/node.cxx
    libxml/node_iterator.cxx
    libxml/node_iterator.h
    libxml/node_manip.cxx
    libxml/node_manip.h
    libxml/nodes_view.cxx
    libxml/relaxng.cxx
    libxml/schema.cxx
    libxml/tree_parser.cxx
    libxml/utility.cxx
    libxml/utility.h
    libxml/version.cxx
    libxml/xpath.cxx
)

target_include_directories(xmlwrapp
  PRIVATE
    ${LIBXML2_INCLUDE_DIRS}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_options(xmlwrapp
  PUBLIC
    ${LIBXML2_LDFLAGS}
)
target_link_libraries(xmlwrapp
  PUBLIC
    ${LIBXML2_LIBRARIES}
)
setup_shared_library(xmlwrapp 6.0.0)
install (TARGETS xmlwrapp EXPORT XmlwrappTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(xmlwrapp::xmlwrapp ALIAS xmlwrapp)

if( XMLWRAPP_WITH_LIBXSLT )
  add_library(xsltwrapp ${XMLWRAPP_LIB_TYPE})
  target_sources(xsltwrapp
    PRIVATE
      libxslt/init.cxx
      libxslt/stylesheet.cxx
      libxslt/result.h
  )
  target_include_directories(xsltwrapp
    PRIVATE
      ${LIBXSLT_INCLUDE_DIRS}
      ${LIBEXSLT_INCLUDE_DIRS}
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  target_link_options(xsltwrapp
    PUBLIC
      ${LIBXSLT_LDFLAGS}
      ${LIBEXSLT_LDFLAGS}
  )
  target_link_libraries(xsltwrapp
    PUBLIC
      xmlwrapp
      ${LIBXSLT_LIBRARIES}
      ${LIBEXSLT_LIBRARIES}
  )
  setup_shared_library(xsltwrapp 4.0.0)
  install (TARGETS xsltwrapp EXPORT XmlwrappTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

  add_library(xmlwrapp::xsltwrapp ALIAS xsltwrapp)
endif( XMLWRAPP_WITH_LIBXSLT )
