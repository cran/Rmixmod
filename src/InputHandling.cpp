//
//  InputHandling.cpp
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//

#include "InputHandling.h"
#include <vector>
#include <stdint.h>

#include "MIXMOD/XEMInput.h"
#include "MIXMOD/XEMModelType.h"
#include "MIXMOD/XEMLabelDescription.h"
#include "MIXMOD/XEMUtil.h"


//default constructor
InputHandling::InputHandling( XEMInput* cInput )
                            : cInput_(cInput)
{}

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
  
  // create vector of XEMModelName
  std::vector<XEMModelName> modelName;
  
  for (int i=0; i < modelList.size(); ++i)
  {
    // get string value
    XEMModelName name = StringToXEMModelName(Rcpp::as<std::string>(Rcpp::StringVector(modelList[i].get())));
    if (name != UNKNOWN_MODEL_NAME)
    { modelName.push_back(name); }
    else
      throw (std::runtime_error("Invalid modelName in setModel : "));
  }
  // add those models
  cInput_->setModel(modelName);
}

//set Weight
void InputHandling::setWeight(Rcpp::NumericVector & Rweight)
{
  std::vector<double> Cweight = Rcpp::as<std::vector<double> >(Rweight);
  
  if (!Cweight.empty()) cInput_->setWeight(&Cweight.front());
}

// set knownPartition
void InputHandling::setKnownPartition(Rcpp::NumericVector & Rpartition)
{
  // create labels from R parameter
  if ( Rpartition.size() ){
    std::vector<int64_t> labels(Rpartition.size());
    for (unsigned int i=0; i<labels.size(); i++ ) labels[i]=Rpartition[i];
    // create XEMLabelDescription
    XEMLabelDescription partition( labels.size(), labels );
    cInput_->setKnownLabelDescription(partition);
  }
}

///setCriterionName
void InputHandling::setCriterionName(Rcpp::CharacterVector & criterion)

{
  // check if it is a NULL value
  if (Rf_isNull( criterion )) return;
  // get criterion
  std::vector<std::string> criterionName = Rcpp::as< std::vector<std::string> >(criterion);
  
  // start by removing default criterion
  cInput_->removeCriterion(0);
  
  // loop over criterion names
  for (unsigned int i=0; i<criterionName.size(); i++){
    // BIC criterion
    if (criterionName[i] == "BIC")
    { cInput_->addCriterion(BIC);}
    // ICL Criterion
    else if(criterionName[i] == "ICL")
    { cInput_->addCriterion(ICL); }
    else if(criterionName[i] == "NEC")
    // NEC Criterion
    { cInput_->addCriterion(NEC); }
    // CV criterion
    else if (criterionName[i] == "CV")
    { cInput_->addCriterion(CV); }
    //exception because wrong criterionName
    else 
    { throw(std::runtime_error("In InputHandling::setCriterionName invalid criterion name")); }
  }
}
