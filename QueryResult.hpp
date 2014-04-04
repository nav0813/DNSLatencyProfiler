/*
 * QueryResult.hpp
 *
 *  Created on: Apr 1, 2014
 *      Author: nav
 */

#ifndef QUERYRESULT_HPP_
#define QUERYRESULT_HPP_

#include <string>
using std::string;

namespace DNSProfiler {

/**
 * Class for encapsulating the result of a DNS query resolution
 */
class QueryResult {
public:
	/**
	 * Ctor
	 * @param sDomain - domain name
	 * @param iRank - alexa rank
	 * @param sQuery - query that is resolved
	 * @param sNSIp - IP address of the nameserver
	 * @param iQueryTime - query time
	 * @param iTimestamp - timestamp of the resolution request
	 */
	QueryResult(string sDomain, int iRank, string sQuery, string sNSIp,
								int iQueryTime, int iTimestamp):m_sDomain(sDomain),
																m_iAlexaRank(iRank),
								   			 	 	 	 	    m_sQuery(sQuery),
								   			 	 	 	 	    m_sNSIp(sNSIp),
								   			 	 	 	 	    m_iQueryTime(iQueryTime),
								   			 	 	 	 	    m_iTimestamp(iTimestamp){}

	/**
	 * Dtor
	 */
	virtual ~QueryResult(){}

	/**
	 * Gets the domain
	 * @return - domain name
	 */
	string m_sGetDomain() const {return m_sDomain;}

	/**
	 * Gets the alexa rank
	 * @return - alexa rank
	 */
	int m_iGetAlexaRank() const {return m_iAlexaRank;}

	/**
	 * Gets the query name
	 * @return - query name
	 */
	string m_sQueryName() const {return m_sQuery;}

	/**
	 * Gets the name server
	 * @return - nameserver
	 */
	string m_sNameServer() const {return m_sNSIp;}

	/**
	 * Gets the query time
	 * @return - query time
	 */
	int m_iGetQueryTime() const {return m_iQueryTime;}

	/**
	 * Gets the timestamp of the resolution
	 * @return - timestamp
	 */
	int m_iGetTimestamp() const {return m_iTimestamp;}

private:
	/**
	 * Member that stores Domain name
	 */
	string m_sDomain;

	/**
	 * Member that stores alexa rank
	 */
	int m_iAlexaRank;

	/**
	 * Member that stores query name
	 */
	string m_sQuery;

	/**
	 * Member that stores IP of the nameserver
	 */
	string m_sNSIp;

	/**
	 * Member that stores query time
	 */
	int m_iQueryTime;

	/**
	 * Member that stores timestamp
	 */
	int m_iTimestamp;

	//Avoiding the compiler give-aways
	QueryResult& operator=(const QueryResult&);
	QueryResult(const QueryResult&);
};

} /* namespace DNSProfiler */

#endif /* QUERYRESULT_HPP_ */
