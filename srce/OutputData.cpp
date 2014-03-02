#include "OutputData.h"

#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
// Other include files, declarations, and non-Xerces-C++ initializations.

#include "AStr.h"
#include "XStr.h"
#include "NStr.h"

#include "Trace.h"

XERCES_CPP_NAMESPACE_USE
using namespace std;

namespace core {

using namespace tools;

COutputData::COutputData(int fibonacciID)
{
    this->fibonacciID = fibonacciID;
	this->fibonacciValue = fibonacci( this->fibonacciID ); 
}


COutputData::~COutputData(void)
{

}

int COutputData::fibonacci( int n ) {
	int retVal = 0;
	
	switch (n) {
		case 0:
			retVal = 0;
			break;
		case 1:
		case 2:
			retVal = 1;
			break;
		default:
			retVal = fibonacci(n-1) + fibonacci(n-2);
			break;
	}

	return retVal;
}

bool COutputData::buildOutputXml( const char* xmlFilePath )
{
	bool retVal = false;

	WriteTrace( TraceDebug, "Building output document %s",
		xmlFilePath );

	DOMImplementation* implementation =  DOMImplementationRegistry::getDOMImplementation(X("Core"));

    if ( 0 != implementation )
    {
        try
        {
            DOMDocument* outDoc = implementation->createDocument(
                            0,                    // root element namespace URI.
                            X("fibonacci"),         // root element name
                            0);                   // document type object (DTD).

			if ( 0 != outDoc ) {
				WriteTrace( TraceDebug, "XML DOM document created." );
				
				int rootId = this->getFibonacciID( );
				DOMElement* rootElem = outDoc->getDocumentElement();

				WriteTrace( TraceDebug, "Adding root node attributes." );
				
				//Add the "id" and "value" attributes to root node  
				rootElem->setAttribute( X ( "id" ), X ( N ( rootId ) ) );
				rootElem->setAttribute( X ( "value" ), X ( N ( this->getFibonacciValue( ) ) ) );

				//Add the child nodes as list of consecutive id values
				for ( int id = 0; id <= rootId; id++ ) {
					WriteTrace( TraceDebug, "Adding child node number, id: %s",
						N( id ) );
					
					DOMElement* number = outDoc->createElement( X ( "number" ) );

					if ( 0 != number ) {
						WriteTrace( TraceDebug, "Adding child node number attributes" );

						//Add the "id" and "value" attributes to root node  
						number->setAttribute( X ( "id" ), X ( N ( id ) ) );
						number->setAttribute( X ( "value" ), X ( N ( this->fibonacci( id ) ) ) );

						rootElem->appendChild( number );
					}
					else {
						WriteTrace( TraceError, "Failed adding child node number %s",
							N( id ) );
					}
				}

				retVal = OutputXML(outDoc, xmlFilePath);

				outDoc->release();
			}
			else {
				WriteTrace( TraceError, "Null DOMDocument. DOMImplementation::createDocument failed!" );
			}
        }
        catch (const OutOfMemoryException& toCatch) {
			WriteTrace( TraceError, "OutOfMemoryException! Exception message is: %s",
					A(toCatch.getMessage()) );
		}
        catch (const DOMException& toCatch)
        {
            WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
        }
        catch(const XMLException& toCatch)
        {
            WriteTrace( TraceError, "DOMException! Exception message is: %s",
					A(toCatch.getMessage()) );
        }
        catch (...)
        {
            WriteTrace( TraceError, "Unexpected Exception!" );
        }
    }  // (inmpl != NULL)
    else
    {
        WriteTrace( TraceError, "Null DOMImplementationRegistry. Requested implementation is not supported!" );
    }

	return retVal;
}

bool COutputData::OutputXML( xercesc::DOMDocument* pmyDOMDocument, const char* xmlFilePath ) 
{
    bool retVal = false;

	WriteTrace( TraceDebug, "Writing output document %s",
		xmlFilePath );

    //Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save. 
    DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation( X ( "LS" ) );

    if ( 0 != implementation ) {
		// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
        DOMLSSerializer *serializer = (static_cast<DOMImplementationLS*>(implementation))->createLSSerializer();
		
		if ( 0 != serializer ) { 

			// Make the output more human readable by inserting line feeds. 
			if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true)) 
				serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true); 

			// The end-of-line sequence of characters to be used in the XML being written out.  
			serializer->setNewLine( X( "\n" ) );  

			// Convert the path into Xerces compatible XMLCh*. 
			// Specify the target for the XML output. 
			XMLFormatTarget *formatTarget = new LocalFileFormatTarget( X( xmlFilePath ) );
			
			if ( 0 != formatTarget ) { 

				// Create a new empty output destination object. 
                DOMLSOutput *output = (static_cast<DOMImplementationLS*>(implementation))->createLSOutput();
				
				if ( 0 != output ) { 

					// Set the stream to our target. 
					output->setByteStream(formatTarget); 

					// Write the serialized output to the destination. 
					serializer->write(pmyDOMDocument, output);
					
					retVal = true;
					
					WriteTrace( TraceInfo, "Document succesfully writen! :)" ); 
					output->release();
				}
				else {
					WriteTrace( TraceError, "NULL DOMLSOutput. Document not writen!" );
				}

				delete formatTarget;
			}
			else {
				WriteTrace( TraceError, "NULL XMLFormatTarget. Document not writen!" );
			}

			// Cleanup. 
			serializer->release();
		}
		else {
		    WriteTrace( TraceError, "NULL DOMLSSerializer. Document not writen!" );
		}
	}
	else {
		WriteTrace( TraceError, "Null DOMImplementationRegistry. Document not writen, requested implementation is not supported!" );
	}

	return retVal;
} 

} //namespace core
