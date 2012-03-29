//
//  LearnOutputHandling.cpp
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#include "Conversion.h"
#include "LearnOutputHandling.h"

#include "MIXMOD/XEMLearnModelOutput.h"
#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMLabel.h"
#include "MIXMOD/XEMProbaDescription.h"
#include "MIXMOD/XEMProba.h"

// constructor
LearnOutputHandling::LearnOutputHandling( XEMLearnModelOutput* lMOutput
                                        , Rcpp::S4& xem
                                        , const bool isGaussian
                                        , Rcpp::CharacterVector const & Rcriterion
                                        , std::vector<int64_t> labels
                                        )
                                        : OutputHandling(lMOutput, xem, isGaussian)     
{
  // get criterion
  std::vector<std::string> criterionName = Rcpp::as< std::vector<std::string> >(Rcriterion);
  // add criterion name
  xem_.slot("criterion") = criterionName;
  
  // fill other slot only if no error
  if ( lMOutput->getStrategyRunError() == noError ){
    
    // declare a vector of criterion
    std::vector<double> criterionValue;
    // loop over criterion names
    for (unsigned int i=0; i<criterionName.size(); i++){
      // BIC criterion
      if (criterionName[i] == "BIC"){ 
        criterionValue.push_back(lMOutput->getCriterionOutput(BIC).getValue());
      }
      // CV Criterion
      else if(criterionName[i] == "CV"){ 
        criterionValue.push_back(1-lMOutput->getCriterionOutput(CV).getValue());
        xem_.slot("CVLabel") = Conversion::VectorToRcppVectorForInt(lMOutput->getCVLabel()->getLabel()->getLabel());
        // define a classification tab
        int64_t** tab = lMOutput->getCVLabel()->getLabel()->getClassificationTab(labels);
        xem_.slot("CVClassification") = Conversion::CMatrixToRcppMatrixForInt(nbCluster_, nbCluster_, tab);
        // release memory
        for ( int i=0; i<nbCluster_; i++ ) delete [] tab[i];
        delete [] tab;
      }
    }
    
    // add criterion value
    xem_.slot("criterionValue") = criterionValue;    
    
    // add partition
    // get labels size
    const int size = labels.size();
    Rcpp::NumericVector Rlabels(size);
    for ( int i=0; i<size; i++ ) Rlabels[i]=labels[i];
    xem_.slot("partition") = Rlabels;
    
    // add MAP values
    // define a classification tab
    int64_t** tab = lMOutput->getLabelDescription()->getLabel()->getClassificationTab(labels);
    xem_.slot("MAPClassification") = Conversion::CMatrixToRcppMatrixForInt(nbCluster_, nbCluster_, tab);
    // release memory
    for ( int i=0; i<nbCluster_; i++ ) delete [] tab[i];
    delete [] tab;
    // add MAP error rate
    xem_.slot("MAPErrorRate") = lMOutput->getLabelDescription()->getLabel()->getErrorRate(labels);
  }
}

// destructor
LearnOutputHandling::~LearnOutputHandling()
{ }
