#ifndef UNUSEDCOLUMNDESCRIPTION_H_
#define UNUSEDCOLUMNDESCRIPTION_H_

#include "mixmod/Kernel/IO/ColumnDescription.h"

namespace XEM {

class UnusedColumnDescription : public ColumnDescription {

public:

	UnusedColumnDescription(int64_t index);

	std::string editType();

	ColumnDescription* clone() const;
};

}

#endif /* UNUSEDCOLUMNDESCRIPTION_H_ */
