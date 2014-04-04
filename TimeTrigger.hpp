/*
 * TimeTrigger.hpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#ifndef TIMETRIGGER_HPP_
#define TIMETRIGGER_HPP_

#include "Trigger.hpp"
#include <iostream>
#include <unistd.h>

using std::cout;

namespace DNSProfiler {
/**
 * Concrete Subject class in the Observer Design Pattern
 * Class that triggers once every specified interval
 */
class TimeTrigger : public Trigger {
public:
	/**
	 * Ctor
	 * @param iMicroSeconds - interval in microseconds
	 */
	explicit TimeTrigger(int iMicroSeconds);

	/**
	 * Dtor
	 */
	virtual ~TimeTrigger();

	/**
	 * Implements the trigger logic
	 */
	virtual void m_vStartTriggers();

private:
	/**
	 * Trigger interval in microseconds
	 */
	int m_iMicroSeconds;

	//Avoiding the compiler give-aways
	TimeTrigger& operator=(const TimeTrigger&);
	TimeTrigger(const TimeTrigger&);
};

} /* namespace DNSProfiler */

#endif /* TIMETRIGGER_HPP_ */
