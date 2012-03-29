/***************************************************************************
                             SRC/MIXMOD/XEMBinaryEjParameter.cpp  description
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
#include "XEMBinaryEjParameter.h"
#include "XEMBinaryData.h"
#include "XEMBinarySample.h"
#include "XEMModel.h"
#include "XEMRandom.h"

//--------------------
// Default constructor
//--------------------
XEMBinaryEjParameter::XEMBinaryEjParameter(){
		throw wrongConstructorType;
}


//-------------------------------
// Constructor called by XEMModel
//-------------------------------
XEMBinaryEjParameter::XEMBinaryEjParameter(XEMModel * iModel, XEMModelType *  iModelType, int64_t * tabNbModality):XEMBinaryParameter(iModel, iModelType, tabNbModality){
	_scatter = new double[_pbDimension];
  for (int64_t j=0; j<_pbDimension; j++){
		_scatter[j] = 0;
	}
}


  

//-----------------
// copy Constructor
//-----------------
XEMBinaryEjParameter::XEMBinaryEjParameter(const XEMBinaryEjParameter * iParameter):XEMBinaryParameter(iParameter){
  _scatter = new double[_pbDimension];
  double * iScatter = iParameter->getScatter();
  for (int64_t j=0; j<_pbDimension; j++){
    _scatter[j] = iScatter[j];
  }


}


//------------------------
// reset to default values
//------------------------
void XEMBinaryEjParameter::reset(){ 
  for (int64_t j=0; j<_pbDimension; j++){
		_scatter[j] = 0;
	}
	XEMBinaryParameter::reset();
}
	
	
	
//---------
// clone 
//---------
XEMParameter * XEMBinaryEjParameter::clone() const{
  XEMBinaryEjParameter * newParam = new XEMBinaryEjParameter(this);
  return(newParam);
}



//-----------
// Destructor
//-----------
XEMBinaryEjParameter::~XEMBinaryEjParameter(){
  if (_scatter){
    delete [] _scatter;
  }
}


//---------------------
/// Comparison operator
//---------------------
bool XEMBinaryEjParameter::operator ==(const XEMBinaryEjParameter & param) const{
  if ( !XEMBinaryParameter::operator==(param) ) return false;
  for (int64_t j=0; j<_pbDimension; j++){
    if ( _scatter[j] != param.getScatter()[j] ) return false;
  }
  return true;
}


//-----------
// getFreeParameter
//-----------
int64_t XEMBinaryEjParameter::getFreeParameter() const{
  int64_t nbFreeParameter = _pbDimension;
  if (_freeProportion){
    nbFreeParameter += _nbCluster - 1;
  }
  return nbFreeParameter;
}




//-------
// getPdf
//-------
double XEMBinaryEjParameter::getPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];

  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[j];
    }
    else{
      bernPdf *= _scatter[j] / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}



//----------
// getLogPdf
//----------
long double XEMBinaryEjParameter::getLogPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  long double bernPdf = 0.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];

  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf += log( 1.0 - _scatter[j]);
    }
    else{
      bernPdf += log (_scatter[j] / (_tabNbModality[j] - 1.0));
    }
  }
  return bernPdf;
}

  
  
//-------
// getPdf
//-------
/* Compute normal probability density function
       for x vector and kCluster th cluster
  */
