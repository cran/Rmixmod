/***************************************************************************
							 SRC/MIXMOD/Clustering/XEMClusteringOutput.cpp  description
	copyright            : (C) MIXMOD Team - 2001-2013
	email                : contact@mixmod.org
 ***************************************************************************/

/***************************************************************************
	This file is part of MIXMOD
    
	MIXMOD is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MIXMOD is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.

	All informations available on : http://www.mixmod.org                                                                                               
 ***************************************************************************/
#include "mixmod/Clustering/ClusteringOutput.h"
#include "mixmod/Clustering/ClusteringModelOutput.h"
#include "mixmod/Kernel/Model/Model.h"
#include "mixmod/Kernel/Criterion/BICCriterion.h"
#include "mixmod/Kernel/Criterion/ICLCriterion.h"
#include "mixmod/Kernel/Criterion/NECCriterion.h"
#include "mixmod/Kernel/IO/ProbaDescription.h"
#include "mixmod/Kernel/IO/Proba.h"
#include "mixmod/Kernel/IO/LabelDescription.h"
#include "mixmod/Kernel/IO/Label.h"

#include <algorithm>
#include <cmath>

namespace XEM {

//--------------------------------------
// Constructor from a vector of criteria
//--------------------------------------
ClusteringOutput::ClusteringOutput(std::vector<CriterionName> const & criterionName) 
: _criterionName(criterionName) {
}

//-----------------
//  Copy constructor
//-----------------
ClusteringOutput::ClusteringOutput(const ClusteringOutput & cOutput)
: _clusteringModelOutput(cOutput.getClusteringModelOutput())
, _criterionName(cOutput.getCriterionName()) {
}

ClusteringOutput* ClusteringOutput::clone() {
	return new ClusteringOutput(*this);
}

// Add one ClusteringModelOutput (incremental construction)
void ClusteringOutput::addEstimation(ClusteringModelOutput* cmoutput) {
	_clusteringModelOutput.push_back(cmoutput);
}

//---------------------------
// Initialization Constructor (unused. TODO: remove)
//---------------------------
ClusteringOutput::ClusteringOutput(std::vector<Model*> const & estimations, 
		std::vector<CriterionName> const & criterionName)
: _clusteringModelOutput(estimations.size())
, _criterionName(criterionName) 
{
	// get a constant of the number of estimations
	const int64_t sizeEstimation = estimations.size();
	// loop over the estimations
	for (unsigned int i = 0; i < sizeEstimation; i++) {
		_clusteringModelOutput[i] = new ClusteringModelOutput(estimations[i]);
	}
}

//-----------
// Destructor
//-----------
ClusteringOutput::~ClusteringOutput() {
	for (unsigned int i = 0; i < _clusteringModelOutput.size(); i++) {
		if (_clusteringModelOutput[i] != NULL) {
			delete _clusteringModelOutput[i];
			_clusteringModelOutput[i] = NULL;
		}
	}
}

//---------------------
/// Comparison operator
//---------------------
bool ClusteringOutput::operator ==(const ClusteringOutput & cOutput) const {

	double EPS = 1e-3; // small value, but not too small... 
	// [TODO: loss of relevant digits when saving to .mixmod file]
	int64_t nbSample = _clusteringModelOutput[0]->getProbaDescription()->getProba()->getNbSample();

	for (int64_t k = 0; k < _clusteringModelOutput.size(); k++) {
		ClusteringModelOutput* cOutputThis = _clusteringModelOutput[k];
		ClusteringModelOutput* cOutputOther = cOutput._clusteringModelOutput[k];

		if ((cOutputThis->getStrategyRunError() == NOERROR && 
				cOutputOther->getStrategyRunError() != NOERROR) ||
			(cOutputThis->getStrategyRunError() != NOERROR && 
				cOutputOther->getStrategyRunError() == NOERROR))
		{
			cout << "UNEQUAL: one model failed, the other one did not" << endl;
			return false;
		}
		
		if (cOutputThis->getStrategyRunError() != NOERROR &&
				cOutputOther->getStrategyRunError() != NOERROR)
		{
			// Skip if both models have error
			continue;
		}
		
		// compare likelihood, probabilities, parameters, labels
		// NOTE: criterion values are never set 
		//       (see XEMModelOutput::XEMModelOutput(XEMModel * estimation))
		//       ==> we don't check this - TODO later.

		//likelihood
		if (fabs(cOutputThis->getLikelihood() - cOutputOther->getLikelihood()) >= EPS) {
			cout << "UNEQUAL: likelihood differ: computed " << cOutputThis->getLikelihood() 
					<< ", expected " << cOutputOther->getLikelihood() << endl;
			return false;
		}

		//precomputation: remap clusters 
		//(may be needed if e.g. (111222333) and (222333111) are obtained)
		int64_t nbClusters = cOutputThis->getNbCluster();
		vector<int64_t> clustersCorrespondence(nbClusters);
		for (int64_t i = 0; i < nbClusters; i++) 
			clustersCorrespondence[i] = -1;
		vector<int64_t> labelsThis = cOutputThis->getLabelDescription()->getLabel()->getLabel();
		vector<int64_t> labelsOther = cOutputOther->getLabelDescription()->getLabel()->getLabel();
		for (int64_t i = 0; i < labelsThis.size(); i++) {
			if (clustersCorrespondence[labelsThis[i] - 1] < 0) {
				//WARNING: labels start at 1, not 0.
				clustersCorrespondence[labelsThis[i] - 1] = labelsOther[i] - 1;

				if (labelsThis[i] != labelsOther[i]) 
					cout << "WARNING: classes misalignment" << endl;
			}
		}

		
		// [TEMPORARY: THIS SHOULD NEVER HAPPEN]
		// sanity check: if some cluster correspondence is unassigned, we have an issue...
		for (int64_t i = 0; i < nbClusters; i++) {
			if (clustersCorrespondence[i] < 0) {
				cout << "ERROR: component " << (i+1) << " is empty" << endl;
				return false;
			}
		}

		
		// NOTE [bauder]: the clean way from here would be to add a 'permutation' parameter 
		// to every sub-class comparison operator. However, it would be quite intrusive since 
		// there are many of them. So, for the moment, only essential checks are done 'by hand'.

		//labels
		for (int64_t i = 0; i < nbSample; i++) {
			if (clustersCorrespondence[labelsThis[i] - 1] != labelsOther[i] - 1) {
				cout << "UNEQUAL: labels differ (at least) at row " << (i+1) << endl;
				return false;
			}
		}

		//probabilities
		int64_t nbCluster = cOutputThis->getProbaDescription()->getProba()->getNbCluster();
		vector<vector<double> > probaThis = 
				cOutputThis->getProbaDescription()->getProba()->getProba();
		vector<vector<double> > probaOther = 
				cOutputOther->getProbaDescription()->getProba()->getProba();
		for (int64_t i = 0; i < nbSample; i++) {
			for (int64_t j = 0; j < nbCluster; j++) {
				if (fabs(probaThis[i][j] - probaOther[i][clustersCorrespondence[j]]) >= EPS) {
					cout << "UNEQUAL: probabilities differ (at least) at row " << (i+1) 
							<< ", cluster " << (j+1) << ": computed " << probaThis[i][j]
							<< ", expected " << probaOther[i][clustersCorrespondence[j]] << endl;
					return false;
				}
			}
		}

//		//parameters: TODO (apply permutation on sub-parameters, and then compare...)
//		if (! (*(cOutputThis->getParameterDescription()) == *(cOutputOther->getParameterDescription())) ) {
//			return false;
//		}
	}

	return true;
}

bool ClusteringOutput::atLeastOneEstimationNoError() const {
	bool res = false;
	unsigned int i = 0;
	while (res == false && i < _clusteringModelOutput.size()) {
		if ( (_clusteringModelOutput[i]->getStrategyRunError()) == NOERROR ) {
			res = true;
		}
		i++;
	}
	return res;
}

const int ClusteringOutput::getNbEstimationWithNoError() const {
	int n = getNbClusteringModelOutput();
	for ( unsigned int i = 0; i < _clusteringModelOutput.size(); i++ ) {
		if (!((_clusteringModelOutput[i]->getStrategyRunError()) == NOERROR )) {
			--n;
		}
	}
	return n;
}

//-----
// sort
//-----
void ClusteringOutput::sort(CriterionName criterionName) {
	std::sort(_clusteringModelOutput.begin(), _clusteringModelOutput.end(), SortByCriterion(criterionName));
}

void ClusteringOutput::editFile() const {
	//TODO 
}

void ClusteringOutput::setClusteringModelOutput(std::vector<ClusteringModelOutput *> & clusteringModelOutput) {
	for (unsigned int i = 0; i < _clusteringModelOutput.size(); i++) {
		delete _clusteringModelOutput[i];
	}
	_clusteringModelOutput = clusteringModelOutput;
}

}
