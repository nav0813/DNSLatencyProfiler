
#include <iostream>
#include <sstream>

#include "Utils.hpp"
#include "DNSLatencyProfiler.hpp"
#include "DBHandler.hpp"

using std::cout;
using std::ostringstream;
using DNSProfiler::Utils;



int main(int argc, char* argv[]){

	string sUname, sPasswd, sDBName, sDomainsFile;
	int iFreq;
	Utils::vParseOptions(argc, argv, sUname, sPasswd, sDBName, sDomainsFile, iFreq);
	Utils::m_sCreateLogFileName();

	DNSProfiler::DBHandler oDBHandler(sUname,
									  sPasswd,
									  sDBName);

	DNSProfiler::DNSLatencyProfiler oDNSLatencyProfiler(sDomainsFile, oDBHandler, iFreq);

	oDNSLatencyProfiler.m_vStartProfiling();
}
