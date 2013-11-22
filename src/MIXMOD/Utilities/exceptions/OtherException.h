/** @file XEMOtherException.h
 *  @brief Exception class for Other types of error handling.
 *  @author Parmeet Bhatia
 **/

#ifndef XEM_OTHEREXCEPTION_H
#define XEM_OTHEREXCEPTION_H

#include "mixmod/Utilities/exceptions/Exception.h"

namespace XEM {

class OtherException : public Exception {

public:
	
	OtherException(std::string file, int line, OtherError error) throw ();
	OtherException(OtherError) throw ();
	Exception * clone() throw ();
	virtual const char* what() const throw ();
	virtual bool operator==(const Exception&) const throw ();
	virtual void run(std::ostream & flux = std::cout) const throw ();

	virtual ~OtherException() throw () {
	}


	static std::map<OtherError, const char*> create_map()
	{
		std::map<OtherError, const char*> m;

		m.insert(std::make_pair(badFormat, "Bad Format"));
		m.insert(std::make_pair(nullPointerError, "Internal error (Null pointer)"));
		m.insert(std::make_pair(wrongMatrixType, "Error : trying to apply a method on a wrong matrix type "));
		m.insert(std::make_pair(wrongConstructorType, "Error : when constructing an object by default"));
		m.insert(std::make_pair(nonImplementedMethod, "non implemented method"));
		m.insert(std::make_pair(badBinaryParameterClass, "Internal Mixmod Error: bad XEMBinrayParameter Class"));
		m.insert(std::make_pair(UnknownReason, "Error occurred due to unknown reason."));
		m.insert(std::make_pair(internalMixmodError, "Internal error in mixmod sofware"));
		m.insert(std::make_pair(FunctionNotYetImplemented, "Function that is called is not yet implemented"));

		return m;
	}

	static std::map<OtherError, const char*> mapErrorMsg;

protected:
	
	OtherError _errorType;
};

}

#endif /* XEMOTHEREXCEPTION_H_ */
