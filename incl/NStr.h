#pragma once

#define N(id) CNStr(id).asciiForm()

namespace core {
namespace tools {

	class CNStr
	{
	public :
		CNStr(const int toTranscode);
		virtual ~CNStr();

		const char* asciiForm() const { return buffer; }

	protected :
		void printInt( const char * format, ... );

	private:
		char buffer[256];
	};

}
}