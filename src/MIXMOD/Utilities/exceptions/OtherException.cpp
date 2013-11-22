#include "mixmod/Utilities/exceptions/OtherException.h"

namespace XEM {

// mapping: error type --> message translations
std::map<OtherError, const char*> OtherException::mapErrorMsg = OtherException::create_map();

OtherException::OtherException(std::string file, int line, OtherError error) throw () {
	_errorType = error;
	_filename = file;
	_linenumber = line;
}

OtherException::OtherException(OtherError error) throw () {
	_errorType = error;
	_filename = "Defaulter";
	_linenumber = 0;
}

Exception * OtherException::clone() throw () {
	return new OtherException(*this);
}

const char* OtherException::what() const throw () {
	return (mapErrorMsg.find(_errorType))->second;
}

bool OtherException::operator ==(const Exception& other) const throw () {
	if (typeid (*this) != typeid (other)) return false;

	return (*this)._errorType == dynamic_cast<const OtherException&> (other)._errorType;
}

void OtherException::run(std::ostream & flux) const throw () {
	flux << "In file: " << _filename << " at line: " << _linenumber << "\n";
	flux << "MIXMOD ERROR (Other Error type " << _errorType << ") :" << "\n"; //new line will flush automatically
	flux << (*this).what() << "\n\n";
}

}
