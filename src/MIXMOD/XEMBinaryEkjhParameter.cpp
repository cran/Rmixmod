/***************************************************************************
                             SRC/MIXMOD/XEMBinaryEkjhParameter.cpp  description
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
#include "XEMBinaryEkjhParameter.h"
#include "XEMBinaryData.h"
#include "XEMBinarySample.h"
#include "XEMModel.h"
#include "XEMRandom.h"

//--------------------
// Default constructor
//--------------------
XEMBinaryEkjhParameter::XEMBinaryEkjhParameter(){
	throw wrongConstructorType;
}


//-------------------------------
// Constructor called by XEMModel
//-------------------------------
XEMBinaryEkjhParameter::XEMBinaryEkjhParameter(XEMModel * iModel, XEMModelType *  iModelType, int64_t * tabNbModality):XEMBinaryParameter(iModel, iModelType, tabNbModality){
  _scatter = new double**[_nbCluster];

  int64_t k,j,h;
	for (k=0; k<_nbCluster; k++){
    _scatter[k] = new double*[_pbDimension];
    for (j=0; j<_pbDimension; j++){
			_scatter[k][j] = new double[_tabNbModality[j]];
			for (h=0; h<_tabNbModality[j]; h++){
				_scatter[k][j][h] = 0.0;
			}
    }
  }
}



//-----------------
// copy Constructor
//-----------------
XEMBinaryEkjhParameter::XEMBinaryEkjhParameter(const XEMBinaryEkjhParameter * iParameter):XEMBinaryParameter(iParameter){
  int64_t k,j,h;
  _scatter = new double**[_nbCluster];

  for (k=0; k<_nbCluster; k++){
    _scatter[k] = new double*[_pbDimension];
    for (j=0; j<_pbDimension; j++){
      _scatter[k][j] = new double[_tabNbModality[j]];
    }
  }
  double *** iScatter = iParameter->getScatter();
  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      for (h=0; h<_tabNbModality[j]; h++){
        _scatter[k][j][h] = iScatter[k][j][h];
      }
    }
  }
}




//------------
// Constructor
// called by XEMStrategyType if USER partition
//------------
XEMBinaryEkjhParameter::XEMBinaryEkjhParameter(int64_t iNbCluster, int64_t iPbDimension, XEMModelType * iModelType, int64_t * tabNbModality, std::string & iFileName):XEMBinaryParameter(iNbCluster,iPbDimension,iModelType,tabNbModality){	
  _scatter = new double**[_nbCluster];
  for (int64_t k=0; k<_nbCluster; k++){
    _scatter[k] = new double*[_pbDimension];
    for (int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = new double[_tabNbModality[j]];
    }
  }

  if (iFileName.compare("") != 0){
    std::ifstream paramFile(iFileName.c_str(), ios::in);
        if (! paramFile.is_open()){
        throw wrongParamFileName;
    }
      input(paramFile);
      paramFile.close();
    }

}

//------------
// Constructor
//------------
XEMBinaryEkjhParameter::XEMBinaryEkjhParameter( int64_t iNbCluster
                                              , int64_t iPbDimension
                                              , XEMModelType * iModelType
                                              , int64_t * tabNbModality
                                              , double * proportions
                                              , double **  centers
                                              , double *** scatters
                                              )
                                              : XEMBinaryParameter(iNbCluster,iPbDimension,iModelType,tabNbModality){	
  _scatter = new double**[_nbCluster];
  for (int64_t k=0; k<_nbCluster; k++){
    _scatter[k] = new double*[_pbDimension];
    for (int64_t j=0; j<_pbDimension; j++){
      _scatter[k][j] = new double[_tabNbModality[j]];
    }
  }
  input(proportions,centers,scatters);  
}


//---------
// clone 
//---------
XEMParameter * XEMBinaryEkjhParameter::clone() const{
  XEMBinaryEkjhParameter * newParam = new XEMBinaryEkjhParameter(this);
  return(newParam);
}




//-----------
// Destructor
//-----------
XEMBinaryEkjhParameter::~XEMBinaryEkjhParameter(){
  if (_scatter){
    for (int64_t k=0; k<_nbCluster; k++){
      for (int64_t j=0; j<_pbDimension; j++){
        delete [] _scatter[k][j];
       }
       delete [] _scatter[k];
    }
    delete [] _scatter;
  }
  _scatter = NULL;
}


//---------------------
/// Comparison operator
//---------------------
bool XEMBinaryEkjhParameter::operator ==(const XEMBinaryEkjhParameter & param) const{
  if ( !XEMBinaryParameter::operator==(param) ) return false;
  for (int64_t k=0; k<_nbCluster; k++){
    for (int64_t j=0; j<_pbDimension; j++){
      for (int64_t h=0; h<_tabNbModality[j]; h++){
        if ( _scatter[k][j][h] != param.getScatter()[k][j][h] ) return false;
      }
    }
  }
  return true;
}


//------------------------
// reset to default values
//------------------------
void XEMBinaryEkjhParameter::reset(){ 
  int64_t k,j,h;
	for (k=0; k<_nbCluster; k++){
		for (j=0; j<_pbDimension; j++){
			for (h=0; h<_tabNbModality[j]; h++){
				_scatter[k][j][h] = 0.0;
			}
		}
	}
	XEMBinaryParameter::reset();
}


//-----------
// getFreeParameter
//-----------
int64_t XEMBinaryEkjhParameter::getFreeParameter() const{
  int64_t nbFreeParameter = 0;
  for (int64_t j=0; j<_pbDimension; j++){
    nbFreeParameter += _tabNbModality[j] - 1;
  }
  nbFreeParameter *= _nbCluster;
  if (_freeProportion){
    nbFreeParameter += _nbCluster - 1;
  }
  return nbFreeParameter;
}




//-------
// getPdf
//-------
double XEMBinaryEkjhParameter::getPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];
  int64_t value;
  
  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    value = curSample->getDataValue(j);
    if ( value == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[kCluster][j][value-1];
    }
    else{
      bernPdf *= _scatter[kCluster][j][value-1];
    }
  }
  return bernPdf;
}

  

//----------
// getLogPdf
//----------
long double XEMBinaryEkjhParameter::getLogPdf(int64_t iSample, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 0.0;
  XEMBinaryData * data = (XEMBinaryData*)_model->getData();
  XEMBinarySample * curSample = (XEMBinarySample*)data->_matrix[iSample];
  int64_t value;
  
  for (j=0; j<_pbDimension; j++){
    // iSample have major modality ?//
    value = curSample->getDataValue(j);
    if ( value == _tabCenter[kCluster][j] ){
      bernPdf += log(1.0 - _scatter[kCluster][j][value-1]);
    }
    else{
      bernPdf += log(_scatter[kCluster][j][value-1]);
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
double XEMBinaryEkjhParameter::getPdf(XEMSample * x, int64_t kCluster) const{
  int64_t j;
  double bernPdf = 1.0;
  XEMBinarySample * binaryX = (XEMBinarySample*) x;
  int64_t value; 
  
  for (j=0; j<_pbDimension; j++){
    value = binaryX->getDataValue(j) ;
    // iSample have major modality ? //
    if ( value == _tabCenter[kCluster][j] ){
      bernPdf *= 1.0 - _scatter[kCluster][j][value-1];
    }
    else{
      bernPdf *= _scatter[kCluster][j][value-1];
    }
  }
  return bernPdf;
}

  


//--------------------
// getlogLikelihoodOne (one cluster)
//--------------------
double XEMBinaryEkjhParameter::getLogLikelihoodOne() const{ 
  int64_t i;
  int64_t j, h;
  double logLikelihoodOne = 0.0, pdf, ** Scatter;
  Scatter = new double*[_pbDimension];
  for (j=0; j<_pbDimension; j++){
    Scatter[j] = new double[_tabNbModality[j]];
  }
  int64_t * Center     = new int64_t[_pbDimension];
  double * tabNbSampleInMajorModality = new double [_pbDimension];
  double ** tabNbSamplePerModality = new double * [_pbDimension];
  for (j=0; j<_pbDimension; j++){
    tabNbSamplePerModality[j] = new double[_tabNbModality[j]];
  }
  int64_t nbSample = _model->getNbSample();
  XEMBinaryData * data    = (XEMBinaryData*)(_model->getData());

  // Compute Center fo One cluster //
  getTabCenterIfOneCluster(Center,  tabNbSampleInMajorModality, tabNbSamplePerModality);
  

  // Compute Scatter for One cluster //
  for (j=0; j<_pbDimension; j++){
    for (h=0; h<_tabNbModality[j]; h++){
      if (h+1 == Center[j]){
        //Scatter[j][h] = 1 - (tabNbSampleInMajorModality[j] / data->_weightTotal);
        Scatter[j][h] = 1 - (tabNbSampleInMajorModality[j] + (1./_tabNbModality[j]) ) / (data->_weightTotal + 1);
      }
      else{
        //Scatter[j][h] = tabNbSamplePerModality[j][h] / data->_weightTotal;
        Scatter[j][h] = (tabNbSamplePerModality[j][h]+ (1./_tabNbModality[j])) / (data->_weightTotal + 1);
      }
    }
  }
  
  // Compute the log-likelihood for one cluster (k=1) //
  //--------------------------------------------------//
  for (i=0; i<nbSample; i++){
    pdf = computePdfOneCluster(data->_matrix[i], Center, Scatter, _tabNbModality);
    logLikelihoodOne += log(pdf) * data->_weight[i];
  }

  delete[] Center;
  for (j=0; j<_pbDimension; j++){
    delete[] Scatter[j];
  }
  delete [] Scatter;
  for (j=0; j<_pbDimension; j++){
    delete[] tabNbSamplePerModality[j];
  }
  delete[] tabNbSamplePerModality;
  delete[] tabNbSampleInMajorModality;
  
  return logLikelihoodOne;
}



//----------------
// Compute scatter 
//----------------

void XEMBinaryEkjhParameter::computeScatter(){
  int64_t j, k, h;
  int64_t i;
  double valueScatter;
  double * tabNk = _model->getTabNk();
  double ** tabCik   = _model->getTabCik();

  XEMBinaryData * data = (XEMBinaryData*)(_model->getData());
  XEMSample ** dataMatrix = data->getDataMatrix();
  XEMBinarySample * curSample;
  int64_t nbSample = _model->getNbSample();
  int64_t value;
  
  for (k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      for (h=0; h<_tabNbModality[j]; h++){
        valueScatter = 0.0;
        for (i=0; i<nbSample; i++){
          curSample = (XEMBinarySample*)dataMatrix[i];
          value = curSample->getDataValue(j);
          if ( value == h+1 ){
              valueScatter += (tabCik[i][k] * data->_weight[i]);
          }
        }
        if (h+1 == _tabCenter[k][j]){
          //_scatter[k][j][h] = (tabNk[k] - valueScatter) / tabNk[k];
          _scatter[k][j][h] = 1 -(( valueScatter+ 1./_tabNbModality[j]) / (tabNk[k] + 1));
        }
        else{
          //_scatter[k][j][h] = valueScatter / tabNk[k];
          _scatter[k][j][h] = (valueScatter + 1./_tabNbModality[j]) / (tabNk[k] + 1);
        }
      }// end for h
    } // end for j
  } // end for k

}
 


//--------------------------
// Compute random scatter(s)
//--------------------------
void XEMBinaryEkjhParameter::computeRandomScatter(){
  for (int64_t k=0; k<_nbCluster; k++){
    for (int64_t j=0; j<_pbDimension; j++){
      double scatterKJOnMajorModality = rnd() / _tabNbModality[j];
      for (int64_t h=0; h<_tabNbModality[j]; h++){
        if (h+1 == _tabCenter[k][j]){// on est sur le centre       
          _scatter[k][j][h] = scatterKJOnMajorModality;
        }
        else{
          _scatter[k][j][h] = scatterKJOnMajorModality / (_tabNbModality[j]-1);
        }
      }
    }
  }
}



//---------------
//recopy scatter from iParam 
//---------------
// Note : iParam must be a XEMBinaryEkjhParameter*
void XEMBinaryEkjhParameter::recopyScatter(XEMParameter * iParam){
  if (typeid(*iParam) != typeid(*this)){
    throw badXEMBinaryParamterClass;
  }
  double *** iScatter = ((XEMBinaryEkjhParameter*)iParam)->getScatter();
  int64_t k,j,h;
  for(k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      for (h=0; h<_tabNbModality[j]; h++){
        _scatter[k][j][h] = iScatter[k][j][h];
      }
    }
  }
}
  

//---------------
//create scatter from Scatter Ekjh 
//---------------
//on fait une moyenne
void XEMBinaryEkjhParameter::createScatter(double *** scatter){
  int64_t k,j,h;
  for(k=0; k<_nbCluster; k++){
    for (j=0; j<_pbDimension; j++){
      for (h=0; h<_tabNbModality[j]; h++){
        _scatter[k][j][h] = scatter[k][j][h];
      }
    }
  }
}
 




//------------
// editScatter (for debug)
//------------
void XEMBinaryEkjhParameter::editScatter(int64_t k){
#ifdef DEBUG
  int64_t j,h;
  for (j=0; j<_pbDimension; j++){
    for (h=0; h<_tabNbModality[j]; h++){
      cout<<"\t"<<_scatter[k][j][h]; 
    }
    cout<<endl;
  }
#endif
}
  
  

// editScatter 
//------------
void XEMBinaryEkjhParameter::editScatter(std::ofstream & oFile, int64_t k, bool text){
  int64_t j,h;
  if(text){
    oFile<<"\t\t\tScattering : \n";
  }
  for (j=0; j<_pbDimension; j++){
    if (text){
      oFile<<"\t\t\t\t\t";;
    }
    for (h=0; h<_tabNbModality[j]; h++){
      oFile<<_scatter[k][j][h]<<"  ";
    }
    oFile<<endl;
  }
}
  

// Read Scatter in input file
//---------------------------
void XEMBinaryEkjhParameter::inputScatter(std::ifstream & fi){
  //Mise �jour de tabCenter, tabScatter et tabProportion
  int64_t j,k,h;

  // verify paramFile
  double tmp;
  int64_t cpt=0;
  fi.clear();
  fi.seekg(0, ios::beg);
  //while (!fi.eof()){
  while (fi>>tmp){ // retourne NULL si on est �la fin
    cpt++;
  }
  int64_t goodValue = _nbCluster;//proportions
  goodValue += _nbCluster * _pbDimension; // centers
  goodValue += _nbCluster * _totalNbModality; // scatters
  
  if(cpt != goodValue){
    throw errorInitParameter;
  }
  
  fi.clear();
  fi.seekg(0, ios::beg);
  
  for (k=0; k<_nbCluster; k++){
    // Proportions //
    fi >> _tabProportion[k];

    // Center  //
    for (j=0; j<_pbDimension; j++){
      fi >> _tabCenter[k][j];
    }

    // Scatter  //
    for (j=0; j<_pbDimension; j++){
      for (h=0; h<_tabNbModality[j]; h++){
        fi >> _scatter[k][j][h];
      }
    }
  } // end for k

}

// Read Scatter in input containers
//---------------------------
void XEMBinaryEkjhParameter::inputScatter( double *** scatters ){
    
  // Scatter  //
  for (int k=0; k<_nbCluster; k++){
    for (int j=0; j<_pbDimension; j++){
      for (int h=0; h<_tabNbModality[j]; h++){
        _scatter[k][j][h]=scatters[k][j][h];
      }
    }
  } // end for k
  
}

double*** XEMBinaryEkjhParameter::scatterToArray() const{
  double *** tabScatter = new double**[_nbCluster];
  int64_t k,j;
  for (k=0 ; k<_nbCluster;++k){
    tabScatter[k] = new double*[_pbDimension];
    for (j=0 ; j<_pbDimension ; ++j){
      tabScatter[k][j] = new double [_tabNbModality[j]];
      recopyTab<double>(_scatter[k][j], tabScatter[k][j],_tabNbModality[j] );
    }
  }

  return tabScatter;
}





