/*
 * Utils.hpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <list>
#include <boost/program_options.hpp>

using std::string;
using std::stringstream;
using std::list;
using std::ifstream;
using std::ofstream;
using std::cout;

#define COOL_PRIME_NUMBER 7
#define ASCII_A_START 97
#define NUM_ALPHABETS 26
#define ASCII_NUM_START 48
#define NUM_NUMERALS 10


namespace DNSProfiler {
/**
 * Static class that provides utility methods for other classes
 */
class Utils {
public:
	/**
	 * Parses the program command line options
	 * @param iArgc - number of options
	 * @param pArgv - options array
	 * @param sUname - OUT argument into which the parsed username has to be copied
	 * @param sPasswd - OUT argument into which the parsed password has to be copied
	 * @param sDBName - OUT argument into which the parsed database has to be copied
	 * @param sDomainFileName - OUT argument into which the parsed filename has to be copied
	 * @param iFreq - OUT argument into which the parsed frequency has to be copied
	 */
	static void vParseOptions(int iArgc, char* pArgv[],
							  string& sUname,
							  string& sPasswd,
							  string& sDBName,
							  string& sDomainFileName,
							  int& iFreq);

	/**
	 * Generates a random string of specified length
	 * @param iLength - length of the randomstring
	 * @return - random string
	 */
	static string m_sGenerateRandomString(int iLength);

	/**
	 * Generates a random number within the specified close interval
	 * @param iMin - lower limit of the closed interval
	 * @param iMax - higher limit of the closed interval
	 * @return - random number
	 */
	static int m_iGenerateRandomLength(int iMin, int iMax);

	/**
	 * Generates a random number less than the specified number
	 * @param iMax - generated random number is modulo iMax
	 * @return - random number
	 */
	static int m_iGenerateRandomNumber(int iMax);

	/**
	 * Reads the file for the top domains
	 * @param sFileName - name of the file
	 * @return - list of strings that are the top domains
	 */
	static list<string> m_aGetTopDomains(string sFilename);

	/**
	 * Writes a single line to a file
	 * @param sFilename - name of the file
	 * @param sLine - line to be written
	 * @return - write status
	 */
	static bool m_bWriteSingleLineToFile(string sFilename, string sLine);

	/**
	 * Fetches the nameserver from a nameserver file
	 * @param sFilename - name of the file
	 * @return - nameserver
	 */
	static string m_sGetNameServerFromNSFile(string sFilename);

	/**
	 * Logs a error record into the log file
	 * @param sError - error to be logged
	 */
	static void m_vLogError(string sError);

	/**
	 * Creates a log file name appended with timestamp
	 * @return - log file name
	 */
	static string m_sCreateLogFileName();

	/**
	 * Fetches the log file name
	 * @return - log file name
	 */
	static string m_sGetLogFileName() {return m_sLogFileName;}

private:
	/**
	 * Stores the log file name
	 */
	static string m_sLogFileName;

	/**
	 * Ctor is private, as this class is static
	 */
	Utils();

	/**
	 * Dtor
	 */
	virtual ~Utils();

	//Avoiding the compiler give-aways
	Utils& operator=(const Utils&);
	Utils(const Utils&);

};

} /* namespace DNSLatencyProfiler */

#endif /* UTILS_HPP_ */
