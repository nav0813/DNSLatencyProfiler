/*
 * Executor.hpp
 *
 *  Created on: Apr 3, 2014
 *      Author: nav
 */

#ifndef EXECUTOR_HPP_
#define EXECUTOR_HPP_

namespace DNSProfiler {

/**
 * Abstract observer class for the Observer Design Pattern
 */
class Executor {

public:
	/**
	 * Ctor
	 */
	Executor();

	/**
	 * Dtor
	 */
	virtual ~Executor();

	/**
	 * Abstract method to be implemented by derived classes to effect some executions
	 */
	virtual void m_vExecute() = 0;

private:
	//Avoiding the compiler give-aways
	Executor& operator=(const Executor&);
	Executor(const Executor&);
};

} /* namespace DNSLatencyProfiler */

#endif /* EXECUTOR_HPP_ */
