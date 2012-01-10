/*
 * GaussianOutputHandling.h
 *
 *  Created on: 14 juin 2011
 *      Author: aude
 */

#ifndef GAUSSIANOUTPUTHANDLING_H
#define GAUSSIANOUTPUTHANDLING_H


#include "./MIXMOD/XEMGaussianEDDAParameter.h"
#include "OutputHandling.h"

/** class for handling the mixmod outputs when the mixture model is gaussian
 *  and filling them in a Rcpp list
 **/
class GaussianOutputHandling : public OutputHandling
{
  public:
    /** Default constructor
     *  @param cMOutput the gaussian model estimated by mixmod
     *  @param output the Rcpp list to fill in
     **/
    GaussianOutputHandling( XEMClusteringModelOutput* cMOutput, Rcpp::S4& xem);
    /** destructor */
    virtual ~GaussianOutputHandling();

  private:
    /** methods for getting variance matrices
     * @param param the list to fill in with the variance matrices
     **/
    void getGaussianInfoCluster(Rcpp::S4& param);

    /** The parameters of the Gaussian mixture model*/
    XEMGaussianEDDAParameter const* param_;
};

#endif
