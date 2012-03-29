/***************************************************************************
                             SRC/MIXMOD/XEMParameter.cpp  description
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
#include "XEMParameter.h"
#include "XEMRandom.h"
#include "XEMModel.h"
#include "XEMData.h"


//------------
// Constructor
//------------
// Default constructor
XEMParameter::XEMParameter(){
	throw wrongConstructorType;
}



//------------
// Constructor
// called if USER initialisation
//------------
XEMParameter::XEMParameter(int64_t  iNbCluster, int64_t  iPbDimension, XEMModelType * iModelType){
  _modelType = iModelType;
  _nbCluster = iNbCluster;
  _pbDimension = iPbDimension;
  _tabProportion = new double[_nbCluster];
  for (int64_t  k=0; k<_nbCluster; k++){
		_tabProportion[k] = 1.0/_nbCluster;
	}
  _model = NULL;
  _filename = "";
  _format = FormatNumeric::defaultFormatNumericFile;
}



//------------
// Constructor
//------------
// called by XEMModel::XEMModel
// ! iModel isn't updated (iModel._param is this)
XEMParameter::XEMParameter(XEMModel * iModel, XEMModelType * iModelType){
  _modelType     = iModelType;
  _model         = iModel;
  _nbCluster     = _model->getNbCluster();
  _pbDimension   = _model->getData()->_pbDimension ;
	_tabProportion = new double[_nbCluster];
	
	// _tabProportion isn't computed because model->_tabNk isn't updated (=0)
  for (int64_t  k=0; k<_nbCluster; k++){
		_tabProportion[k] = 1.0/_nbCluster;
	}
  _filename = "";
  _format = FormatNumeric::defaultFormatNumericFile;
}




//------------
// Constructor
//------------
XEMParameter::XEMParameter(const XEMParameter * iParameter){
  _nbCluster      = iParameter->getNbCluster();
  _pbDimension    = iParameter->getPbDimension();
  _tabProportion  = copyTab(iParameter->getTabProportion(),_nbCluster);
  _model          = iParameter->getModel();
  _modelType      = iParameter->getModelType();
  _freeProportion = iParameter->getFreeProportion();
  _filename	= iParameter->getFilename();
  _format	= iParameter->getFormat();
}



//-----------
// Destructor
//-----------
XEMParameter::~XEMParameter(){

  if (_tabProportion){
    delete[] _tabProportion;
    _tabProportion = NULL;	
  }
}	

//---------------------
/// Comparison operator
//---------------------
bool XEMParameter::operator ==(const XEMParameter & param) const{
  if ( _pbDimension != param.getPbDimension() ) return false;
  if ( _nbCluster != param.getNbCluster() ) return false;
  if ( _freeProportion != param.getFreeProportion() ) return false;
  for (int64_t  k=0; k<_nbCluster; k++){
    if ( _tabProportion[k] != param.getTabProportion()[k] ) return false;
  }
  return true;
}


//------------------------
// reset to default values
//------------------------
void XEMParameter::reset(){
  for (int64_t  k=0; k<_nbCluster; k++){
		_tabProportion[k] = 1.0/_nbCluster;
	}
}




//-------------------
//generateRandomIndex
//-------------------
int64_t  XEMParameter::generateRandomIndex(bool * tabIndividualCanBeUsedForInitRandom, double * weight, double totalWeight){
	double rndWeight, sumWeight;
  int64_t  idxSample;

	/* Generate a random integer between 0 and _nbSample-1 */
	bool IdxSampleCanBeUsed = false;  // idxSample can be used
	while (!IdxSampleCanBeUsed){
		// get index of sample with weight //
    rndWeight = (int64_t)(totalWeight*rnd()+1);
		sumWeight = 0.0;
		idxSample = -1;
		while(sumWeight < rndWeight){
			idxSample++;
			sumWeight += weight[idxSample];
		}
		//cout<<"index tir� au hasard :"<<idxSample<<endl;
		IdxSampleCanBeUsed = tabIndividualCanBeUsedForInitRandom[idxSample];
	}
		// on indique que cet individu ne pourra pas �tre tir� au  hasard pour une autre classe
	tabIndividualCanBeUsedForInitRandom[idxSample] = false;
	//cout<<"choisi"<<endl;
	return idxSample;
}




//-----------------
//-----------------
// compute methods
//-----------------
//-----------------

//-----------------------
// compute tab proportion
//-----------------------
void XEMParameter::computeTabProportion(){
  int64_t  k;
  double * tabNk     = _model->getTabNk();
  double weightTotal = (_model->getData())->_weightTotal;

  if (_freeProportion){
    for (k=0; k<_nbCluster; k++)
      _tabProportion[k] = tabNk[k] / weightTotal;
  }
  else{
    for (k=0; k<_nbCluster; k++)
      _tabProportion[k] = 1.0 / _nbCluster;
  }

}



//-----------------------------------------
// computeTik when underflow
// -model->_tabSumF[i] pour ith sample = 0
// i : 0 ->_nbSample-1
//-----------------------------------------
void XEMParameter::computeTikUnderflow(int64_t  i, double ** tabTik){
  throw nonImplementedMethod;
}

