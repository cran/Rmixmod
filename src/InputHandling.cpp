/*
 * XEMClusteringInitializationInput.cpp
 *
 *  Created on: 23 mai 2011
 *      Author: aude
 */

#include "InputHandling.h"
#include <vector>

#include "./MIXMOD/XEMUtil.h"


//default constructor
InputHandling::InputHandling( XEMClusteringInput* cInput
                            , Rcpp::S4& algoOptions
                            )
                            : cInput_(cInput)
                            , algoOptions_(algoOptions)
{ run();}

//destructor
InputHandling::~InputHandling()
{ }

//setModel
void InputHandling::setModel( Rcpp::S4& model)
{
  // check if it is a NULL value
  if (Rf_isNull( model.slot("listModels") )) return;

  // get the list of models
  Rcpp::CharacterVector modelList(model.slot("listModels"));
  // remove the model set by default
  if ( modelList.size()>0) cInput_->removeModelType(0);
  
  for (int i=0; i < modelList.size(); ++i)
  {
    // get string value
    XEMModelName name = StringToXEMModelName(Rcpp::as<std::string>(Rcpp::StringVector(modelList[i].get())));
    if (name != UNKNOWN_MODEL_NAME)
    {
      XEMModelType modelType(name);
      // Take care that this method will use the copy constructor of
      // XEMModelType (FIXME !) and thus insert a copy of modelType
      cInput_->insertModelType(&modelType, i);
    }
    else
      throw (std::runtime_error("Invalid modelName in setModel : "));
  }
}

//set Weight
void InputHandling::setWeight(Rcpp::NumericVector & Rweight)
{
  std::vector<double> Cweight = Rcpp::as<std::vector<double> >(Rweight);
  
  if (!Cweight.empty()) cInput_->setWeight(&Cweight.front());
}

///setCriterionName
void InputHandling::setCriterionName(Rcpp::StringVector& criterion)

{
  // check if it is a NULL value
  if (Rf_isNull( criterion )) return;
  // get criterion
  std::string criterionName = Rcpp::as<std::string>(criterion);
  // BIC criterion
  if (criterionName.compare("BIC") == 0)
  { cInput_->setCriterionName(BIC,0); return;}
  // ICL Criterion
  if(criterionName.compare("ICL") == 0)
  { cInput_->setCriterionName(ICL,0); return; }
  if(criterionName.compare("NEC") == 0)
  // NEC Criterion
  { cInput_->setCriterionName(NEC,0); return; }
  //exception because wrong criterionName
  throw(std::runtime_error("In InputHandling::setCriterionName invalid criterion name"));
}

///setAlgo
void InputHandling::setAlgo()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("name") )) return;
  // get algo
  std::string algo = Rcpp::as<std::string>(algoOptions_.slot("name"));
  // EM Algo
  if (algo == "EM")
  {
    cInput_->getStrategy()->setAlgo(EM, 0);
    setNbIterationInAlgo();
    setEpsilonInAlgo();
    return;
  }
  // SEM Algo
  if (algo == "SEM")
  {
    cInput_->getStrategy()->setAlgo(SEM, 0);
    setNbIterationInAlgo();
    return;
  }
  // CEM Algo
  if (algo == "CEM")
  {
    cInput_->getStrategy()->setAlgo(CEM, 0);
    setNbIterationInAlgo();
    setEpsilonInAlgo();
    return;
  }
  //exception because wrong criterionName
  throw(std::runtime_error("In InputHandling::setAlgo Invalid algo"));
}

/// setInitAlgo
void InputHandling::setInitAlgo()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("initMethod") )) return;
  // get initialization algorithm name
  std::string init = Rcpp::as<std::string>(algoOptions_.slot("initMethod"));
  // random initialization
  if (init == "random")
  {
    setNbTry();
    setNbTryInInit();
    return;
  }
  else if (init == "smallEM")
  {
    setNbTry();
    setNbTryInInit();
    setNbIterationInInit();
    setEpsilonInInit();
    return;
  }
  else if (init == "CEM")
  {
    setNbTry();
    setNbTryInInit();
    return;
  }
  else if (init == "SEMMax")
  {
    setNbTry();
    setNbIterationInInit();
    return;
  }
  else if (init == "parameter")
  {
    throw(std::runtime_error("Error in InputHandling::run"
                             " parameter initialization is not implemented"));

  }
  else if (init == "partition")
  {
    // TODO initialize partition
    return;
  }
