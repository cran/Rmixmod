/***************************************************************************
                             SRC/MIXMOD/XEMProbaOutput.cpp  description
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

#include "XEMProbaOutput.h"
#include "XEMModel.h"
#include "XEMBinaryModel.h"
#include "XEMModelType.h"
#include <vector>

//------------
// Constructor
//------------
XEMProbaOutput::XEMProbaOutput(){
  _CVLabelAvailable  = false;
  _tabLabel          = NULL;
  _tabCVLabel        = NULL;
  _tabPartition      = NULL;
  _tabPostProba      = NULL;
}



//------------
// Constructor
//------------

XEMProbaOutput::XEMProbaOutput(XEMModel * model){
  
  _CVLabelAvailable  = false;
  _tabCVLabel        = NULL;
  _nbCluster         = model->getNbCluster();
  
  if (model == NULL){
    throw internalMixmodError;
  }
  
  bool binary = isBinary(model->getModelType()->_nameModel);
  if (!binary){
    // gaussian case 
    _nbSample  = model->getNbSample();
    _tabLabel          = new int64_t[_nbSample];
    _tabPartition      = new int64_t *[_nbSample];
    for (int64_t  i=0; i<_nbSample; i++){
      _tabPartition[i] = new int64_t[_nbCluster];
    }
    model->getLabelAndPartitionByMAPOrKnownPartition(_tabLabel, _tabPartition);
    _tabPostProba      = copyTab(model->getPostProba(), _nbSample, _nbCluster); // copy
  }
  else{
    const std::vector<int64_t> & correspondenceOriginDataToReduceData = dynamic_cast<XEMBinaryModel*>(model)->getCorrespondenceOriginDataToReduceData();
    //binary case
    _nbSample = correspondenceOriginDataToReduceData.size();
    _tabLabel          = new int64_t[_nbSample];
    _tabPartition      = new int64_t *[_nbSample];
    for (int64_t  i=0; i<_nbSample; i++){
      _tabPartition[i] = new int64_t[_nbCluster];
    }
    _tabPostProba      = new double *[_nbSample];
    for (int64_t  i=0; i<_nbSample; i++){
      _tabPostProba[i] = new double[_nbCluster];
    }
    
    //label et partition on reduceData
    int64_t nbSampleOfDataReduce = model->getNbSample();
    int64_t ** tabPartitionReduce = new int64_t*[nbSampleOfDataReduce];
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      tabPartitionReduce[i] = new int64_t[_nbCluster];
    }
    int64_t * tabLabelReduce = new int64_t[nbSampleOfDataReduce];
    model->getLabelAndPartitionByMAPOrKnownPartition(tabLabelReduce, tabPartitionReduce);
  
    double ** tabPostProbaReduce = NULL;
    tabPostProbaReduce = copyTab(model->getPostProba(), nbSampleOfDataReduce, _nbCluster); // copy
    
      // convert labelReduce, partitionReduce, postProbaReduce to label, partition, postProba
    for (int64_t i=0; i<_nbSample; i++){
      _tabLabel[i] = tabLabelReduce[correspondenceOriginDataToReduceData[i]];
      for (int64_t k=0; k<_nbCluster ;k++){ 
        int64_t index = correspondenceOriginDataToReduceData[i];
        _tabPostProba[i][k] = tabPostProbaReduce[index][k];
        _tabPartition[i][k] = tabPartitionReduce[index][k];
      }
    }
  
    //delete
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      delete [] tabPartitionReduce[i];
    }
    delete [] tabPartitionReduce;
  
    for (int64_t i=0; i<nbSampleOfDataReduce; i++){
      delete [] tabPostProbaReduce[i];
    }
    delete [] tabPostProbaReduce;
  
    delete[] tabLabelReduce;
  }
}

XEMProbaOutput::XEMProbaOutput(XEMProbaOutput * iProbaOutput){
  _nbSample = iProbaOutput->getNbSample();
  _nbCluster = iProbaOutput->getNbCluster();

  _tabPostProba  = copyTab(iProbaOutput->getTabPostProba(), _nbSample, _nbCluster);
  _tabLabel      = copyTab(iProbaOutput->getTabLabel(), _nbSample);

  _tabPartition = NULL;
  _tabCVLabel = NULL;
  _CVLabelAvailable = false;

}




//-----------
// Destructor
//-----------
XEMProbaOutput::~XEMProbaOutput(){
  if (_tabLabel){
     delete[] _tabLabel;
     _tabLabel = NULL;
  }

  if (_tabCVLabel){
     delete[] _tabCVLabel;
     _tabCVLabel = NULL;
  }

  int64_t  i;
  if (_tabPartition){
    for (i=0; i<_nbSample; i++){
      delete[] _tabPartition[i];
      _tabPartition[i] = NULL;
    }
    delete [] _tabPartition;
    _tabPartition = NULL;
  }

  if (_tabPostProba){
    for (i=0; i<_nbSample; i++){
      delete[] _tabPostProba[i];
      _tabPostProba[i] = NULL;
    }
    delete [] _tabPostProba;
    _tabPostProba = NULL;
  }

}


//-----------
// setCVLabel
//-----------
void XEMProbaOutput::setCVLabel(int64_t  * CVLabel){
  _CVLabelAvailable = true;
  _tabCVLabel = new int64_t[_nbSample];
  recopyTab(CVLabel,_tabCVLabel,_nbSample);
}



//--------------
// editPartition
//--------------
void XEMProbaOutput::editPartition(ofstream & oFile){
  int64_t  k, i;

  int64_t  ** p_tabPartition;
  int64_t   * p_tabPartition_i;
  
    
    p_tabPartition = _tabPartition;
    for (i=0; i<_nbSample; i++){
      p_tabPartition_i = *p_tabPartition;
      for (k=0; k<_nbCluster; k++){
        oFile << p_tabPartition_i[k] << "\t" ;
      }
      oFile<<endl;
      
      p_tabPartition++;
    }
 
}



//----------
// editLabel
//----------
void XEMProbaOutput::editLabel(ofstream & oFile){
  int64_t  i;
    for (i=0; i<_nbSample; i++)
      oFile << _tabLabel[i] << endl;
 
}



//----------
// editLabel
//----------
void XEMProbaOutput::editLabel(){
  
#ifdef VERBOSE
  int64_t  i;

    for (i=0; i<_nbSample; i++)
      cout << _tabLabel[i] << endl;
#endif
}


//--------------
// editPostProba
//--------------
void XEMProbaOutput::editPostProba(ofstream & oFile){
  
  oFile.setf(ios::fixed, ios::floatfield);
    editTab(_tabPostProba,_nbSample,_nbCluster,oFile,"\t","");
 
  
}



//------------
// editCVLabel
//------------
void XEMProbaOutput::editCVLabel(ofstream & oFile){
  
  if (_CVLabelAvailable){
    int64_t  i;

      for (i=0; i<_nbSample; i++)
        oFile << _tabCVLabel[i] << endl;
  
     
  } // end if CVLabel
}


XEMProbaOutput * XEMProbaOutput::clone(){
  XEMProbaOutput * newProbaOutput = new XEMProbaOutput(this);
  return newProbaOutput; // ceci n'etait pas fait...
}



