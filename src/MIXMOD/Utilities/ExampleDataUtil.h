/**
 * @file ExampleDataUtil
 * @brief Alternative functions to build Clustering and Discriminant Analysis inputs from files
 * @author Benjamin Auder
 */

#ifndef XEM_EXAMPLEDATAUTIL_H
#define XEM_EXAMPLEDATAUTIL_H

#include "mixmod/Kernel/IO/Data.h"
#include "mixmod/Kernel/IO/CompositeData.h"
#include "mixmod/Kernel/IO/GaussianData.h"
#include "mixmod/Kernel/IO/BinaryData.h"
#include "mixmod/Kernel/IO/DataDescription.h"
#include "mixmod/Kernel/IO/LabelDescription.h"
#include "mixmod/Clustering/ClusteringInput.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnInput.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictInput.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnModelOutput.h"

namespace XEM {

// @param fileName Input file name. It should be CSV file with first row containing type for corresponding column.
// Type can be 'C' for continuous (gaussian) or 'B' for binary (categories).
ClusteringInput* getClusteringInput(string fileName, const vector<int64_t>& nbCluster);
LearnInput* getLearnInput(string fileName);
PredictInput* getPredictInput(string fileName, LearnModelOutput* lOutput);

template<class T>
inline void DeleteData(T ** data, int nbSample){
	for (int i=0; i<nbSample; i++)
		delete [] data[i];
	delete [] data;
}

}

#endif
