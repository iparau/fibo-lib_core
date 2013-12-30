! include( ../../common.pri ) {
    error( Couldn't find the common.pri file! )
}

TEMPLATE = lib

CONFIG *= staticlib create_prl link_prl

CONFIG -= qt

TARGET = core

DEFINES *= XERCES_STATIC_LIBRARY

INCLUDEPATH += $$LIB_CPPUNIT_DIR_INC
INCLUDEPATH += $$LIB_XERCES_DIR_INC
INCLUDEPATH += $$LIB_CORE_DIR_INC

CONFIG(debug,debug|release) {
	LIBS *= -L"$$LIB_CPPUNIT_DIR_DBG"	
	LIBS *= -L"$$LIB_XERCES_DIR_DBG"
	
	win32 {
		PRE_TARGETDEPS *= $$LIB_CPPUNIT_DIR_DBG/cppunit.lib
		PRE_TARGETDEPS *= $$LIB_XERCES_DIR_DBG/xerces.lib
	}
	
	unix {
		PRE_TARGETDEPS *= $$LIB_CPPUNIT_DIR_DBG/libcppunit.a
		PRE_TARGETDEPS *= $$LIB_XERCES_DIR_DBG/libxerces.a
	}
} else {

	LIBS *= -L"$$LIB_CPPUNIT_DIR_RLS"
	LIBS *= -L"$$LIB_XERCES_DIR_RLS"	
	
	win32 {	
	    PRE_TARGETDEPS *= $$LIB_CPPUNIT_DIR_RLS/cppunit.lib
		PRE_TARGETDEPS *= $$LIB_XERCES_DIR_RLS/xerces.lib
	}
	
	unix {    		
	    PRE_TARGETDEPS *= $$LIB_CPPUNIT_DIR_RLS/libcppunit.a
		PRE_TARGETDEPS *= $$LIB_XERCES_DIR_RLS/libxerces.a
	}
}

LIBS *= -lcppunit
LIBS *= -lxerces

#Include file(s)
include(lib_core.pri)
