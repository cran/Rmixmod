/***************************************************************************
                             SRC/MIXMOD/XEMBinaryEkjParameter.cpp  description
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
#include "XEMBinaryEkjParameter.h"
#include "XEMBinaryData.h"
#include "XEMBinarySample.h"
#include "XEMModel.h"
#include "XEMRandom.h"

//--------------------
// Default constructor
//--------------------
XEMBinaryEkjParameter::XEMBinaryEkjParameter(){
	throw wrongConstructorType;
}


//-------------------------------
// Constructor called by XEMModel
//-------------------------------
XEMBinaryEkjParameter::XEMBinaryEkjParameter(XEMModel * iModel, XEMModelType *  iModelType, int64_t * tabNbModality):XEMBinaryParameter(iModel, iModelType, tabNbModality){
  _scatter = new double*[_nbCluster];
  for (int64_t k=0; k<_nbCluster; k++){
		_scatter[k] = new double[_pbDimension];
    for (int64_t j=0; j<_pbDimension; j++){
				_scatter[k][j] = 0.0;
			}
  }
}


  
//-----------------
// copy Constructor
//-----------------
XEMBinaryEkjParameter::XEMBinaryEkjParameter(const XEMBinaryEkjParameter * iParameter):XEMBinaryParameter(iParameter){
  _scatter = new double*[_nbCluster];
  for (int64_t k=0; k<_nbCluster; k++){
    _scatter[k] = new double[_pbDimension];
  }
  double ** iScatter = iParameter->getScatter();
  for (int64_t k=0; k<_nbCluster; k++){
    for (int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = iScatter[k][j];
    }
  }

}



//---------
// clone 
//---------
XEMParameter * XEMBinaryEkjParameter::clone() const{
  XEMBinaryEkjParameter * newParam = new XEMBinaryEkjParameter(this);
  return(newParam);
}



//-----------
// Destructor
//-----------
XEMBinaryEkjParameter::~XEMBinaryEkjParameter(){
  if (_scatter){
    for (int64_t k=0; k<_nbCluster; k++){
      delete [] _scatter[k];
    }
  }
  delete [] _scatter;
  _scatter = NULL;
}



//---------------------
/// Comparison operator
//---------------------
bool XEMBinaryEkjParameter::operator ==(const XEMBinaryEkjParameter & param) const{
  if ( !XEMBinaryParameter::operator==(param) ) return false;
  for (int64_t k=0; k<_nbCluster; k++){
    for (int64_t j=0; j<_pbDimension; j++){
      if ( _scatter[k][j] != param.getScatter()[k][j] ) return false;
    }
  }
  return true;
}


//------------------------
// reset to default values
//------------------------
void XEMBinaryEkjParameter::reset(){ 
  int64_t k,j;
	for (k=0; k<_nbCluster; k++){
		for (j=0; j<_pbDimension; j++){
			_scatter[k][j] = 0.0;
		}
	}
	XEMBinaryParameter::reset();
}



//-----------
// getFreeParameter
//-----------
int64_t XEMBinaryEkjParameter::getFreeParameter() const{
  int64_t nbFreeParameter = _pbDimension * _nbCluster;
  if (_freeProportion){
    nbFreeParameter += _nbCluster - 1;
  }
  return nbFreeParameter;
}






//-------
// getPdf
//-------
double XEMBinaryEkjParameter::getPdf(int64_t iSample, int64_t kCluster) const{
//cout<<" XEMBinaryEkjParameter::getPdf"<<endl;
  int64_t j;
  double bernPdf = 1.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];

  for (j=0; j<_pbDimension; j++){
//cout<<"curSample :  "<<curSample->getDataValue(j)<<endl;
//cout<<" _tabCenter[kCluster][j] :  "<< _tabCenter[kCluster][j]<<endl;
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[kCluster][j];
    }
    else{
      bernPdf *= _scatter[kCluster][j] / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}

  

//----------
// getLogPdf
//----------
long double XEMBinaryEkjParameter::getLogPdf(int64_t iSample, int64_t kCluster) const{
//cout<<" XEMBinaryEkjParameter::getPdf"<<endl;
  int64_t j;
  double bernPdf = 0.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];

  for (j=0; j<_pbDimension; j++){
//cout<<"curSample :  "<<curSample->getDataValue(j)<<endl;
//cout<<" _tabCenter[kCluster][j] :  "<< _tabCenter[kCluster][j]<<endl;
    // iSample have major modality ?//
    if ( curSample->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf += log(1.0 - _scatter[kCluster][j]);
    }
    else{
      bernPdf += log(_scatter[kCluster][j] / (_tabNbModality[j] - 1.0));
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
double XEMBinaryEkjParameter::getPdf(XEMSample * x, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinarySample * binaryX = (XEMBinarySample*) x;

  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ? //
    if ( binaryX->getDataValue(j) == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[kCluster][j];
    }
    else{
      bernPdf *= _scatter[kCluster][j] / (_tabNbModality[j] - 1.0);
    }
  }
  return bernPdf;
}

  


//--------------------
// getlogLikelihoodOne (one cluster)
//--------------------
double XEMBinaryEkjParameter::getLogLikelihoodOne() const{ 
  int64_t i;
  int64_t j;
  double logLikelihoodOne = 0.0, pdf, * Scatter;
  Scatter = new double[_pbDimension];
  int64_t * Center     = new int64_t[_pbDimension];
  double * tabNbSampleInMajorModality = new double[_pbDimension];
  int64_t nbSample = _model->getNbSample();
  XEMBinaryData * data    = (XEMBinaryData*)(_model->getData());

  // Compute Center fo One cluster //
  getTabCenterIfOneCluster(Center,  tabNbSampleInMajorModality);

  // Compute Scatter for One cluster //
  for (j=0; j<_pbDimension; j++)
    //Scatter[j] = 1- (tabNbSampleInMajorModality[j] / data->_weightTotal);
    Scatter[j] = 1- ((tabNbSampleInMajorModality[j] + 1./_tabNbModality[j]) / (data->_weightTotal + 1));

  // Compute the log-likelihood for one cluster (k=1) //
  //--------------------------------------------------//
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
void XEMBinaryEkjParameter::computeScatter(){
  int64_t j, k;
  int64_t i;
  double ekj; // nb d'individus de la classe k prenant la modalit�maj sur la variable j
  double * tabNk = _model->getTabNk();
  double ** tabCik   = _model->getTabCik();

  XEMBinaryData * data = (XEMBinaryData*)(_model->getData());
  XEMSample ** dataMatrix = data->getDataMatrix();
  XEMBinarySample * curSample;
  int64_t nbSample = _model->getNbSample();

  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      ekj = 0.0;
      for (i=0; i<nbSample; i++){
        curSample = (XEMBinarySample*)dataMatrix[i];
        if ( curSample->getDataValue(j) == _tabCenter[k][j] ){
            ekj += (tabCik[i][k] * data->_weight[i]);
        }
      }
      //_scatter[k][j] = 1 - (ekj / tabNk[k]);
      _scatter[k][j] = 1 - ((ekj + 1./_tabNbModality[j]) / (tabNk[k] + 1));
    } // end for j
  } // end for k
}


//--------------------------------------------------
// Compute scatter(s)  as if there was only one cluster
//---------------------------------------------------
/*void XEMBinaryEkjParameter::computeScatterIfOneCluster(double totalWeight, double * tabNbSampleInMajorModality, double ** tabNbSamplePerModality){
  for (int64_t k=0; k<_nbCluster; k++){
    for (int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = 1 - (tabNbSampleInMajorModality[j] / totalWeight);
    }
  }
}*/



