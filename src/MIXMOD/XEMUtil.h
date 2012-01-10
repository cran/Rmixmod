/***************************************************************************
                             SRC/MIXMOD/XEMUtil.h  description
    copyright            : (C) MIXMOD Team - 2001-2011
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
#ifndef XEMUTIL_H
#define XEMUTIL_H

#ifndef WANT_STREAM
#define WANT_STREAM
#endif

#ifndef WANT_MATH
#define WANT_MATH
#endif

/* Need matrix applications */
#include "../NEWMAT/newmatap.h"
#include "../NEWMAT/newmatio.h"



/* Need matrix output routines */
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <ctype.h>
#include <stdint.h>
#include <memory>

//#include <fstream.h>
//#include <string.h>
//#include "ctype.h"

#include <typeinfo>

/* Preprocessor constants */
#define CLOCK 0  /* 1 for execution time information */
#define DEBUG 0 /* >0 for debug information */
  // 0 : no debug
  // 1 : param debug
  // 2 : param and fik, tik, zik debug

#define DATA_REDUCE 1 // to activate DATA_REDUCE : 1

class XEMModelType;
class XEMMatrix;
class XEMAlgo;

/* Constants definitions */
/// Define PI
const double XEMPI = 3.14159265358979323846; /* Pi number                                                 */
/// Define number of maximum samples
const int64_t maxNbSample = 1000000;             /* Maximum sample size                                       */
const int64_t maxPbDimension = 10000;              /* Maximum sample dimension                                 */
const int64_t minNbIteration = 1;		     /* Minimum number of iterations				  */
const int64_t minNbIterationForSEM = 50;          /* Minimum number of iterations for SEM       */
const int64_t maxNbIteration = 100000;           /* Maximum number of iterations                              */
const int64_t defaultNbIteration = 200;          /* Default number of iteration*/
const double minEpsilon = 0;                /* Minimum value of eps                                      */
const double maxEpsilon = 1;		     /* Maximum value of eps 					*/


const double defaultEpsilon = 1.0E-3;        /* Default value of eps*/
const int64_t maxNbNbCluster = 10;               /* Maximum size of cluster list                              */
const int64_t maxNbAlgo = 5;                     /* Maximum number of algorithms                              */
const int64_t defaultNbAlgo = 1;                 /* Default number of algorithms                              */
const int64_t maxNbStrategy = 10;                /* Maximum number of strategies                              */
const int64_t defaultNbStrategy = 1;             /* Default number of strategies                              */
const int64_t maxNbModel = 100;                   /* Maximum number of models                                  */
const int64_t defaultNbModel = 1;                /* Default number of models                                  */
const int64_t maxNbCriterion = 4;                /* Maximum number of criteria                                */
const int64_t defaultNbCriterion = 1;            /* Default number of criteria                                */

const int64_t minNbTryInStrategy = 1;	     /* min of strategies repeats */
const int64_t maxNbTryInStrategy = 100;	     /* max of strategies repeats */
const int64_t defaultNbTryInStrategy=1;      /* number of strategies repeats */
const int64_t nbTryInDefaultClusteringStrategy=1;      /* number of strategies repeats */

const int64_t minNbTryInInit = 1;		     /* min of repeats in init*/
const int64_t maxNbTryInInit = 1000;	     /* max of repeats in init*/
const int64_t defaultNbTryInInit = 10;	     /* number of repeats in init*/

const int64_t minNbIterationInInit = 1;         /* min number of iterations in init*/
const int64_t maxNbIterationInInit = 1000;      /* max number of iterations in init*/
const int64_t defaultNbIterationInInit = 5;    /* default number of iterations in init*/
const int64_t defaultNbIterationInInitForSemMax = 100;    /* default number of iterations in init*/

const double minEpsilonInInit = 0;         /* min number of iterations in init*/
const double maxEpsilonInInit = 1;        /* max number of iterations in init*/
const double defaultEpsilonInInit = 0.001;   /* default number of iterations in init*/

