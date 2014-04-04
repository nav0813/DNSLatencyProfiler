/*
 * TimeTrigger.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#include "TimeTrigger.hpp"

namespace DNSProfiler {

TimeTrigger::TimeTrigger(int iMicroSeconds): Trigger(),
											m_iMicroSeconds(iMicroSeconds) {

}

TimeTrigger::~TimeTrigger() {

}

void TimeTrigger::m_vStartTriggers(){
	cout << "Profiling...";
    while(true){
    	m_vFireExecutors();
    	cout << ".";
    	sleep(m_iMicroSeconds/1000);
    }
}
} /* namespace DNSProfiler */
