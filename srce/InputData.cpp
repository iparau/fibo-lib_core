#include "InputData.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax/HandlerBase.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#include "AStr.h"
#include "XStr.h"

#include "Trace.h"

XERCES_CPP_NAMESPACE_USE
using namespace std;

namespace core {

using namespace tools;

CInputData::CInputData(void)
{

}


CInputData::~CInputData(void)
{

}


// Used to initialize the xml input data from file
bool CInputData::Initialize(const char * xmlFilePath)
{
	bool retVal = false;

	XercesDOMParser* parser = new XercesDOMParser();

    //memory security check
	if ( 0 != parser ) {
		parser->setValidationScheme(XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);    // optional

		ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();

		if (0 != errHandler) {
			parser->setErrorHandler(errHandler);

			try {
				WriteTrace( TraceDebug, "Parsing input document %s", xmlFilePath);
				parser->parse(xmlFilePath);

				retVal = true;
			}
			catch (const OutOfMemoryException& toCatch) {
				WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (const XMLException& toCatch) {
				WriteTrace( TraceError, "XMLException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (const DOMException& toCatch) {
				WriteTrace( TraceError, "DOMException! Exception message is: %s",
						A(toCatch.getMessage()) );
			}
			catch (...) {
				WriteTrace( TraceError, "Unexpected Exception" );
			}

	
			if ( true == retVal ) {
				//no error occured during parsing
				WriteTrace( TraceDebug, "Parse succeded!" );

				DOMDocument* inputDoc = parser->getDocument();

				retVal = parseDoc( inputDoc );
			}

			//
			//  Clean up the error handler. The parser does not adopt handlers
			//  since they could be many objects or one object installed for multiple
			//  handlers.
			//
			if (0 != errHandler) {
				delete errHandler;
			}
		}

		//
		//  Delete the parser itself.  Must be done prior to calling Terminate.
		// 
		if (0 != parser) {
			delete parser;
		}
	}

 	return retVal;
}

// Used to initialize the xml input data from xml DOM doc
bool  CInputData::parseDoc(DOMDocument* inputDoc)
{
	bool retVal = false;

	if ( 0 != inputDoc ) {
		WriteTrace( TraceDebug, "Extracting fibonacci id from DOM document" );

		try {
			DOMElement* rootNode = inputDoc->getDocumentElement();
				
			if (0 != rootNode) {
				const XMLCh *rootTagName = rootNode->getTagName();
					 
				WriteTrace( TraceDebug, "Root node Tag name is %s", 
					A(rootTagName) );

				if ( rootNode ->hasAttributes() ) {
					const char* attrIDTagName = "id";
						 
					if ( rootNode->hasAttribute( X( attrIDTagName ) ) ) {
						DOMAttr* attrID = rootNode->getAttributeNode( X( attrIDTagName ) );

						if (0 != attrID) {
							const XMLCh* attrIDValue = attrID->getValue();
								 
							WriteTrace( TraceInfo, "Extract succceded, id attribute value is %s", 
								A( attrIDValue ) );

							fibonacciID = XMLString::parseInt( attrIDValue );

							retVal = true;
						}
						else {
							//detected internal document error
							retVal = false;

							WriteTrace( TraceError, "Document root node invalid, atribute 'id' not detected!" );
						}
					}
					else {
						//detected internal document error
						retVal = false;

						WriteTrace( TraceError, "Document root node invalid, atribute 'id' not identified!" );
					}
				}
				else {
					//detected internal document error
					retVal = false;

					WriteTrace( TraceError, "Document root node invalid, no attributes detected!" );
				}
			}
			else {
				//detected internal document error
				retVal = false;

				WriteTrace( TraceError, "Document root node invalid, not identified!" );
			}
		}
		catch (const OutOfMemoryException& toCatch) {
			WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const XMLException& toCatch) {
			WriteTrace( TraceError, "XMLException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (const DOMException& toCatch) {
			WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
		catch (...) {
			WriteTrace( TraceError, "Unexpected Exception" );
		}
	}
	else {
		//detected internal document error
		retVal = false;

		WriteTrace( TraceError, "Document content invalid!" );
	}

	return retVal;
}

} //namespace core