const int64_t maxNbIterationInCEM_INIT = 100;  /* Maximum number of iterations of CEM in CEM_INIT */

const double minOverflow = 1.e-100;          /* Minimum value for overflow                                */
const double minUnderflow = 1.e-100;         /* Minimum value for underflow                               */
const int64_t nbMaxSelection  = 5;               /* Maximum number of selection                               */
const int64_t maxNbOutputFiles = 52;             /* Maximum number of output Files                            */
const int64_t nbTestOutputFiles = 25;            /* Number of output files to compare in test                 */
const double defaultFluryEpsilon = 0.001;    /* default value for espilon in flury algorthm               */
const int64_t maxFluryIter = 7;                  /* maximum of number of Flury iterations                     */
const double minDeterminantValue = 1.e-100;  /* minimum value of determinant of sigma                     */
const double maxRelativeDiffValueTest = 1.0E-5;      /* Maximum difference between 2 value in test                */
const double maxAbsoluteDiffValueTest = 1.0E-8;      /* Maximum difference between 2 value in test                */

const int64_t  defaultDCVnumberOfBlocks = 10 ;   // DCV
const int64_t defaultCVnumberOfBlocks  = 10 ;   // CV

const double minValueForLLandLLOne = 1.e-10;  // minimum value for LL - LLone    

const int64_t int64_t_max = +9223372036854775807LL;

const int64_t nbQualitativeGraphics = 2;
const int64_t nbQuantitativeGraphics = 3;

/*
Notes :
Enumeration types will be called ...Name
Ex : XEMStrategyInitName
*/



enum XEMCVinitBlocks{
	CV_RANDOM = 0,     // initialize the CV blocks by random
  
        CV_DIAG   = 1      // initialize the CV blocks by assiging 
             /* sample 1 : for w=1 to its weight : sample 1 in block w
              sample 2 : for w=1 to its weight : sample w1+1 in block w1+w
              
            Ex : 1
            //-----
                   ind  weight
                    1     1
                    2     1
                    3     1
                  ...
              ind 1 -> block1
              ind 2 -> block2
              ...
              ind V -> blockV
              ind V+1->block1
              ...  
            
            Ex 2 : 
            //-----
                ind weight
                1     2
                2     2
                3     2
                4     2
                5     2
            if V=4
            ind 1 -> block1
            ind 1 -> block2
            ind 2 -> block3
            ind 2 -> block4
            ind 3 -> block1
            ind 3 -> block2
            ind 4 -> block3
            ind 4 -> block4
            ind 5 -> block1
            ind 5 -> block2
            
            ->>  block 1 : ind 1 - 3 - 5
                block 2 : ind 1 - 3 - 5
                block 3 : ind 2 - 4
                block 4 : ind 4 - 5
              
             
             */                   
        
};
const XEMCVinitBlocks defaultCVinitBlocks = CV_RANDOM;


enum XEMDCVinitBlocks{
	DCV_RANDOM = 0,     // initialize the DCV blocks by random
  
	DCV_DIAG   = 1      // initialize the DCV blocks by the same way that CV_DIAG
            /*
            Ex : 1
            //-----
            ind  weight
            1     1
            2     1
            3     1
            ...
            ind 1 -> blockTest1
            ind 2 -> blockTest2
            ...
            ind V -> blockTestV
            ind V+1->blockTest1
            ...  
            
            Ex 2 : 
            //-----
            ind weight
            1     10
            2     10
            3     10
            4     10
            5     10
            if V=4
            ind 1 -> blockTest1
            ind 1 -> blockTest2
            ind 1 -> blockTest3
            ind 1 -> blockTest4
            ind 1 -> blockTest1
            ind 1 -> blockTest2
            ind 1 -> blockTest3
            ind 1 -> blockTest4
            ind 1 -> blockTest1
            ind 1 -> blockTest2
            ind 2 -> blockTest3
            ind 2 -> blockTest4
            ind 2 -> blockTest1
            ind 2 -> blockTest2
            ind 2 -> blockTest3
            ind 2 -> blockTest4
            ind 2 -> blockTest1
            ind 2 -> blockTest2
            ind 2 -> blockTest3
            ind 2 -> blockTest4
            
            ->>  blockTest 1 : ind 1(x3) - 2(x2) - 3(x3) - 4(x2) - 5(x3) - 6(x2) - 7(x3) - 8(x2) - 9(x3) - 10(x2)
            blockLearning 1 : ind 1(x7) - 2(x8) - 3(x7) - 4(x8) - 5(x7) - 6(x8) - 7(x7) - 8(x8) - 9(x7) - 10(x8)
              ...
              
            */
};
const XEMDCVinitBlocks defaultDCVinitBlocks = DCV_RANDOM;

