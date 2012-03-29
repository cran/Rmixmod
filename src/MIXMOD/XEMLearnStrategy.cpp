/***************************************************************************
 SRC/MIXMOD/XEMPredictStrategy.cpp  description
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

#include "XEMLearnStrategy.h"
#include "XEMMAlgo.h"
#include "XEMMAPAlgo.h"
#include "XEMModel.h"



//-----------
//Constructor
//-----------
XEMLearnStrategy::XEMLearnStrategy()
{}



//-----------
//Copy constructor
//-----------
XEMLearnStrategy::XEMLearnStrategy(const XEMLearnStrategy & strategy)
{}


//----------
//Destructor
//----------
XEMLearnStrategy::~XEMLearnStrategy()
{}


//---
//run
//---
void XEMLearnStrategy::run(XEMModel * model){ 
  // 1rst step of Discriminant analysis : USER_PARTITION with complete partition and M algorithm : only M step is done
  XEMMAlgo MAlgo;
  MAlgo.run(model);
  // 2nd step : run the MAP Algo
  XEMMAPAlgo MAPAlgo;
  MAPAlgo.run(model);
}


// verify method
bool XEMLearnStrategy::verify()
{
  return true;
}
