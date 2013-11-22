/***************************************************************************
							 SRC/MIXMOD/Clustering/XEMClusteringMain.cpp  description
	copyright            : (C) MIXMOD Team - 2001-2013
	email                : contact@mixmod.org
 ***************************************************************************/

/***************************************************************************
	This file is part of MIXMOD
    
	MIXMOD is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MIXMOD is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with MIXMOD.  If not, see <http://www.gnu.org/licenses/>.

	All informations available on : http://www.mixmod.org                                                                                               
 ***************************************************************************/

#include "mixmod/Clustering/ClusteringMain.h"
#include "mixmod/Clustering/ClusteringInput.h"
#include "mixmod/Clustering/ClusteringOutput.h"
#include "mixmod/Clustering/ClusteringModelOutput.h"
#include "mixmod/Clustering/ClusteringStrategy.h"
#include "mixmod/Clustering/ClusteringStrategyInit.h"
#include "mixmod/Utilities/Random.h"
#include "mixmod/Kernel/IO/BinaryData.h"
#include "mixmod/Kernel/IO/Label.h"
#include "mixmod/Kernel/Model/BinaryModel.h"
#include "mixmod/Kernel/IO/LabelDescription.h"
#include "mixmod/Kernel/IO/Partition.h"
#include "mixmod/Utilities/Error.h"
#include "mixmod/Kernel/Criterion/BICCriterion.h"
#include "mixmod/Kernel/Criterion/ICLCriterion.h"
#include "mixmod/Kernel/Criterion/NECCriterion.h"
#include "mixmod/Kernel/IO/ModelOutput.h"

