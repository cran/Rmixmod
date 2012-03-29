/***************************************************************************
                             SRC/MIXMOD/XEMBinaryData.cpp  description
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

#include "XEMBinaryData.h"
#include "XEMBinarySample.h"
#include "XEMPartition.h"
#include "XEMDataDescription.h"
#include "XEMWeightColumnDescription.h"
#include "XEMQualitativeColumnDescription.h"
#include "XEMIndividualColumnDescription.h"
#include "XEMUtil.h"
#include <list>



//------------
// Constructor
//------------
XEMBinaryData::XEMBinaryData(){
}


//------------
// Constructor
//------------
XEMBinaryData::XEMBinaryData(const XEMBinaryData & iData):XEMData(iData){
  int64_t i;

  XEMSample ** matrix = iData._matrix;
  
  _matrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++)
    _matrix[i] = new XEMBinarySample((XEMBinarySample*)matrix[i]);

  _tabNbModality = new int64_t[_pbDimension];
  recopyTab<int64_t>(iData._tabNbModality,_tabNbModality,_pbDimension);
  /*for (j=0; j<_pbDimension; j++)
    _tabNbModality[j] = iData->_tabNbModality[j];*/
}


//------------
// Constructor
//------------
XEMBinaryData::XEMBinaryData(int64_t nbSample, int64_t pbDimension, vector<int64_t>  nbModality):XEMData(nbSample,pbDimension){
  int64_t j;
  int64_t i;
  
  _matrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++){
    _matrix[i] = new XEMBinarySample(_pbDimension);
  }

  _tabNbModality = new int64_t[_pbDimension];
  for (j=0; j<_pbDimension; j++){
    _tabNbModality[j] = nbModality[j];
  }

}



//------------
// Constructor
//------------
XEMBinaryData::XEMBinaryData(int64_t nbSample, int64_t pbDimension, vector<int64_t>  nbModality, int64_t ** matrix):XEMData(nbSample,pbDimension){

  int64_t j;
  int64_t i;
  
  _matrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++){
    _matrix[i] = new XEMBinarySample(_pbDimension, matrix[i]);
  }

  _tabNbModality = new int64_t[_pbDimension];
  for (j=0; j<_pbDimension; j++){
    _tabNbModality[j] = nbModality[j];
  }
}



//------------
// Constructor
//------------
XEMBinaryData::XEMBinaryData(int64_t nbSample, int64_t pbDimension, const string & dataFileName, int64_t * tabNbModality):XEMData(nbSample,pbDimension){
  int64_t j;
  int64_t i;
  
  _matrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++){
    _matrix[i] = new XEMBinarySample(_pbDimension);
  }

  _tabNbModality = new int64_t[_pbDimension];
  for (j=0; j<_pbDimension; j++)
    _tabNbModality[j] = tabNbModality[j];

  ifstream dataFileStream(dataFileName.c_str(), ios::in);
 if (! dataFileStream.is_open()){
    dataFileStream.close();
    throw wrongDataFileName;
  }
  input(dataFileStream);
  dataFileStream.close();
  _fileNameData = dataFileName;
}


//------------
// Constructor 
//------------
XEMBinaryData::XEMBinaryData(int64_t nbSample, int64_t pbDimension, int64_t * tabNbModality, double weightTotal, XEMSample **& matrix, double * weight):XEMData(nbSample,pbDimension,weightTotal,weight){
  _matrix = matrix;
  _tabNbModality = new int64_t[_pbDimension];
  for( int64_t i=0; i<_pbDimension; i++){
    _tabNbModality[i] = tabNbModality[i];
  }
}



