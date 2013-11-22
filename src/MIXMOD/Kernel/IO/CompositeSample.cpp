#include "mixmod/Kernel/IO/CompositeSample.h"

namespace XEM {

CompositeSample::CompositeSample() {
	// TODO Auto-generated constructor stub
}

CompositeSample::CompositeSample(Sample* bsample, Sample* gsample) {
	_sampleComponent.resize(2);
	_sampleComponent[0] = bsample;
	_sampleComponent[1] = gsample;
}

CompositeSample::~CompositeSample() {
	for (int i = 0; i < _sampleComponent.size(); ++i) {
		if (_sampleComponent[i]) {
			delete _sampleComponent[i];
			_sampleComponent[i] = NULL;
		}
	}
}

}
