/***************************************************************************
                             SRC/MIXMOD/XEMLabel.h  description
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
#ifndef XEMLabel_H
#define XEMLabel_H
#include "XEMEstimation.h"
#include "XEMCVCriterion.h"



/** @brief Base class for Label(s)
    @author F Langrognet & A Echenim
*/

class XEMLabel{

public:

	/// Default constructor
	XEMLabel();
  
  /// Constructor
  XEMLabel(int64_t nbSample);

  /// Constructor
	XEMLabel(XEMEstimation * estimation);
  
  XEMLabel(const XEMLabel & iLabel);
	
  /// Destructor
	virtual ~XEMLabel();
  
  /// editProba
  void edit(ostream & stream);

  /// getProba
  int64_t * getTabLabel() const;
  
  /// getProba
  vector<int64_t> const & getLabel() const;
  
  /// set Label
  void setLabel(int64_t * label, int64_t nbSample);
  void setLabel(vector<int64_t> label);
  
  /// Selector
  int64_t  getNbSample() const;

  ///input stream
  void input(ifstream & flux, int64_t nbCluster);
  void input(const XEMLabelDescription & labelDescription);
  
private :

  /// Number of samples
  int64_t  _nbSample;

  /// dim : _nbSample *_nbCluster
  vector<int64_t> _label;

};


inline vector<int64_t> const & XEMLabel::getLabel() const{
  return _label;
}

inline int64_t  XEMLabel::getNbSample() const{
  return _nbSample;
}




#endif
