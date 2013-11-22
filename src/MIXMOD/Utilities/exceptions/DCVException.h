/** @file XEMDCVException.h
 *  @brief Exception class for DCV errors.
 *  @author Parmeet Bhatia
 **/

#ifndef XEM_DCVEXCEPTION_H
#define XEM_DCVEXCEPTION_H

#include "mixmod/Utilities/exceptions/Exception.h"

namespace XEM {

class DCVException : public Exception {

public:
	
	DCVException(std::string, int, DCVError) throw ();
	Exception * clone() throw ();
	DCVException(DCVError) throw ();
	virtual const char* what() const throw ();
	virtual bool operator==(const Exception&) const throw ();
	virtual void run(std::ostream & flux = std::cout) const throw ();

	virtual ~DCVException() throw () {
	}

  static std::map<DCVError, const char*> create_map()
  {
    std::map<DCVError, const char*> m;
    m.insert(std::make_pair(wrongDCVinitBlocks, "DCV error : wrong init block specification, must be either RANDOM or DIAG"));
    m.insert(std::make_pair(wrongDCVnumberOfBlocks, "DCV error : wrong number of blocks, must be between 2 and the number of samples"));
    m.insert(std::make_pair(DCVmustBeDIAG, "DCV error : in this situation DCV init block specification must be DIAG"));
    m.insert(std::make_pair(forbiddenCallToGetBestCVModel, "DCV error : call to getBestCVModel is forbidden in the current context"));

    return m;
  }

	static std::map<DCVError, const char*> mapErrorMsg;

protected:
	
	DCVError _errorType;
};

}

#endif /* XEMDCVEXCEPTION_H_ */
