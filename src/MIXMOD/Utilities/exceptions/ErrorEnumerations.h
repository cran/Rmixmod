/** @file ErrorEnumerations.h
 *  @brief Enumerations for various types of errors.
 *  @author Parmeet Bhatia
 **/

#ifndef ERRORENUMERATIONS_H
#define ERRORENUMERATIONS_H

namespace XEM {

enum InputError {

	noError = 0,
	nbLinesTooLarge,
	nbLinesTooSmall,
	pbDimensionTooLarge,
	pbDimensionTooSmall,
	nbCriterionTooLarge,
	nbCriterionTooSmall,
	wrongCriterionName,
	nbNbClusterTooLarge,
	nbNbClusterTooSmall,
	nbModelTypeTooLarge,
	nbModelTypeTooSmall,
	wrongModelType,
	wrongCVinitType,
	wrongDCVinitType,
	nbStrategyTypeTooLarge,
	badNbParameterInInit,
	badNbPartitionInInit,
	nbStrategyTypeTooSmall,
	wrongStrategyInitName,
	errorInitParameter,
	nbAlgoTooLarge,
	nbAlgoTooSmall,
	wrongAlgoType,
	nbIterationTooLarge,
	nbIterationTooSmall,
	epsilonTooSmall,
	epsilonTooLarge,
	wrongDataFileName,
	wrongWeightFileName,
	wrongParamFileName,
	wrongLabelFileName,
	wrongPartitionFileName,
	wrongAlgoStopName,
	wrongOutputType,
	wrongInputFileName,
	wrongXEMNbParam,
	errorNbLines,
	errorPbDimension,
	errorNbCriterion,
	errorListCriterion,
	errorNbNbCluster,
	errorListNbCluster,
	errorNbModel,
	errorListModel,
	errorNbStrategy,
	errorInitType,
	errorInitFile,
	errorNbAlgo,
	errorAlgo,
	errorStopRule,
	errorStopRuleValue,
	errorDataFile,
	nbAlgoTypeTooSmall,
	badStrategyInitName,
	errorOpenFile,
	errorNbModality,
	knownPartitionNeeded,
	badKnownPartition,
	endDataFileReach,
	wrongNbKnownPartition,
	SubDimensionFreeTooLarge,
	SubDimensionFreeTooSmall,
	SubDimensionEqualTooLarge,
	SubDimensionEqualTooSmall,
	weightTotalIsNotAnInteger,
	ungivenSubDimension,
	inputNotFinalized,
	wrongNbAlgoWhenMorMAP,
	BadInitialsationWhenM,
	BadInitialsationWhenMAP,
	partitionMustBeComplete,
	algorithmMustBeM,
	knownPartitionAndInitPartitionMustBeEqual,
	nbStrategyMustBe1,
	wrongNbKnownPartitionOrInitPartition,
	tooManySampleInInitPartitionAndTooManyClusterNotRepresented,
	notAvailableForPrediction,
	ColumnTypeNotValid,
	errorInPartitionInput,
	wrongValueInMultinomialCase,
	notEnoughValuesInLabelInput,
	notEnoughValuesInProbaInput,
	badValueInLabelInput,

	badStopNameWithSEMAlgo,
	badAlgorithmInHDContext,
	differentSubDimensionsWithMAP,
	wrongSubDimension,
	missingRequiredInputs,
	// the following errors should not be occur because they must have been see by MVCControler
	wrongCriterionPositionInSet,
	wrongCriterionPositionInGet,
	wrongCriterionPositionInInsert,
	wrongCriterionPositionInRemove,
	wrongModelPositionInSet,
	wrongModelPositionInGet,
	wrongModelPositionInInsert,
	wrongModelPositionInRemove,
	wrongModelPositionInSetSubDimensionEqual,
	wrongModelPositionInSetSubDimensionFree,
	wrongKnownPartitionPositionInSet,
	wrongKnownPartitionPositionInRemove,
	badSetKnownPartition,
	wrongStrategyPositionInSetOrGetMethod,
	nbTryInStrategyTooSmall,
	nbTryInStrategyTooLarge,
	nbTryInInitTooSmall,
	nbTryInInitTooLarge,
	epsilonInInitTooSmall,
	epsilonInInitTooLarge,
	nbIterationInInitTooSmall,
	nbIterationInInitTooLarge,
	wrongNbStrategyTryValue,
	badInitPart,
	badSetNbTry,
	badSetNbTryInInit,
	badSetNbIterationInInit,
	badSetEpsilonInInit,

	badSetStopNameInInit,
	badCriterion,
	badAlgo,
	badAlgoStop,
	badInputType,
	DAInput,
	wrongModelName,
	knownPartitionNotAvailable,
	tooManyWeightColumnDescription,
	badDataDescription,
	badLabelDescription,
	errorInColumnDescription,
	errorInXEMInputSelector,
	wrongIndexInGetMethod
};

enum DCVError {

	wrongDCVinitBlocks,
	wrongDCVnumberOfBlocks,
	DCVmustBeDIAG,
	forbiddenCallToGetBestCVModel
};

enum DCVonlyInGaussianCaseError {

	allCVCriterionErrorForAnEstimationInDCVContext,
	NbDCVBlocksTooSmall
};

enum NumericError {

	int64_t_max_error,
	CEM_INIT_error,
	SEM_MAX_error,
	SMALL_EM_error,
	tabNkNotInteger,
	sumFiNullAndfkTPrimNull,
	sumFiNullInMultinomialCase,
	nonPositiveDefiniteMatrix,
	nullDeterminant,
	randomProblem,
	nullLikelihood,
	noProbability,
	pbNEC,
	nullNk,

	numericError,
	errorSigmaConditionNumber,
	minDeterminantSigmaValueError,
	minDeterminantWValueError,
	minDeterminantDiagWkValueError,

	minDeterminantDiagWValueError,
	minDeterminantBValueError,
	minDeterminantRValueError,
	minDeterminantWkValueError,
	minDeterminantShapeValueError,
	minDeterminantDiagQtmpValueError
};

enum OtherError {

	badFormat,
	nullPointerError,
	wrongMatrixType,
	wrongConstructorType,
	nonImplementedMethod,
	badBinaryParameterClass,
	internalMixmodError,
	FunctionNotYetImplemented,
	UnknownReason
};

}

#endif /* ERRORENUMERATIONS_H_ */
