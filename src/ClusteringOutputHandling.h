//
//  ClusteringOutputHandling.h
//  Rmixmod
//
//  Created by Rémi Lebret on 06/02/12.
//  Copyright (c) 2012 CNRS. All rights reserved.
//


//Functions get

#ifndef Rmixmod_ClusteringOUTPUTHANDLING_H_
#define Rmixmod_ClusteringOUTPUTHANDLING_H_

#include "OutputHandling.h"
#include "mixmod/Utilities/mixmod.h"

/** base class for handling the mixmod outputs and filling them in
 *  a Rcpp list */
class ClusteringOutputHandling : public OutputHandling
{
  public:
    /** Default constructor
     *  @param cMOutput the model estimated by mixmod
     *  @param output the Rcpp list to fill in
     **/
    ClusteringOutputHandling( XEM::ClusteringModelOutput* cMOutput
                             , Rcpp::S4& xem
                             , const XEM::DataType dataType
                             , Rcpp::CharacterVector const & Rcriterion
                             );

    /** destructor */
    ~ClusteringOutputHandling();
};


#endif 
