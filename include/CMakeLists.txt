set(XMLWRAPP_HEADERS
  xmlwrapp/attributes.h
  xmlwrapp/_cbfo.h
  xmlwrapp/document.h
  xmlwrapp/event_parser.h
  xmlwrapp/errors.h
  xmlwrapp/export.h
  xmlwrapp/init.h
  xmlwrapp/node.h
  xmlwrapp/nodes_view.h
  xmlwrapp/relaxng.h
  xmlwrapp/schema.h
  xmlwrapp/tree_parser.h
  xmlwrapp/version.h
  xmlwrapp/xmlwrapp.h
  xmlwrapp/xpath.h
)

set(XSLTWRAPP_HEADERS
  xsltwrapp/init.h
  xsltwrapp/stylesheet.h
  xsltwrapp/xsltwrapp.h
)

install(FILES ${XMLWRAPP_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmlwrapp)

if( XMLWRAPP_WITH_LIBXSLT )
  install (FILES ${XSLTWRAPP_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xsltwrapp)
endif( XMLWRAPP_WITH_LIBXSLT )
