/***************************************************************************
 SRC/MIXMOD/XEMLearnModelOutput.cpp  description
 copyright            : (C) MIXMOD Team - 2001-2011
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

#include "XEMLearnModelOutput.h"
#include "XEMLabelDescription.h"
#include "XEMBinaryModel.h"



//--------------------
// Default Constructor
//--------------------
XEMLearnModelOutput::XEMLearnModelOutput()
{}


//-----------------
//  Copy constructor
//-----------------
XEMLearnModelOutput::XEMLearnModelOutput(const XEMLearnModelOutput & cModelOutput){
  throw internalMixmodError;
}



//-----------------
//  Initialization Constructor
//-----------------
XEMLearnModelOutput::XEMLearnModelOutput(XEMModel * estimation) : XEMModelOutput(estimation)
{}

//-----------------
//  Initialization Constructor
//-----------------
XEMLearnModelOutput::XEMLearnModelOutput( XEMModelType & modelType
                                        , int64_t nbCluster
                                        , vector< XEMCriterionOutput >& criterionOutput
                                        , double likelihood
                                        , XEMParameterDescription& parameterDescription
                                        , XEMLabelDescription& labelDescription
                                        , XEMProbaDescription& probaDescription
                                        )
                                        :XEMModelOutput(modelType, nbCluster, criterionOutput, likelihood, parameterDescription, labelDescription, probaDescription)
{}

//-----------------
//  Initialization Constructor
//-----------------
XEMLearnModelOutput::XEMLearnModelOutput(XEMModelType& modelType, int64_t nbCluster, XEMErrorType error): XEMModelOutput(modelType, nbCluster, error)
{}

//-----------
// Destructor
//-----------
XEMLearnModelOutput::~XEMLearnModelOutput(){
  if ( _CVLabel ) delete _CVLabel;
}



/// set CV Labels
void XEMLearnModelOutput::setCVLabel(XEMModel * estimation, std::vector<int64_t> & cvLabel)
{
  if (isBinary(estimation->getModelType()->_nameModel)){ 
    ////
    //binary case
    
    // cmake a copy of cvLabel
    std::vector<int64_t> cvLabelCopy(cvLabel);
    
    const vector<int64_t> & correspondenceOriginDataToReduceData = dynamic_cast<XEMBinaryModel*>(estimation)->getCorrespondenceOriginDataToReduceData();
    // get the true number of sample
    const int64_t nbSample = correspondenceOriginDataToReduceData.size();
    
    // resize cvLabel
    cvLabel.resize(nbSample);
    
    // convert labelReduce, to label
    for (int64_t i=0; i<nbSample; i++){
      cvLabel[i] = cvLabelCopy[correspondenceOriginDataToReduceData[i]];
    }    
  }
  // create a new instance of XEMLabelDescription
  _CVLabel = new XEMLabelDescription(cvLabel.size(),cvLabel);
}


