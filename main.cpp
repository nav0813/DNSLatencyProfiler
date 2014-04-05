//standard library includes
#include <string>

//Core-classes includes
#include "Utils.hpp"
#include "DNSLatencyProfiler.hpp"
#include "DBHandler.hpp"


int main(int argc, char* argv[]){

	// local variables to store the parsed program options
	string sUname, sPasswd, sDBName, sDomainsFile;
	int iFreq;

	//parse program options
	DNSProfiler::Utils::vParseOptions(argc, argv, sUname, sPasswd, sDBName, sDomainsFile, iFreq);

	//create file for logging
	DNSProfiler::Utils::m_sCreateLogFileName();

	//create DB handler
	DNSProfiler::DBHandler oDBHandler(sUname,
									  sPasswd,
									  sDBName);

	//instantiate the core profiling class
	DNSProfiler::DNSLatencyProfiler oDNSLatencyProfiler(sDomainsFile, oDBHandler, iFreq);

	//start profiling
	oDNSLatencyProfiler.m_vStartProfiling();
}