//
//  XEMStrategyInitName initName = cInput_->getStrategy()->getStrategyInit()->getStrategyInitName();
//  cInput_->getStrategy()->setStrategyInitName(initName);
}

/// setNbTry
void InputHandling::setNbTry()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("nbTry") )) return;
  // get int value
  int64_t nbTry = (int64_t)Rcpp::as<int>(algoOptions_.slot("nbTry"));
  // set value
  if (nbTry > 0)
  { cInput_->getStrategy()->setNbTry(nbTry); }
}

/// setInitParameter
void InputHandling::setInitParameter(XEMParameter** parameter)
{
  //get initName
  XEMStrategyInitName initName = cInput_->getStrategy()->getStrategyInit()->getStrategyInitName();
  //get nbInitParameter value
  int64_t nbInitParameter = cInput_->getStrategy()->getStrategyInit()->getNbInitParameter();
  //get parameter
  //XEMParameter** parameter = cInput_->getStrategy()->getStrategyInit()->getTabInitParameter();
  //get nbCluster value
  int64_t nbCluster = cInput_->getNbCluster(0);
  if (initName == USER)
  {
    if (nbCluster == 1)
    {
      cInput_->getStrategy()->setTabInitParameter(parameter, nbInitParameter);
    }
  }
}

/// setInitPartition
void InputHandling::setInitPartition( XEMPartition ** tabPartition
                                    , int64_t nbPartition
                                    )
{ cInput_->getStrategy()->setTabPartition(tabPartition, nbPartition);}

/// setNbTryInInit
void InputHandling::setNbTryInInit()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("nbTryInInit"))) return;
  // get int value
  int64_t nbTryInInit = (int64_t)Rcpp::as<int>(algoOptions_.slot("nbTryInInit"));
  // set value
  if (nbTryInInit > 0)
  { cInput_->getStrategy()->setNbTryInInit(nbTryInInit); }
}

///setNbIterationInInit
void InputHandling::setNbIterationInInit()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("nbIterationInInit") )) return;
  // get int value
  int64_t nbIterationInInit = (int64_t)Rcpp::as<int>(algoOptions_.slot("nbIterationInInit"));
  // set value
  if (nbIterationInInit > 0)
  { cInput_->getStrategy()->setNbIterationInInit(nbIterationInInit); }
}

///setEpsilonInInit
void InputHandling::setEpsilonInInit()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("epsilonInInit") )) return;
  // get double value
  double epsilonInInit = Rcpp::as<double>(algoOptions_.slot("epsilonInInit"));
  // set value
  if (epsilonInInit > 0)
  { cInput_->getStrategy()->setEpsilonInInit(epsilonInInit); }
}

/// setNbIterationInAlgo
void InputHandling::setNbIterationInAlgo()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("nbIterationInAlgo") )) return;
  // get int value
  int64_t nbIterationInAlgo = (int64_t)Rcpp::as<int>(algoOptions_.slot("nbIterationInAlgo"));
  // set value
  if (nbIterationInAlgo > 0)
  { cInput_->getStrategy()->setAlgoIteration(0, nbIterationInAlgo); }
}

///setEpsilonInAlgo
void InputHandling::setEpsilonInAlgo()
{
  // check if it is a NULL value
  if (Rf_isNull( algoOptions_.slot("epsilonInAlgo") )) return;
  // get double value
  double epsilonInAlgo = Rcpp::as<double>(algoOptions_.slot("epsilonInAlgo"));
  // set value
  if (epsilonInAlgo > 0)
  { cInput_->getStrategy()->setAlgoEpsilon(0, epsilonInAlgo); }
}

void InputHandling::run()
{
  try
  {
    // set Algo name
    setAlgo();
    // set init Algo name
    setInitAlgo();
  }
  catch(...)
  {
    throw(std::runtime_error("InputHandling::run get a mixmod error"));
  }
}

