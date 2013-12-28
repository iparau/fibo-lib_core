#pragma once

#include <xercesc/dom/DOM.hpp>	
// Other include files, declarations, and non-Xerces-C++ initializations.

XERCES_CPP_NAMESPACE_USE

namespace core {

	class CInputData
	{
	public:
		CInputData(void);
		virtual ~CInputData(void);
		// Used to initialize the xml input data from file
		bool Initialize(const char * xmlFilePath);

		int getFibonacciID() { return fibonacciID; };

	private:
		bool parseDoc(DOMDocument* inputDoc);

	protected:
		int              fibonacciID;
	};

}

