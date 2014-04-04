/*
 * DBHandler.hpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#ifndef DBHANDLER_HPP_
#define DBHANDLER_HPP_


#include <mysql++.h>

#include <string>
#include <sstream>
#include <cmath>

#include "QueryResult.hpp"
#include "Utils.hpp"

using std::string;
using std::ostringstream;

namespace DNSProfiler {

/**
 * Core class that iteracts with the database
 */
class DBHandler {
public:
	/**
	 * Ctor
	 */
	DBHandler(string sUser, string sPassword, string sDbName);

	/**
	 * Dtor
	 */
	virtual ~DBHandler();

	/**
	 * Inserts a query result into the database
	 * @param oQueryResult - QueryResult instance
	 * @return  - status
	 */
	bool m_bInsert(const QueryResult& oQueryResult);

	/**
	 * Closes the database connections
	 */
	void m_vCloseDBConnection();

private:
	/**
	 * Stores the user name
	 */
	string m_sUser;

	/**
	 * Stores the password
	 */
	string m_sPasswd;

	/**
	 * Stores the database name
	 */
	string m_sDatabase;

	/**
	 * Stores the instance of connection instance
	 */
	mysqlpp::Connection m_oConn;

	/**
	 * Stores the database connection status
	 */
	bool m_bDbConnStat;

	/**
	 * Stores the status of database creation
	 */
	bool m_bDbCreateStat;

	/**
	 * Performs the necessary initialization
	 */
	void m_vInitialize();

	/**
	 * Checks if the database exists
	 * @return truth value
	 */
	bool m_bDbExists();

	/**
	 * Creates the database and selects it
	 * @return status
	 */
	bool m_bCreateNSelectDb();

	/**
	 * Creates the TimeSeries table
	 * @return status
	 */
	bool m_bCreateTSTable();

	/**
	 * Creates the Aggregates table
	 * @return status
	 */
	bool m_bCreateAggTable();

	/**
	 * Inserts the query result instance to TimeSeries table
	 * @param oQueryResult - QueryResult instance
	 * @return status
	 */
	bool m_bInsert2TimeSeries(const QueryResult& oQueryResult);

	/**
	 * Inserts the query result instance to Aggregates table
	 * @param oQueryResult - QueryResult instance
	 * @return status
	 */
	bool m_bInsert2Aggregates(const QueryResult& oQueryResult);

	/**
	 * Calculates the standard deviation of query times
	 * @param sDomain - domain name
	 * @param iAvg - average query time
	 * @param iQueries - number of queries made
	 * @return Standard deviation value
	 */
	int m_iCalculateSD(string sDomain, int iAvg, int iQueries);

	//Avoiding the compiler give-aways
	DBHandler& operator=(const DBHandler&);
	DBHandler(const DBHandler&);
};

} /* namespace DNSProfiler */

#endif /* DBHANDLER_HPP_ */
