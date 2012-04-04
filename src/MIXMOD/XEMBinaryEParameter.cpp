/***************************************************************************
                             SRC/MIXMOD/XEMBinaryEParameter.cpp  description
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
#include "XEMBinaryEParameter.h"
#include "XEMBinaryData.h"
#include "XEMBinarySample.h"
#include "XEMModel.h"
#include "XEMRandom.h"

//--------------------
// Default constructor
//--------------------
XEMBinaryEParameter::XEMBinaryEParameter(){
	throw wrongConstructorType;
}


//-------------------------------
// Constructor called by XEMModel
//-------------------------------
XEMBinaryEParameter::XEMBinaryEParameter(XEMModel * iModel, XEMModelType  * iModelType, int64_t * tabNbModality):XEMBinaryParameter(iModel, iModelType, tabNbModality){
  _scatter = 0;

}



//-----------------
// copy Constructor
//-----------------
XEMBinaryEParameter::XEMBinaryEParameter(const XEMBinaryEParameter * iParameter):XEMBinaryParameter(iParameter){
  _scatter = iParameter->getScatter();
}



//---------
// clone 
//---------
XEMParameter * XEMBinaryEParameter::clone() const{
  XEMBinaryEParameter * newParam = new XEMBinaryEParameter(this);
  return(newParam);
}




//-----------
// Destructor
//-----------
XEMBinaryEParameter::~XEMBinaryEParameter(){
}


//---------------------
/// Comparison operator
//---------------------
bool XEMBinaryEParameter::operator ==(const XEMBinaryEParameter & param) const{
  if ( !XEMBinaryParameter::operator==(param) ) return false;
  if ( _scatter != param.getScatter() ) return false;
  return true;
}


//------------------------
// reset to default values
//------------------------
void XEMBinaryEParameter::reset(){ 
	_scatter = 0.0;
	XEMBinaryParameter::reset();
}

//-----------
// getFreeParameter
//-----------
int64_t XEMBinaryEParameter::getFreeParameter() const{
  int64_t nbFreeParameter = 1;
  if (_freeProportion){
    nbFreeParameter += _nbCluster - 1;
  }
  return nbFreeParameter;
}



//-------
// getPdf
//-------
double XEMBinaryEParameter::getPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];


  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter;
    }
    else{
      bernPdf *= _scatter / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}


//----------
// getLogPdf
//----------
long double XEMBinaryEParameter::getLogPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 0.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];


  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf += log(1.0 - _scatter);
    }
    else{
      bernPdf += log(_scatter / (_tabNbModality[j] - 1.0));
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
double XEMBinaryEParameter::getPdf(XEMSample * x, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinarySample * binaryX = (XEMBinarySample*) x;

  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ? //
    if ( binaryX->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter;
    }
    else{
      bernPdf *= _scatter / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}

  


//--------------------
// getlogLikelihoodOne (one cluster)
//--------------------
double XEMBinaryEParameter::getLogLikelihoodOne() const{ 
  int64_t i;
  int64_t j;
  double logLikelihoodOne = 0.0,  value, pdf, Scatter;
  int64_t *  Center     = new int64_t[_pbDimension];
  double* tabNbSampleInMajorModality = new double[_pbDimension];
  int64_t nbSample = _model->getNbSample();
  XEMBinaryData * data    = (XEMBinaryData*)(_model->getData());

  // Compute Center fo One cluster //
  getTabCenterIfOneCluster(Center,  tabNbSampleInMajorModality);

  // Compute Scatter for One cluster //
  value = 0.0;
  for (j=0; j<_pbDimension; j++){
    //value += tabNbSampleInMajorModality[j];
    value += tabNbSampleInMajorModality[j] + 1./_tabNbModality[j];
  }
  Scatter = 1 - ( value / ((data->_weightTotal +1 ) * _pbDimension));

  // Compute the log-likelihood for one cluster (k=1) //
  //--------------------------------------------------//
  for (i=0; i<nbSample; i++){
    pdf = computePdfOneCluster(data->_matrix[i], Center, Scatter, _tabNbModality);
    logLikelihoodOne += log(pdf) * data->_weight[i];
  }

  delete[] Center;
  delete[] tabNbSampleInMajorModality;
  
  return logLikelihoodOne;
}




//----------------
// Compute scatter 
//----------------
void XEMBinaryEParameter::computeScatter(){
  int64_t j, k;
  int64_t i;
  double ** tabCik   = _model->getTabCik();
  
  XEMBinaryData * data = (XEMBinaryData*)(_model->getData());
  XEMSample ** dataMatrix = data->getDataMatrix();
  XEMBinarySample * curSample;
  double totalWeight  = data->_weightTotal;
  int64_t nbSample = _model->getNbSample();

  double e = 0.0; // nb d'individus prenant la modalit�majoritaire (pour toutes les classes, pour toutes les variables)
  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      for (i=0; i<nbSample; i++){
        curSample = (XEMBinarySample*)dataMatrix[i];
        if ( curSample->getDataValue(j) == _tabCenter[k][j] ){
            e += (tabCik[i][k] * data->_weight[i]);
        }
      } // end for i
      e += 1./_tabNbModality[j];
    } // end for j
  } // end for k
  //_scatter = 1- (e / (totalWeight *_pbDimension));
  _scatter = 1- (e / ((totalWeight + _nbCluster)*_pbDimension));

}

//----------------------------------------------------
// Compute scatter(s)  as if there was only one cluster
//---------------------------------------------------
/*void XEMBinaryEParameter::computeScatterIfOneCluster(double totalWeight, double * tabNbSampleInMajorModality, double ** tabNbSamplePerModality){
  double value = 0.0;
  for (int64_t j=0; j<_pbDimension; j++){
    cout<<"tabNbSampleInMajorModality[j] : "<<tabNbSampleInMajorModality[j]<<endl;
    value += tabNbSampleInMajorModality[j];
}
  _scatter = 1-  (value / (totalWeight * _pbDimension));
}
*/


