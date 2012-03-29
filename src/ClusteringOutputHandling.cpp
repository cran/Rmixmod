//
//  ClusteringOutputHandling.cpp
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#include "Conversion.h"
#include "ClusteringOutputHandling.h"

#include "MIXMOD/XEMClusteringModelOutput.h"
#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMLabel.h"
#include "MIXMOD/XEMProbaDescription.h"
#include "MIXMOD/XEMProba.h"

// constructor
ClusteringOutputHandling::ClusteringOutputHandling( XEMClusteringModelOutput* cMOutput
                                                  , Rcpp::S4& xem
                                                  , const bool isGaussian
                                                  , Rcpp::CharacterVector const & Rcriterion
                                                  )
                                                  : OutputHandling(cMOutput, xem, isGaussian)    
{  
  // get criterion
  std::vector<std::string> criterionName = Rcpp::as< std::vector<std::string> >(Rcriterion);
  // add criterion name
  xem_.slot("criterion") = criterionName;
  
  // fill other slot only if no error
  if ( cMOutput->getStrategyRunError() == noError ){
    
    // declare a vector of criterion
    std::vector<double> criterionValue;
    // loop over criterion names
    for (unsigned int i=0; i<criterionName.size(); i++){
      // BIC criterion
      if (criterionName[i] == "BIC"){ 
        criterionValue.push_back(cMOutput->getCriterionOutput(BIC).getValue());
      }
      // ICL Criterion
      else if(criterionName[i] == "ICL"){ 
        criterionValue.push_back(cMOutput->getCriterionOutput(ICL).getValue());
      }
        // NEC Criterion
      else if(criterionName[i] == "NEC"){ 
        criterionValue.push_back(cMOutput->getCriterionOutput(NEC).getValue());
      }
    }
  
    // add criterion value
    xem_.slot("criterionValue") = criterionValue;
    
    // add labels
    xem_.slot("partition") = Conversion::VectorToRcppVectorForInt(MOutput_->getLabelDescription()->getLabel()->getLabel());
    
    //add proba
    xem_.slot("proba") = Conversion::XEMMatrixToRcppMatrix(MOutput_->getProbaDescription()->getProba()->getProba());
  }
}

// destructor
ClusteringOutputHandling::~ClusteringOutputHandling()
{ }
