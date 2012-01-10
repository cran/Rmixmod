/*
 * GaussianOutputHandling.cpp
 *
 *  Created on: 14 juin 2011
 *      Author: aude
 */


#include "Conversion.h"
#include "GaussianOutputHandling.h"



GaussianOutputHandling::GaussianOutputHandling( XEMClusteringModelOutput* cMOutput
                                              , Rcpp::S4& xem
                                              )
                                              : OutputHandling(cMOutput, xem)
                                              , param_(dynamic_cast<XEMGaussianEDDAParameter const*>(cMOutput_->getParameterDescription()->getParameter()))                                              
{
  // get the number of variables
  nbVariable_ = param_->getPbDimension();
  // create parameter object
  Rcpp::S4 param(xem.slot("parameters"));
  // add proportions values
  param.slot("proportions") = Conversion::CVectorToRcppVector(nbCluster_,param_->getTabProportion());
  // add proportions values
  param.slot("mean") = Conversion::CMatrixToRcppMatrix(nbCluster_,nbVariable_,param_->getTabMean());
  // add variances
  getGaussianInfoCluster(param);
  // add parameters to the output list
  xem.slot("parameters") = param;
}

// destructor
GaussianOutputHandling::~GaussianOutputHandling()
{ }

void GaussianOutputHandling::getGaussianInfoCluster(Rcpp::S4& param)
{
  // define list of variance matrix
  Rcpp::GenericVector varianceMatrices(nbCluster_);
  /** Array with matrix variance for each class*/
  XEMMatrix** matrixVariance_ = param_->getTabSigma();

  for (int i=0; i<nbCluster_; i++)
  {
    varianceMatrices[i] = Conversion::CMatrixToRcppMatrix(nbVariable_,nbVariable_,matrixVariance_[i]->storeToArray());
  }
  // add variances to S4 object
  param.slot("variance") = varianceMatrices;
}
