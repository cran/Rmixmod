/***************************************************************************
                             SRC/mixmod/DiscriminantAnalysis/Predict/PredictOutput.cpp  description
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

#include "mixmod/DiscriminantAnalysis/Predict/PredictOutput.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictModelOutput.h"

namespace XEM {

//--------------------
// Default Constructor
//--------------------
PredictOutput::PredictOutput() {
}

//-----------------
//  Copy constructor
//-----------------
PredictOutput::PredictOutput(const PredictOutput & pOutput) {
	_predictModelOutput.push_back(pOutput.getPredictModelOutput().front());
}

//---------------------------
// Initialisation Constructor
//---------------------------
PredictOutput::PredictOutput(Model * estimation) {
	_predictModelOutput.push_back(new PredictModelOutput(estimation));
}

//-----------
// Destructor
//-----------
PredictOutput::~PredictOutput() {
	for (unsigned int i = 0; i < _predictModelOutput.size(); i++) {
		delete _predictModelOutput[i];
	}
}

//---------------------
/// Comparison operator
//---------------------
bool PredictOutput::operator ==(const PredictOutput & output) const {

	for (unsigned int i = 0; i < _predictModelOutput.size(); i++) {
		if (!(_predictModelOutput[i] == output.getPredictModelOutput(i)))
			return false;
	}
	return true;
}

}