namespace XEM {

ClusteringMain::ClusteringMain() {
	THROW(OtherException, internalMixmodError);
}

ClusteringMain::ClusteringMain(ClusteringInput * input, ClusteringOutput * output)
: _input(input), _output(output) {
}

ClusteringMain::~ClusteringMain() {
	// Deleting NULL or nullptr is fine.
	// No check, because two calls of the destructor on the same object is a bug.
	delete _output;
}

void ClusteringMain::run(int seed = -1) {

	// Call randomize() function to use a random seed, or antiRandomize() for deterministic seed.
	initRandomize(seed);

	if (!_input)
		THROW(OtherException, nullPointerError);
	// TODO: finalization should occur (only) here.
	if (! _input->isFinalized())
		THROW(InputException, inputNotFinalized);

	//----------------------------------------------
	// 0. Data preparation, variables initialization
	//----------------------------------------------

	// Check if the user provided a partition (hard or fuzzy)
	// TODO [bauder]: fuzzy partitions should be allowed, and used as t_ik initial values.
	//                In this case, we would start with an M-step and then iterate as usual.
	Partition* workingKnownPartition = _input->getKnownPartition();
	if (_input->getKnownLabelDescription()) {
		const Label * lab = _input->getKnownLabelDescription()->getLabel();
		workingKnownPartition = new Partition(lab, _input->getNbCluster()[0]);
		if (_input->getKnownPartition()) {
			// knowPartition and knownLabelDescription can't be both not NULL !
			THROW(OtherException, internalMixmodError);
		}
	}

	Data* workingData = _input->getDataDescription().getData();
	ClusteringStrategy* workingStrategy = _input->getStrategy();

	// Prepare reduced data if qualitative data and DATA_REDUCE==true
	std::vector<int64_t> correspondenceOriginDataToReduceData;
	if (_input->getDataType() == QualitativeData && DATA_REDUCE) {
		BinaryData * bData = dynamic_cast<BinaryData*> (_input->getDataDescription().getData());
		
		// initPartition
		Partition* inputInitPartition = NULL;
		Partition* workingInitPartition = NULL;
		if (_input->getStrategy()->getStrategyInit()->getStrategyInitName() == USER_PARTITION) {
			inputInitPartition = _input->getStrategy()->getStrategyInit()->getPartition(0);
		}
		
		try {
			// NOTE: workingInitPartition is always NULL
			workingData = bData->reduceData(
					correspondenceOriginDataToReduceData, workingKnownPartition,
					inputInitPartition, workingKnownPartition, workingInitPartition);
			workingStrategy = new ClusteringStrategy(*(_input->getStrategy()));
		}
		catch (Exception& errorType) {
			throw;
		}
	}

	// Initialize output [HACK: using pointer because of an error when retrieving CriterionOutput later]
	std::vector<CriterionName>* vCriterion = new std::vector<CriterionName>();
	const int nbCriterion = _input->getCriterionName().size();
	for (unsigned int iCriterion = 0; iCriterion<nbCriterion; iCriterion++)
		vCriterion->push_back(_input->getCriterionName()[iCriterion]);
	_output = new ClusteringOutput(*vCriterion);

	// Main loop : build and run every model, and incrementally fill output
	// NOTE: potential parallelization here (OpenMP at least)
	const int nbnbCluster = _input->getNbCluster().size();
	for (unsigned int nbCluster_i= 0; nbCluster_i<nbnbCluster; nbCluster_i++) {
		int64_t nbCluster = _input->getNbCluster()[nbCluster_i];
		const int nbModel = _input->getModelType().size();
		for (unsigned int nbModel_i=0; nbModel_i<nbModel; nbModel_i++) {
			ModelType* modelType = _input->getModelType()[nbModel_i];
			
			//-----------------
			// 1. Create model
			//-----------------

			if (VERBOSE)
				std::cout << "Model name : " 
					<< ModelNameToString(modelType->getModelName()) << std::endl;
			
			Model* model = NULL;
			switch (_input->getDataType()) {
			case QualitativeData:
				model = new BinaryModel(modelType, nbCluster, workingData,
						workingKnownPartition, correspondenceOriginDataToReduceData);
				break;
			case QuantitativeData:
				model = new Model(
						modelType, nbCluster, workingData, workingKnownPartition);
				break;
			case HeterogeneousData:
				model = new Model(
						modelType, nbCluster, workingData, workingKnownPartition);
				break;
			}

			//-----------------------------
			// 2. run parameters estimation
			//-----------------------------
			
			try {
				workingStrategy->run(model);
			}
			catch (Exception& e) {
				if (VERBOSE) {
					Error error(e);
					error.run();
				}
				model->setError(e);
			}
			catch (...) {
				Exception * unknown = new OtherException(UnknownReason);
				model->setError(*unknown);
				delete unknown;
			}

			//--------------------
			// 3. compute criteria
			//--------------------
			
			ClusteringModelOutput* cmoutput = new ClusteringModelOutput(model);
			_output->addEstimation(cmoutput);
			if (model->getErrorType() == NOERROR) {
				// Loop over criterion name
				for (std::vector<CriterionName>::const_iterator it= vCriterion->begin(); it != vCriterion->end(); it++) {
					CriterionName criterion = *it;
					switch (criterion) {
					case BIC:
					{
						BICCriterion bic(model);
						bic.run(cmoutput->getCriterionOutput(BIC));
						break;
					}
					case CV:
						THROW(InputException, DAInput);
						break;
					case ICL:
					{
						ICLCriterion icl(model);
						icl.run(cmoutput->getCriterionOutput(ICL));
						break;
					}
					case NEC:
					{
						NECCriterion nec(model);
						nec.run(cmoutput->getCriterionOutput(NEC));
						break;
					}
					case UNKNOWN_CRITERION_NAME:
						THROW(OtherException, internalMixmodError);
						break;
					default:
						THROW(OtherException, internalMixmodError);
					}
				}
			}
			else {
				// Set criterion error (over all criterion names)
				for (std::vector<CriterionName>::const_iterator it= vCriterion->begin(); it != vCriterion->end(); it++) {
					CriterionName criterion = *it;
					cmoutput->setCriterionOutput(
						CriterionOutput(criterion, 0.0, model->getErrorType()));
				}
			}

			// model cannot be null (especially when we'll use enum class)
			delete model;
		}
	}

	// Conditionally release memory
	if (_input->getKnownLabelDescription()) delete workingKnownPartition;
	if (_input->getDataType() == QualitativeData && DATA_REDUCE) {
		delete workingData;
		delete workingStrategy;
	}
}

}
