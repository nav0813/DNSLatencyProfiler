/*
 * QueryExecutor.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#include "QueryExecutor.hpp"

namespace DNSProfiler {

QueryExecutor::QueryExecutor(string sDomain, int iRank, DBHandler& oDBHandler): m_iAlexaRank(iRank),
																			   m_sDomain(sDomain),
																			   m_oDBHandler(oDBHandler){
	m_vInitialize();
}

QueryExecutor::~QueryExecutor() {
}

void QueryExecutor::m_vInitialize(){
	m_vGenerateQueryName();
	m_vFetchNameServers();
}

void QueryExecutor::m_vExecute(){
	list<int> lQueryTimes;
	string sNameServer;
	int iQueryTime = 0;
	for(string sFileName : m_aNsFileNames){
		sNameServer = "";iQueryTime = 0;
		sNameServer = Utils::m_sGetNameServerFromNSFile(sFileName);
		time_t tTimestamp = time(NULL);
		m_vResolveQuery(sFileName, iQueryTime);
		QueryResult oQueryResult(m_sDomain,
								 m_iAlexaRank,
								 m_sQueryName,
								 sNameServer,
								 iQueryTime, tTimestamp);
		if(iQueryTime > 0){
			m_oDBHandler.m_bInsert(oQueryResult);
		}
	}
}

void QueryExecutor::m_vGenerateQueryName(){
	m_sQueryName = Utils::m_sGenerateRandomString(
						  Utils::m_iGenerateRandomLength(QUERY_MIN_LENGTH, QUERY_MAX_LENGTH));
	m_sQueryName += m_sDomain;
}

void QueryExecutor::m_vFetchNameServers(){
	ldns_resolver *res = NULL;
		ldns_rdf *name = NULL;
		ldns_pkt *nsResponse = NULL;;
		ldns_rr_list *ns;
		ldns_status s;

		name = ldns_dname_new_frm_str(m_sDomain.c_str());
		if(!name){
			Utils::m_vLogError(string("Invalid domain name"));;

		}

		s = ldns_resolver_new_frm_file(&res, NULL);
		if(s != LDNS_STATUS_OK){
			Utils::m_vLogError(string("Resolver setup failed in getNameservers"));

		}

		nsResponse = ldns_resolver_query(res,
										 name,
										 LDNS_RR_TYPE_NS,
										 LDNS_RR_CLASS_IN,
										 LDNS_RD);
		if(!nsResponse){
			Utils::m_vLogError(string("Response from ldns_resolver_query is null"));
		} else{
			ns = ldns_pkt_rr_list_by_type(nsResponse,
									  LDNS_RR_TYPE_A,
									  LDNS_SECTION_ADDITIONAL);
		}

		if (!ns) {
			ldns_pkt_free(nsResponse);
			ldns_resolver_deep_free(res);
		} else {
			ldns_rr_list_sort(ns);
			for(size_t i = 0; i < ldns_rr_list_rr_count(ns); i++){
				ldns_rr* rec = ldns_rr_list_rr(ns, i);
				ldns_rdf* nsIp = ldns_rr_a_address(rec);
				Utils::m_bWriteSingleLineToFile(m_sGenerateNSFileName(i),
												m_sGenerateNSFileEntry(string (ldns_rdf2str(nsIp))));
				m_aNsFileNames.push_back(m_sGenerateNSFileName(i));
			}
			ldns_rr_list_deep_free(ns);
		}

		ldns_pkt_free(nsResponse);
	    ldns_resolver_deep_free(res);
}

string QueryExecutor::m_sGenerateNSFileName(int iNsFileNum) const {
	string trunDomain(m_sDomain, 0, m_sDomain.find("."));
	string filename("-NS-");
	string prefix ("nameservers//");
	filename = trunDomain + filename;
	std::stringstream ss;
	ss << iNsFileNum;
	filename = prefix + filename + string(ss.str());
	return filename;
}

string QueryExecutor::m_sGenerateNSFileEntry(string sIpAddress) const {
	string sNSFileEntry("nameserver ");
	sNSFileEntry += sIpAddress;
	return sNSFileEntry;
}

void QueryExecutor::m_vResolveQuery(string sNsFilename, int& iQueryTime) const{
	ldns_resolver *res = NULL;
	ldns_rdf *name = NULL;
	ldns_pkt *queryResponse = NULL;
	ldns_rr_list *aRecs;
	ldns_status s;

	name = ldns_dname_new_frm_str(m_sQueryName.c_str());
	if(name){
		s = ldns_resolver_new_frm_file(&res, sNsFilename.c_str());
		if(s != LDNS_STATUS_OK){
			return;
		} else {
			queryResponse = ldns_resolver_query(res,
												name,
												LDNS_RR_TYPE_A,
												LDNS_RR_CLASS_IN,
												LDNS_RD);
			if(!queryResponse){
				return;
			} else{
				aRecs = ldns_pkt_rr_list_by_type(queryResponse,
													LDNS_RR_TYPE_A,
													LDNS_SECTION_ANSWER);

				if(!ldns_pkt_aa(queryResponse) ){
					Utils::m_vLogError(string("Authorized answer not recvd for query: ")
												+ m_sQueryName
												+string(" from NS in file: ")
												+ sNsFilename);
				}

				iQueryTime = (int)ldns_pkt_querytime(queryResponse);

				if(!aRecs){
					//Utils::m_vLogError(string("No A records in answer for query ")
					//							+ m_sQueryName);
				} else{
					//ldns_rr_list_print(stdout, aRecs);
					ldns_rr_list_deep_free(aRecs);
				}
			}
			ldns_pkt_free(queryResponse);
			ldns_resolver_deep_free(res);
		}
	}
}

} /* namespace DNSLatencyProfiler */
