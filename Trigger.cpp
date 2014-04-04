/*
 * Trigger.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#include "Trigger.hpp"

namespace DNSProfiler {

Trigger::Trigger(){

}

Trigger::~Trigger() {
	for(Executor* pExecutor : m_pExecutors){
		delete pExecutor;
	}
}

void Trigger::m_vAttachExecutor(Executor* pExecutor){
	m_pExecutors.push_back(pExecutor);
}

void Trigger::m_vFireExecutors(){
	for(Executor* pExecutor : m_pExecutors){
		pExecutor->m_vExecute();
	}
}
} /* namespace DNSProfiler */
