/***************************************************************************
                             SRC/MIXMOD/XEMInputControler.h  description
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
#ifndef XEMINPUTCONTROLER_H
#define XEMINPUTCONTROLER_H

#include "XEMOldInput.h"



/** @brief Base class for Input(s) Controler
    @author F Langrognet & A Echenim
*/

class XEMInputControler{

public:

  /// Default constructor
  XEMInputControler();

  /// Constructor
  XEMInputControler(XEMOldInput * input);

  /// Destructor
  virtual ~XEMInputControler();

  
  /** @brief run method
      @param fileName : the input file
  */
  void run(const string iFileName ="geyser.xem");

  /// Current Input
  XEMOldInput * _input;
};

#endif
