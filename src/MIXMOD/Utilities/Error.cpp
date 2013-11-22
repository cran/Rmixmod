#include "mixmod/Utilities/Error.h"

namespace XEM {

//-----------
//Constructor
//-----------
Error::Error() : _errorType(NOERROR.clone()) {
}

//-----------
//Constructor
//-----------
Error::Error(Exception& errorType) : _errorType((((errorType)).clone())) {
}

//----------
//Destructor
//----------
Error::~Error() {
	if (_errorType) delete _errorType;
}

//---
//Run
//---
void Error::run() {
	((Exception*) _errorType)->run();
}

}
