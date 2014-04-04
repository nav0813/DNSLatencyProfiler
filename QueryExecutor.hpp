/*
 * QueryExecutor.hpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#ifndef QUERYEXECUTOR_HPP_
#define QUERYEXECUTOR_HPP_

//LDNS specific headers
#include "config.h"
#include <ldns/ldns.h>

#include "Utils.hpp"
#include "QueryResult.hpp"
#include "DBHandler.hpp"
#include "Executor.hpp"

#include <string>
#include <list>
using std::string;
using std::list;

#define QUERY_MIN_LENGTH 5
#define QUERY_MAX_LENGTH 5

namespace DNSProfiler {

/**
 * Concrete Observer class in the Observer Design Pattern
 * Core Class that performs DNS resolution
 */
class QueryExecutor : public Executor{

public:
	/**
	 * Ctor
	 * @param sDomain - domain name that need to be profiled
	 * @param iRank - alexa rank of the domain
	 * @param oDBHandler - database handler which is used to store the results
	 */
	QueryExecutor(string sDomain, int iRank, DBHandler& oDBHandler);

	/**
	 * Dtor
	 */
	virtual ~QueryExecutor();

	/**
	 * Performs the DNS resolution
	 */
	virtual void m_vExecute();

private:
	/**
	 * Member that stores alexa rank
	 */
	int m_iAlexaRank;

	/**
	 * Member that stores Domain name
	 */
	string m_sDomain;

	/**
	 * Member that stores query name
	 */
	string m_sQueryName;

	/**
	 * List of nameserver of the domain
	 */
	list<string> m_aNsFileNames;

	/**
	 * Database handler instance
	 */
	DBHandler& m_oDBHandler;

	/**
	 * Performs required initialization to start DNS query execution
	 */
	void m_vInitialize();

	/**
	 * Generates a random query name
	 */
	void m_vGenerateQueryName();

	/**
	 * Core method that uses LDNS functions to fetch the nameservers of the domain
	 */
	void m_vFetchNameServers();

	/**
	 * Generates nameserver file entry for an IP address
	 * @return - nameserver file entry
	 */
	string m_sGenerateNSFileEntry(string sIp) const;

	/**
	 * Generates nameserver file name
	 * @return - nameserver file name
	 */
	string m_sGenerateNSFileName(int iNum) const;

	/**
	 * Core method that uses LDNS functions to resolve a query name
	 * @param sNsFile - file name of the nameserver file
	 * @param iQueryTime - OUT argument that is stored with query time
	 */

	void m_vResolveQuery(string sNsFile, int& iQueryTime) const;

	//Avoiding the compiler give-aways
	QueryExecutor& operator=(const QueryExecutor&);
	QueryExecutor(const QueryExecutor&);

};

} /* namespace DNSLatencyProfiler */

#endif /* QUERYEXECUTOR_HPP_ */
