//
//  InputHandling.cpp
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#ifndef INPUTHANDLING_H_
#define INPUTHANDLING_H_

#include <iostream>
#include <string>

#include <Rcpp.h>

// pre-declaration
class XEMInput;
class XEMPartition;

class InputHandling
{

  public:
    /// Default constructor
    InputHandling( XEMInput* cInput );
    
    /// Destructor
    virtual ~InputHandling();
  
    ///setCriterionName
    void setCriterionName(Rcpp::CharacterVector & criterion);
  
    /**  Add Models to estimate. The default model @c defaultGaussianModelName
     *  is set by default and will not be removed from the lsit of model.
     *  @param A vector of model name
     */
    void setModel(Rcpp::S4& model);
  
    /// set weight
    void setWeight(Rcpp::NumericVector & Rweight);

    /// set knownPartition
    void setKnownPartition(Rcpp::NumericVector & Rpartition);
  
  protected:

    // pointer to XEMInput
    XEMInput* cInput_;

};


#endif /* INPUTHANDLING_H_ */