/** @enum XEMStrategyInitName
    @brief Enumeration of differents strategy initialization


*/
enum XEMStrategyInitName {
  RANDOM = 0,         /* Random centers                       */
  USER = 1,           /* Initial parameters specified by user */
  USER_PARTITION = 2, /* Partition specified by user          */
  SMALL_EM = 3,       /* em strategy for initial parameters   */
  CEM_INIT = 4,       /* initialization with cem              */
  SEM_MAX = 5         /* initialization with sem max          */
};
const XEMStrategyInitName defaultStrategyInitName = SMALL_EM;
const XEMStrategyInitName defaultClusteringStrategyInitName = SMALL_EM;


/* Type of convergence for each algorithm */
/** @enum XEMAlgoStopName
    @brief Enumeration of differents type of converge of algorithm (stop rule)
 */
enum XEMAlgoStopName {
  NO_STOP_NAME = -1,         /* for MAP or M algo*/
  NBITERATION = 0,           /* Number of iterations specified by user */
  EPSILON = 1,               /* Stationarity of the xml criterion      */
  NBITERATION_EPSILON = 2    /* Number of iterations & xml criterion   */
};
const XEMAlgoStopName defaultAlgoStopName = NBITERATION_EPSILON;


/** @enum XEMCriterionName
@brief Enumeration of Criterion type
*/
enum XEMCriterionName {
  UNKNOWN_CRITERION_NAME = -1,    /* Unknown criterion               */

  BIC = 0,                      /* Bayesian information criterion    */
  CV  = 1,                      /* Cross validation criterion        */
  ICL = 2,                      /* Integrated completed likelihood   */
  NEC = 3,                      /* Entropy criterion                 */
  DCV = 4                       /* Double Cross validation criterion */
};
const XEMCriterionName defaultCriterionName = BIC;

/** @enum XEMAlgoName
@brief Enumeration of Algo type
 */
enum XEMAlgoName {
  UNKNOWN_ALGO_NAME = -1, /* Unknown algorithm        */
  MAP = 0,                /* Maximum a posteriori     */
  EM = 1,                 /* Expectation maximization */
  CEM = 2,                /* Classification EM        */
  SEM = 3,                /* Stochastic EM            */
  M = 4                   /* Maximization             */
};
const XEMAlgoName defaultAlgoName = EM;
const XEMAlgoName defaultClusteringAlgoName = EM;

/** @enum XEMFormatFile
    @brief Format of differents data file


*/
const int64_t nbFormatNumeric = 3;
namespace FormatNumeric
{

enum XEMFormatNumericFile {
  txt = 0,         	/* Format txt (ascii)                       */
  hdf5 = 1,           	/* Format hdf5 */
  XML = 2, 		/* Format XML         */
};
const XEMFormatNumericFile defaultFormatNumericFile = txt;
}

/** @enum XEMTypePartition
    @brief type of partition


*/

namespace TypePartition
{
enum XEMTypePartition {
  UNKNOWN_PARTITION = 0,
  label = 1,
  partition = 2,
};
const XEMTypePartition defaultTypePartition = label;
}

struct XEMNumericPartitionFile{
  string _fileName;
  FormatNumeric::XEMFormatNumericFile _format;
  TypePartition::XEMTypePartition _type;
};

