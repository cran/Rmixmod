#include "mixmod/Utilities/exceptions/Exception.h"

namespace XEM {

Exception::Exception(const std::string& msg, const std::string& file, int line) throw () {
	_errorMsg = msg;
	_filename = file;
	_linenumber = line;
}

Exception::Exception(const std::string& msg) throw () {
	_errorMsg = msg;
	_filename = "Defaulter";
	_linenumber = 0;
}

Exception::Exception(const Exception & exception) {
	_errorMsg = exception._errorMsg;
	_filename = exception._filename;
	_linenumber = exception._linenumber;
}

const char* Exception::what() const throw () {
	return _errorMsg.c_str();
}

Exception * Exception::clone() throw () {
	return new Exception(*this);
}

bool Exception::operator ==(const Exception& other) const throw () {
	if (typeid (*this) != typeid (other)) return false;
	if (strcmp(((*this)._errorMsg).c_str(), (other._errorMsg).c_str()) == 0)
		return true;
	else
		return false;
}

void Exception::run(std::ostream & flux) const throw () {
	flux << (*this).what() << "\n\n";
}

}
