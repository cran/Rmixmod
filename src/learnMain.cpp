/**
 * Project:  Rmixmod
 * created on: 06 feb. 2012
 * Purpose:  Create the main for the mixmod call.
 * Author:   lebret, remi.lebret@math.univ-lille1.fr
 *
 **/

/***************************************************************************
 learnMain.cpp  description
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
/** @file learnMain.cpp
 *  @brief In this file we implement the wrapper .
 **/

#include <vector>
#include <string>

#include "MIXMOD/XEMUtil.h"
#include "MIXMOD/XEMLearnMain.h"
#include "MIXMOD/XEMLearnInput.h"
#include "MIXMOD/XEMLearnOutput.h"
#include "MIXMOD/XEMLearnModelOutput.h"
#include "MIXMOD/XEMParameterDescription.h"
#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMGaussianParameter.h"
#include "MIXMOD/XEMGaussianData.h"
#include "MIXMOD/XEMBinaryData.h"
#include "MIXMOD/XEMMatrix.h"

#include <Rcpp.h>

#include "Conversion.h"
#include "InputHandling.h"
#include "LearnOutputHandling.h"


/** This is the main method doing the interface between R and Mixmod for Discriminant Analysis learning step.
 *  The method will create a matrix in the format of mixmod and copy the data
 *  inside
 *
 * @param xem R S4 object containing data and options for mixmod
 */
RcppExport SEXP learnMain( SEXP xem )
{
  BEGIN_RCPP
  
  // wrap S4 object
  Rcpp::S4 mixmodLearn(xem);
  // wrap data in Rcpp matrix
  Rcpp::NumericMatrix RData(SEXP(mixmodLearn.slot("data"))); // creates Rcpp matrix from SEXP
  // wrap partition matrix in Rcpp matrix
  Rcpp::NumericVector RPartition(mixmodLearn.slot("knownLabels"));
  // wrap criterion
  Rcpp::StringVector Rcriterion(mixmodLearn.slot("criterion"));
  // wrap models
  Rcpp::S4 Rmodel(mixmodLearn.slot("models"));
  // wrap type of data
  Rcpp::StringVector Rtype(mixmodLearn.slot("dataType"));
  // wrap weight in Rcpp
  Rcpp::NumericVector Rweight(mixmodLearn.slot("weight")); 
  // wrap nbCVBlock
  Rcpp::NumericVector RnbCVBlock(mixmodLearn.slot("nbCVBlocks"));
  // gaussian or binary ?
  bool isGaussian = Rcpp::as<std::string>(Rtype) == std::string("quantitative");

  // data descritor
  XEMDataDescription* dataDescription;
  XEMGaussianData* gData(0);
  XEMBinaryData* bData(0);
  if (isGaussian)
  {
    /*===============================================*/
    /*  Create XEM gaussian data set and description */
    gData = Conversion::DataToXemGaussianData(RData);
    dataDescription = new XEMDataDescription(gData);
  }
  else
  {
    // wrap factor in Rcpp
    Rcpp::NumericVector Rfactor(mixmodLearn.slot("factor")); 
    /*===============================================*/
    /* Create XEM binary data set and description */
    bData = Conversion::DataToXemBinaryData(RData, Rfactor);
    dataDescription = new XEMDataDescription(bData);
  }
  
  /*===============================================*/
  /* Initialize input parameters in MIXMOD         */
  // create labels from R parameter
  std::vector<int64_t> labels(RPartition.size());
  for (unsigned int i=0; i<labels.size(); i++ ) labels[i]=RPartition[i];
  XEMLabelDescription * labelDescription = new XEMLabelDescription( labels.size(), labels );
  
  // create XEMMLearnInput
  XEMLearnInput * lInput =  new XEMLearnInput( dataDescription, labelDescription );
  // initialize the parameters using user defined values (see Algo.R)
  InputHandling initInput(lInput);
  
  // initialize criterion name (BIC, CV, ...)
  initInput.setCriterionName(Rcriterion);
  
  // initialize the model (gaussian_...)
  initInput.setModel(Rmodel);
  
  // set weight
  initInput.setWeight(Rweight);
  
  // set nbCVBlocks
  lInput->setNbCVBlock(RnbCVBlock[0]);
  
  /* XEM will check if the option are corrects (should be the case) */
  lInput->finalize();
  
  //std::cout << "Input finished" << std::endl;
  /*===============================================*/
  /* Computation of the estimates */
  // XEMMLearnMain
  XEMLearnMain lMain(lInput);
  // lmain run
  lMain.run();
  //std::cout << "Run finished" << std::endl;
  /*===============================================*/
  // get XEMLearnOutput object from cMain
  XEMLearnOutput * lOutput = lMain.getLearnOutput();
  // Treatment : sort with the first criterion (there is only one criterion)
  lOutput->sort(lInput->getCriterionName(0));
  //std::cout << "Sort finished" << std::endl;
  
  /*===============================================*/
  /* get output parameters from MIXMOD             */
  if ( lOutput->atLeastOneEstimationNoError() )
  {
    // define a new MixmodDAResults object
    Rcpp::S4 iResults(mixmodLearn.slot("bestResult"));
    // create a list of results
    Rcpp::List resList(lOutput->getNbLearnModelOutput());
    
    // loop over all estimation
    for ( int i=0; i<lOutput->getNbLearnModelOutput(); i++ )
    {
      // create the output object for R
      LearnOutputHandling(lOutput->getLearnModelOutput(i), iResults, isGaussian, Rcriterion, labels);
      // add those results to the list
      resList[i] = Rcpp::clone(iResults);
    } 
    // add all results
    mixmodLearn.slot("results") = resList;
    
    // add the best results
    LearnOutputHandling(lOutput->getLearnModelOutput().front(), iResults, isGaussian, Rcriterion, labels);
    mixmodLearn.slot("bestResult") = Rcpp::clone(iResults);
  }
  
  // add error
  mixmodLearn.slot("error") = !lOutput->atLeastOneEstimationNoError();
  
  //std::cout << "Output finished" << std::endl;

  // release memory
  if (dataDescription) delete dataDescription;
  if (gData) delete gData;
  if (bData) delete bData;
  
  // return final output
  return mixmodLearn;
  
  END_RCPP
  
}
