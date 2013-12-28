#pragma once

#include <xercesc/dom/DOM.hpp>	
// Other include files, declarations, and non-Xerces-C++ initializations.

XERCES_CPP_NAMESPACE_USE

namespace core {

	class COutputData
	{
		public:
			COutputData(int fibonacciID);
			virtual ~COutputData(void);

			static int fibonacci( int n );

			bool 	buildOutputXml( const char* xmlFilePath );

		protected:
			int     getFibonacciID() { return fibonacciID; };
			int     getFibonacciValue() { return fibonacciValue; };
			bool    OutputXML( DOMDocument* pmyDOMDocument, const char* xmlFilePath );

		protected:
			int     fibonacciID;
			int     fibonacciValue;
	};

}

