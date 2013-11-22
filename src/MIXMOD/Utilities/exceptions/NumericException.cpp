#include "mixmod/Utilities/exceptions/NumericException.h"

namespace XEM {

// mapping: error type --> message translations
std::map<NumericError, const char*> NumericException::mapErrorMsg = NumericException::create_map();

NumericException::NumericException(std::string file, int line, NumericError error) throw () {
	_errorType = error;
	_filename = file;
	_linenumber = line;
}

NumericException::NumericException(NumericError error) throw () {
	_errorType = error;
	_filename = "Defaulter";
	_linenumber = 0;
}

Exception * NumericException::clone() throw () {
	return new NumericException(*this);
}

const char* NumericException::what() const throw () {
	return (mapErrorMsg.find(_errorType))->second;
}

bool NumericException::operator ==(const Exception& other) const throw () {
	if (typeid (*this) != typeid (other)) return false;

	return (*this)._errorType == dynamic_cast<const NumericException&> (other)._errorType;
}

void NumericException::run(std::ostream & flux) const throw () {
	flux << "In file: " << _filename << " at line: " << _linenumber << "\n";
	flux << "MIXMOD ERROR (Numeric Error type " << _errorType << ") :" << "\n"; //new line will flush automatically
	flux << (*this).what() << "\n\n";
}

}
