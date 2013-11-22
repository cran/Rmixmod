/**
  @file Error.h
  @brief Base class for Error(s)
  @author F Langrognet
 */

#ifndef XEMERROR_H
#define XEMERROR_H

#include "mixmod/Utilities/Util.h"

namespace XEM {

/**
  @brief Base class for Error(s)
  @author F Langrognet
 */
class Error {

public:

	/// Default constructor
	Error();

	/// Constructor
	Error(Exception & errorType);

	/// Destructor
	~Error();

	// setter
	inline void setError(Exception & errorType) {
		delete _errorType;
		_errorType = errorType.clone();
	}

	// getter
	inline Exception& getError() const {
		return *_errorType;
	}

	/// Run method (for debug)
	void run();

private:

	/// Type of error	
	Exception * _errorType;
};

}

#endif
