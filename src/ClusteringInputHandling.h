//
//  ClusteringInputHandling.h
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#ifndef Rmixmod_ClusteringInputHandling_h
#define Rmixmod_ClusteringInputHandling_h

#include "InputHandling.h"

#include <Rcpp.h>

// pre-declaration
class XEMClusteringInput;
class XEMClusteringStrategy;
class XEMPartition;
class XEMParameter;

class ClusteringInputHandling : public InputHandling
{
  
public:
  /// Default constructor
  ClusteringInputHandling( XEMClusteringInput* cInput, Rcpp::S4& algoOptions );
 
  /// Destructor
  virtual ~ClusteringInputHandling();
  
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
  
private:
  
  // pointer to the clustering strategy
  XEMClusteringStrategy* cStrategy_;
  
  // structure containing algorithm options
  Rcpp::S4& algoOptions_;
  
};


#endif