//--------------------------
// Compute random scatter(s)
//--------------------------
void XEMBinaryEkjParameter::computeRandomScatter(){
  for (int64_t k=0; k<_nbCluster; k++){
    // tirage d'une valeur comprise entre 0 et 1./_tabNbModality[j] 
    for (int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = rnd()/_tabNbModality[j];
    }
  }
}


//---------------
//recopy scatter from iParam 
//---------------
// Note : iParam must be a XEMBinaryEkjParameter*
void XEMBinaryEkjParameter::recopyScatter(XEMParameter * iParam){
  if (typeid(*iParam) != typeid(*this)){
    throw badXEMBinaryParamterClass;
  } 
  double ** iScatter = ((XEMBinaryEkjParameter*)iParam)->getScatter();
  for( int64_t k=0; k<_nbCluster; k++){
    for( int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = iScatter[k][j];
    }
  }
}
  
 


//---------------
//create scatter from Scatter Ekjh 
//---------------
void XEMBinaryEkjParameter::createScatter(double *** scatter){
  int64_t k,j,h;
  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      h = _tabCenter[k][j];
      _scatter[k][j] = scatter[k][j][h-1];
    }
  }
}





//------------
// editScatter (for debug)
//------------
void XEMBinaryEkjParameter::editScatter(int64_t k){
#ifdef DEBUG
  int64_t j,h;
  for (j=0; j<_pbDimension; j++){
    for (h=1; h<=_tabNbModality[j]; h++){
      if (h == _tabCenter[k][j]){ 
        cout<<"\t"<<_scatter[k][j]; 
      }
      else{
        cout<<"\t"<<_scatter[k][j]/(_tabNbModality[j]-1); 
      }
    }
    cout<<endl;
  }
#endif
}
  
  

// editScatter 
//------------
void XEMBinaryEkjParameter::editScatter(std::ofstream & oFile, int64_t k, bool text){
  int64_t j,h;
  if(text){
    oFile<<"\t\t\tScattering : \n";
  }
  for (j=0; j<_pbDimension; j++){
    if (text){
      oFile<<"\t\t\t\t\t";;
    }
    for (h=1; h<=_tabNbModality[j]; h++){
      if (h == _tabCenter[k][j]){ 
        oFile<<_scatter[k][j]<<"  ";
      }
      else{
        oFile<<_scatter[k][j]/(_tabNbModality[j]-1)<<"  "; 
      }
    }
    oFile<<endl;
  }
}
  

// Read Scatter in input file
//---------------------------
void XEMBinaryEkjParameter::inputScatter(std::ifstream & fi){
  throw internalMixmodError;
}

// Read Scatter in input containers
//---------------------------
void XEMBinaryEkjParameter::inputScatter( double *** scatters ){
  throw internalMixmodError;
}


 double *** XEMBinaryEkjParameter::scatterToArray() const{
   int64_t k,j,h;
   double *** tabScatter = new double**[_nbCluster];
   for (k=0; k<_nbCluster; k++){
     tabScatter[k] = new double*[_pbDimension];
     for (j=0; j<_pbDimension; j++){
       tabScatter[k][j] = new double[_tabNbModality[j]];
      for (h=1; h<=_tabNbModality[j] ; h++){
        if (h == _tabCenter[k][j]){ 
          tabScatter[k][j][h-1] = _scatter[k][j]; 
        }
        else{
          tabScatter[k][j][h-1] = _scatter[k][j]/(_tabNbModality[j]-1); 
        }
      }
     }
   }
   return tabScatter;
 }



