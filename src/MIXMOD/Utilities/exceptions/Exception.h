/** @file XEMException.h
 *  @brief Base class for MIXMOD Exceptions. All XEMException classes must inherit this class.
 *  @author Parmeet Bhatia
 **/

#ifndef XEM_EXCEPTION_H
#define XEM_EXCEPTION_H

#include <exception>
#include <string.h>
#include <map>
#include <iostream>
#include <typeinfo>
#include "mixmod/Utilities/exceptions/ErrorEnumerations.h"

namespace XEM {

class Exception : public std::exception {

public:

	Exception() {
	}
	Exception(const std::string& what_arg) throw ();
	Exception(const std::string& msg, const std::string& file, int line) throw ();
	Exception(const Exception & exception);
	virtual Exception* clone() throw ();

	/** Same as standard what() function.*/
	virtual const char* what() const throw ();

	/**Getter function for error.*/
	virtual Exception const& getError() const throw () {
		return *this;
	}
	/** Equality check : Return true if both the Exception class as well as it's ErrorType_ is same.*/
	virtual bool operator==(const Exception&) const throw ();

	/**Inequality check: Return true if equality check fails.*/
	virtual bool operator!=(const Exception& other) const throw () {
		return !(*this == other);
	}
	/**Interface for runner method. It will print the error and its content to the stream passed as
	 * argument.*/
	virtual void run(std::ostream & flux = std::cout) const throw ();

	virtual ~Exception() throw () {
	}

protected:
	
	std::string _errorMsg;
	std::string _filename;
	int _linenumber;
};

}

#endif /* XEMEXCEPTION_H_ */
