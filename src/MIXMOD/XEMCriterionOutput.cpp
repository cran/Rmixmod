/***************************************************************************
                             SRC/MIXMOD/XEMCriterionOutput.cpp  description
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
#include "XEMCriterionOutput.h"


//------------
// Constructor
//------------
XEMCriterionOutput::XEMCriterionOutput(){
  _criterionName = UNKNOWN_CRITERION_NAME;
  _value = 0;
  _error = noError;
}

//------------
// Constructor
//------------
XEMCriterionOutput::XEMCriterionOutput(XEMCriterionName criterionName){
  _value         = 0;;
  _criterionName = criterionName;
  _error         = noError;
}


//------------
// Constructor
//------------
XEMCriterionOutput::XEMCriterionOutput(XEMCriterionName criterionName,  double criterionValue, XEMErrorType criterionErrorType){
  _value         = criterionValue;
  _criterionName = criterionName;
  _error         = criterionErrorType;
}


//-----------
// Destructor
//-----------
XEMCriterionOutput::~XEMCriterionOutput(){
}



//----------
// edit Type
//----------
void XEMCriterionOutput::editType(ofstream & oFile){
  oFile<<"Criterion Name : ";
  if (_criterionName == BIC){
     oFile<<"BIC";
  }
  else if (_criterionName == CV){
     oFile<<"CV";
  }
  else if (_criterionName == DCV){
     oFile<<"DCV";
  }
  else if (_criterionName == NEC){
     oFile<<"NEC";
  }
  else if (_criterionName == ICL){
     oFile<<"ICL";
  }
  oFile<<endl<<"---------------"<<endl<<endl;
}



//-----------
// edit Value
//-----------
void XEMCriterionOutput::editValue(ofstream & oFile, bool text){
  if (text){
    oFile<<"\t\t\tCriterion Value : ";
    if (_error == noError){
      oFile<<_value<<endl<<endl;
    }
    else{
      oFile<<"numeric Error"<<endl<<endl;
    }
  }
  else{
    if (_error == noError){
      oFile<<_value<<endl<<endl;
    }
  }
}



//--------------------
// edit Type And Value
//--------------------
void XEMCriterionOutput::editTypeAndValue(ofstream & oFile){
  if (_criterionName == BIC){
    oFile<<"\t\t\tBIC ";
  }
  else if (_criterionName == CV ){
    oFile<<"\t\t\tCV ";
  }
  else if (_criterionName == DCV ){
    oFile<<"\t\t\tDCV ";
  }
  else if (_criterionName == NEC){
    oFile<<"\t\t\tNEC ";
  }
  else if (_criterionName == ICL){
    oFile<<"\t\t\tICL ";
  }

  oFile<<"Criterion Value : ";
  if (_error == noError){
    oFile<<_value<<endl<<endl;
  }
  else{
    //cout << "----<<<<<<<>>>>>>>>>>" << << endl;
    oFile<<"numeric Error"<<endl<<endl;
  }
}




