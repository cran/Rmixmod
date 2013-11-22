#ifndef XEMCOMPOSITEDATA_H_
#define XEMCOMPOSITEDATA_H_
/**@file XEMCompositeData.h
 * @brief Composite data class for heterogeneous clustering.
 * @author Parmeet Bhatia
 */
#include "mixmod/Kernel/IO/Data.h"
#include "mixmod/Kernel/IO/GaussianData.h"
#include "mixmod/Kernel/IO/BinaryData.h"

namespace XEM {

class CompositeData : public Data {

public:
	
	//default constructor
	CompositeData();
	//copy constructor
	CompositeData(const CompositeData*);
	//Initialization constructor
	CompositeData(Data*, Data*);
	virtual CompositeData * clone() const;

	virtual void input(std::ifstream&) {
		THROW(OtherException, FunctionNotYetImplemented);
	}

	virtual void input(const DataDescription&);

	virtual void output(std::ostream&) {
		THROW(OtherException, FunctionNotYetImplemented);
	}

	virtual Sample** cloneMatrix() {
		THROW(OtherException, FunctionNotYetImplemented);
	}

	/**type-cast overloading to return CompositeData::_dataComponent[1]*/
	inline operator GaussianData*() {
		return (GaussianData*) _dataComponent[1];
	}

	/**type-cast overloading to return CompositeData::_dataComponent[0]*/
	inline operator BinaryData*() {
		return (BinaryData*) _dataComponent[0];
	}

	/** get Gaussian data */
	virtual GaussianData* getGaussianData() {
		return (GaussianData*) _dataComponent[1];
	}

	/** get Gaussian data */
	virtual BinaryData* getBinaryData() {
		return (BinaryData*) _dataComponent[0];
	}
	/**Virtual Destructor*/
	virtual ~CompositeData();
	
protected:
	
	vector<Data*> _dataComponent;
};

}

#endif /* XEMCOMPOSITEDATA_H_ */