//------------
// Constructor (used in DCV context)
//------------
// originalData contains all the data set
XEMBinaryData::XEMBinaryData(int64_t nbSample, int64_t pbDimension, XEMData * originalData, XEMCVBlock & block):XEMData(nbSample, pbDimension){
  XEMBinaryData * origData = (XEMBinaryData *)(originalData);
  XEMSample ** origMatrix = origData->_matrix ; 

  _tabNbModality = new int64_t[_pbDimension];
  for (int64_t j=0; j<_pbDimension; j++)
    _tabNbModality[j] = origData->_tabNbModality[j];

    _weightTotal = block._weightTotal; 
    _matrix = new XEMSample*[_nbSample] ; 

    for (int64_t i=0; i<_nbSample; i++){
    _matrix[i] = new XEMBinarySample(pbDimension,((XEMBinarySample *)origMatrix[block._tabWeightedIndividual[i].val])->getTabValue());
    //cout<<"ind : "<<block._tabWeightedIndividual[i].val;

    _weight[i] = block._tabWeightedIndividual[i].weight;
    //cout<<" - weight : "<<block._tabWeightedIndividual[i].weight<<endl;
  }

}





//----------
//Destructor
//----------
XEMBinaryData::~XEMBinaryData(){
  int64_t i;

  if (_matrix){
    for (i=0; i<_nbSample; i++){
      delete _matrix[i];
      //_matrix[i] = NULL;
    }
    delete[] _matrix;
    _matrix = NULL;
  }
  
  if (_tabNbModality){
    delete[] _tabNbModality;
    _tabNbModality = NULL;
  }
  
}



//-----------
// Clone data
//-----------
XEMData * XEMBinaryData::clone() const{
	XEMBinaryData * newData = new XEMBinaryData(*this);
	return(newData);	
}



//------------------
// Clone data matrix
//------------------
XEMSample ** XEMBinaryData::cloneMatrix(){
  int64_t i;

  XEMSample ** newMatrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++){
    newMatrix[i] = new XEMBinarySample((XEMBinarySample*)_matrix[i]);
  }

  return newMatrix;
}





//------
// input
//------
void XEMBinaryData::input(ifstream & fi){
  int64_t i;
  int64_t j;
  int64_t * curSampleValue = new int64_t[_pbDimension];
  
  for (i=0; i<_nbSample; i++){
    for (j=0; j<_pbDimension; j++){
      if (fi.eof()){
      	throw endDataFileReach;	
      }
      fi>>curSampleValue[j];

      // Test data value //
      if (curSampleValue[j]>_tabNbModality[j] || curSampleValue[j]<=0){
          throw wrongValueInMultinomialCase;
        }
    }// end j
    ((XEMBinarySample*)_matrix[i])->setDataTabValue(curSampleValue);
    _weight[i]=1;
  }  
  _weightTotal = _nbSample;
  
  delete [] curSampleValue;
  
  
}




//------
// input
//------
void XEMBinaryData::input(const XEMDataDescription & dataDescription){
  int64_t i;
  int64_t j;
  int64_t * curSampleValue = new int64_t[_pbDimension];
  double tmp;
  _weightTotal = 0;
  
  _fileNameData  = dataDescription.getFileName();
  ifstream fi((_fileNameData).c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongDataFileName;
  }
  
  int64_t g=0;
  for (i=0; i<_nbSample; i++){
    g=0;
    for(j=0; j<dataDescription.getNbColumn(); j++){
      if (fi.eof()){
        throw endDataFileReach; 
      }
      if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMQualitativeColumnDescription)){
        fi>>curSampleValue[g];
        // Test data value //
        if (curSampleValue[g]>_tabNbModality[g] || curSampleValue[g]<=0){
          throw wrongValueInMultinomialCase;
        }
        g++;
      }
      else{
        if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMWeightColumnDescription)){
          fi>>_weight[i];
        }
        else{
          if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMIndividualColumnDescription)){
            string stringTmp;
            fi>>stringTmp;
           // cout<<stringTmp<<endl;
          }
          else{
            fi>>tmp; // on avance !
          }
        }
      }
    }// end j
    ((XEMBinarySample*)_matrix[i])->setDataTabValue(curSampleValue);
    _weightTotal += _weight[i];
  }  // end i
  
  delete [] curSampleValue;
  
}



