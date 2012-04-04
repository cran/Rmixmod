/***************************************************************************
                             SRC/MIXMOD/XEMGaussianData.cpp  description
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

#include "XEMGaussianData.h"
#include "XEMGaussianSample.h"
#include "XEMDataDescription.h"
#include "XEMWeightColumnDescription.h"
#include "XEMQuantitativeColumnDescription.h"
#include "XEMIndividualColumnDescription.h"

//-----------
//Constructor
//-----------
XEMGaussianData::XEMGaussianData(){
}



//------------
// Constructor
//------------
XEMGaussianData::XEMGaussianData(const XEMGaussianData & iData):XEMData(iData){
  int64_t i;
  XEMSample ** matrix = iData._matrix;

  _matrix = new XEMSample*[_nbSample];
  _yStore = new double* [_nbSample];

  for (i=0; i<_nbSample; i++){
    _matrix[i] = new XEMGaussianSample((XEMGaussianSample*)matrix[i]);
    _yStore[i] = ((XEMGaussianSample *)_matrix[i])->getTabValue() ;
  }
  _Inv2PiPow                = iData.getInv2PiPow();
  _pbDimensionLog2Pi        = iData.getPbDimensionLog2Pi();
  _halfPbDimensionLog2Pi    = _pbDimensionLog2Pi / 2.0 ;
  __tmpTabOfSizePbDimension = new double[_pbDimension];
  _deleteSamples            = true;
}



//------------
// Constructor
//------------
XEMGaussianData::XEMGaussianData(int64_t nbSample, int64_t pbDimension):XEMData(nbSample,pbDimension){
  int64_t i;
  _Inv2PiPow                =  1.0 / pow(2.0 * XEMPI, pbDimension/2.0 );
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI); 
  _halfPbDimensionLog2Pi    =  _pbDimensionLog2Pi  / 2.0;
  __tmpTabOfSizePbDimension = new double[_pbDimension];
  
  
  _matrix    = new XEMSample*[_nbSample];
  _yStore    = new double * [_nbSample];

  for (i=0; i<_nbSample; i++){
    _weight[i] = 1.0;
    _matrix[i] = new XEMGaussianSample(_pbDimension);
    _yStore[i] = ((XEMGaussianSample *)(_matrix[i]))->getTabValue();
  }
  _weightTotal = _nbSample;

}



//------------
// Constructor
//------------
XEMGaussianData::XEMGaussianData(int64_t nbSample, int64_t pbDimension,  double ** matrix):XEMData(nbSample,pbDimension){
   int64_t i;
  
   if (matrix == NULL) throw internalMixmodError;
  
  _Inv2PiPow                =  1.0 / pow(2.0 * XEMPI, pbDimension/2.0 );
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI); 
  _halfPbDimensionLog2Pi    =  _pbDimensionLog2Pi  / 2.0;
  __tmpTabOfSizePbDimension = new double[_pbDimension];
  
  _matrix    = new XEMSample*[_nbSample];
  _yStore    = new double * [_nbSample];

  for (i=0; i<_nbSample; i++){
    _weight[i] = 1.0;
    _matrix[i] = new XEMGaussianSample(_pbDimension, matrix[i]);
    _yStore[i] = ((XEMGaussianSample *)(_matrix[i]))->getTabValue();
  }
  _weightTotal = _nbSample;
}



//------------
// Constructor
//------------
XEMGaussianData::XEMGaussianData(int64_t nbSample, int64_t pbDimension, const std::string & dataFileName):XEMData(nbSample,pbDimension){
  int64_t i;

  _Inv2PiPow                =  1.0 / pow(2.0 * XEMPI, pbDimension/2.0 );
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI); 
  _halfPbDimensionLog2Pi    =  _pbDimensionLog2Pi  / 2.0;
  __tmpTabOfSizePbDimension = new double[_pbDimension];
  
  
  _matrix    = new XEMSample*[_nbSample];
  _yStore    = new double * [_nbSample];

  for (i=0; i<_nbSample; i++){
    _matrix[i] = new XEMGaussianSample(_pbDimension);
    _yStore[i] = ((XEMGaussianSample *)(_matrix[i]))->getTabValue();
  }

  std::ifstream dataStream((dataFileName).c_str(), ios::in);
  if (! dataStream.is_open()){
    throw wrongDataFileName;
  }
  input(dataStream);
  dataStream.close();
  _deleteSamples = true;
  _fileNameData = dataFileName;
}



//------------
// Constructor for dataReduce
//------------
XEMGaussianData::XEMGaussianData(int64_t nbSample, int64_t pbDimension, double weightTotal, XEMSample **& matrix, double * weight):XEMData(nbSample,pbDimension,weightTotal,weight){
  
    // 1/ (2 * pi)^(d/2)
  _Inv2PiPow                =  1.0 / pow(2.0 * XEMPI, pbDimension/2.0 );
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI); 
  _halfPbDimensionLog2Pi    =  _pbDimensionLog2Pi  / 2.0;
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI);
  __tmpTabOfSizePbDimension = new double[_pbDimension];

  _matrix = matrix;
  _yStore = new double *[nbSample];
  int64_t i;

  for (i=0; i<_nbSample; i++){
    _yStore[i] = ((XEMGaussianSample *)(_matrix[i]))->getTabValue();
  }
  _deleteSamples = true;
}



//------------
// Constructor (used in DCV context)
//------------
XEMGaussianData::XEMGaussianData(int64_t nbSample, int64_t pbDimension, XEMData * originalData, XEMCVBlock & block):XEMData(nbSample, pbDimension){

  XEMGaussianData * origData = (XEMGaussianData *)(originalData);
  XEMSample ** origMatrix = origData->_matrix ; 
  
    // 1/ (2 * pi)^(d/2)
  _Inv2PiPow                =  1.0 / pow(2.0 * XEMPI, pbDimension/2.0 );
  _pbDimensionLog2Pi        =  pbDimension  * log(2.0 * XEMPI);
  _halfPbDimensionLog2Pi    =  _pbDimensionLog2Pi  / 2.0;
  __tmpTabOfSizePbDimension = new double[_pbDimension];
  _deleteSamples = false;
  
 
  _weightTotal = block._weightTotal;
  _matrix = new XEMSample*[_nbSample] ;  
  for (int64_t i=0; i<_nbSample; i++){
    _matrix[i] = origMatrix[block._tabWeightedIndividual[i].val];
    //cout<<"ind : "<<block._tabWeightedIndividual[i].val;
    _weight[i] = block._tabWeightedIndividual[i].weight;
    //cout<<" - weight : "<<block._tabWeightedIndividual[i].weight<<endl;
  }
  
  _yStore = new double *[nbSample];
  for (int64_t j=0; j<_nbSample; j++){
    _yStore[j] = ((XEMGaussianSample *)(_matrix[j]))->getTabValue();
  }
}




//----------
//Destructor
//----------
XEMGaussianData::~XEMGaussianData(){
  int64_t i;
  if (_matrix){
  
    if(_deleteSamples){
      for (i=0; i<_nbSample; i++){
        delete _matrix[i];
        _matrix[i] = NULL;
      }
    }
    
    delete[] _matrix;
    _matrix = NULL;
  }
  if(_yStore){
    delete[] _yStore;
    _yStore = NULL;
  }
  if(__tmpTabOfSizePbDimension){
    delete[] __tmpTabOfSizePbDimension;
    __tmpTabOfSizePbDimension = NULL;
  }

}



//-----------
// Clone data
//-----------
XEMData * XEMGaussianData::clone() const{
  XEMGaussianData * newData = new XEMGaussianData(*this);
  return(newData);
}



//------------------
// Clone data matrix
//------------------
XEMSample ** XEMGaussianData::cloneMatrix(){
  int64_t i;

  XEMSample ** newMatrix = new XEMSample*[_nbSample];
  for (i=0; i<_nbSample; i++){
    newMatrix[i] = new XEMGaussianSample((XEMGaussianSample*)_matrix[i]);
  }

  return newMatrix;
}



//------
// input
//------
void XEMGaussianData:: input(std::ifstream & fi){

  int64_t j;
  int64_t i;
  double ** p_y;
  double *  p_y_i;
    
  p_y = _yStore;
  for(i=0; i<_nbSample; i++){
    p_y_i = *p_y;
    for(j=0; j<_pbDimension; j++){
      if (fi.eof()){
      	throw endDataFileReach;	
      }
      fi >> p_y_i[j];
    }
    _weight[i] = 1.0;
    p_y++;
  }
  _weightTotal = _nbSample;
  
}



//------
// input
//------
void XEMGaussianData::input(const XEMDataDescription & dataDescription){
  _fileNameData  = dataDescription.getFileName();
  
  _weightTotal = 0;
  std::ifstream fi((_fileNameData).c_str(), ios::in);
  if (! fi.is_open()){
    throw wrongDataFileName;
  }
  int64_t j;
  int64_t i;
  double tmp;
  double ** p_y;
  double *  p_y_i;
  
  p_y = _yStore;
  int64_t g=0;
  for(i=0; i<_nbSample; i++){
    _weight[i] = 1.0;
    p_y_i = *p_y;
    g=0;
    for(j=0; j<dataDescription.getNbColumn(); j++){
      if (fi.eof()){
        throw endDataFileReach; 
      }
      if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMQuantitativeColumnDescription)){
        fi >> p_y_i[g];
        g++;
        //cout<<p_y_i[j]<<endl;
      }
      else{
        if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMWeightColumnDescription)){
          fi>>_weight[i];
        }
        else{
          if (typeid(*(dataDescription.getColumnDescription(j)))==typeid(XEMIndividualColumnDescription)){
            std::string stringTmp;
            fi>>stringTmp;
            //cout<<stringTmp<<endl;
          }
          else{ 
            fi>>tmp; // on avance !
          }
        }
      }
    }// end j
    p_y++;
    _weightTotal += _weight[i];
  }// end i
}



// output
//-------
void XEMGaussianData:: output(std::ostream & fo){
#ifdef VERBOSE
 cout<<"Sample size: "<<_nbSample << endl;
 cout<<"  Dimension: "<<_pbDimension << endl ;
#endif
 editTab(_yStore,_nbSample,_pbDimension,fo," ","");
 
}


bool XEMGaussianData::verify()const{
  bool res = XEMData::verify();
  
  // others verif  ?
  return res;
}
