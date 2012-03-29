/***************************************************************************
                             SRC/MIXMOD/XEMClusteringMain.h  description
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
#ifndef XEMCLUSTERINGMMAIN_H
#define XEMCLUSTERINGMMAIN_H

#include <vector>
#include "XEMUtil.h"

// pre-declaration
class XEMModel;
class XEMClusteringInput;
class XEMClusteringOutput;

class XEMClusteringMain{

public:

	/// Default constructor
	XEMClusteringMain();

  /// Constructor
	XEMClusteringMain(XEMClusteringInput * cInput,  XEMClusteringOutput * output = NULL);
	
	/// Destructor
	virtual ~XEMClusteringMain();

	
	/** @brief Selector
      @return XEMClusteringOutput
  */
  XEMClusteringOutput * getClusteringOutput() const;
  
  // get Input
  XEMClusteringInput * getInput();
  
  /// Run method
  void run();
  
  // set output pointer as null
	void setOutputNull();
	
private :

  // Input 
  XEMClusteringInput * _input;
  // Output
  XEMClusteringOutput * _output; 
};

inline XEMClusteringOutput * XEMClusteringMain::getClusteringOutput() const{
  return _output;
}

inline void XEMClusteringMain::setOutputNull()
{
  _output = NULL;
}

//------------------------
// return pointer to Input
//------------------------
inline  XEMClusteringInput * XEMClusteringMain::getInput(){
  if (_input){
    return _input;
  }
  else{
    throw nullPointerError;
  }
}


#endif
