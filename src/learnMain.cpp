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
 copyright            : (C) MIXMOD Team - 2001-2013
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

#include "mixmod/Utilities/Util.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnMain.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnInput.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnOutput.h"
#include "mixmod/DiscriminantAnalysis/Learn/LearnModelOutput.h"
#include "mixmod/Kernel/IO/ParameterDescription.h"
#include "mixmod/Kernel/IO/LabelDescription.h"
#include "mixmod/Kernel/Parameter/GaussianParameter.h"
#include "mixmod/Kernel/IO/GaussianData.h"
#include "mixmod/Kernel/IO/BinaryData.h"
#include "mixmod/Kernel/IO/CompositeData.h"
#include "mixmod/Matrix/Matrix.h"

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
  //Rcpp::StringVector XmlIn(mixmodLearn.slot("xmlIn"));
  std::string xmlInput = "";
  //if(XmlIn.size()>0)  xmlInput = Rcpp::as< std::vector<std::string> >(XmlIn)[0];
  Rcpp::StringVector XmlOut(mixmodLearn.slot("xmlOut"));
  std::string xmlOutput = "";
  if(XmlOut.size()>0)  xmlOutput = Rcpp::as< std::vector<std::string> >(XmlOut)[0];
  //std::string xmlInput = Rcpp::as<std::string>(XmlIn);
  //std::cout << "xmlIn:::::: " << XmlIn << std::endl;
  int seed = Rcpp::as<int>(mixmodLearn.slot("seed"));
  int trace = Rcpp::as<int>(mixmodLearn.slot("trace"));
  int massiccc = Rcpp::as<int>(mixmodLearn.slot("massiccc"));    
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
  // gaussian, binary or heterogeneous ?
  XEM::DataType dataType;
  if(Rcpp::as<std::string>(Rtype) == std::string("quantitative"))
    dataType = XEM::QuantitativeData;
  else if(Rcpp::as<std::string>(Rtype) == std::string("qualitative"))
    dataType = XEM::QualitativeData;
  else
    dataType = XEM::HeterogeneousData;
  // data descritor
  XEM::DataDescription* dataDescription;
  XEM::GaussianData* gData(0);
  XEM::BinaryData* bData(0);
  XEM::CompositeData* cData(0);
  // wrap factor in Rcpp
  Rcpp::NumericVector Rfactor(mixmodLearn.slot("factor"));
  switch (dataType) {
    case XEM::QuantitativeData:
      /*===============================================*/
      /*  Create XEM gaussian data set and description */
      gData = Conversion::DataToXemGaussianData(RData);
      dataDescription = new XEM::DataDescription(gData);
      break;
    case XEM::QualitativeData:
      /*===============================================*/
      /* Create XEM binary data set and description */
      bData = Conversion::DataToXemBinaryData(RData, Rfactor);
      dataDescription = new XEM::DataDescription(bData);
      break;
    case XEM::HeterogeneousData:
      /*===============================================*/
      /* Create XEM binary data set and description */
      cData = Conversion::DataToXemCompositeData(RData, Rfactor);
      dataDescription = new XEM::DataDescription(cData);
      break;
    default:
      break;
  }
  
  /*===============================================*/
  /* Initialize input parameters in MIXMOD         */
  // create labels from R parameter
  std::vector<int64_t> labels(RPartition.size());
  for (unsigned int i=0; i<labels.size(); i++ ) labels[i]=RPartition[i];
  XEM::LabelDescription * labelDescription = new XEM::LabelDescription( labels.size(), labels );
  
  // create LearnInput
  XEM::LearnInput * lInput =  new XEM::LearnInput( dataDescription, labelDescription );
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
  // LearnMain
  XEM::LearnMain lMain(lInput);
  // lmain run
  lMain.run(seed, XEM::IoMode::NUMERIC, trace, massiccc);

  //std::cout << "Run finished" << std::endl;
  /*===============================================*/
  // get LearnOutput object from cMain
  XEM::LearnOutput * lOutput = lMain.getLearnOutput();
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
      LearnOutputHandling(lOutput->getLearnModelOutput(i), iResults, dataType, Rcriterion, labels);
      // add those results to the list
      resList[i] = Rcpp::clone(iResults);
    } 
    // add all results
    mixmodLearn.slot("results") = resList;
    
    // add the best results
    LearnOutputHandling(lOutput->getLearnModelOutput().front(), iResults, dataType, Rcriterion, labels);
    mixmodLearn.slot("bestResult") = Rcpp::clone(iResults);
  }
  
  // add error
  mixmodLearn.slot("error") = !lOutput->atLeastOneEstimationNoError();
  
  //std::cout << "Output finished" << std::endl;

  // release memory
  if (dataDescription) delete dataDescription;
  if (gData) delete gData;
  if (bData) delete bData;
  if (cData) delete cData;
  
  // return final output
  return mixmodLearn;
  
  END_RCPP
  
}
