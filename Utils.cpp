/*
 * Utils.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#include "Utils.hpp"

namespace DNSProfiler {

string Utils::m_sLogFileName = string("");

Utils::Utils() {
	//empty for a static class
}

Utils::~Utils() {
	//empty for a static class
}

void Utils::vParseOptions(int iArgc, char* pArgv[],
							string& sUname,
							string& sPasswd,
							string& sDBName,
							string& sFileName,
							int& iFreq){

	namespace po = boost::program_options;
	po::options_description oDesc("CmdLineOptions");
	oDesc.add_options()
			("user", po::value<string>(), "User name for MySQL")
			("passwd", po::value<string>(), "Password for MySQL")
			("database", po::value<string>(), "Name of the database in MySQL")
			("file", po::value<string>(), "File name that contains the top domains")
			("freq", po::value<int>(), "Time interval between queries to nameservers")
			("help", "produce help message");
	po::variables_map oVarMap;
	po::store(po::parse_command_line(iArgc, pArgv, oDesc), oVarMap);
	po::notify(oVarMap);

	if(oVarMap.count("help")){
		cout << oDesc << "\n";
		return;
	}

	if(oVarMap.count("user")){
		sUname = oVarMap["user"].as<string>();
	}

	if(oVarMap.count("passwd")){
		sPasswd = oVarMap["passwd"].as<string>();
	}
	if(oVarMap.count("database")){
		sDBName = oVarMap["database"].as<string>();
	}
	if(oVarMap.count("file")){
		sFileName = oVarMap["file"].as<string>();
	}
	if(oVarMap.count("freq")){
		iFreq = oVarMap["freq"].as<int>();
	}
}

string Utils::m_sGenerateRandomString(int length){
	static time_t now = time(NULL);
	stringstream ss;
	now++;
	srand(now);

	for(int i=0; i<length; i++){
		if((rand() % COOL_PRIME_NUMBER )){
			ss << char(ASCII_A_START + m_iGenerateRandomNumber(NUM_ALPHABETS));
		} else {
			ss << char(ASCII_NUM_START + m_iGenerateRandomNumber(NUM_NUMERALS));
		}
	}
	ss << '.';
	return ss.str();
}

int Utils::m_iGenerateRandomLength(int min, int max){
	return m_iGenerateRandomNumber(max-min+1) + min;
}

int Utils::m_iGenerateRandomNumber(int max){
	static time_t now = time(NULL);
	now++;
	srand(now);
	return (rand() % max);
}

list<string> Utils::m_aGetTopDomains(string sFilename){
	ifstream file(sFilename.c_str());
	list<string> topDomains;
	string line;
	if(file.is_open()){
		while(getline(file, line)){
			topDomains.push_back(line);
		}
	} else {
		m_vLogError(string("Open top domains file failed!!!"));
		exit(EXIT_FAILURE);
	}
	return topDomains;
}

bool Utils::m_bWriteSingleLineToFile(string sFilename, string sLine){
	ofstream outfile;
	outfile.open(sFilename.c_str());
	if(!outfile.is_open()){
		//log err here
		return false;
	}
	outfile << sLine << "\n";
	outfile.close();
	return true;
}

string Utils::m_sGetNameServerFromNSFile(string sFileName){
	string sNameServer;
	ifstream fFile(sFileName.c_str());
	if(fFile.is_open()){
		getline(fFile, sNameServer);
		sNameServer =sNameServer.substr(sNameServer.find(" ") + 1);
	}
	return sNameServer;
}

string Utils::m_sCreateLogFileName(){
	std::ostringstream sLogFileName;
	sLogFileName << "ErrorLog-" << time(NULL);
	m_sLogFileName = sLogFileName.str();
	return sLogFileName.str();
}

void Utils::m_vLogError(string sError){
	std::ofstream outfile;
	outfile.open(m_sGetLogFileName().c_str(), std::ios::out | std::ios::app);
	if(outfile.is_open()){
		outfile << "[ "<< time(NULL) << " ] " << sError <<"\n";
	}
}

} /* namespace DNSLatencyProfiler */
