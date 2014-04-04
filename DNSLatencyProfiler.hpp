/*
 * DNSLatencyProfiler.hpp
 *
 *  Created on: Mar 31, 2014
 *      Author: nav
 */

#ifndef DNSLATENCYPROFILER_HPP_
#define DNSLATENCYPROFILER_HPP_

#include "QueryExecutor.hpp"
#include "DBHandler.hpp"
#include "TimeTrigger.hpp"
#include "Utils.hpp"

#include <string>
#include <list>
#include <boost/filesystem.hpp>

using std::string;
using std::list;

namespace DNSProfiler {
/**
 * Main class that performs DNS latency profiling
 */
class DNSLatencyProfiler {
public:
	/**
	 * Ctor
	 * @param sFileName - name of the file that contains the top domains
	 * @param oDBHandler - database handler instance
	 * @param iFreq - frequency at which DNS queries have to be resolved
	 */
	DNSLatencyProfiler(string sFilename, DBHandler& oDBHandler, int iFreq);

	/**
	 * Dtor
	 */
	virtual ~DNSLatencyProfiler();

	/**
	 * Starts the profiling
	 */
	void m_vStartProfiling();

private:
	/**
	 * Name of the file that stores the top domains
	 */
	string m_sDomainsFileName;

	/**
	 * Database handler instance
	 */
	DBHandler& m_oDBHandler;

	/**
	 * Frequency at which profiling has to be done
	 */
	int m_iQueryFreq;

	/**
	 * List of top domains
	 */
	list<string> m_aTopDomains;

	/**
	 * List of executor instances, one per domain
	 */
	list<QueryExecutor*> m_pQueryExecutors;

	/**
	 * Pointer to the trigger instance
	 */
	Trigger* m_pTrigger;

	/**
	 * Performs the required initialization to start profiling
	 */
	void m_vInitialize();

	/**
	 * Does the needed filesystem initialization
	 */
	void m_vFSInitialize() const;

	//Avoiding the compiler give-aways
	DNSLatencyProfiler& operator=(const DNSLatencyProfiler&);
	DNSLatencyProfiler(const DNSLatencyProfiler&);
};

} /* namespace DNSLatencyProfiler */

#endif /* DNSLATENCYPROFILER_HPP_ */