//---------------------------
// Compute random scatter(s) 
//---------------------------
void XEMBinaryEParameter::computeRandomScatter(){
  int64_t minNbModality = _tabNbModality[0];
  for (int64_t j=1; j<_pbDimension; j++){
    if (_tabNbModality[j] < minNbModality){
      minNbModality = _tabNbModality[j];
    }
  }
  
  // tirage d'une valeur comprise entre 0 et 1/minNbModality
  _scatter = rnd()/minNbModality;
}



//---------------
//recopy scatter from iParam 
//---------------
// Note : iParam must be a XEMBinaryEParameter*
void XEMBinaryEParameter::recopyScatter(XEMParameter * iParam){
  if (typeid(*iParam) != typeid(*this)){
    throw badXEMBinaryParamterClass;
  }
  double iScatter = ((XEMBinaryEParameter*)iParam)->getScatter();
  _scatter = iScatter;
}
  
 


//---------------
//create scatter from Scatter Ekjh 
//---------------
//on fait une moyenne
void XEMBinaryEParameter::createScatter(double *** scatter){
  _scatter = 0.0;
  int64_t k,j,h;
  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      h = _tabCenter[k][j];
      _scatter += scatter[k][j][h-1];
    }
  }
  _scatter /= (_nbCluster*_pbDimension);
}




//------------
// editScatter (for debug)
//------------
void XEMBinaryEParameter::editScatter(int64_t k){
#ifdef DEBUG
  int64_t j,h;
  for (j=0; j<_pbDimension; j++){
    for (h=1; h<=_tabNbModality[j]; h++){
      if (h == _tabCenter[k][j]){ 
        cout<<"\t"<<_scatter; 
      }
      else{
        cout<<"\t"<<_scatter/(_tabNbModality[j]-1); 
      }
    }
    cout<<endl;
  }
#endif
}
  
  

// editScatter 
//------------
void XEMBinaryEParameter::editScatter(std::ofstream & oFile, int64_t k, bool text){
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
        oFile<<_scatter<<"  ";
      }
      else{
        oFile<<_scatter/(_tabNbModality[j]-1)<<"  "; 
      }
    }
    oFile<<endl;
  }
}
  

// Read Scatter in input file
//---------------------------
void XEMBinaryEParameter::inputScatter(std::ifstream & fi){
  throw internalMixmodError;
}

// Read Scatter in input containers
//---------------------------
void XEMBinaryEParameter::inputScatter( double *** scatters ){
  throw internalMixmodError;
}

double *** XEMBinaryEParameter::scatterToArray() const{
  double*** tabScatter = new double**[_nbCluster];
  int64_t k,j,h;
  for (k=0; k<_nbCluster; k++){
    tabScatter[k] = new double*[_pbDimension];
    for (j=0; j<_pbDimension; j++){
      tabScatter[k][j] = new double[_tabNbModality[j] ];
      for (h=1; h<=_tabNbModality[j]; h++){
        if (h == _tabCenter[k][j]){ 
          tabScatter[k][j][h-1] = _scatter;
        }
        else{
          tabScatter[k][j][h-1] = _scatter/(_tabNbModality[j]-1);
        }
      }
    }
  }

  return tabScatter;

}







