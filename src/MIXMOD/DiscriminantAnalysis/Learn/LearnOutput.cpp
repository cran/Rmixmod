/***************************************************************************
                             SRC/mixmod/DiscriminantAnalysis/Learn/LearnOutput.cpp  description
    copyright            : (C) MIXMOD Team - 2001-2014
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

#include "mixmod/DiscriminantAnalysis/Learn/LearnOutput.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnModelOutput.h"
#include "mixmod/Kernel/Criterion/BICCriterion.h"
#include "mixmod/Kernel/Criterion/CVCriterion.h"
#include "mixmod/Utilities/Error.h"
#include <algorithm>

namespace XEM {

//--------------------
// Default Constructor
//--------------------
LearnOutput::LearnOutput() {
}

//-----------------
//  Copy constructor
//-----------------
LearnOutput::LearnOutput(const LearnOutput & lOutput)
: _learnModelOutput(lOutput.getLearnModelOutput()) {
}

//---------------------------
// Initialisation Constructor
//---------------------------
LearnOutput::LearnOutput(std::vector<Model*> const & estimations)
: _learnModelOutput(estimations.size()) 
{
	int64_t sizeEstimation = estimations.size();
	// loop over the estimations
	for (unsigned int i = 0; i < sizeEstimation; i++) {
		_learnModelOutput[i] = new LearnModelOutput(estimations[i]);
	}
}

//-----------
// Destructor
//-----------
LearnOutput::~LearnOutput() {
	for (unsigned int i = 0; i < _learnModelOutput.size(); i++) {
		delete _learnModelOutput[i];
	}
}

//---------------------
/// Comparison operator
//---------------------
bool LearnOutput::operator ==(const LearnOutput & output) const {

	for (unsigned int i = 0; i < _learnModelOutput.size(); i++) {
		if (!(_learnModelOutput[i] == output.getLearnModelOutput(i)))
			return false;
	}
	return true;
}

//--------------------
// XEMLearnOutput
//--------------------
bool LearnOutput::atLeastOneEstimationNoError() const {
	bool res = false;
	unsigned int i = 0;
	while (res == false && i < _learnModelOutput.size()) {
		if ((_learnModelOutput[i]->getStrategyRunError()) == NOERROR) {
			res = true;
		}
		i++;
	}
	return res;
}

const int LearnOutput::getNbEstimationWithNoError() const {
	int n = getNbLearnModelOutput();
	for (unsigned int i = 0; i < _learnModelOutput.size(); i++) {
		if (!((_learnModelOutput[i]->getStrategyRunError()) == NOERROR)) {
			--n;
		}
	}
	return n;
}

//-----
// sort
//-----
void LearnOutput::sort(CriterionName criterionName) {

	// sort
	std::sort(_learnModelOutput.begin(), _learnModelOutput.end(), SortByCriterion(criterionName));
}

void LearnOutput::editFile() const {
	//TODO 
}

void LearnOutput::setLearnModelOutput(std::vector<LearnModelOutput *> & learnModelOutput) {
	for (unsigned int i = 0; i < _learnModelOutput.size(); i++) {
		delete _learnModelOutput[i];
	}
	_learnModelOutput = learnModelOutput;
}

}