enum XEMKeyword{
  NbLines,
  PbDimension,
  NbNbCluster,
  ListNbCluster,
  NbModality,
  NbCriterion,
  ListCriterion,
  NbModel,
  ListModel,
  subDimensionEqual,
  subDimensionFree=10,
  NbStrategy,
  InitType,
  InitFile,
  NbAlgorithm,
  Algorithm,
  PartitionFile,
  DataFile,
  WeightFile,
  NbCVBlocks,
  CVinitBlocks=20,
  NbDCVBlocks,
  DCVinitBlocks,
  SizeKeyword
};

string XEMKeywordToString(const XEMKeyword & keyword);

bool isKeyword(string  name);


/** @struct TWeightedIndividual
    @brief Structure for chain list of differents sample 

*/
struct TWeightedIndividual{
        int64_t val; // index of individual
	double weight;
};

/// XEMCVBlock
struct XEMCVBlock{
  int64_t   _nbSample;               // number of samples in this CV Block
  double _weightTotal;            // weight Total of this CV Block
  TWeightedIndividual * _tabWeightedIndividual;// array (size=nbSample) of weighted individual 
};



/** @enum XEMModelName 
@brief Enumeration of model name
*/
enum XEMModelName {

  ///////////////////////
  //                   //
  //  Gaussian Models  //
  //                   //


  ///////////////////////

  /* Unknown model type */
  UNKNOWN_MODEL_NAME = -1,

	/* 28 Gaussian 'Classical' models */
	
  /* Spherical Gaussian model: proportion fixed */
  Gaussian_p_L_I = 0,
  Gaussian_p_Lk_I ,

	Gaussian_pk_L_I ,
 	Gaussian_pk_Lk_I,

  /* Diagonal Gaussian model: proportion fixed */
  Gaussian_p_L_B,
  Gaussian_p_Lk_B,
  Gaussian_p_L_Bk,
  Gaussian_p_Lk_Bk,
	
	Gaussian_pk_L_B,
 Gaussian_pk_Lk_B,
 Gaussian_pk_L_Bk,
 Gaussian_pk_Lk_Bk,

  /* Ellipsoidal Gaussian model: proportion fixed */
  Gaussian_p_L_C,
  Gaussian_p_Lk_C,
  Gaussian_p_L_D_Ak_D,
  Gaussian_p_Lk_D_Ak_D,
  Gaussian_p_L_Dk_A_Dk,
  Gaussian_p_Lk_Dk_A_Dk,
  Gaussian_p_L_Ck,
  Gaussian_p_Lk_Ck,

	Gaussian_pk_L_C,
 	Gaussian_pk_Lk_C,
 	Gaussian_pk_L_D_Ak_D,
 	Gaussian_pk_Lk_D_Ak_D,
 	Gaussian_pk_L_Dk_A_Dk,
 	Gaussian_pk_Lk_Dk_A_Dk,
 	Gaussian_pk_L_Ck,
 	Gaussian_pk_Lk_Ck,

	/*----------------*/
	/* 16 HD models*/
	/*----------------*/
 Gaussian_HD_p_AkjBkQkDk,
 Gaussian_HD_p_AkBkQkDk,
 Gaussian_HD_p_AkjBkQkD ,
 Gaussian_HD_p_AjBkQkD ,
 Gaussian_HD_p_AkjBQkD  , 
 	Gaussian_HD_p_AjBQkD  ,
 	Gaussian_HD_p_AkBkQkD ,
 	Gaussian_HD_p_AkBQkD , 
	
	Gaussian_HD_pk_AkjBkQkDk,
 	Gaussian_HD_pk_AkBkQkDk ,
	Gaussian_HD_pk_AkjBkQkD,
 Gaussian_HD_pk_AjBkQkD,
 	Gaussian_HD_pk_AkjBQkD,
 	Gaussian_HD_pk_AjBQkD ,
 	Gaussian_HD_pk_AkBkQkD,
 	Gaussian_HD_pk_AkBQkD ,


  /////////////////////
  //                 //
  //  10 Binary Models  //
  //                 //
  /////////////////////

