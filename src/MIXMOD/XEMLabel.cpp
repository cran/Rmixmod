/***************************************************************************
                             SRC/MIXMOD/XEMLabel.cpp  description
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
#include "XEMLabel.h"
#include "XEMLabelDescription.h"

//------------
// Constructor
//------------
XEMLabel::XEMLabel(){
  _nbSample = 0;
}


//------------
// Constructor
//------------
XEMLabel::XEMLabel(int64_t nbSample){
  _nbSample = nbSample;
  _label.resize(_nbSample);
}



//------------
// Constructor
//------------
XEMLabel::XEMLabel(XEMEstimation * estimation){
  
  XEMModel * model = estimation->getModel();
  if (model == NULL){
    throw internalMixmodError;
  }
  
  // compute tabLabel
  //-----------------
  int64_t * tabLabel = NULL;
  int64_t nbCluster = estimation->getNbCluster();
  
  XEMModelType * modelType = model->getParameter()->getModelType();
  bool binary = isBinary(modelType->_nameModel);
  if (!binary){ 
    _nbSample = estimation->getNbSample();
    int64_t ** tabPartition = new int64_t*[_nbSample];
    for (int64_t i=0; i<_nbSample; i++){
      tabPartition[i] = new int64_t[nbCluster];
    }
    tabLabel    = new int64_t[_nbSample];
    estimation->getModel()->getLabelAndPartitionByMAPOrKnownPartition(tabLabel, tabPartition);
  }
  ////
  else{
  ////
    //binary case
    const vector<int64_t> & correspondenceOriginDataToReduceData = estimation->getcorrespondenceOriginDataToReduceData();
    _nbSample = correspondenceOriginDataToReduceData.size();
    tabLabel    = new int64_t[_nbSample];
    
    //label et partition on reduceData
    int64_t nbSampleOfDataReduce = estimation->getModel()->getNbSample();
    int64_t * tabLabelReduce = new int64_t[nbSampleOfDataReduce];
    int64_t ** tabPartitionReduce = new int64_t*[nbSampleOfDataReduce];
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      tabPartitionReduce[i] = new int64_t[nbCluster];
    }
    estimation->getModel()->getLabelAndPartitionByMAPOrKnownPartition(tabLabelReduce, tabPartitionReduce);
  
  //  double ** tabPostProbaReduce = NULL;
  //  tabPostProbaReduce = copyTab(estimation->getModel()->getPostProba(), nbSampleOfDataReduce, nbCluster); // copy
    
      // convert labelReduce, partitionReduce, postProbaReduce to label, partition, postProba
    for (int64_t i=0; i<_nbSample; i++){
      tabLabel[i] = tabLabelReduce[correspondenceOriginDataToReduceData[i]];
    }
  
    //delete
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      delete [] tabPartitionReduce[i];
    }
    delete [] tabPartitionReduce;
  
  
    delete[] tabLabelReduce;
    
  }
  
  
  // compute _label
  recopyTabToVector(tabLabel, _label, _nbSample);
  delete [] tabLabel;
}



//------------
// Constructor
//------------
XEMLabel::XEMLabel(const XEMLabel & iLabel){
  _nbSample = iLabel.getNbSample();
  _label = iLabel.getLabel();
}




//-----------
// Destructor
//-----------
XEMLabel::~XEMLabel(){
}




//----------
// editProba
//----------
void XEMLabel::edit(ostream & stream){
  stream.setf(ios::fixed, ios::floatfield);
  for (int64_t i=0; i<_nbSample; i++){
    stream<<_label[i]<<endl;
  }
}


//---------
// getProba
//---------
int64_t * XEMLabel::getTabLabel() const{
  int64_t * res;
  recopyVectorToTab(_label, res);
  return res;
}




// -----------
//input stream
// read labels between 1 and nbCluster
// -----------
void XEMLabel::input(ifstream & flux, int64_t nbCluster){
  int64_t i = 0;
  int64_t read;
  
  while (i<_nbSample && !flux.eof()){
    flux>>read;
    if (read>=1 && read<=nbCluster){
      _label[i] = read;
    }
    else{
      throw badValueInLabelInput;
    }
    i++;
  }
  
  if (!flux.eof() && i!=_nbSample){
    throw notEnoughValuesInLabelInput;
  }
  
}

// -----------
//input stream
// read labels between 1 and nbCluster
// -----------
void XEMLabel::input(const XEMLabelDescription & labelDescription){
  int64_t i = 0;
  int64_t readLabel;
 
  string labelFilename  = labelDescription.getFileName();
  
  _nbSample = labelDescription.getNbSample();
  
  ifstream fi((labelFilename).c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongDataFileName;
  }
  
  while (i<_nbSample && !fi.eof()){
      for (int64_t j =0; j<labelDescription.getNbColumn();++j){
	if (fi.eof()){
	  throw endDataFileReach; 
	}
	if (typeid(*(labelDescription.getColumnDescription(j)))==typeid(XEMIndividualColumnDescription)){
            string stringTmp;
            fi>>stringTmp;
            //cout<<stringTmp<<endl;
        }else{
	    fi>>readLabel;
	    _label.push_back(readLabel);
	}	
      }
      ++i;
  }
  if (!fi.eof() && i!=_nbSample){
    throw notEnoughValuesInLabelInput;
  }
}
