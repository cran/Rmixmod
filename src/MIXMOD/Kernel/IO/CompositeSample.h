#ifndef XEMCOMPOSITESAMPLE_H_
#define XEMCOMPOSITESAMPLE_H_
/**@file XEMCompositeSample.h
 * @brief Composite sample class for heterogeneous data.
 * @author Parmeet Bhatia
 */
#include "mixmod/Kernel/IO/Sample.h"

namespace XEM {

class BinarySample;
class GaussianSample;

class CompositeSample : public Sample {

public:
	
	//Default constructor
	CompositeSample();
	//Initialization constructor
	CompositeSample(Sample*, Sample*);
	/**type-cast overloading for Binary sample*/
	operator BinarySample*();
	/**type-cast overloading for Gaussian sample*/
	operator GaussianSample*();

	/** get gaussian sample*/
	virtual GaussianSample* getGaussianSample() const {
		return (GaussianSample*) _sampleComponent[1];
	}

	/** get Binary sample*/
	virtual BinarySample* getBinarySample() const {
		return (BinarySample*) _sampleComponent[0];
	}
	virtual ~CompositeSample();
	
protected:
	
	vector<Sample*> _sampleComponent;
};

inline CompositeSample::operator BinarySample *() {
	return _sampleComponent[0]->getBinarySample();
}

inline CompositeSample::operator GaussianSample *() {
	return _sampleComponent[1]->getGaussianSample();
}

}

#endif /* XEMCOMPOSITESAMPLE_H_ */
