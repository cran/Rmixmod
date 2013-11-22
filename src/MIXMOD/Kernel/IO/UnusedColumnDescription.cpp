#include "mixmod/Kernel/IO/UnusedColumnDescription.h"

namespace XEM {

UnusedColumnDescription::UnusedColumnDescription(int64_t index)
: ColumnDescription(index)
{ }

std::string UnusedColumnDescription::editType()
{
	return "Unused";
}

ColumnDescription* UnusedColumnDescription::clone() const
{
	return new UnusedColumnDescription(_index);
}

}
