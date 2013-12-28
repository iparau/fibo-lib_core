#pragma once
	
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#define X(str) CXStr(str).unicodeForm()

XERCES_CPP_NAMESPACE_USE

namespace core {
namespace tools {

	class CXStr
	{
	public :
		CXStr(const char* const toTranscode);
		virtual ~CXStr();

		const XMLCh* unicodeForm() const { return fUnicodeForm; }

	private :
		XMLCh*   fUnicodeForm;
	};

}
}