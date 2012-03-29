//
//  OutputHandling.cpp
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#include "Conversion.h"
#include "OutputHandling.h"

#include "MIXMOD/XEMModelOutput.h"

#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMLabel.h"

#include "MIXMOD/XEMProbaDescription.h"
#include "MIXMOD/XEMProba.h"

#include "MIXMOD/XEMBinaryEParameter.h"
#include "MIXMOD/XEMBinaryEkParameter.h"
#include "MIXMOD/XEMBinaryEjParameter.h"
#include "MIXMOD/XEMBinaryEkjParameter.h"
#include "MIXMOD/XEMBinaryEkjhParameter.h"
#include "MIXMOD/XEMGaussianEDDAParameter.h"
#include "MIXMOD/XEMParameterDescription.h"

#include "MIXMOD/XEMMatrix.h"

#include <algorithm>

// constructor
OutputHandling::OutputHandling( XEMModelOutput* MOutput, Rcpp::S4& xem, const bool isGaussian )
                              : MOutput_(MOutput)
                              , xem_(xem)
                              , nbCluster_((int)MOutput_->getNbCluster())       
{
  
  // add nbCluster value
  xem_.slot("nbCluster") = nbCluster_;
  // add model selected
  xem_.slot("model") = XEMModelNameToString(MOutput->getModelType().getModelName());  
  // add the error
  xem_.slot("error") = XEMErrorTypeToString(MOutput->getStrategyRunError());
  
  // fill other slot only if no error
  if ( MOutput->getStrategyRunError() == noError ){
    // add likelihood value
    xem_.slot("likelihood") = MOutput_->getLikelihood();
    
    // add parameters
    ( isGaussian ) ? setGaussianParameter() : setMultinomialParameter() ;
  }
}

// destructor
OutputHandling::~OutputHandling()
{ }


// set gaussian paramaters 
void OutputHandling::setGaussianParameter()
{
  // get pointer to gaussian parameters
  const XEMGaussianEDDAParameter * gParam = dynamic_cast<XEMGaussianEDDAParameter const *>(MOutput_->getParameterDescription()->getParameter());
  // get the number of variables
  nbVariable_ = gParam->getPbDimension();
  
  // create parameter object
  Rcpp::S4 param(xem_.slot("parameters"));
  
  // add proportions values
  param.slot("proportions") = Conversion::CVectorToRcppVector(nbCluster_,gParam->getTabProportion());
  
  // add proportions values
  param.slot("mean") = Conversion::CMatrixToRcppMatrix(nbCluster_,nbVariable_,gParam->getTabMean());
  
  // add variances
  // define list of variance matrix
  Rcpp::GenericVector varianceMatrices(nbCluster_);
  /** Array with matrix variance for each class*/
  XEMMatrix** matrixVariance = gParam->getTabSigma();
  // loop over clusters
  for (int i=0; i<nbCluster_; i++)
  {
    varianceMatrices[i] = Conversion::CMatrixToRcppMatrix(nbVariable_,nbVariable_,matrixVariance[i]->storeToArray());
  }
  // add variances to S4 object
  param.slot("variance") = varianceMatrices;

  // add parameters to the output list
  xem_.slot("parameters") = param;
}

// set multinomial parameters 
void OutputHandling::setMultinomialParameter()
{
  // get pointer to multinomial parameters
  const XEMBinaryParameter * bParam = static_cast<XEMBinaryParameter const *>(MOutput_->getParameterDescription()->getParameter());
  
  // get the number of variables
  nbVariable_ = bParam->getPbDimension();
  // create parameter object
  Rcpp::S4 param(xem_.slot("parameters"));
  
  // add proportions values
  param.slot("proportions") = Conversion::CVectorToRcppVector(nbCluster_,bParam->getTabProportion());
  
  // add means values
  param.slot("center") = Conversion::CMatrixToRcppMatrixForInt(nbCluster_,nbVariable_,bParam->getTabCenter());
  
  //add factor
  param.slot("factor") = Conversion::CVectorToRcppVectorForInt(nbVariable_,bParam->getTabNbModality());
  
  
  //-------------------
  // get scatter
  //-------------------
  // get pointer to scatter
  double *** scatter = bParam->scatterToArray();
  // get tab of modalities
  int64_t* tabNbModality = bParam->getTabNbModality();
  // get maximum number of modality
  int64_t max = *max_element(tabNbModality,tabNbModality+nbVariable_);
  
  //VectorMatrix of NumericMatrix
  Rcpp::GenericVector vectorOutput(nbCluster_);
  
  // loop over clusters
  for (int k=0; k<nbCluster_; k++){
    //NumericMatrix for matrix
    Rcpp::NumericMatrix matrixOutput(nbVariable_,max);
    // loop over variables
    for(int j=0; j<nbVariable_; j++){
      // loop over modalities
      for (int h=0; h<tabNbModality[j]; h++) {
        matrixOutput(j,h) = scatter[k][j][h];
      }
    }
    vectorOutput(k) = matrixOutput;
  }  
  // add scatters
  param.slot("scatter") = vectorOutput;
  
  // add parameters to the output list
  xem_.slot("parameters") = param;
}
