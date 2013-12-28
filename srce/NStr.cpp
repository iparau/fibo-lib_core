#include "StdAfx.h"
#include "NStr.h"

#include <stdio.h>
#include <stdarg.h>

// Other include files, declarations, and non-Xerces-C++ initializations.

namespace core {
namespace tools {

CNStr::CNStr(const int toTranscode)
{
	printInt("%d", toTranscode);
}

CNStr::~CNStr()
{

}

void CNStr::printInt( const char * format, ... )
{
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	va_end (args); 
}

} //namespace tools
} //namespace core