	/* : proportion fixed  */
	Binary_p_E ,
 Binary_p_Ek,
 Binary_p_Ej ,
 Binary_p_Ekj,
 Binary_p_Ekjh ,
 /* : proportion free  */
 Binary_pk_E ,
 Binary_pk_Ek,
 Binary_pk_Ej ,
 Binary_pk_Ekj,
 Binary_pk_Ekjh ,
 
 nbXEMModelName = 54

};


const XEMModelName defaultGaussianModelName = Gaussian_pk_Lk_C;
const XEMModelName defaultBinaryModelName = Binary_pk_Ekjh;
const XEMModelName defaultGaussianHDDAModelName = Gaussian_HD_pk_AkjBkQkD;


/* Output mode */
enum XEMOutputType {
  BICstandardOutput = 0,         /* Standard output mode            */
  BICnumericStandardOutput,      /* Numerical standard output       */
  BIClabelOutput,                /* Label output                    */
  BICparameterOutput,            /* Parameter output (nmerical)     */
  BICtikOutput,                  /* Posterior probabilities output  */
  BICzikOutput,                  /* Partition output (notation 1/0) */
  BIClikelihoodOutput,           /* Log-likelihood, entropy & completed log-likelihood output */
  BICnumericLikelihoodOutput,
  BICErrorOutput = 8,            /* error code for BIC Criterion for each sestimation */

  CVstandardOutput,         /* CV Standard output mode            */
  CVnumericStandardOutput,  /* CV Numerical standard output       */
  CVlabelOutput,            /* CV Label output                    */
  CVparameterOutput,        /* CV Parameter output (nmerical)     */
  CVtikOutput,              /* CV Posterior probabilities output  */
  CVzikOutput,              /* CV Partition output (notation 1/0) */
  CVlikelihoodOutput,      /* CV Log-likelihood, entropy & completed log-likelihood output */
  CVnumericLikelihoodOutput,
  CVErrorOutput = 17,        /* error code for CV Criterion for each sestimation */

  ICLstandardOutput,         /* ICL Standard output mode            */
  ICLnumericStandardOutput,  /* ICL Numerical standard output       */
  ICLlabelOutput,            /* ICL Label output                    */
  ICLparameterOutput,        /* ICL Parameter output (nmerical)     */
  ICLtikOutput,              /* ICL Posterior probabilities output  */
  ICLzikOutput,              /* ICL Partition output (notation 1/0) */
  ICLlikelihoodOutput,      /* ICL Log-likelihood, entropy & completed log-likelihood output */
  ICLnumericLikelihoodOutput,
  ICLErrorOutput = 26,       /* error code for ICL Criterion for each sestimation */

  NECstandardOutput,         /* NEC Standard output mode            */
  NECnumericStandardOutput,  /* NEC Numerical standard output       */
  NEClabelOutput,            /* NEC Label output                    */
  NECparameterOutput,        /* NEC Parameter output (nmerical)     */
  NECtikOutput,              /* NEC Posterior probabilities output  */
  NECzikOutput,              /* NEC Partition output (notation 1/0) */
  NEClikelihoodOutput,       /* NEC Log-likelihood, entropy & completed log-likelihood output */
  NECnumericLikelihoodOutput,
  NECErrorOutput = 35,       /* error code for NEC Criterion for each sestimation */
  
  DCVstandardOutput,         /* DCV Standard output mode            */
  DCVnumericStandardOutput,  /* DCV Numerical standard output       */
  DCVlabelOutput,            /* DCV Label output                    */
  DCVparameterOutput,        /* DCV Parameter output (nmerical)     */
  DCVtikOutput,              /* DCV Posterior probabilities output  */
  DCVzikOutput,              /* DCV Partition output (notation 1/0) */
  DCVlikelihoodOutput,      /* DCV Log-likelihood, entropy & completed log-likelihood output */
  DCVnumericLikelihoodOutput,
  DCVErrorOutput = 44,       /* error code for DCV Criterion for each sestimation */
  
  completeOutput ,         /* Complete output mode            */
  numericCompleteOutput,  /* Numerical complete output       */

