/***************************************************************************
                             SRC/MIXMOD/XEMProba.cpp  description
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

#include "XEMProba.h"
#include "XEMModel.h"
#include "XEMBinaryModel.h"
#include "XEMModelType.h"

//------------
// Constructor
//------------
XEMProba::XEMProba(){
  _nbCluster = 0;
  _nbSample = 0;
}


//------------
// Constructor
//------------
XEMProba::XEMProba(int64_t nbSample, int64_t nbCluster){
  _nbCluster = nbCluster;
  _nbSample = nbSample;
  _proba.resize(_nbSample);
  for (int64_t i = 0; i<_nbSample; i++){
    _proba[i].resize(_nbCluster);
  }
}



//------------
// Constructor
//------------
XEMProba::XEMProba(XEMModel * model){
  _nbCluster = model->getNbCluster();

  if (model == NULL){
    throw internalMixmodError;
  }
  
  // compute tabProba
  //-----------------
  double ** tabProba = NULL;
  
  XEMModelType * modelType = model->getModelType();
  bool binary = isBinary(modelType->_nameModel);
  if (!binary){ 
    _nbSample = model->getNbSample();
    tabProba      = copyTab(model->getPostProba(), _nbSample, _nbCluster); // copy
  }
  ////
  else{
  ////
    //binary case
    const vector<int64_t> & correspondenceOriginDataToReduceData = dynamic_cast<XEMBinaryModel*>(model)->getCorrespondenceOriginDataToReduceData();
    _nbSample = correspondenceOriginDataToReduceData.size();
    tabProba = new double*[_nbSample];
    for (int64_t i=0; i<_nbSample; i++){
      tabProba[i] = new double[_nbCluster];
    }
    int64_t nbSampleOfDataReduce = model->getNbSample();
    double ** tabPostProbaReduce = NULL;
    tabPostProbaReduce = copyTab(model->getPostProba(), nbSampleOfDataReduce, _nbCluster); // copy
    //editTab<double>(tabPostProbaReduce,nbSampleOfDataReduce, _nbCluster);
      // convert labelReduce, partitionReduce, postProbaReduce to label, partition, postProba
    for (int64_t i=0; i<_nbSample; i++){
      for (int64_t k=0; k<_nbCluster ;k++){ 
        int64_t index = correspondenceOriginDataToReduceData[i];
        tabProba[i][k] = tabPostProbaReduce[index][k];
      }
    }
  
    //delete
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      delete [] tabPostProbaReduce[i];
    }
    delete [] tabPostProbaReduce;
  }
  
  
  // compute _proba
  recopyTabToVector(tabProba, _proba, _nbSample, _nbCluster);
  for (int64_t i=0; i<_nbSample; i++){
    delete[] tabProba[i];
  } 
  delete [] tabProba;
}



//------------
// Constructor
//------------
XEMProba::XEMProba(const XEMProba & iProba){
  _nbSample = iProba.getNbSample();
  _nbCluster = iProba.getNbCluster();
  _proba = iProba.getProba();
}




//-----------
// Destructor
//-----------
XEMProba::~XEMProba(){
}


//--------------------
/// Comparison operator
//--------------------
bool XEMProba::operator ==(const XEMProba & proba) const{
  if ( _nbSample != proba.getNbSample() ) return false;
  if ( _nbCluster != proba.getNbCluster() ) return false;
  for (int64_t i=0; i<_nbSample; i++ ){
    for (int64_t k=0; k<_nbCluster ;k++){
      if ( _proba[i][k] != proba.getProba()[i][k] ) return false;
    }
  }
  return true;
}


//----------
// editProba
//----------
void XEMProba::edit(ostream & stream){
    stream.setf(ios::fixed, ios::floatfield);
    for (int64_t i=0; i<_nbSample; i++){
      for (int64_t k=0; k<_nbCluster; k++){
        stream<<_proba[i][k]<<"\t";
      }
      stream<<endl;
    }
}


//---------
// getProba
//---------
double ** XEMProba::getTabProba() const{
  double ** res;
  recopyVectorToTab(_proba, res);
  return res;
}


// -----------
//input stream
// -----------
void XEMProba::input(ifstream & flux){
  int64_t i = 0;
  int64_t k = 0;
  
  while(i<_nbSample && !flux.eof()){
    k = 0;
    while (k<_nbCluster &&  !flux.eof()){
      double value;
      flux>>value;
      _proba[i][k]=value;
      k++;
    }
    if (!flux.eof() && k!=_nbCluster){
      throw notEnoughValuesInProbaInput;
    }
    i++;
  }
  if (!flux.eof() && i!=_nbSample){
    throw notEnoughValuesInProbaInput;
  }
}


