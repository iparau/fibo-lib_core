#include "StdAfx.h"
#include "XStr.h"

// Other include files, declarations, and non-Xerces-C++ initializations.

XERCES_CPP_NAMESPACE_USE

namespace core {
namespace tools {

CXStr::CXStr(const char* const toTranscode)
{
    // Call the private transcoding method
    fUnicodeForm = XMLString::transcode(toTranscode);
}

CXStr::~CXStr()
{
    XMLString::release(&fUnicodeForm);
}

} // namespace tools
} // namespace core