  CVlabelClassificationOutput,/* label of classification CV method*/

  errorMixmodOutput,         /* error code for mixmod execution*/
  errorModelOutput,                 /* error code for NEC Criterion for each sestimation */
  
  DCVinfo,                   // double cross validation information
  DCVnumericInfo = 51        // numeric double cross validation information (for validation test)
};


/** @enum ErrorError
@brief Enumeration of Error type
*/
enum XEMErrorType {
  /* Input errors */
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
  errorAllEstimation,
  errorOpenFile,
  noSelectionError,
  errorInputSymmetricMatrix,
  errorInputDiagonalMatrix,

  errorNbModality,

  knownPartitionNeeded,
  badKnownPartition,
  endDataFileReach,

  /* Numeric errors */
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
  minDeterminantDiagQtmpValueError,



  /* Others errors */
  internalMixmodError,
  errorComputationCriterion,
  wrongValueInMultinomialCase,
  errorPartitionSameSampleNotEqual,

  errorInPartitionInput,
  notEnoughValuesInLabelInput,
  notEnoughValuesInProbaInput,
  badValueInLabelInput,
  
  
  // DCV errors,
 wrongDCVinitBlocks,
  wrongDCVnumberOfBlocks,
  DCVmustBeDIAG,
  forbiddenCallToGetBestCVModel,
  //DCVonlyInGaussianCase,
  allCVCriterionErrorForAnEstimationInDCVContext,
  NbDCVBlocksTooSmall,
  
  // Multinomial
  sumFiNullAndfkTPrimNull,
  sumFiNullInMultinomialCase,
  badXEMBinaryParamterClass,
  
  
  weightTotalIsNotAnInteger,

   wrongNbKnownPartition,
   SubDimensionFreeTooLarge,
   SubDimensionFreeTooSmall,
   SubDimensionEqualTooLarge,
   SubDimensionEqualTooSmall,
   //errorSubDimension,

  wrongSymmetricMatrixDimension,
  wrongGeneralMatrixDimension,
  nonImplementedMethod,
  tabNkNotInteger,
  ungivenSubDimension,
  wrongMatrixType,
  wrongConstructorType,
  wrongNbAlgoWhenMorMAP,
  BadInitialsationWhenM,
	 BadInitialsationWhenMAP,
	 partitionMustBeComplete,
  inputNotFinalized,
	 algorithmMustBeM,
	 knownPartitionAndInitPartitionMustBeEqual,
	 nbStrategyMustBe1,
	 wrongNbKnownPartitionOrInitPartition,
	 tooManySampleInInitPartitionAndTooManyClusterNotRepresented,
	 CEM_INIT_error,
	 SEM_MAX_error,
	 SMALL_EM_error,
	 badStopNameWithSEMAlgo,
  badAlgorithmInHDContext,
   differentSubDimensionsWithMAP,
   selectionErrorWithNoEstimation,
  wrongSubDimension,
  functionShouldNotBeCalled,
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
  int64_t_max_error,
  badSetStopNameInInit, 
  badCriterion, 
  badAlgo, 
  badAlgoStop,
  badInputType,
  XEMDAInput,
  wrongModelName,
  knownPartitionNotAvailable,
  tooManyWeightColumnDescription,
  badDataDescription,
  badLabelDescription,
  errorInColumnDescription,
  errorInXEMInputSelector,
  wrongIndexInGetMethod,
  nullPointerError,
  errorEstimationStrategyRun, 
  wrongSortCallInXEMModelOutput,
  badFormat, 
  ColumnTypeNotValid
};


/// compute the determinant of a matrix and throw an exception if is is too small

//double determinant(const XEMMatrix & A,XEMErrorType errorType);
/// compute a^b and throw an error if it's equal to zero
double powAndCheckIfNotNull(double a, double b, XEMErrorType errorType = nullDeterminant);

/// compute the svd decomposition with eigenvalues in a decreasing order
void XEMSVD(const Matrix& A, DiagonalMatrix& Q, Matrix& U);

