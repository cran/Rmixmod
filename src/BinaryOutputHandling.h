/*
 * BinaryOutputHandling.h
 *
 *  Created on: 15 juin 2011
 *      Author: aude
 */

#ifndef BINARYOUTPUTHANDLING_H
#define BINARYOUTPUTHANDLING_H


#include "./MIXMOD/XEMBinaryParameter.h"
#include "OutputHandling.h"


/** class for handling the mixmod outputs when the mixture model is binary
 *  and filling them in a Rcpp list
 **/
class BinaryOutputHandling : public OutputHandling
{
  public:
    /** Default constructor
     *  @param cMOutput the binary model estimated by mixmod
     *  @param output the Rcpp list to fill in
     **/
    BinaryOutputHandling( XEMClusteringModelOutput* cMOutput, Rcpp::S4& xem);
    /** destructor */
    virtual ~BinaryOutputHandling();

    /** methods for getting scatters
     * @param param the list to fill in with the scatters
     **/
    void getBinaryInfoCluster(Rcpp::S4& param);

  private:
    /** The parameters of the Binary mixture model*/
    XEMBinaryParameter const* param_;
};

#endif
