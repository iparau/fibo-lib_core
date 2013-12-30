#include "AStr.h"

namespace core {
namespace tools {

// Other include files, declarations, and non-Xerces-C++ initializations.

XERCES_CPP_NAMESPACE_USE

CAStr::CAStr(const XMLCh* const toTranscode)
{
    // Call the private transcoding method
    fAsciiForm = XMLString::transcode(toTranscode);
}

CAStr::~CAStr()
{
    XMLString::release(&fAsciiForm);
}

} //namespace tools
} //namespace core