/**
 * Project:  Rmixmod
 * created on: 4 avr. 2011
 * Purpose:  Create the main for the mixmod call.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/***************************************************************************
                             ClusteringMain.cpp  description
                             ------------------------
    copyright            : (C) MIXMOD Team - 2001-2003-2004-2005-2006-2007-2008-2009
    email                : mixmod@univ-fcomte.fr
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
/** @file ClusteringMain.cpp
 *  @brief In this file we implement the wrapper .
 **/

/*
 *
 ***** File "rcpp_hello_world.h"
 *
 *
 * #ifndef _Rmixmod_RCPP_HELLO_WORLD_H
 * #define _Rmixmod_RCPP_HELLO_WORLD_H
 *
 * #include <Rcpp.h>
 *
 * note : RcppExport is an alias to `extern "C"` defined by Rcpp.
 *
 * It gives C calling convention to the rcpp_hello_world function so that
 * it can be called from .Call in R. Otherwise, the C++ compiler mangles the
 * name of the function and .Call can't find it.
 *
 * It is only useful to use RcppExport when the function is intended to be called
 * by .Call. See the thread http://thread.gmane.org/gmane.comp.lang.r.rcpp/649/focus=672
 * on Rcpp-devel for a misuse of RcppExport
 *
 * RcppExport SEXP rcpp_hello_world() ;
 *
 * #endif
 *
 *
 ***** File "rcpp_hello_world.cpp"
 *
 * #include "rcpp_hello_world.h"
 *
 * SEXP rcpp_hello_world(){
 *   using namespace Rcpp ;
 *
 *   CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
 *   NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
 *   List z            = List::create( x, y ) ;
 *
 *   return z ;
 * }
 *
 */

#include <vector>
#include <string>

#include "MIXMOD/XEMUtil.h"
#include "MIXMOD/XEMClusteringMain.h"
#include "MIXMOD/XEMClusteringInput.h"
#include "MIXMOD/XEMClusteringOutput.h"
#include "MIXMOD/XEMClusteringModelOutput.h"
#include "MIXMOD/XEMParameterDescription.h"
#include "MIXMOD/XEMGaussianParameter.h"
#include "MIXMOD/XEMGaussianData.h"
#include "MIXMOD/XEMBinaryData.h"
#include "MIXMOD/XEMMatrix.h"

#include <Rcpp.h>

#include "Conversion.h"
#include "ClusteringInputHandling.h"
#include "ClusteringOutputHandling.h"


/** This is the main method doing the interface between R and Mixmod for Clustering.
 *  The method will create a matrix in the format of mixmod and copy the data
 *  inside
 *
 * @param xem R S4 object containing data and options for clustering
 */
RcppExport SEXP clusteringMain( SEXP xem )
{
BEGIN_RCPP

  // wrap S4 object
  Rcpp::S4 mixmodClustering(xem);
  // wrap data in Rcpp matrix
  Rcpp::NumericMatrix RData(SEXP(mixmodClustering.slot("data"))); // creates Rcpp matrix from SEXP
  // wrap clusters in Rcpp
  Rcpp::NumericVector RnbCluster(mixmodClustering.slot("nbCluster")); // keep a copy (as the classic version does)
  // wrap partition matrix in Rcpp matrix
  Rcpp::NumericVector RPartition(mixmodClustering.slot("knownLabels"));
  // wrap list algoOptions
  Rcpp::S4 RalgoOptions(mixmodClustering.slot("strategy"));
  // wrap criterion
  Rcpp::CharacterVector Rcriterion(mixmodClustering.slot("criterion"));
  // wrap models
  Rcpp::S4 Rmodel(mixmodClustering.slot("models"));
  // wrap type of data
  Rcpp::StringVector Rtype(mixmodClustering.slot("dataType"));
  // wrap weight in Rcpp
  Rcpp::NumericVector Rweight(mixmodClustering.slot("weight")); 

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
    Rcpp::NumericVector Rfactor(mixmodClustering.slot("factor")); 
    /*===============================================*/
    /* Create XEM binary data set and description */
     bData = Conversion::DataToXemBinaryData(RData, Rfactor);
     dataDescription = new XEMDataDescription(bData);
  }

  /*===============================================*/
  /*  Create XEM cluster array */
  std::vector<int64_t> clusterArray;
  Rcpp::NumericVector::iterator it;
  for( it = RnbCluster.begin(); it != RnbCluster.end(); ++it)
  { clusterArray.push_back((int64_t)(*it));}
  
  /*===============================================*/
  /* Initialize input parameters in MIXMOD         */
  // create XEMClusteringInput
  XEMClusteringInput * cInput =  new XEMClusteringInput(clusterArray, *dataDescription);
  
  // initialize the parameters using user defined values (see Algo.R)
  ClusteringInputHandling initInput(cInput, RalgoOptions);

  // initialize criterion name (BIC, AIC, ...)
  initInput.setCriterionName(Rcriterion);

  // initialize the model (gaussian_...)
  initInput.setModel(Rmodel);

  // set weight
  initInput.setWeight(Rweight);
  
  // set knownPartition
  initInput.setKnownPartition(RPartition);
  
  /* XEM will check if the option are corrects (should be the case) */
  cInput->finalize();
  
//  cInput->edit(std::cout);
//  std::cout << "Input finished" << std::endl;
  /*===============================================*/
  /* Computation of the estimates */
  // XEMClusteringMain
  XEMClusteringMain cMain(cInput);
  // xmain run
  cMain.run();
//  std::cout << "Run finished" << std::endl;
  /*===============================================*/
  // get XEMClusteringOutput object from cMain
  XEMClusteringOutput * cOutput = cMain.getClusteringOutput();
  // Treatment : sort with the first criterion (there is only one criterion)  
  cOutput->sort(cInput->getCriterionName(0));
//  std::cout << "Sort finished" << std::endl;
  
  
  /*===============================================*/
  /* get output parameters from MIXMOD             */
  
  if ( cOutput->atLeastOneEstimationNoError() )
  {
    // define a new MixmodResults object
    Rcpp::S4 iResults(mixmodClustering.slot("bestResult"));
    // create a list of results
    Rcpp::List resList(cOutput->getNbClusteringModelOutput());
    
    // loop over all estimation
    for ( int i=0; i<cOutput->getNbClusteringModelOutput(); i++ )
    {
      // create the output object for R
      ClusteringOutputHandling(cOutput->getClusteringModelOutput(i), iResults, isGaussian, Rcriterion);
      // add those results to the list
      resList[i] = Rcpp::clone(iResults);
    } 
    // add all results
    mixmodClustering.slot("results") = resList;
    
    // TODO: if no criterion...
    
    // add the best results
    ClusteringOutputHandling(cOutput->getClusteringModelOutput().front(), iResults, isGaussian, Rcriterion);
    mixmodClustering.slot("bestResult") = Rcpp::clone(iResults);
  }
  
  // add error
  mixmodClustering.slot("error") = !cOutput->atLeastOneEstimationNoError();
  
//  std::cout << "Output finished" << std::endl;
  // release memory
  if (dataDescription) delete dataDescription;
  if (gData) delete gData;
  if (bData) delete bData;
  
  // return final output
  return mixmodClustering;

END_RCPP

}
