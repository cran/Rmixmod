/*
 * BinaryOutputHandling.cpp
 *
 *  Created on: 15 juin 2011
 *      Author: aude
 */

#include "Conversion.h"
#include "BinaryOutputHandling.h"

#include "./MIXMOD/XEMBinaryEParameter.h"
#include "./MIXMOD/XEMBinaryEkParameter.h"
#include "./MIXMOD/XEMBinaryEjParameter.h"
#include "./MIXMOD/XEMBinaryEkjParameter.h"
#include "./MIXMOD/XEMBinaryEkjhParameter.h"

BinaryOutputHandling::BinaryOutputHandling( XEMClusteringModelOutput* cMOutput
                                          , Rcpp::S4& xem
                                          )
                                          : OutputHandling(cMOutput, xem)
                                          , param_(static_cast<XEMBinaryParameter const*>(cMOutput_->getParameterDescription()->getParameter()))
{
  // get the number of variables
  nbVariable_ = param_->getPbDimension();
  // create parameter object
  Rcpp::S4 param(xem.slot("parameters"));
  // add proportions values
  param.slot("proportions") = Conversion::CVectorToRcppVector(nbCluster_,param_->getTabProportion());
  // add means values
  param.slot("center") = Conversion::CMatrixToRcppMatrixForInt(nbCluster_,nbVariable_,param_->getTabCenter());
  //add factor
  param.slot("factor") = Conversion::CVectorToRcppVectorForInt(nbVariable_,param_->getTabNbModality());
  //get cluster
  getBinaryInfoCluster(param);
  // add parameters to the output list
  xem.slot("parameters") = param;
  
}

// destructor
BinaryOutputHandling::~BinaryOutputHandling()
{ }

void BinaryOutputHandling::getBinaryInfoCluster(Rcpp::S4& param)
{
  XEMModelName model = cMOutput_->getModelType().getModelName();
  if( (model == Binary_pk_E) || (model == Binary_p_E) )
  {
    XEMBinaryEParameter const* underParam = dynamic_cast<XEMBinaryEParameter const*>(param_);
    param.slot("scatter") = underParam->getScatter();;
  }
  else if( (model == Binary_pk_Ek) || (model == Binary_p_Ek) )
  {
    XEMBinaryEkParameter const* underParam = dynamic_cast<XEMBinaryEkParameter const*>(param_);
    param.slot("scatter") = Conversion::CVectorToRcppVector(nbCluster_,underParam->getScatter());
  }
  else if( (model == Binary_pk_Ej) || (model == Binary_p_Ej) )
  {
    XEMBinaryEjParameter const* underParam = dynamic_cast<XEMBinaryEjParameter const*>(param_);
    param.slot("scatter") = Conversion::CVectorToRcppVector(nbCluster_,underParam->getScatter());
  }
  else if( (model == Binary_pk_Ekj) || (model == Binary_p_Ekj) )
  {
    XEMBinaryEkjParameter const* underParam = dynamic_cast<XEMBinaryEkjParameter const*>(param_);
    param.slot("scatter") = Conversion::CMatrixToRcppMatrix(nbCluster_,nbVariable_,underParam->getScatter());
  }
  if( (model == Binary_pk_Ekjh) || (model == Binary_p_Ekjh) )
  {
    XEMBinaryEkjhParameter const* underParam = dynamic_cast<XEMBinaryEkjhParameter const*>(param_);
    param.slot("scatter") = Conversion::CMatrixToVectorOfRcppMatrix(nbCluster_,nbVariable_, param_->getTabNbModality(), underParam->getScatter());
  }
}



