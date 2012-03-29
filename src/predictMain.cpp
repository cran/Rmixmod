/**
 * Project:  Rmixmod
 * created on: 15 feb. 2012
 * Purpose:  Create the main for the mixmod call.
 * Author:   lebret, remi.lebret@math.univ-lille1.fr
 *
 **/

/***************************************************************************
 predictMain.cpp  description
 ------------------------
 copyright            : (C) MIXMOD Team - 2001-2012
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
 
 ***************************************************************************/
/** @file predictMain.cpp
 *  @brief In this file we implement the wrapper .
 **/

#include <vector>
#include <string>

#include "MIXMOD/XEMUtil.h"
#include "MIXMOD/XEMPredictMain.h"
#include "MIXMOD/XEMPredictInput.h"
#include "MIXMOD/XEMPredictOutput.h"
#include "MIXMOD/XEMPredictModelOutput.h"
#include "MIXMOD/XEMParameterDescription.h"
#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMLabel.h"
#include "MIXMOD/XEMProbaDescription.h"
#include "MIXMOD/XEMProba.h"
#include "MIXMOD/XEMGaussianParameter.h"
#include "MIXMOD/XEMGaussianData.h"
#include "MIXMOD/XEMBinaryData.h"
#include "MIXMOD/XEMMatrix.h"

#include <Rcpp.h>

#include "Conversion.h"
#include "InputHandling.h"


/** This is the main method doing the interface between R and Mixmod for Discriminant Analysis predict step.
 *  The method will create a matrix in the format of mixmod and copy the data
 *  inside
 *
 * @param xem R S4 object containing data and options for mixmod
 */
RcppExport SEXP predictMain( SEXP xem )
{
  BEGIN_RCPP
  
  // wrap S4 object
  Rcpp::S4 mixmodPredict(xem);
  // wrap data in Rcpp matrix
  Rcpp::NumericMatrix RData(SEXP(mixmodPredict.slot("data"))); // creates Rcpp matrix from SEXP
  // wrap type of data
  Rcpp::StringVector Rtype(mixmodPredict.slot("dataType"));
  // wrap S4 Results object
  Rcpp::S4 RDAResult(mixmodPredict.slot("classificationRule"));
  // wrap parameters object
  Rcpp::S4 RParameter(RDAResult.slot("parameters"));
  // get clusters in Rcpp
  int nbCluster = RDAResult.slot("nbCluster");
  // get nbVariable
  int nbVariable = mixmodPredict.slot("nbVariable");
  // wrap model name
  XEMModelName modelName = StringToXEMModelName(Rcpp::as<std::string>(RDAResult.slot("model")));
  
  // gaussian or binary ?
  bool isGaussian = Rcpp::as<std::string>(Rtype) == std::string("quantitative");

  // data descritor
  XEMDataDescription * dataDescription;
  XEMGaussianData * gData(0);
  XEMBinaryData * bData(0);
  XEMParameterDescription * paramDescription(0);
  
  if (isGaussian)
  {
    /*===============================================*/
    /*  Create XEM gaussian data set and description */
    gData = Conversion::DataToXemGaussianData(RData);
    dataDescription = new XEMDataDescription(gData);
    
    /*===============================================*/
    /* Initialize input parameters in MIXMOD         */
    // wrap proportions
    Rcpp::NumericVector Rproportions(RParameter.slot("proportions"));
    // create proportions from R parameter
    double * proportions = Conversion::RcppVectorToCArray(Rproportions);
    // wrap means
    Rcpp::NumericMatrix Rmean(SEXP(RParameter.slot("mean")));
    // create means from R parameter
    double ** means = Conversion::RcppMatrixToC2DArray(Rmean);
    // create variances from R parameter
    Rcpp::List Rvariance(RParameter.slot("variance"));
    // wrap variances
    double *** variances = Conversion::RcppListOfMatrixToC3DArray(Rvariance);

    // create new instance of XEMParameterDescription
    paramDescription = new XEMParameterDescription((int64_t)nbCluster, (int64_t)nbVariable, modelName , proportions, means, variances);
  }
  else
  {
    
    /*===============================================*/
    /* Initialize input parameters in MIXMOD         */
    // wrap proportions
    Rcpp::NumericVector Rproportions(RParameter.slot("proportions"));
    // create proportions from R parameter
    double * proportions = Conversion::RcppVectorToCArray(Rproportions);
    // wrap centers
    Rcpp::NumericMatrix Rcenters(SEXP(RParameter.slot("center")));
    // create centers from R parameter
    double ** centers = Conversion::RcppMatrixToC2DArray(Rcenters);
    // wrap factors
    Rcpp::NumericVector Rfactors(SEXP(RParameter.slot("factor")));
    // create factors from R parameter
    std::vector<int64_t> factors(Rfactors.size());
    for (unsigned int i=0; i<factors.size(); i++) factors[i] = Rfactors[i];
    // create scatters from R parameter
    Rcpp::List Rscatters(RParameter.slot("scatter"));
    // wrap scatters
    double *** scatters = Conversion::RcppListOfMatrixToC3DArray(Rscatters);
    
    /*===============================================*/
    /* Create XEM binary data set and description */
    bData = Conversion::DataToXemBinaryData(RData, Rfactors);
    dataDescription = new XEMDataDescription(bData);
    // create new instance of XEMParameterDescription
    paramDescription = new XEMParameterDescription((int64_t)nbCluster, (int64_t)nbVariable, factors, modelName , proportions, centers, scatters);
  }

  // create predict input
  XEMPredictInput * pInput =  new XEMPredictInput( dataDescription, paramDescription );
 
  /* XEM will check if the option are corrects (should be the case) */
  pInput->finalize();
  
  /*===============================================*/
  /* Computation of the estimates */
  // XEMMPredictMain
  XEMPredictMain pMain(pInput);
  // pmain run
  pMain.run();
  /*===============================================*/
  // get XEMPredictModelOutput object from cMain
  XEMPredictModelOutput * pMOutput = pMain.getPredictOutput()->getPredictModelOutput().front();
  
  if ( pMOutput->getStrategyRunError() == noError ){
    // add labels
    mixmodPredict.slot("partition") = Conversion::VectorToRcppVectorForInt(pMOutput->getLabelDescription()->getLabel()->getLabel());
    // add proba
    mixmodPredict.slot("proba") = Conversion::XEMMatrixToRcppMatrix(pMOutput->getProbaDescription()->getProba()->getProba());
  }
  // add error
  mixmodPredict.slot("error") = XEMErrorTypeToString(pMOutput->getStrategyRunError());
  
  // release memory
  if (dataDescription) delete dataDescription;
  if (gData) delete gData;
  if (bData) delete bData;
  
  // return final output
  return mixmodPredict;
  
  END_RCPP
  
}
