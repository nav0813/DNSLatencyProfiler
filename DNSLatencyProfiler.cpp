/*
 * DNSLatencyProfiler.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#include "DNSLatencyProfiler.hpp"

namespace DNSProfiler {

DNSLatencyProfiler::DNSLatencyProfiler(string sFilename, DBHandler& oDBHandler, int iQueryFreq):m_sDomainsFileName(sFilename),
														 	 	 	 	 	   m_oDBHandler(oDBHandler),
														 	 	 	 	 	   m_iQueryFreq(iQueryFreq),
														 	 	 	 	 	   m_pTrigger(new TimeTrigger(iQueryFreq)){
	m_vInitialize();
}

DNSLatencyProfiler::~DNSLatencyProfiler() {
	m_oDBHandler.m_vCloseDBConnection();
	delete m_pTrigger;
	for(QueryExecutor* pQueryExecutor : m_pQueryExecutors){
		delete pQueryExecutor;
	}
}

void DNSLatencyProfiler::m_vInitialize(){
	list<string> aTopDomains = Utils::m_aGetTopDomains(m_sDomainsFileName);
	m_vFSInitialize();
	int i = 1;
	for(string domain : aTopDomains){
		m_pQueryExecutors.push_back(new QueryExecutor(domain, i, m_oDBHandler));
		i++;
	}
	for(QueryExecutor* pQueryExecutor : m_pQueryExecutors){
		m_pTrigger->m_vAttachExecutor(pQueryExecutor);
	}
}

void DNSLatencyProfiler::m_vFSInitialize() const {
	boost::filesystem::path dir("nameservers//");
	try{
		if(!boost::filesystem::is_directory(dir)) {
			if(!boost::filesystem::create_directory(dir)){
				//log err here
				Utils::m_vLogError(string("Create folder fail"));
				exit(EXIT_FAILURE);
			}
		} else {
			boost::filesystem::remove_all(dir);
			if(!boost::filesystem::create_directory(dir)){
				//log err here
				Utils::m_vLogError(string("Clean and create folder fail"));
				exit(EXIT_FAILURE);
			}
		}
	} catch(boost::filesystem::filesystem_error& err){
		Utils::m_vLogError(string("FS exception: ") + string(err.what()));
		exit(EXIT_FAILURE);
	}

}

void DNSLatencyProfiler::m_vStartProfiling(){
	m_pTrigger->m_vStartTriggers();
}

} /* namespace DNSLatencyProfiler */