double XEMBinaryEjParameter::getPdf(XEMSample * x, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinarySample * binaryX = (XEMBinarySample*) x;

  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ? //
    if ( binaryX->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[j];
    }
    else{
      bernPdf *= _scatter[j] / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}

  


//--------------------
// getlogLikelihoodOne (one cluster)
//--------------------
double XEMBinaryEjParameter::getLogLikelihoodOne() const{ 
  int64_t i;
  int64_t j;
  double logLikelihoodOne = 0.0,  pdf,  * Scatter;
  Scatter = new double[_pbDimension];
  int64_t * Center     = new int64_t[_pbDimension];
  double * tabNbSampleInMajorModality = new double[_pbDimension];
  int64_t nbSample = _model->getNbSample();
  XEMBinaryData * data    = (XEMBinaryData*)(_model->getData());

  // Compute Center fo One cluster //
  getTabCenterIfOneCluster(Center,  tabNbSampleInMajorModality);

  // Compute Scatter for One cluster //
  for (j=0; j<_pbDimension; j++){
    //Scatter[j] = 1- (tabNbSampleInMajorModality[j] / data->_weightTotal);
    Scatter[j] = 1- ((tabNbSampleInMajorModality[j] + (1./_tabNbModality[j])) / (data->_weightTotal + 1));
  }
  
  // Compute the log-likelihood for one cluster (k=1) //
  for (i=0; i<nbSample; i++){
    pdf = computePdfOneCluster(data->_matrix[i], Center, Scatter, _tabNbModality);
    logLikelihoodOne += log(pdf) * data->_weight[i];
  }

  delete[] Center;
  delete[] Scatter;
  delete[] tabNbSampleInMajorModality;
  
  return logLikelihoodOne;
}




//----------------
// Compute scatter 
//----------------
void XEMBinaryEjParameter::computeScatter(){
  int64_t j, k;
  int64_t i;
  double ej; // nb d'individus prenant la modalit�majoritaire sur la variable j
  double ** tabCik   = _model->getTabCik();

  XEMBinaryData * data = (XEMBinaryData*)(_model->getData());
  XEMSample ** dataMatrix = data->getDataMatrix();
  XEMBinarySample * curSample;
  double totalWeight  = data->_weightTotal;
  int64_t nbSample = _model->getNbSample();
  
  for (j=0; j<_pbDimension; j++){
    ej = 0.0;
    for (k=0; k<_nbCluster; k++){
      for (i=0; i<nbSample; i++){
        curSample = (XEMBinarySample*)dataMatrix[i];
        if ( curSample->getDataValue(j) == _tabCenter[k][j] ){
            ej += (tabCik[i][k] * data->_weight[i]);
        }
      } // end for i
    } // end for k
    //_scatter[j] = 1 - (ej / totalWeight);
    _scatter[j] = 1 - ((ej + ((_nbCluster*1.)/_tabNbModality[j])) / (totalWeight + _nbCluster));
  } // end for j
  
  /*
Version issue directement des formules mais moins rapide :
  for (j=0; j<_pbDimension; j++){
  ej = 0.0;
  for (k=0; k<_nbCluster; k++){
  for (i=0; i<nbSample; i++){
  curSample = (XEMBinarySample*)dataMatrix[i];
  if ( curSample->getDataValue(j) == _tabCenter[k][j] ){
  if (tabZikKnown[i])
  ej += (tabZik[i][k] * data->_weight[i]);
  else
  ej += (tabTik[i][k] * data->_weight[i]);
}
} // end for i
  ej += 1./_tabNbModality[j];
} // end for k
    //_scatter[j] = 1 - (ej / totalWeight);
  _scatter[j] = 1 - (ej  / (totalWeight + _nbCluster));
} // end for j
  */

}


//--------------------------
// Compute random scatter(s)
//--------------------------
void XEMBinaryEjParameter::computeRandomScatter(){
  // tirage d'une valeur comprise entre 0 et 1/_tabNbModality[j] 
  for (int64_t j=0; j<_pbDimension; j++){
    _scatter[j] = rnd()/_tabNbModality[j];
  }
}




//---------------
//recopy scatter from iParam 
//---------------
// Note : iParam must be a XEMBinaryEjParameter*
void XEMBinaryEjParameter::recopyScatter(XEMParameter * iParam){
  if (typeid(*iParam) != typeid(*this)){
    throw badXEMBinaryParamterClass;
  } 
  double * iScatter = ((XEMBinaryEjParameter*)iParam)->getScatter();
  for( int64_t j=0; j<_pbDimension; j++){
    _scatter[j] = iScatter[j];
  }
}
  
 


//---------------
//create scatter from Scatter Ekjh 
//---------------
//on fait une moyenne
void XEMBinaryEjParameter::createScatter(double *** scatter){
  int64_t k,j,h;
  for (j=0; j<_pbDimension; j++){
    _scatter[j] = 0.0;
    for (k=0; k<_nbCluster; k++){
      h = _tabCenter[k][j];
      _scatter[j] += scatter[k][j][h-1];
    }
  _scatter[j] /= _nbCluster;
  }
}






//------------
// editScatter (for debug)
//------------
void XEMBinaryEjParameter::editScatter(int64_t k){
#ifdef DEBUG
  int64_t j,h;
  for (j=0; j<_pbDimension; j++){
    for (h=1; h<=_tabNbModality[j]; h++){
      if (h == _tabCenter[k][j]){ 
        cout<<"\t"<<_scatter[j]; 
      }
      else{
        cout<<"\t"<<_scatter[j]/(_tabNbModality[j]-1); 
      }
    }
    cout<<endl;
  }
#endif
}
  
  

// editScatter 
//------------
void XEMBinaryEjParameter::editScatter(ofstream & oFile, int64_t k, bool text){
  int64_t j,h;
  if(text){
    oFile<<"\t\t\tScattering : \n";
  }
  for (j=0; j<_pbDimension; j++){
    if(text){
      oFile<<"\t\t\t\t\t";;
    }
    for (h=1; h<=_tabNbModality[j]; h++){
      if (h == _tabCenter[k][j]){ 
        oFile<<_scatter[j]<<"  ";
      }
      else{
        oFile<<_scatter[j]/(_tabNbModality[j]-1)<<"  "; 
      }
    }
    oFile<<endl;
  }
}
  

// Read Scatter in input file
//---------------------------
void XEMBinaryEjParameter::inputScatter(ifstream & fi){
  throw internalMixmodError;
}

// Read Scatter in input containers
//---------------------------
void XEMBinaryEjParameter::inputScatter( double *** scatters ){
  throw internalMixmodError;
}

double *** XEMBinaryEjParameter::scatterToArray() const{
 int64_t k,j,h;
 double *** tabScatter = new double**[_nbCluster];
 for (k=0; k<_nbCluster; k++){
   tabScatter[k] = new double*[_pbDimension];
   for (j=0; j<_pbDimension; j++){
     tabScatter[k][j] = new double[_tabNbModality[j]];
    for (h=1; h<=_tabNbModality[j] ; h++){
      if (h == _tabCenter[k][j]){ 
        tabScatter[k][j][h-1] = _scatter[j]; 
      }
      else{
        tabScatter[k][j][h-1] = _scatter[j]/(_tabNbModality[j]-1); 
      }
    }
   }
 }
 return tabScatter;
}





