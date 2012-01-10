/*
 * OutputHandling.h
 *
 *  Created on: 1 juin 2011
 *      Author: aude
 */




//Functions get

#ifndef OUTPUTHANDLING_H_
#define OUTPUTHANDLING_H_

#include "./MIXMOD/XEMClusteringOutput.h"
//#include "./MIXMOD/XEMGaussianParameter.h"

#include <Rcpp.h>

/** base class for handling the mixmod outputs and filling them in
 *  a Rcpp list */
class OutputHandling
{
  public:
    /** Default constructor
     *  @param cMOutput the model estimated by mixmod
     *  @param output the Rcpp list to fill in
     **/
    OutputHandling( XEMClusteringModelOutput* cMOutput, Rcpp::S4& xem);

    /** destructor */
    virtual ~OutputHandling();

  protected:
    /** A pointer on the MIXMOD output */
    //XEMClusteringModelOutput const* cMOutput_;
    XEMClusteringModelOutput* cMOutput_;
    /** A reference on the R output */
    Rcpp::S4& xem_;
  
    /** Number of variables */
    int nbCluster_;
    /** Number of variables */
    int64_t nbVariable_;
};


#endif /* OUTPUTHANDLING_H_ */
