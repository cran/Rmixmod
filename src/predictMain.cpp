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
/** @file predictMain.cpp
 *  @brief In this file we implement the wrapper .
 **/

#include <vector>
#include <string>

#include "mixmod/Utilities/Util.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictMain.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictInput.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictOutput.h"
#include "mixmod/DiscriminantAnalysis/Predict/PredictModelOutput.h"
#include "mixmod/Kernel/IO/ParameterDescription.h"
#include "mixmod/Kernel/IO/LabelDescription.h"
#include "mixmod/Kernel/IO/Label.h"
#include "mixmod/Kernel/IO/ProbaDescription.h"
#include "mixmod/Kernel/IO/Proba.h"
#include "mixmod/Kernel/Parameter/GaussianParameter.h"
#include "mixmod/Kernel/IO/GaussianData.h"
#include "mixmod/Kernel/IO/BinaryData.h"
#include "mixmod/Matrix/Matrix.h"

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
  //Rcpp::StringVector XmlIn(mixmodPredict.slot("xmlIn"));
  //std::string xmlInput = "";
  //if(XmlIn.size()>0)  xmlInput = Rcpp::as< std::vector<std::string> >(XmlIn)[0];
  //Rcpp::StringVector XmlOut(mixmodPredict.slot("xmlOut"));
  //std::string xmlOutput = "";
  //if(XmlOut.size()>0)  xmlOutput = Rcpp::as< std::vector<std::string> >(XmlOut)[0];
  //std::string xmlInput = Rcpp::as<std::string>(XmlIn);
  //std::cout << "xmlIn:::::: " << XmlIn << std::endl;
  int trace = Rcpp::as<int>(mixmodPredict.slot("trace"));
  int massiccc = Rcpp::as<int>(mixmodPredict.slot("massiccc"));   
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
  XEM::ModelName modelName = XEM::StringToModelName(Rcpp::as<std::string>(RDAResult.slot("model")));
  
  // gaussian, binary or heterogeneous ?
  XEM::DataType dataType;
  if(Rcpp::as<std::string>(Rtype) == std::string("quantitative"))
    dataType = XEM::QuantitativeData;
  else if(Rcpp::as<std::string>(Rtype) == std::string("qualitative"))
    dataType = XEM::QualitativeData;
  else
    dataType = XEM::HeterogeneousData;
  // data descritor
  XEM::DataDescription * dataDescription;
  XEM::GaussianData * gData(0);
  XEM::BinaryData * bData(0);
  XEM::CompositeData * cData(0);
  XEM::ParameterDescription * paramDescription(0);
  
  switch (dataType) {
    case XEM::QuantitativeData:
    {
      /*===============================================*/
      /*  Create XEM gaussian data set and description */
      gData = Conversion::DataToXemGaussianData(RData);
      dataDescription = new XEM::DataDescription(gData);

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

      // create new instance of ParameterDescription
      paramDescription = new XEM::ParameterDescription((int64_t)nbCluster, (int64_t)nbVariable, modelName , proportions, means, variances);
      break;
    }
    case XEM::QualitativeData:
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
      dataDescription = new XEM::DataDescription(bData);
      // create new instance of ParameterDescription
      paramDescription = new XEM::ParameterDescription((int64_t)nbCluster, (int64_t)nbVariable, modelName , proportions, centers, scatters, factors);

      break;
    }
    case XEM::HeterogeneousData:{
      /*===============================================*/
      /*  Create XEM heterogeneous data set and description */
      Rcpp::NumericVector Rfactors(SEXP(RParameter.slot("factor")));
      // create factors from R parameter

      std::vector<int64_t> factors(Rfactors.size());
      for (unsigned int i=0; i<factors.size(); i++) factors[i] = Rfactors[i];
      cData = Conversion::DataToXemCompositeData(RData,Rfactors);
      dataDescription = new XEM::DataDescription(cData);
      // wrap proportions
      Rcpp::NumericVector Rproportions(RParameter.slot("proportions"));
      // create proportions from R parameter
      double * proportions = Conversion::RcppVectorToCArray(Rproportions);
      /*===============================================*/
      /* Initialize input parameters in MIXMOD for gaussian parameters        */
      // wrap gaussian parameters object
      Rcpp::S4 RParameterg(RParameter.slot("g_parameter"));
      // wrap means
      Rcpp::NumericMatrix Rmean(SEXP(RParameterg.slot("mean")));
      // create means from R parameter
      double ** means = Conversion::RcppMatrixToC2DArray(Rmean);
      // create variances from R parameter
      Rcpp::List Rvariance(RParameterg.slot("variance"));
      // wrap variances
      double *** variances = Conversion::RcppListOfMatrixToC3DArray(Rvariance);

      /*===============================================*/
      /* Initialize input parameters in MIXMOD         */
      // wrap binary parameters object
      Rcpp::S4 RParameterb(RParameter.slot("m_parameter"));
      // wrap centers
      Rcpp::NumericMatrix Rcenters(SEXP(RParameterb.slot("center")));
      // create centers from R parameter
      double ** centers = Conversion::RcppMatrixToC2DArray(Rcenters);
      // wrap factors
      Rcpp::NumericVector Rfactorsb(SEXP(RParameterb.slot("factor")));
      // create factors from R parameter
      std::vector<int64_t> factorsb(Rfactorsb.size());
      for (unsigned int i=0; i<factorsb.size(); i++) factorsb[i] = Rfactorsb[i];
      // create scatters from R parameter
      Rcpp::List Rscatters(RParameterb.slot("scatter"));
      // wrap scatters
      double *** scatters = Conversion::RcppListOfMatrixToC3DArray(Rscatters);
      paramDescription = new XEM::ParameterDescription(int64_t(nbCluster)
                                                     , int64_t(factorsb.size())
                                                     , int64_t(factors.size()-factorsb.size())
                                                     , modelName
                                                     , proportions
                                                     , centers
                                                     , scatters
                                                     , means
                                                     , variances
                                                     , factorsb);
      break;
    }
    default:
      break;
  }
  // create predict input
  XEM::PredictInput * pInput =  new XEM::PredictInput( dataDescription, paramDescription );
 
  /* XEM will check if the option are corrects (should be the case) */
  pInput->finalize();
  
  /*===============================================*/
  /* Computation of the estimates */
  // PredictMain
  XEM::PredictMain pMain(pInput);
  // pmain run
  try {
    pMain.run(XEM::IoMode::NUMERIC, trace, massiccc);
  } catch (XEM::Exception & e) {
    // add error
     mixmodPredict.slot("error") = e.what();
     if (dataDescription) delete dataDescription;
     if (gData) delete gData;
     if (bData) delete bData;
     if (cData) delete cData;

     // return final output
     return mixmodPredict;
  }

  /*===============================================*/
  // get XEMPredictModelOutput object from cMain
  XEM::PredictModelOutput * pMOutput = pMain.getPredictOutput()->getPredictModelOutput().front();
  
  if ( dynamic_cast<XEM::Exception&>(pMOutput->getStrategyRunError()) == XEM::NOERROR ){
    // add labels
    mixmodPredict.slot("partition") = Conversion::VectorToRcppVectorForInt(pMOutput->getLabelDescription()->getLabel()->getLabel());
    // add proba
    mixmodPredict.slot("proba") = Conversion::XEMMatrixToRcppMatrix(pMOutput->getProbaDescription()->getProba()->getProba());
  }
  // add error
  mixmodPredict.slot("error") = (pMOutput->getStrategyRunError()).what();
  
  // release memory
  if (dataDescription) delete dataDescription;
  if (gData) delete gData;
  if (bData) delete bData;
  
  // return final output
  return mixmodPredict;
  
  END_RCPP
  
}
