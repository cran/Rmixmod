/***************************************************************************
                             SRC/MIXMOD/XEMOutput.h  description
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
#ifndef XEMOUTPUT_H
#define XEMOUTPUT_H


#include "XEMCondExe.h"
#include "XEMOldModelOutput.h"
#include "XEMDCVCriterion.h"
#include "XEMStrategy.h"
#include "XEMClusteringStrategy.h"
#include <iostream>

class XEMClusteringMain;
/**
	@brief Base class for Output(s)
	@author F Langrognet & A Echenim
  */
class XEMMain ;
  
class XEMOutput{

public:

	/// Default constructor
	XEMOutput();

  /// Constructor
  XEMOutput(XEMOldInput * input, XEMMain& xmain);
  
  /// Constructor
  XEMOutput(XEMClusteringMain & xmain);
	
	/// Destructor
	virtual ~XEMOutput();


  /// set error
  void setErrorMixmod(XEMErrorType errorType);
	
  /// edit Method in files
  void editFile(ofstream ** oFile);

  /// update
  void update(XEMSelection ** tabSelection, int64_t nbSelection, XEMEstimation ** tabEstimation, int64_t nbEstimation, int64_t nbModeltype);
	

  /// Number of estimation ( = nb of nbcluster * nb of models * nb of strategies)
  int64_t _nbEstimation;

  /// Number of best model
  int64_t _nbBestModel;

  /// condition of execution
  XEMCondExe * _condExe;

  /// Table of best model output by criterion (size : nbCriterionType = _nbSelection)
  XEMOldModelOutput ** _tabBestModelOutput;

  /// Table of XEMAllModelOuput (size : _nbEstimation)
  XEMOldModelOutput ** _tabAllModelOutput;
  
  /// DCVCriterion for DCVinfo.txt
  XEMDCVCriterion * _DCVCriterion;


  XEMOldModelOutput ** getTabAllModelOutput() const;
  int64_t getNbEstimation() const;
  
	private:
	//void getCriterionSize();
};

  inline XEMOldModelOutput ** XEMOutput::getTabAllModelOutput() const{
    return _tabAllModelOutput; 
  }

  inline int64_t XEMOutput::getNbEstimation() const{
    return _nbEstimation;
  }


#endif
