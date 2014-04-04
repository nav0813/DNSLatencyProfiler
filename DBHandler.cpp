/*
 * DBHandler.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#include "DBHandler.hpp"

namespace DNSProfiler {

DBHandler::DBHandler(string sUser, string sPassword, string sDbName): m_sUser(sUser),
																	  m_sPasswd(sPassword),
																	  m_sDatabase(sDbName),
																	  m_oConn(false),
																	  m_bDbConnStat(false){
	m_vInitialize();
}

void DBHandler::m_vInitialize(){
	m_bDbConnStat = m_oConn.connect("", "localhost", m_sUser.c_str(),
										m_sPasswd.c_str());
	if(!m_bDbConnStat){
		Utils::m_vLogError(string("Database connection creation failed"));
		exit(EXIT_FAILURE);
	}
	if(!m_bDbExists()){
		if(m_bCreateNSelectDb()){
			if(!(m_bCreateTSTable() && m_bCreateAggTable())){
				Utils::m_vLogError(string("Create TimeSeries / Aggregate table failed"));
				exit(EXIT_FAILURE);
			}
		}
	}
}

DBHandler::~DBHandler() {
	m_vCloseDBConnection();
}

void DBHandler::m_vCloseDBConnection(){
	if(m_bDbConnStat){
		m_oConn.disconnect();
	}
}

bool DBHandler::m_bDbExists(){
	if(!m_bDbConnStat){
		Utils::m_vLogError(string("Database not connected"));
		//exit(EXIT_FAILURE);
		return false;
	}
	if(!m_oConn.select_db(m_sDatabase)){
		Utils::m_vLogError(string("Use database query failed: "));
		return false;
	}
	return true;
}

bool DBHandler::m_bCreateNSelectDb(){
	if(!m_bDbConnStat){
		Utils::m_vLogError(string("Database not connected"));
		exit(EXIT_FAILURE);
		return false;
	}
	ostringstream sQuery;
	sQuery <<"CREATE DATABASE " << m_sDatabase << ";";

	//create database
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	oQuery.execute();
	if(strlen(oQuery.error()) != 0 ){
		Utils::m_vLogError(string("Create database failed"));
		exit(EXIT_FAILURE);
		return false;
	}

	if(!m_oConn.select_db(m_sDatabase)){
		Utils::m_vLogError(string("Use database query failed: " ) + string(oQuery.error()));
		return false;
		exit(EXIT_FAILURE);
	}
	return true;
}

bool DBHandler::m_bCreateTSTable(){
	if(!m_bDbConnStat){
		Utils::m_vLogError(string("Database not connected"));
		exit(EXIT_FAILURE);
		return false;
	}

	//build the create table query
	ostringstream sQuery;
	sQuery << "CREATE TABLE TimeSeries(";
	sQuery << "Domain VARCHAR(20) NOT NULL,";
	sQuery << "Query VARCHAR(30) NOT NULL,";
	sQuery << "NameServer VARCHAR(16) NOT NULL,";
	sQuery << "Timestamp INT NOT NULL,";
	sQuery << "QueryTime INT NULL)";
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	oQuery.execute();
	if(strlen(oQuery.error()) != 0 ){
		Utils::m_vLogError(string("Create TimeSeries table failed: " ) + string(oQuery.error()));
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

bool DBHandler::m_bCreateAggTable(){
	if(!m_bDbConnStat){
		Utils::m_vLogError(string("Database not connected"));
		return false;
	}

	//build the create table query
	ostringstream sQuery;
	sQuery << "CREATE TABLE Aggregates(";
	sQuery << "AlexaRank INT NOT NULL,";
	sQuery << "Domain VARCHAR(20) NOT NULL,";
	sQuery << "Queries INT,";
	sQuery << "AvgQueryTime INT NOT NULL,";
	sQuery << "StdDeviation INT NOT NULL,";
	sQuery << "FirstQuery INT NOT NULL,";
	sQuery << "LastQuery INT NOT NULL,";
	sQuery << "PRIMARY KEY(AlexaRank))";

	//execute the query
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	oQuery.execute();
	if(strlen(oQuery.error()) != 0 ){
		Utils::m_vLogError(string("Create Aggregates table failed: " ) + string(oQuery.error()));
		exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

bool DBHandler::m_bInsert(const QueryResult& oVal){
	return m_bInsert2TimeSeries(oVal) && m_bInsert2Aggregates(oVal);
}

bool DBHandler::m_bInsert2TimeSeries(const QueryResult& oVal){
	ostringstream sQuery;

	//build the insert query
	sQuery.str(string(""));
	sQuery << "INSERT INTO TimeSeries";
	sQuery << "(Domain, Query, NameServer, QueryTime, Timestamp) VALUES ( ";
	sQuery << "'" << oVal.m_sGetDomain() << "',";
	sQuery << "'" << oVal.m_sQueryName() << "',";
	sQuery << "'" << oVal.m_sNameServer() << "',";
	sQuery << oVal.m_iGetQueryTime() << ",";
	sQuery << oVal.m_iGetTimestamp() << ");";

	//execute the query
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	oQuery.execute();
	if(strlen(oQuery.error()) != 0 ){
		Utils::m_vLogError(string("Inserting into TimeSeries table failed: " ) + string(oQuery.error()));
		return false;
	}
	return true;
}

bool DBHandler::m_bInsert2Aggregates(const QueryResult& oVal){

	ostringstream sQuery;
	sQuery << "SELECT * FROM Aggregates WHERE Domain='" << oVal.m_sGetDomain() <<"';";
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	mysqlpp::StoreQueryResult oRes = oQuery.store();

	// Check if Aggregates table has a tuple with the domain name
	if(!oRes.empty()){
		int iAvgQueryTime = oRes[0]["AvgQueryTime"];
		int iQueries = oRes[0]["Queries"];
		int iNewAvg = ((iAvgQueryTime * iQueries) + oVal.m_iGetQueryTime()) / (iQueries + 1);

		//build the insert query
		sQuery.str(string(""));
		sQuery << "UPDATE Aggregates SET ";
		sQuery << "AvgQueryTime = " << iNewAvg << ", ";
		sQuery << "Queries = " << (iQueries + 1) << ", ";
		sQuery << "StdDeviation = " << m_iCalculateSD(oVal.m_sGetDomain(), iNewAvg, iQueries + 1) << ", ";
		sQuery << "LastQuery = " << oVal.m_iGetTimestamp();
		sQuery <<" WHERE Domain = '" << oVal.m_sGetDomain() << "'";

		//execute the query
		mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
		oQuery.execute();
		if(strlen(oQuery.error()) != 0 ){
			Utils::m_vLogError(string("Updating Aggregates table failed: " ) + string(oQuery.error()));
			return false;
		}
	}else{ // Aggregates table does not have an tuple with the domain name
		//build the insert query
		sQuery.str(string(""));
		sQuery << "INSERT INTO Aggregates";
		sQuery << "(Domain, AlexaRank, AvgQueryTime, Queries, StdDeviation, FirstQuery, LastQuery) VALUES ( ";
		sQuery << "'" << oVal.m_sGetDomain() << "',";
		sQuery << oVal.m_iGetAlexaRank() << ",";
		sQuery << oVal.m_iGetQueryTime() << ",";
		sQuery << 1 << ",";
		sQuery << 0 << ",";
		sQuery << oVal.m_iGetTimestamp() << ",";
		sQuery << oVal.m_iGetTimestamp() << ");";

		//execute the query
		oQuery = m_oConn.query(sQuery.str());
		oQuery.execute();
		if(strlen(oQuery.error()) != 0 ){
			Utils::m_vLogError(string("Inserting new tuples to Aggregates table failed: " ) + string(oQuery.error()));
			return false;
		}
	}
	return true;
}

int DBHandler::m_iCalculateSD(string sDomain, int iAvg, int iQueries){
	int iSD = 0;
	ostringstream sQuery;
	sQuery << "SELECT QueryTime FROM TimeSeries WHERE Domain='" << sDomain <<"';";
	mysqlpp::Query oQuery = m_oConn.query(sQuery.str());
	mysqlpp::StoreQueryResult oRes = oQuery.store();

	if(! oRes.empty()){
		for(int i = 0; i < (int)oRes.size(); i++){
			iSD += (abs(iAvg - oRes[i]["QueryTime"]))*(abs(iAvg - oRes[i]["QueryTime"]));
		}
	}
	iSD /= iQueries;
	return (int)sqrt(iSD);
}

} /* namespace DNSProfiler */
