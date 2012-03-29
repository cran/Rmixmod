/***************************************************************************
 SRC/MIXMOD/XEMBinaryModel.h  description
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

#include "XEMBinaryModel.h"


/// Default constructor
XEMBinaryModel::XEMBinaryModel()
{}

/// Constructor
XEMBinaryModel::XEMBinaryModel( XEMBinaryModel * iModel )
                              : XEMModel(iModel)
                              , _correspondenceOriginDataToReduceData(iModel->getCorrespondenceOriginDataToReduceData())
{}

/// Constructor
XEMBinaryModel::XEMBinaryModel( XEMModelType * modelType
                              , int64_t  nbCluster
                              , XEMData *& data
                              , XEMPartition * knownPartition
                              , std::vector<int64_t> const & correspondenceOriginDataToReduceData
                              )
                              : XEMModel(modelType, nbCluster, data, knownPartition)
                              , _correspondenceOriginDataToReduceData(correspondenceOriginDataToReduceData)
{}

/// Destructor
XEMBinaryModel::~XEMBinaryModel()
{}
  