//-------
// output
//-------
void XEMBinaryData:: output(ostream & fo){
  fo<<"Sample size: "<<_nbSample;	
  fo<<"  Dimension: "<<_pbDimension;
  fo<<" values : "<<endl;
  for (int64_t i=0; i<_nbSample; i++){
    int64_t * values = getDataTabValue(i);
    for (int64_t j=0; j<_pbDimension; j++){
      fo<<values[j]<<" ";
    }
    fo<<" - weight : "<<_weight[i];
    fo<<endl;
  }
}





//-------------------
// Create reduce data
//--------------------
XEMData * XEMBinaryData::reduceData(vector<int64_t> & correspondcenceOriginDataToReduceData, XEMPartition * knownPartition, XEMPartition * initPartition, XEMPartition *& oKnownPartition, XEMPartition *& oInitPartition){
  int64_t rOld, value;
  int64_t * tabBaseMj = new int64_t[_nbSample];
  int64_t j, k, sizeTabFactor;
  int64_t idxDim;
  int64_t idxSample, sizeList, i;
  double * weight;
  //XEMSample ** data;

  correspondcenceOriginDataToReduceData.resize(_nbSample); 
  
  // test for int64_t limit
  double maxTabFactor = 1;
  for (j=1; j<_pbDimension; j++){
    maxTabFactor = maxTabFactor* _tabNbModality[j-1];
  }
  if (knownPartition && initPartition){
    maxTabFactor = maxTabFactor * _tabNbModality[_pbDimension-1];
    maxTabFactor = maxTabFactor * (knownPartition->_nbCluster + 1);
  }
  if ( (knownPartition && !initPartition) || (!knownPartition && initPartition) ){
    maxTabFactor = maxTabFactor * _tabNbModality[_pbDimension-1];
  }
  if (maxTabFactor > int64_t_max){
    throw int64_t_max_error;
  }
  
  
  list<TWeightedIndividual*> listDiffIndiv;
  list<TWeightedIndividual*>::iterator listIterator;
  list<TWeightedIndividual*>::iterator listBegin;
  list<TWeightedIndividual*>::iterator listEnd;

  // tab of multiplicator value to set sample in base mj //
  //-----------------------------------------------------//
  sizeTabFactor = _pbDimension;
  if (knownPartition)
    sizeTabFactor++;
  if (initPartition)
    sizeTabFactor++;
  int64_t * tabFactor = new int64_t[sizeTabFactor];
  tabFactor[0] = 1;
  for (j=1; j<_pbDimension; j++){
    tabFactor[j] = tabFactor[j-1]* _tabNbModality[j-1];
    //cout<<"_tabNbModality["<<j-1<<"] : "<<_tabNbModality[j-1]<<endl;
    //cout<<"tabFactor["<<j<<"] : "<<tabFactor[j]<<endl;
  }
  if (knownPartition && initPartition){ 
    tabFactor[_pbDimension]   = tabFactor[_pbDimension-1]* _tabNbModality[_pbDimension-1];
    //cout<<"tabFactor["<<_pbDimension<<"] : "<<tabFactor[_pbDimension]<<endl;
    tabFactor[_pbDimension+1] = tabFactor[_pbDimension]* (knownPartition->_nbCluster + 1);
    //cout<<"tabFactor["<<_pbDimension+1<<"] : "<<tabFactor[_pbDimension+1]<<endl;
  }
  if ( (knownPartition && !initPartition) || (!knownPartition && initPartition) ){
    tabFactor[_pbDimension] = tabFactor[_pbDimension-1]* _tabNbModality[_pbDimension-1];
    //cout<<"tabFactor["<<_pbDimension<<"] : "<<tabFactor[_pbDimension]<<endl;
  }

  sizeList = 0;
  
  // compute all samples in base mj //
  // create list of differents samples in base mj //
  //----------------------------------------------//
  for (i=0; i<_nbSample; i++){

    value = 0;
    for (j=0; j<_pbDimension; j++)
      value += ( ((XEMBinarySample*)_matrix[i])->getDataValue(j) - 1 ) * tabFactor[j] ;

    if (knownPartition && initPartition){
      value += (knownPartition->getGroupNumber(i)+1) * tabFactor[_pbDimension];
      value += (initPartition->getGroupNumber(i)+1) * tabFactor[_pbDimension+1];
    }
    if (knownPartition && !initPartition)
      value += (knownPartition->getGroupNumber(i)+1) * tabFactor[_pbDimension];
    if (!knownPartition && initPartition)
      value += (initPartition->getGroupNumber(i)+1) * tabFactor[_pbDimension];

    //cout<<"value de l'indvidu "<<i+1<<" : "<<value<<endl;
    tabBaseMj[i] = value;

    // Search if sample already exist in list //
    listBegin = listDiffIndiv.begin();
    listEnd   = listDiffIndiv.end();

    listIterator = listBegin;    
    while ( (listIterator != listEnd) && (value > (*listIterator)->val))
      listIterator++;

    // list empty //  
    if (listBegin==listEnd){
      TWeightedIndividual * elem = new TWeightedIndividual;
      elem->val = value;
      elem->weight = _weight[i];

      listDiffIndiv.push_front(elem);
      sizeList++;      
    }
    
    else{
      // if elemn in end of list //
      if (listIterator==listEnd){
        listIterator--;
        if ((*listIterator)->val==value)
          (*listIterator)->weight += _weight[i];
        else{
          TWeightedIndividual * elem = new TWeightedIndividual;
          elem->val = value;
          elem->weight = _weight[i];

          listDiffIndiv.push_back(elem);
          sizeList++;
        }
      }    

      // elemen in begin or in middle of list //
      else{
        if ((*listIterator)->val==value)
          (*listIterator)->weight += _weight[i];
        else{
          TWeightedIndividual * elem = new TWeightedIndividual;
          elem->val = value;
          elem->weight = _weight[i];

          if (listIterator == listDiffIndiv.begin())
            listDiffIndiv.push_front(elem);
          else
            listDiffIndiv.insert(listIterator,1,elem);

          sizeList++;
        }
      }
    }
  } // end for i



  // Create reduce matrix with reduce weight //
  //-----------------------------------------//
  weight = new double[sizeList];
  XEMSample ** data   = new XEMSample *[sizeList];
  idxSample = 0;
  listBegin = listDiffIndiv.begin();
  listEnd   = listDiffIndiv.end();
  for (listIterator = listBegin; listIterator != listEnd; listIterator++){

    data[idxSample] = new XEMBinarySample(_pbDimension);
    weight[idxSample] = (*listIterator)->weight;

    idxDim = _pbDimension-1;
    rOld   = (*listIterator)->val;
    // knownLabel and initLabel included in computation of base Mj //
    if (knownPartition && initPartition){
      rOld = rOld % tabFactor[_pbDimension+1];
      rOld = rOld % tabFactor[_pbDimension];
    }
    // knownLabel or initLabel included in computation of base Mj //
    if ( (knownPartition && !initPartition) || (!knownPartition && initPartition) )
      rOld = rOld % tabFactor[_pbDimension];

    XEMBinarySample * curDataSample = (XEMBinarySample*)data[idxSample];
    while (idxDim>=0){
      curDataSample->setDataValue(idxDim,1 + rOld/tabFactor[idxDim]);
      rOld = rOld % tabFactor[idxDim];
      idxDim--;
    }
    idxSample++;

  }

  // Set array of correspondence //
  for (i=0; i<_nbSample; i++){
    listIterator = listBegin;
    idxSample = 0;
    while ( (listIterator != listEnd) && (tabBaseMj[i] != (*listIterator)->val)){
      listIterator++;
      idxSample++;
    }
    correspondcenceOriginDataToReduceData[i] = idxSample;
    //cout<<correspondcenceOriginDataToReduceData[i]<<endl;
  }

  // Set reduce labels //
  int64_t nbCluster;
  if (initPartition){
    nbCluster = initPartition->_nbCluster;
    oInitPartition = new XEMPartition();
    oInitPartition->setDimension(sizeList, nbCluster);
    oInitPartition->_tabValue = new int64_t*[sizeList];
    for (i=0; i<sizeList; i++){
      oInitPartition->_tabValue[i] = new int64_t[nbCluster];
    }      
    // l'algo suivant peut �re optimis�(on fait eventuellement plusieurs fois la m�e chose)
    for (i=0; i<_nbSample; i++){
      for (k=0; k<nbCluster; k++)
        oInitPartition->_tabValue[correspondcenceOriginDataToReduceData[i]][k] = initPartition->_tabValue[i][k];
    }
    //oInitLabel->_complete = true;
  }
  
  if (knownPartition){
    nbCluster = knownPartition->_nbCluster;
    oKnownPartition = new XEMPartition();
    oKnownPartition->setDimension(sizeList, nbCluster);
    oKnownPartition->_tabValue = new int64_t*[sizeList];
    for (i=0; i<sizeList; i++)
      oKnownPartition->_tabValue[i] = new int64_t[nbCluster];
    // l'algo suivant peut �re optimis�(on fait eventuellement plusieurs fois la m�e chose)
    for (i=0; i<_nbSample; i++){
      for (k=0; k<nbCluster; k++)
        oKnownPartition->_tabValue[correspondcenceOriginDataToReduceData[i]][k] = knownPartition->_tabValue[i][k];
    }
    //oKnownLabel->_complete = true;
  }

  

  ///////////////////////////////////////////////
 /*cout<<endl<<"nb d'individus differents : "<<sizeList<<endl;
  cout<<"individu | knownLabel | initLabel | Poids  "<<endl<<endl;
  for (listIterator = listDiffIndiv.begin(); listIterator != listDiffIndiv.end(); listIterator++)
  cout<<(*listIterator)->val<<"   "<<(*listIterator)->weight<<endl;

  for (i=0; i<sizeList; i++){
  for (j=0; j<_pbDimension; j++){
  cout<<" "<<((XEMBinarySample*)data[i])->getDataValue(j)<<" ";
}
  cout<<"  | ";
  if (knownPartition){
  cout<<oKnownPartition->getGroupNumber(i)+1;
}// 0 if unknown
  cout<<"  | ";
  if (initPartition){
  cout<<oInitPartition->getGroupNumber(i)+1;
}// 0 if unknown
  cout<<"  | ";
  cout<<weight[i]<<" ";
  cout<<endl;
}
   
  for (i=0; i<_nbSample; i++){
  cout<<"correspondcenceOriginDataToReduceData["<<i<<"] : "<<correspondcenceOriginDataToReduceData[i]<<endl;
}*/
  //////////////////////////////////////////////


  XEMData * dataReduce = new XEMBinaryData(sizeList, _pbDimension, _tabNbModality, _weightTotal, data, weight);

  delete[] tabFactor;
  delete[] tabBaseMj;
  listIterator = listDiffIndiv.begin();


  while(! listDiffIndiv.empty()){
    TWeightedIndividual * pelem = * listDiffIndiv.begin();
    listDiffIndiv.pop_front();
    delete pelem;
  }



  return dataReduce;
}


//-------
// Verify
//-------
bool XEMBinaryData::verify()const{
  bool res = XEMData::verify();
  
  // others verif  ?
  // les valeurs sont bien comprises entre 1 et _tabModality[i] , ...
  return res;
}


//------------------
// Return tab value
//-----------------
int64_t * XEMBinaryData::getDataTabValue(int64_t idxSample) const{
  return ((XEMBinarySample*)_matrix[idxSample])->getTabValue();
}
