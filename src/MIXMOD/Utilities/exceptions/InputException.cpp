#include "mixmod/Utilities/exceptions/InputException.h"

namespace XEM {

// mapping: error type --> message translations
std::map<InputError, const char*> InputException::mapErrorMsg = InputException::create_map();

InputException::InputException(std::string file, int line, InputError error) throw () {
	_errorType = error;
	_filename = file;
	_linenumber = line;
}

InputException::InputException(InputError error) throw () {
	_errorType = error;
	_filename = "Defaulter";
	_linenumber = 0;
}

Exception * InputException::clone() throw () {
	return new InputException(*this);
}

InputException::InputException(const InputException & inputException) : Exception(inputException) {
	_errorType = inputException._errorType;
}

const char* InputException::what() const throw () {
	return (mapErrorMsg.find(_errorType))->second;
}

bool InputException::operator ==(const Exception& other) const throw () {
	if (typeid (*this) != typeid (other)) return false;

	return (*this)._errorType == dynamic_cast<const InputException&> (other)._errorType;
}

void InputException::run(std::ostream & flux) const throw () {
	flux << "In file: " << _filename << " at line: " << _linenumber << "\n";
	flux << "MIXMOD ERROR (Input Error type " << _errorType << ") :" << "\n"; //new line will flush automatically
	flux << (*this).what() << "\n\n";
}

}
