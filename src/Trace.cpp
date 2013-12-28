#include "StdAfx.h"

#include "Trace.h"   

#include <stdio.h>
#include <time.h> 
#include <string.h>
#include <stdarg.h>
   
// private helper class   
class XYTraceHelper   
{   
    // friend functions of this class   
    friend void SetTraceFilePrefix(const char* strFilePrefix);   
    friend void SetTraceLevel(const int nLevel);   
    friend void WriteTrace(const int nLevel, const char* strFormat, ...);
	   
    // internal data members   
    FILE* m_hFile;   
    int m_nLevel;   
    long m_nThreadId;   
    char* m_pTraceFilePrefix;   
    time_t m_timeStart;   
    // close the current trace file   
    void CloseTraceFile()   
    {   
        if(m_hFile) fclose(m_hFile);   
        m_hFile = NULL;   
    }   
    // open a new trace file   
    FILE* OpenTraceFile()   
    {   
        // construct the new trace file path   
        char strFilePath[1001];   
        
		time_t sysTime;
        time(&sysTime);   
		struct tm* timeinfo = localtime(&sysTime);

        sprintf   
        (   
            strFilePath,   
            "%s_%04d%02d%02d_%02d%02d%02d.txt",   
            m_pTraceFilePrefix?m_pTraceFilePrefix:"Trace",   
            timeinfo->tm_year,
            timeinfo->tm_mon,   
            timeinfo->tm_mday,   
            timeinfo->tm_hour,   
            timeinfo->tm_min,   
            timeinfo->tm_sec    
        );   
        // create the new trace file   
        m_hFile = fopen   
        (   
            strFilePath,   
            "a+"   
        );   
        // if successful, save the start time variable   
        if(m_hFile) m_timeStart = sysTime;   
        // return the file handle   
        return m_hFile;   
    }   
    // set lock to gain exclusive access to trace   
    // functions   
    void Lock()   
    {   
        //Currently not implemented   
    }   
    // release lock so that other threads can access    
    // trace functions   
    void Unlock()   
    {   
        //Currently not implemented   
    }   
    // set the current trace level   
    void SetTraceLevel(const int nLevel) { m_nLevel = nLevel>0?nLevel:0; }   
    // set the trace file name prefix   
    void SetTraceFilePrefix(const char* strFilePrefix)   
    {   
        // close existing trace file first   
        CloseTraceFile();   
        // copy the file name prefix   
        size_t nSize = strFilePrefix ? strlen(strFilePrefix):0;   
        delete []m_pTraceFilePrefix;   
        m_pTraceFilePrefix = new char[nSize+1];   
        strcpy(m_pTraceFilePrefix, nSize>0?strFilePrefix:"");   
    }   
public:   
    // constructor and destructor   
    XYTraceHelper()   
    {   
        m_hFile = NULL;   
        m_nLevel = 0;   
        m_nThreadId = 0;   
        m_pTraceFilePrefix = NULL;   
    }   
    ~XYTraceHelper()   
    {   
        CloseTraceFile();   
        delete []m_pTraceFilePrefix;   
    }   
};   
   
// the one and only instance of XYTraceHelper   
XYTraceHelper theHelper;   
   
void SetTraceFilePrefix(const char* strFilePrefix)   
{   
    // set lock   
    theHelper.Lock();   
    // set trace file name prefix   
    theHelper.SetTraceFilePrefix(strFilePrefix);   
    // release lock   
    theHelper.Unlock();   
}   
   
void SetTraceLevel(const int nLevel)   
{   
    // set lock   
    theHelper.Lock();   
    // set trace level   
    theHelper.SetTraceLevel(nLevel);   
    // release lock   
    theHelper.Unlock();   
}   
   
void WriteTrace(const int nLevel, const char* strFormat, ...)   
{   
    // if the specified trace level is greater than   
    // the current trace level, return immediately   
    if(theHelper.m_nLevel==0||nLevel>theHelper.m_nLevel) return;   
    // set lock   
    theHelper.Lock();   
    try   
    {   
        // get local time   
        time_t sysTime;
        time(&sysTime);   
		struct tm* timeinfo = localtime(&sysTime);
		struct tm* startinfo = localtime(&theHelper.m_timeStart);
        // get trace file handle       
        FILE* hFile = theHelper.m_hFile;   
        // open the trace file if not already open   
        if(hFile==NULL) hFile = theHelper.OpenTraceFile();   
        // if it is already a new day, close the old   
        // trace file and open a new one   
        else if   
        (   
            timeinfo->tm_year!=startinfo->tm_year||   
            timeinfo->tm_mon!=startinfo->tm_mon||   
            timeinfo->tm_mday!=startinfo->tm_mday)   
        {   
            theHelper.CloseTraceFile();   
            theHelper.OpenTraceFile();   
        }   
        // write the trace message   
        if(hFile)   
        {   
            // declare buffer (default max buffer size = 32k)   
            const int nMaxSize = 32*1024;   
            char pBuffer[nMaxSize+51];   
            // print time stamp and thread id to buffer   
            int nPos = sprintf   
            (   
                pBuffer,   
                "%02d:%02d:%02d: ",    
                timeinfo->tm_hour,   
                timeinfo->tm_min,   
                timeinfo->tm_sec
            );
			
			switch(nLevel) {
				case TraceError:
					nPos += sprintf
					(
						pBuffer+nPos,
						"Error: "
					);
					break;
				case TraceInfo: 
					nPos += sprintf
					(
						pBuffer+nPos,
						"Info: "
					);
					break;
				case TraceDebug:
					nPos += sprintf
					(
						pBuffer+nPos,
						"Debug: "
					);
					break;
				case TraceDetail:
					nPos += sprintf
					(
						pBuffer+nPos,
						"Debug: "
					);
					break;
				default:
					nPos += sprintf
					(
						pBuffer+nPos,
						"None: "
					);
					break;
			}
            // print the trace message to buffer   
            va_list args;   
            va_start(args, strFormat);   
            nPos += vsprintf(pBuffer+nPos,strFormat,args);   
            va_end(args);   
            // print the end of the line to buffer   
            sprintf(pBuffer+nPos,"\n");   
            // write the buffer to the trace file   
            fputs(pBuffer,hFile);   
        }   
    }   
    catch(...)   
    {   
        // add code to handle exception (if needed)   
    }   
    // release lock   
    theHelper.Unlock();   
}   
