/***************************************************************************
                             SRC/MIXMOD/XEMOutputControler.h  description
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
#ifndef XEMOUTPUTCONTROLER_H
#define XEMOUTPUTCONTROLER_H

#include "XEMOutput.h"
#include "XEMUtil.h"

/**
	@brief Base class for Output(s) Controler
	@author F Langrognet & A Echenim
  */


class XEMOutputControler {

public:

  /// Default constructor
  XEMOutputControler();

  /// Constructor
  XEMOutputControler(XEMOutput * output);

  /// Destructor
  virtual ~XEMOutputControler();


  /** @brief Selector
      @param output Output to control
  */
  void setOutput(XEMOutput * output);

  /// editFile
  void editFile();
  
  void editErroMixmodFile(XEMErrorType error);


private :

  /// Number of output files
  int64_t _nbOutputFiles;

  /// Create empty output files
  void createEmptyFiles();

  /// Current output
  XEMOutput * _output;
  
  /// Table of output files
  ofstream * _tabFiles[maxNbOutputFiles];

  /// Table of output files types
  XEMOutputType _tabOutputTypes[maxNbOutputFiles];

};

#endif
