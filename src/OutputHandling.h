//
//  OutputHandling.h
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#ifndef Rmixmod_OutputHandling_h
#define Rmixmod_OutputHandling_h

#include <Rcpp.h>

// pre-declaration
class XEMModelOutput;

/** base class for handling the mixmod outputs and filling them in
 *  a Rcpp list */
class OutputHandling
{
  
public:
  /** Default constructor
   *  @param cMOutput the model estimated by mixmod
   *  @param output the Rcpp list to fill in
   **/
  OutputHandling( XEMModelOutput* MOutput, Rcpp::S4& xem, const bool isGaussian);
  
  /** destructor */
  ~OutputHandling();
  
private:
  /** set gaussian paramaters */
  void setGaussianParameter();
  
  /** set multinomial parameters */
  void setMultinomialParameter();
  
protected:
  /** A pointer on the MIXMOD output */
  XEMModelOutput* MOutput_;
  
  /** A reference on the R output */
  Rcpp::S4& xem_;
  
  /** Number of variables */
  int nbCluster_;
  
  /** Number of variables */
  int64_t nbVariable_;
};


#endif
