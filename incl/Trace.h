#pragma once

enum TraceLevel 
{ 
	TraceNone = 0, // no trace 
	TraceError = 10, // only trace error 
	TraceInfo = 20, // some extra info 
	TraceDebug = 30, // debugging info 
	TraceDetail = 40 // detailed debugging info 
}; 
 
extern void SetTraceFilePrefix(const char* strFilePrefix); 
extern void SetTraceLevel(const int nLevel); 
extern void WriteTrace(const int nLevel, const char* strFormat, ...); 
