/*
 * Trigger.hpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#ifndef TRIGGER_HPP_
#define TRIGGER_HPP_

#include "Executor.hpp"

#include <list>
using std::list;

namespace DNSProfiler {

/**
 * Abstract subject class for the Observer Design Pattern
 */
class Trigger {
public:
	/**
	 * Ctor
	 */
	Trigger();

	/**
	 * Dtor
	 */
	virtual ~Trigger();

	/**
	 * Adds an executor to the list of executors
	 * @param pExecutor - pointer to an executor
	 */
	void m_vAttachExecutor(Executor* pExecutor);

	/**
	 * Invokes the executors in the list to execute something
	 */
	void m_vFireExecutors();

	/**
	 * Abstract method that need to be implemented by the
	 * derived class. It encapsulates the events on which executors
	 * have to be invoked.
	 */
	virtual void m_vStartTriggers() = 0;

private:
	/**
	 * List of pointers to executors instances
	 */
	list<Executor*> m_pExecutors;

	//Avoiding the compiler give-aways
	Trigger& operator=(const Trigger&);
	Trigger(const Trigger&);
};

} /* namespace DNSProfiler */

#endif /* TRIGGER_HPP_ */
