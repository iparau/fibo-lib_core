#pragma once
	
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#define A(str) CAStr(str).asciiForm()

XERCES_CPP_NAMESPACE_USE

namespace core {
namespace tools {

	class CAStr
	{
	public :
		CAStr(const XMLCh* const toTranscode);
		virtual ~CAStr();

		const char* asciiForm() const { return fAsciiForm; }

	private :
		char*   fAsciiForm;
	};

}
}