/***************************************************************************
 SRC/MIXMOD/XEMLearnMain.h  description
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
#ifndef XEMLEARNMAIN_H
#define XEMLEARNMAIN_H

#include "XEMUtil.h"

// pre-declaration
class XEMInput;
class XEMLearnInput;
class XEMLearnOutput;
class XEMModel;

class XEMLearnMain{
  
public:
  
	/// Default constructor
	XEMLearnMain();
  
  /// Constructor
	XEMLearnMain(XEMLearnInput * input,  XEMLearnOutput * output = NULL);
	
	/// Destructor
	virtual ~XEMLearnMain();
  
	
	// Accessor
  XEMLearnOutput * getLearnOutput() const;
  
  // get Input
  XEMInput * getInput();
  
  /// Run method
  void run();
	void setOutputNull();
  
  private :
  // input container
  XEMLearnInput * _input;
  // output container
  XEMLearnOutput * _output;
};

inline XEMLearnOutput * XEMLearnMain::getLearnOutput() const{
  return _output;
}

inline void XEMLearnMain::setOutputNull()
{
  _output = NULL;
}



#endif
