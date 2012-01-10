/*
 * InputHandling.h
 *
 *  Created on: 20 mai 2011
 *      Author: aude
 */

#ifndef INPUTHANDLING_H_
#define INPUTHANDLING_H_

#include <iostream>
#include <string>

#include <Rcpp.h>

#include "./MIXMOD/XEMClusteringStrategy.h"
#include "./MIXMOD/XEMClusteringInput.h"
#include "./MIXMOD/XEMAlgo.h"
#include "./MIXMOD/XEMClusteringStrategyInit.h"
#include "./MIXMOD/XEMParameter.h"
#include "./MIXMOD/XEMPartition.h"

class InputHandling
{

  public:
    /// Default constructor
    InputHandling( XEMClusteringInput* cInput, Rcpp::S4& algoOptions);
    /// Destructor
    virtual ~InputHandling();
    ///setCriterionName
    void setCriterionName(Rcpp::StringVector& criterion);
    /**  Add Models to estimate. The default model @c defaultGaussianModelName
     *  is set by default and will not be removed from the lsit of model.
     *  @param A vector of model name
     */
    void setModel(Rcpp::S4& model);
    /// set weight
    void setWeight(Rcpp::NumericVector & Rweight);

  protected:

    ///run
    void run();

    /** @brief Set the algorithm to use
     *  @code
     *  enum XEMAlgoName {
     *  UNKNOWN_ALGO_NAME = -1, // Unknown algorithm
     *  MAP = 0,                // Maximum a posteriori
     *  EM = 1,                 // Expectation maximization
     *  CEM = 2,                // Classification EM
     *  SEM = 3,                // Stochastic EM
     *  M = 4                   // Maximization
     *  };
     *  const XEMAlgoName defaultAlgoName = EM;
     *  const XEMAlgoName defaultClusteringAlgoName = EM;
     *  @endcode
     **/
    void setAlgo();
    /** @brief Set the initialization algorithm to use
     *  @code
     *   enum XEMStrategyInitName {
     *   RANDOM = 0,         // Random centers
     *   USER = 1,           // Initial parameters specified by user
     *   USER_PARTITION = 2, // Partition specified by user
     *   SMALL_EM = 3,       // em strategy for initial parameters
     *   CEM_INIT = 4,       // initialization with cem
     *   SEM_MAX = 5         // initialization with sem max
     * };
     *  @endcode
     */
    void setInitAlgo();
  
  
    /// setNbTry
    void setNbTry();
    /// setNbTryInInit
    void setNbTryInInit();
    /// setNbIterationInInit
    void setNbIterationInInit();
    /// setEpsilonInInit
    void setEpsilonInInit();
    ///setInitParameter NOT used
    void setInitParameter(XEMParameter** parameter);
    ///setInitPartition
    void setInitPartition( XEMPartition ** tabPartition, int64_t nbPartition);
    /// setNbIterationInAlgo
    void setNbIterationInAlgo();
    /// setEpsilonInAlgo
    void setEpsilonInAlgo();

  private:

    XEMClusteringInput* cInput_;
    Rcpp::S4& algoOptions_;

    /// create a XEMPartition from the algoOption list furnish by R
    XEMPartition ** createPartition();

};


#endif /* INPUTHANDLING_H_ */
