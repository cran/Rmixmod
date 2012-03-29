/***************************************************************************
                             SRC/MIXMOD/XEMLikelihoodOutput.h  description
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
#ifndef XEMLikelihoodOutput_H
#define XEMLikelihoodOutput_H

#include <fstream>
#include <stdint.h>

// pre-declaration
class XEMModel;

/** @brief Base class for Label(s)
    @author F Langrognet & A Echenim
*/

class XEMLikelihoodOutput{

public:

	/// Default constructor
  XEMLikelihoodOutput(double logLikelihood, double completeLogLikelihood, double entropy, int64_t nbFreeParam);

        XEMLikelihoodOutput();
  /// Constructor
	XEMLikelihoodOutput(XEMModel * model);

	/// Destructor
	virtual ~XEMLikelihoodOutput();
	
 /// Edit
 void edit(std::ofstream & oFile, bool text=false);

 
  ///Selector
  double getLogLikelihood() const;
  double getCompleteLogLikelihood() const;

private :

  /// Value of logLikelihood
  double _logLikelihood;

  /// Value of completed logLikelihood
  double _completeLogLikelihood;

  /// Value of entropyd
  double _entropy;

  /// Number of free parameter
  int64_t _nbFreeParam;

};

  inline double XEMLikelihoodOutput::getLogLikelihood() const{
    return _logLikelihood;
  }

  inline double XEMLikelihoodOutput::getCompleteLogLikelihood() const{
    return _completeLogLikelihood;
  }



#endif
