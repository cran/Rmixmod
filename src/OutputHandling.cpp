/*
 * OutputHandling.cpp
 *
 *  Created on: 1 juin 2011
 *      Author: aude
 */


#include "Conversion.h"
#include "OutputHandling.h"


// constructor
OutputHandling::OutputHandling( XEMClusteringModelOutput* cMOutput, Rcpp::S4& xem)
                              : cMOutput_(cMOutput)
                              , xem_(xem)
                              , nbCluster_((int)cMOutput_->getNbCluster())       
{
  // add nbCluster value
  xem_.slot("nbClusterInBestModel") = nbCluster_;
  // add likelihood value
  xem_.slot("likelihood") = cMOutput_->getLikelihood();
  // add criterion value
  xem_.slot("criterionValue") = cMOutput_->getCriterionOutput()[0].getValue();
  // add model selected
  xem_.slot("bestModel") = XEMModelNameToString(cMOutput->getModelType().getModelName());
  // add labels
  xem_.slot("labels") = Conversion::VectorToRcppVectorForInt(cMOutput_->getLabelDescription()->getLabel()->getLabel());
  // add partition
//  xem_.slot("partition") = Conversion::LabelToPartition((int)cMOutput_->getNbCluster(), cMOutput_->getLabelDescription()->getLabel()->getLabel());
  //add proba
  xem_.slot("proba") = Conversion::XEMMatrixToRcppMatrix(cMOutput_->getProbaDescription()->getProba()->getProba());
}

// destructor
OutputHandling::~OutputHandling()
{ }