/// return the nearest int64_t
int64_t XEMRound(double d);

/// convert big char of a string in low char
void ConvertBigtoLowString(string & str);



//XEMModelNameToString
string XEMModelNameToString(const XEMModelName & modelName);

//StringToXEMModelName
XEMModelName StringToXEMModelName(const string & strModelName);

//XEMErrorTypeToString
string XEMErrorTypeToString(const XEMErrorType & errorType);

// edit modelName
void edit(const XEMModelName & modelName);


//criterionNameToString
string XEMCriterionNameToString(const XEMCriterionName & criterionName);

//StringtoXEMCriterionName
XEMCriterionName StringtoXEMCriterionName(const string & str);

//

// edit XEMCriterionName
void edit(const XEMCriterionName & criterionName);




/// editModelType
void editModelType(ofstream & oFile, XEMModelType * modelType);






// XEMAlgoNameToString
string XEMAlgoNameToString(const XEMAlgoName & typeAlgo);

//StringToAlgoName
XEMAlgoName StringToAlgoName(const string & str);

// edit XEMAlgoName
void edit(const XEMAlgoName & typeAlgo);


//XEMFormatFileToString
string XEMFormatNumericFileToString(const FormatNumeric::XEMFormatNumericFile & formatNumericFile);

//StringToXEMFormatFile
FormatNumeric::XEMFormatNumericFile StringToXEMFormatNumericFile(const string & strFormatNumericFile);

//XEMTypePartitionToString
string XEMTypePartitionToString(const TypePartition::XEMTypePartition & typePartition);

//StringToXEMTypePartition
TypePartition::XEMTypePartition StringToXEMTypePartition(const string & strTypePartition);

//XEMStrategyInitNameToString
string XEMStrategyInitNameToString(const XEMStrategyInitName & strategyInitName);

//StringToStrategyInitName
XEMStrategyInitName StringToStrategyInitName(const string & str);

// edit XEMStrategyInitName
void edit(const XEMStrategyInitName & strategyInitName);

// XEMAlgoStopNameToString
string XEMAlgoStopNameToString(const XEMAlgoStopName & algoStopName);

// void XEMAlgoStopName
void edit(const XEMAlgoStopName & algoStopName);


// is modelName has free proportion
bool hasFreeProportion(XEMModelName modelName);

// is modelName a diagonal Gaussian Model
bool isDiagonal(XEMModelName modelNamee);

// is modelName a spherical Gaussian Model
bool isSpherical(XEMModelName modelName);

// is modelName a general Gaussian Model
bool isGeneral(XEMModelName modelName);

// is modelName a EDDA (Classical Gaussian)
bool isEDDA(XEMModelName modelName);

// is modelName a HD (or HDk)
bool isHD(XEMModelName modelName);

bool isFreeSubDimension(XEMModelName modelName);


bool isBinary(XEMModelName modelName);

// is Matrix symmetric
bool isSymmetric(Matrix * mat, int64_t n);


// is Matrix Diagonal
bool isDiagonal(Matrix * mat, int64_t n);


// traitement sur les tableaux
//----------------------------

// T* copyTab(T * tab, int64_t dim)
template<typename T> T * copyTab(T * tab, int64_t dim){
	T * res = new T[dim];
  int64_t i;
	for (i=0; i<dim; i++){
		res[i] = tab[i];
	}
	return res;
}


// T ** copyTab(T ** tab, int64_t dim1, int64_t dim2)
template<typename T> T ** copyTab(T ** tab, int64_t dim1, int64_t dim2){
	T ** res = new T*[dim1];
  int64_t i,j;
	for (i=0; i<dim1; i++){
		res[i] = new T[dim2];
		for (j=0 ; j<dim2; j++)
			res[i][j] = tab[i][j];
	}
	return res;	
}



// void recopyTab(T * source, T * destination,int64_t dim)
template<typename T> void recopyTab(T * source, T * destination, int64_t dim){
  int64_t i;
	for(i=0; i<dim; i++){
		destination[i] = source[i];
	}
}

inline void recopyTab(int64_t * source, double * destination, int64_t dim){
  int64_t i;
  for(i=0; i<dim; i++){
    destination[i] = source[i];
  }
}


inline void recopyTabToVector(double ** source, vector<vector<double> > & destination, int64_t dim1, int64_t dim2){
  destination.resize(dim1); 
   int64_t i,j;
   for(i=0; i<dim1; i++){
     destination[i].resize(dim2);
     for(j=0; j<dim2; j++){
       destination[i][j] = source[i][j];
     }
   }
}


inline void recopyTabToVector(int64_t * source, vector<int64_t> & destination, int64_t dim1){
  destination.resize(dim1); 
  int64_t i;
  for(i=0; i<dim1; i++){
    destination[i] = source[i];
  }
}

inline void recopyVectorToTab(vector<vector<double> > source, double **&  destination){
  int64_t dim1 = source.size();
  int64_t dim2 = source[0].size();
  destination = new double*[dim1];
  for (int64_t i=0; i<dim1; i++){
    destination[i] = new double[dim2];
    for (int64_t k=0; k<dim2; k++){
      destination[i][k] = source[i][k];
    }
  }
}



inline void recopyVectorToTab(vector<int64_t> source, int64_t *&  destination){
  int64_t dim1 = source.size();
  destination = new int64_t[dim1];
  for (int64_t i=0; i<dim1; i++){
    destination[i] = source[i];
  }
}



// void recopyTab(T ** source, T ** destination, int64_t dim1, int64_t dim2)
template<typename T> void recopyTab(T ** source, T ** destination, int64_t dim1, int64_t dim2){
  int64_t i,j;
	for (i=0; i<dim1; i++){
		for (j=0; j<dim2; j++){
			destination[i][j] = source[i][j];
		}
	}
}


void editSimpleTab(double * tab, int64_t n, string sep = " ",string before=" ", ostream & flux = cout);
void editSimpleTab(int64_t    * tab, int64_t n, ostream & flux = cout);



template<typename T> void editTab(T ** tab, int64_t dim1, int64_t dim2, string sep=" ", string before="", ostream & flux=cout){
	T ** p_tab = tab;
	T *  p_tab_i;
  int64_t i, j ;
	for(i=0; i< dim1; i++){
		p_tab_i = *p_tab;
		flux << before;
		for(j=0; j<dim2; j++){
			flux << p_tab_i[j] << sep;
		}
		flux << endl;
		p_tab++;
	}
}


void editMatrix(GeneralMatrix & mat, int64_t nRows, ostream & flux = cout, string before = "");
void editSphericalMatrix(double mat, int64_t nRows, ostream & flux = cout, string before = "");
void editDiagMatrix(double * mat, int64_t nRows, ostream & flux = cout, string before="");

// move on a file until *what* is reached

void moveUntilReach(ifstream & fi, string  what="datafile");

void readTabFileName(ifstream & fi, int64_t nbNbCluster, string * tabFileName, string & keyWord);

void initToZero(double * tab,int64_t n);
void initToZero(double * tab,int64_t n);

void printShortcutModelType(const XEMModelType * const modelType,ostream & flux = cout);

void printModelType(const XEMModelType * const modelType,ostream & flux = cout);

const int64_t SMALL_ENOUGH_TO_USE_SELECTION_SORT = 15;
void echange(double * tab, int64_t i1, int64_t i2);
void echange(int64_t * tab  , int64_t i1, int64_t i2);

void selectionSortWithOrder(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right);

int64_t partition(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right);

void quickSortWithOrder(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right);


int64_t generateRandomIndex(bool * tabIndividualCanBeUsedForInitRandom, double * weight, double totalWeight);

void inputCriterion(ifstream & fi,XEMCriterionName & criterionName);

void inputCVinitBlocks(ifstream & fi,XEMCVinitBlocks & CVinitBlocks);

void inputDCVinitBlocks(ifstream & fi,XEMDCVinitBlocks & CVinitBlocks);

XEMAlgo * createDefaultAlgo();

#endif
