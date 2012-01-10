/***************************************************************************
                             SRC/MIXMOD/XEMUtil.cpp  description
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

#include "XEMUtil.h"
#include "XEMEMAlgo.h"
#include "XEMRandom.h"
#include "XEMModelType.h"
#include <ctype.h>



/*double determinant(const XEMMatrix& A,XEMErrorType errorType){
  XEMGeneralMatrix * A_bis;
  A_bis = (XEMGeneralMatrix * A);
  int64_t dim = A_bis.getStoreDim();
  Matrix * M = new Matrix(dim, dim);
  double * storeM = M.Store();
  double * storeA = A_bis.getStore();

  recopyTad(storeA, storeM, dim);
  double det;
  det = LogDeterminant(M).Value();
  if (det < minDeterminantValue)
      throw errorType;
  return det;
}*/

double powAndCheckIfNotNull(double a, double b, XEMErrorType errorType) {
    double res;
    res = pow(a,b);
    if (res==0.0) throw errorType;
    return res;
}


// compute the svd decomposition with eigenvalues in a decreasing order
void XEMSVD(const Matrix& A, DiagonalMatrix& Q, Matrix& U) {

    // call the SVD fucntion of Newmat
    SVD(A,Q,U);
    Real * storeQ = Q.Store();

    // Sort
    int64_t nbCols = A.Ncols();
    int64_t max;
    for (int64_t i=1; i<=nbCols ;i++) {
        //search the max eigenvalue
        max = i;
        for (int64_t j=i+1; j<=nbCols; j++) {
            if (storeQ[j-1] > storeQ[max-1])
                max = j;
            if (max != i) {
                // switch
                Real tmp = storeQ[max-1];
                storeQ[max-1] = storeQ[i-1];
                storeQ[i-1] = tmp;

                ColumnVector tmp2 = U.Column(max);
                U.Column(max) = U.Column(i);
                U.Column(i) = tmp2;
            }
        }
    }
}



//-----------------------
// return the nearest int64_t
//-----------------------
int64_t XEMRound(double d) {
    int64_t res = (int64_t)(d+0.5);
    return res;
}



//------------------------------------
// Convert big char of str in low char
//------------------------------------
void ConvertBigtoLowString(string & str) {
    for (int64_t i=0; i<str.length(); i++) {
        if (isupper(str[i]))
            str[i]=tolower(str[i]);
    }
}




//XEMCriterionNameToString
string XEMModelNameToString(const XEMModelName & modelName) {
    string res;

    switch (modelName) {
    case (UNKNOWN_MODEL_NAME) :
        res = "UNKNOWN_MODEL_TYPE";
        break;
        // Gaussian models //
    case (Gaussian_p_L_B) :
        res = "Gaussian_p_L_B";
        break;
    case (Gaussian_p_Lk_B) :
        res = "Gaussian_p_Lk_B";
        break;
    case (Gaussian_p_L_Bk) :
        res = "Gaussian_p_L_Bk";
        break;
    case (Gaussian_p_Lk_Bk) :
        res = "Gaussian_p_Lk_Bk";
        break;
    case (Gaussian_pk_L_B) :
        res = "Gaussian_pk_L_B";
        break;
    case (Gaussian_pk_Lk_B) :
        res = "Gaussian_pk_Lk_B";
        break;
    case (Gaussian_pk_L_Bk) :
        res = "Gaussian_pk_L_Bk";
        break;
    case (Gaussian_pk_Lk_Bk) :
        res = "Gaussian_pk_Lk_Bk";
        break;
    case (Gaussian_p_L_I) :
        res = "Gaussian_p_L_I";
        break;
    case (Gaussian_p_Lk_I) :
        res = "Gaussian_p_Lk_I";
        break;
    case (Gaussian_pk_L_I) :
        res = "Gaussian_pk_L_I";
        break;
    case (Gaussian_pk_Lk_I) :
        res = "Gaussian_pk_Lk_I";
        break;
    case (Gaussian_p_L_C):
        res = "Gaussian_p_L_C";
        break;
    case (Gaussian_p_Lk_C):
        res = "Gaussian_p_Lk_C";
        break;
    case (Gaussian_p_L_D_Ak_D):
        res = "Gaussian_p_L_D_Ak_D";
        break;
    case (Gaussian_p_Lk_D_Ak_D):
        res = "Gaussian_p_Lk_D_Ak_D";
        break;
    case (Gaussian_p_L_Dk_A_Dk):
        res = "Gaussian_p_L_Dk_A_Dk";
        break;
    case (Gaussian_p_Lk_Dk_A_Dk):
        res = "Gaussian_p_Lk_Dk_A_Dk";
        break;
    case (Gaussian_p_L_Ck):
        res = "Gaussian_p_L_Ck";
        break;
    case (Gaussian_p_Lk_Ck):
        res = "Gaussian_p_Lk_Ck";
        break;
    case (Gaussian_pk_L_C):
        res = "Gaussian_pk_L_C";
        break;
    case (Gaussian_pk_Lk_C):
        res = "Gaussian_pk_Lk_C";
        break;
    case (Gaussian_pk_L_D_Ak_D):
        res = "Gaussian_pk_L_D_Ak_D";
        break;
    case (Gaussian_pk_Lk_D_Ak_D):
        res = "Gaussian_pk_Lk_D_Ak_D";
        break;
    case (Gaussian_pk_L_Dk_A_Dk):
        res = "Gaussian_pk_L_Dk_A_Dk";
        break;
    case (Gaussian_pk_Lk_Dk_A_Dk):
        res = "Gaussian_pk_Lk_Dk_A_Dk";
        break;
    case (Gaussian_pk_L_Ck):
        res = "Gaussian_pk_L_Ck";
        break;
    case (Gaussian_pk_Lk_Ck):
        res = "Gaussian_pk_Lk_Ck";
        break;

        // Binary models //
    case (Binary_p_E):
        res = "Binary_p_E";
        break;
    case (Binary_p_Ek):
        res = "Binary_p_Ek";
        break;
    case (Binary_p_Ej):
        res = "Binary_p_Ej";
        break;
    case (Binary_p_Ekj):
        res = "Binary_p_Ekj";
        break;
    case (Binary_p_Ekjh):
        res = "Binary_p_Ekjh";
        break;
    case (Binary_pk_E):
        res = "Binary_pk_E";
        break;
    case (Binary_pk_Ek):
        res = "Binary_pk_Ek";
        break;
    case (Binary_pk_Ej):
        res = "Binary_pk_Ej";
        break;
    case (Binary_pk_Ekj):
        res = "Binary_pk_Ekj";
        break;
    case (Binary_pk_Ekjh):
        res = "Binary_pk_Ekjh";
        break;

    case (Gaussian_HD_pk_AkjBkQkD) :
        res = "Gaussian_HD_pk_AkjBkQkD";
        break;
    case (Gaussian_HD_pk_AkjBkQkDk) :
        res = "Gaussian_HD_pk_AkjBkQkDk";
        break;
    case (Gaussian_HD_pk_AkjBQkD) :
        res = "Gaussian_HD_pk_AkjBQkD";
        break;
    case (Gaussian_HD_pk_AjBkQkD) :
        res = "Gaussian_HD_pk_AjBkQkD";
        break;
    case (Gaussian_HD_pk_AjBQkD) :
        res = "Gaussian_HD_pk_AjBQkD";
        break;
    case (Gaussian_HD_pk_AkBkQkD) :
        res = "Gaussian_HD_pk_AkBkQkD";
        break;
    case (Gaussian_HD_pk_AkBkQkDk) :
        res = "Gaussian_HD_pk_AkBkQkDk";
        break;
    case (Gaussian_HD_pk_AkBQkD) :
        res = "Gaussian_HD_pk_AkBQkD";
        break;
    case (Gaussian_HD_p_AkjBkQkD) :
        res = "Gaussian_HD_p_AkjBkQkD";
        break;
    case (Gaussian_HD_p_AkjBkQkDk) :
        res = "Gaussian_HD_p_AkjBkQkDk";
        break;
    case (Gaussian_HD_p_AkjBQkD) :
        res = "Gaussian_HD_p_AkjBQkD";
        break;
    case (Gaussian_HD_p_AjBkQkD) :
        res = "Gaussian_HD_p_AjBkQkD";
        break;
    case (Gaussian_HD_p_AjBQkD) :
        res = "Gaussian_HD_p_AjBQkD";
        break;
    case (Gaussian_HD_p_AkBkQkD) :
        res = "Gaussian_HD_p_AkBkQkD";
        break;
    case (Gaussian_HD_p_AkBkQkDk) :
        res = "Gaussian_HD_p_AkBkQkDk";
        break;
    case (Gaussian_HD_p_AkBQkD) :
        res = "Gaussian_HD_p_AkBQkD";
        break;

    default :
        throw wrongModelType;
    }

    return res;
}

//StringToXEMModelName
XEMModelName StringToXEMModelName(const string & strModelName) {
    XEMModelName res = UNKNOWN_MODEL_NAME;

    if (strModelName.compare("UNKNOWN_MODEL_NAME") == 0)
        res = UNKNOWN_MODEL_NAME;

    // Gaussian models //
    if (strModelName.compare("Gaussian_p_L_B") == 0)
        res = Gaussian_p_L_B;
    if (strModelName.compare("Gaussian_p_Lk_B")== 0)
        res = Gaussian_p_Lk_B;
    if (strModelName.compare("Gaussian_p_L_Bk")== 0)
        res = Gaussian_p_L_Bk;
    if (strModelName.compare("Gaussian_p_Lk_Bk")== 0)
        res = Gaussian_p_Lk_Bk;
    if (strModelName.compare("Gaussian_pk_L_B")== 0)
        res = Gaussian_pk_L_B;
    if (strModelName.compare("Gaussian_pk_Lk_B")== 0)
        res = Gaussian_pk_Lk_B;
    if (strModelName.compare("Gaussian_pk_L_Bk")== 0)
        res = Gaussian_pk_L_Bk;
    if (strModelName.compare("Gaussian_pk_Lk_Bk")== 0)
        res = Gaussian_pk_Lk_Bk;
    if (strModelName.compare("Gaussian_p_L_I")== 0)
        res = Gaussian_p_L_I;
    if (strModelName.compare("Gaussian_p_Lk_I")== 0)
        res = Gaussian_p_Lk_I;
    if (strModelName.compare("Gaussian_pk_L_I")== 0)
        res = Gaussian_pk_L_I;
    if (strModelName.compare("Gaussian_pk_Lk_I")== 0)
        res = Gaussian_pk_Lk_I;
    if (strModelName.compare("Gaussian_p_L_C")== 0)
        res = Gaussian_p_L_C;
    if (strModelName.compare("Gaussian_p_Lk_C")== 0)
        res = Gaussian_p_Lk_C;
    if (strModelName.compare("Gaussian_p_L_D_Ak_D")== 0)
        res = Gaussian_p_L_D_Ak_D;
    if (strModelName.compare("Gaussian_p_Lk_D_Ak_D")== 0)
        res = Gaussian_p_Lk_D_Ak_D;
    if (strModelName.compare("Gaussian_p_L_Dk_A_Dk")== 0)
        res = Gaussian_p_L_Dk_A_Dk;
    if (strModelName.compare("Gaussian_p_Lk_Dk_A_Dk")== 0)
        res = Gaussian_p_Lk_Dk_A_Dk;
    if (strModelName.compare("Gaussian_p_L_Ck")== 0)
        res = Gaussian_p_L_Ck;
    if (strModelName.compare("Gaussian_p_Lk_Ck")== 0)
        res = Gaussian_p_Lk_Ck;
    if (strModelName.compare("Gaussian_pk_L_C")== 0)
        res = Gaussian_pk_L_C;
    if (strModelName.compare("Gaussian_pk_Lk_C")== 0)
        res = Gaussian_pk_Lk_C;
    if (strModelName.compare("Gaussian_pk_L_D_Ak_D")== 0)
        res = Gaussian_pk_L_D_Ak_D;
    if (strModelName.compare("Gaussian_pk_Lk_D_Ak_D")== 0)
        res = Gaussian_pk_Lk_D_Ak_D;
    if (strModelName.compare("Gaussian_pk_L_Dk_A_Dk")== 0)
        res = Gaussian_pk_L_Dk_A_Dk;
    if (strModelName.compare("Gaussian_pk_Lk_Dk_A_Dk")== 0)
        res = Gaussian_pk_Lk_Dk_A_Dk;
    if (strModelName.compare("Gaussian_pk_L_Ck")== 0)
        res = Gaussian_pk_L_Ck;
    if (strModelName.compare("Gaussian_pk_Lk_Ck")== 0)
        res = Gaussian_pk_Lk_Ck;

    // Binary models //
    if (strModelName.compare("Binary_p_E")== 0)
        res = Binary_p_E;
    if (strModelName.compare("Binary_p_Ek")== 0)
        res = Binary_p_Ek;
    if (strModelName.compare("Binary_p_Ej")== 0)
        res = Binary_p_Ej;
    if (strModelName.compare("Binary_p_Ekj")== 0)
        res = Binary_p_Ekj;
    if (strModelName.compare("Binary_p_Ekjh")== 0)
        res = Binary_p_Ekjh;
    if (strModelName.compare("Binary_pk_E")== 0)
        res = Binary_pk_E;
    if (strModelName.compare("Binary_pk_Ek")== 0)
        res = Binary_pk_Ek;
    if (strModelName.compare("Binary_pk_Ej")== 0)
        res = Binary_pk_Ej;
    if (strModelName.compare("Binary_pk_Ekj")== 0)
        res = Binary_pk_Ekj;
    if (strModelName.compare("Binary_pk_Ekjh")== 0)
        res = Binary_pk_Ekjh;

    //HDModel
    if (strModelName.compare("Gaussian_HD_pk_AkjBkQkD")== 0)
        res = Gaussian_HD_pk_AkjBkQkD;
    if (strModelName.compare("Gaussian_HD_pk_AkjBkQkDk")== 0)
        res = Gaussian_HD_pk_AkjBkQkDk;
    if (strModelName.compare("Gaussian_HD_pk_AkjBQkD")== 0)
        res = Gaussian_HD_pk_AkjBQkD;
    if (strModelName.compare("Gaussian_HD_pk_AjBkQkD")== 0)
        res = Gaussian_HD_pk_AjBkQkD;
    if (strModelName.compare("Gaussian_HD_pk_AjBQkD")== 0)
        res = Gaussian_HD_pk_AjBQkD;
    if (strModelName.compare("Gaussian_HD_pk_AkBkQkD")== 0)
        res = Gaussian_HD_pk_AkBkQkD;
    if (strModelName.compare("Gaussian_HD_pk_AkBkQkDk")== 0)
        res = Gaussian_HD_pk_AkBkQkDk;
    if (strModelName.compare("Gaussian_HD_pk_AkBQkD")== 0)
        res = Gaussian_HD_pk_AkBQkD;
    if (strModelName.compare("Gaussian_HD_p_AkjBkQkD")== 0)
        res = Gaussian_HD_p_AkjBkQkD;
    if (strModelName.compare("Gaussian_HD_p_AkjBkQkDk")== 0)
        res = Gaussian_HD_p_AkjBkQkDk;
    if (strModelName.compare("Gaussian_HD_p_AkjBQkD")== 0)
        res = Gaussian_HD_p_AkjBQkD;
    if (strModelName.compare("Gaussian_HD_p_AjBkQkD")== 0)
        res = Gaussian_HD_p_AjBkQkD;
    if (strModelName.compare("Gaussian_HD_p_AjBQkD")== 0)
        res = Gaussian_HD_p_AjBQkD;
    if (strModelName.compare("Gaussian_HD_p_AkBkQkD")== 0)
        res = Gaussian_HD_p_AkBkQkD;
    if (strModelName.compare("Gaussian_HD_p_AkBkQkDk")== 0)
        res = Gaussian_HD_p_AkBkQkDk;
    if (strModelName.compare("Gaussian_HD_p_AkBQkD")== 0)
        res = Gaussian_HD_p_AkBQkD;

    if (res == UNKNOWN_MODEL_NAME)
        throw wrongModelType;

    return res;
}

// edit modelName
void edit(const XEMModelName & modelName) {
#ifdef DEBUG
    cout<<XEMModelNameToString(modelName);
#endif
}


//XEMErrorTypeToString
string XEMErrorTypeToString(const XEMErrorType & errorType) {
    string res;
    switch (errorType) {
    case (noError) :
        res = "No error";
        break ;
    case (nbLinesTooLarge) :
        res = "Number of lines too large";
        break ;
    case (nbLinesTooSmall) :
        res = "Number of lines too small";
        break ;
    case (pbDimensionTooLarge) :
        res = "Dimension size too large";
        break ;
    case (pbDimensionTooSmall) :
        res = "Dimension size too small";
        break ;
    case (nbCriterionTooLarge) :
        res = "Number of criterion too large";
        break ;
    case (nbCriterionTooSmall) :
        res ="Number of criterion too small";
        break ;
    case (wrongCriterionName) :
        res = "Wrong criterion name/type";
        break ;
    case (nbNbClusterTooLarge) :
        res = "List of number of clusters too large";
        break ;
    case (nbNbClusterTooSmall) :
        res = "List of number of clusters too small";
        break ;
    case (nbModelTypeTooLarge) :
        res = "Number of models too large";
        break ;
    case (nbModelTypeTooSmall) :
        res = "Number of models too small";
        break ;
    case (wrongModelType) :
        res = "Wrong model name/type";
        break ;
    case (wrongCVinitType) :
        res = "Wrong CVinitType";
        break ;
    case (wrongDCVinitType) :
        res = "Wrong DCVinitType";
        break ;
    case (nbStrategyTypeTooLarge) :
        res = "Number of strategies too large";
        break ;

    case (nbStrategyTypeTooSmall) :
        res = "Number of strategies too small";
        break ;
    case (wrongStrategyInitName) :
        res = "Wrong strategy initialization name";
        break ;
    case (errorInitParameter) :
        res = "Error with USER initialisation";
        break ;
    case (nbAlgoTooLarge) :
        res = "Number of algorithms too large";
        break ;
    case (nbAlgoTooSmall) :
        res = "Number of algorithms too small";
        break ;
    case wrongAlgoType :
        res = "Wrong algorithm type";
        break ;
    case nbIterationTooLarge :
        res = "Number of iterations too large";
        break ;
    case nbIterationTooSmall :
        res = "Number of iterations too small";
        break ;
    case epsilonTooSmall :
        res = "Value of epsilon too small";
        break ;
    case epsilonTooLarge :
        res = "Value of epsilon too large";
        break;
    case wrongDataFileName :
        res = "Wrong data file name";
        break ;
    case wrongLabelFileName :
        res = "Wrong label file name";
        break ;
    case wrongWeightFileName :
        res = "Wrong weight file name";
        break ;
    case wrongParamFileName :
        res = "Wrong parameter file name";
        break ;
    case wrongPartitionFileName :
        res = "Wrong label file name";
        break ;
    case wrongAlgoStopName :
        res = "Wrong stopping rules for algorithm";
        break ;
    case wrongOutputType :
        res = "Wrong output mode type";
        break ;
    case wrongInputFileName :
        res = "Wrong input file name";
        break ;
    case wrongXEMNbParam :
        res = "Wrong number of paramaters for Mixmod call";
        break ;
    case errorNbLines :
        res = "Bad writing \"NbLines\" key word";
        break ;
    case errorPbDimension :
        res = "Bad writing \"PbDimension\" key word";
        break ;
    case errorNbCriterion :
        res = "Bad writing \"NbCriterion\" key word";
        break ;
    case errorListCriterion :
        res = "Bad writing \"ListCriterion\" key word";
        break ;
    case errorNbNbCluster :
        res = "Bad writing \"NbNbCluster\" key word";
        break ;
    case errorListNbCluster :
        res = "Bad writing \"ListNbCluster\" key word";
        break ;
    case errorNbModel :
        res = "Bad writing \"NbModel\" key word";
        break ;
    case errorListModel :
        res = "Bad writing \"ListModel\" key word";
        break ;
    case errorNbStrategy :
        res = "Bad writing \"NbStrategy\" key word";
        break ;

    case errorInitType :
        res = "Bad writing \"InitType\" key word";
        break ;
    case errorInitFile :
        res = "Bad writing \"InitFile\" key word";
        break ;
    case errorNbAlgo :
        res = "Bad writing \"NbAlgorithm\" key word";
        break ;
    case errorAlgo :
        res = "Bad writing \"Algorithm\" key word";
        break ;
    case errorStopRule :
        res = "Bad writing \"StopRule\" key word";
        break ;
    case errorStopRuleValue :
        res = "Bad writing \"StopRuleValue\" key word";
        break ;

    case errorDataFile :
        res = "Bad writing \"DataFile\" key word";
        break ;
    case nbAlgoTypeTooSmall :
        res = "number of algoType too small";
        break ;
    case badStrategyInitName :
        res = "strategyInitName incompatible with algoType";
        break ;
    case errorAllEstimation :
        res = "All estimations generate error. so, no selection have been done";
        break ;
    case errorOpenFile :
        res = "Error when opening a output file";
        break ;
    case noSelectionError :
        res = "Error in all selections : parameters are computed but there is no 'best model'";
        break ;
    case errorInputSymmetricMatrix :
        res = "Input Sigma Matrix is not symmetric";
        break ;
    case errorInputDiagonalMatrix :
        res = "Input Sigma Matrix is not diagonal";
        break ;

    case errorNbModality :
        res = "Minimum number of modality is 2";
        break ;

    case knownPartitionNeeded :
        res = "known partition is needed for M algorithm";
        break ;
    case badKnownPartition :
        res = "bad known partition";
        break ;
    case endDataFileReach :
        res = "the end of data file has been reached before reading all samples : verify nbSample or data file";
        break ;


        /* Numeric errors */
    case nonPositiveDefiniteMatrix :
        res = "Variance matrix is non positive definite";
        break ;
    case nullDeterminant :
        res = "Matrix determinant null";
        break ;
    case randomProblem :
        res = "Problem in Random function call";
        break ;
    case nullLikelihood :
        res = "Null likelihood";
        break ;
    case noProbability :
        res = "Probability (cf. NormPdf function) not between 0 and 1";
        break ;
    case pbNEC :
        res = "NEC criterion, the diferrence between log-likelihood for k and 1 cluster is null";
        break ;
    case nullNk :
        res = "Empty mixture component";
        break ;

    case numericError :
        res = "A numeric error occured";
        break ;
    case errorSigmaConditionNumber :
        res = "error in variance matrix condition number";
        break ;
    case minDeterminantSigmaValueError :
    case minDeterminantWValueError :
    case minDeterminantDiagWkValueError :
    case minDeterminantDiagWValueError :
    case minDeterminantBValueError :
    case minDeterminantRValueError :
    case minDeterminantWkValueError :
    case minDeterminantShapeValueError :
    case minDeterminantDiagQtmpValueError :
        res = "determinant of matrix too small";
        break ;


        /* Others errors */
    case internalMixmodError :
        res = "Internal error in mixmod sofware";
        break ;
    case wrongValueInMultinomialCase :
        res = "wrong value in data set : use 1,2...nbModality";
        break ;
    case errorPartitionSameSampleNotEqual :
        res = "2 same samples must have same label";
        break ;

    case errorInPartitionInput :
        res = "Error in partition file : there is not enough lines in the file (nbSample is required)";
        break ;

    case notEnoughValuesInLabelInput :
        res = "Error in label file : there is not enough values in the file (nbSample is required)";
        break ;
    case notEnoughValuesInProbaInput :
        res = "Error in proba file : there is not enough values in the file (nbSample*nbCluster is required)";
        break ;

    case badValueInLabelInput :
        res = "Error in label file : label must be between 1 and nbCluster";
        break ;


        // DCV errors,
    case wrongDCVinitBlocks :
        res = "DCV error : wrong init block specification, must be either RANDOM or DIAG";
        break ;
    case wrongDCVnumberOfBlocks :
        res = "DCV error : wrong number of blocks, must be between 2 and the number of samples";
        break ;
    case DCVmustBeDIAG :
        res = "DCV error : in this situation DCV init block specification must be DIAG";
        break ;
    case forbiddenCallToGetBestCVModel :
        res = "DCV error : call to getBestCVModel is forbidden in the current context";
        break ;
        //DCVonlyInGaussianCase,
    case allCVCriterionErrorForAnEstimationInDCVContext :
        res = "All CV Criterion have error for an estimation in DCV context";
        break ;
    case NbDCVBlocksTooSmall :
        res = "Number of DCV Blocks is too small (or NbSample is too small)";
        break ;

        // Multinomial
    case sumFiNullAndfkTPrimNull :
        res = "SumF[i]=0 in multinomial case";
        break ;
    case sumFiNullInMultinomialCase :
        res = "SumF[i]=0 in multinomial case. Please report this bug to Mixmod team !";
        break ;
    case badXEMBinaryParamterClass :
        res = "Internal Mixmod Error: bad XEMBinrayParameter Class";
        break ;


    case weightTotalIsNotAnInteger :
        res = "Error : weightTotal must be an integer";
        break ;

    case wrongNbKnownPartition :
        res = "Error : wrong number of known Partition in input object";
        break ;

    case ungivenSubDimension :
        res = "Error : sub dimensions are not given for one or several models";
        break ;
    case wrongMatrixType :
        res = "Error : trying to apply a method on a wrong matrix type ";
        break ;
    case wrongConstructorType :
        res = "Error : when constructing an object by default";
        break ;
    case wrongNbAlgoWhenMorMAP :
        res = "Error : wrong number of algortihms if M or MAP are used";
        break ;
    case BadInitialsationWhenM :
        res = "Error : USER_PARTITION must be the initialisation if M is used";
        break ;
    case BadInitialsationWhenMAP :
        res = "Error : USER must be the initialisation if MAP is used";
        break ;
    case partitionMustBeComplete :
        res = "Error : partition must be complete";
        break ;
    case inputNotFinalized :
        res = "Error : input is not finalized";
        break ;
    case algorithmMustBeM :
        res = "Error : algorithm must be M";
        break ;
    case knownPartitionAndInitPartitionMustBeEqual :
        res = "Error : knownLabel And InitLabel must be equal";
        break ;
    case nbStrategyMustBe1 :
        res = "Error : nbStrategy must be equal to 1";
        break ;
    case wrongNbKnownPartitionOrInitPartition :
        res = "Error : wrong number of knownLabel or InitLabel";
        break ;
    case tooManySampleInInitPartitionAndTooManyClusterNotRepresented :
        res = "Error : error in USER_PARTITION initialization : Too many sample in InitPartition and too many cluster not represented";
        break ;
    case CEM_INIT_error :
        res = "Error : error in CEM_INIT initialization : in all tries occurs an error";
        break ;
    case SEM_MAX_error :
        res = "Error : error in SEM_MAX initialization : in all tries occurs an error";
        break ;
    case SMALL_EM_error :
        res = "Error : error in SMALL_EM initialization : in all tries occurs an error";
        break ;
    case badStopNameWithSEMAlgo :
        res = "Error : bad stop type with SEM : this algortihm must be stopped after a predefined number of iterations";
        break ;
    case badAlgorithmInHDContext :
        res = "Error : bad algorithm in HD context : only M or MAP is available";
        break ;
    case differentSubDimensionsWithMAP :
        res = "Error : given subDimensions in init file and input file are different";
        break ;
    case selectionErrorWithNoEstimation :
        res = "Error : All estimation generated error and no selection has been done";
        break ;
    case wrongSubDimension :
        res = "Error : Wrong sub dimension type for given model";
        break ;
    case functionShouldNotBeCalled :
        res = "Internal Error : a bad function has been called";
        break ;
    case missingRequiredInputs:
        res = "Error : Missing required inputs (data, nbSample, pbDimension, tabNbCluster, nbNbCluster)";
        break ;
    case wrongCriterionPositionInSet :
        res = "Wrong criterion position in set";
        break ;
    case wrongCriterionPositionInGet :
        res = "Wrong criterion position in get";
        break ;
    case wrongCriterionPositionInInsert :
        res = "Wrong criterion position in insert";
        break ;
    case wrongCriterionPositionInRemove :
        res = "Wrong criterion position in remove";
        break ;
    case wrongModelPositionInSet :
        res = "Wrong model position in set";
        break ;
    case wrongModelPositionInGet :
        res = "Wrong model position in get";
        break ;
    case wrongModelPositionInInsert :
        res = "Wrong model position in insert";
        break ;
    case wrongModelPositionInRemove :
        res = "Wrong model position in remove";
        break ;
    case wrongModelPositionInSetSubDimensionEqual :
        res = "Wrong model position in set sub dimension equal";
        break ;
    case wrongModelPositionInSetSubDimensionFree :
        res = "Wrong model position in set sub dimension free";
        break ;
    case badSetKnownPartition :
        res = "Error in setKnownPartition (impossible if nbNbCluster>1)";
        break ;
    case wrongStrategyPositionInSetOrGetMethod :
        res = "Wrong strategy position in set or get method";
        break ;
    case badInitPart :
        res = "Bad Initialization Partition";
        break;
    case nbTryInStrategyTooSmall :
        res = "Number of tries in strategy too small";
        break;
    case nbTryInStrategyTooLarge :
        res = "Number of tries in strategy too large";
        break;
    case nbTryInInitTooSmall :
        res = "Number of tries in init too small";
        break;
    case nbTryInInitTooLarge :
        res = "Number of tries in init too large";
        break;
    case nbIterationInInitTooSmall :
        res = "Number of iterations in init too small";
        break;
    case nbIterationInInitTooLarge :
        res = "Number of iterations in init too large";
        break;
    case epsilonInInitTooSmall :
        res = "Epsilon in init too small";
        break;
    case epsilonInInitTooLarge :
        res = "Epsilon in init too large";
        break;
    case wrongNbStrategyTryValue :
        res = "Wrong number of tries in strategy";
        break;
    case badSetNbTry :
        res = "Number of tries in strategy could not change";
        break;
    case badSetNbTryInInit :
        res = "Number of tries in init could not change";
        break;
    case badSetNbIterationInInit :
        res = "Number of iterations in init could not change";
        break;
    case badSetEpsilonInInit :
        res = "Epsilon in init could not change";
        break;
    case int64_t_max_error :
        res = "int64_t limits reached";
        break;
    case badSetStopNameInInit :
        res = "Stop name could not change in this context";
        break;
    case badCriterion :
        res = "Bad Criterion";
        break;
    case badAlgo :
        res = "Bad Algorithm";
        break;
    case badAlgoStop :
        res = "Bad Algorithm Stop Name";
        break;
    case XEMDAInput :
        res = "XEMDAInput not implemented";
        break;
    case wrongModelName :
        res = "Wrong model Name";
        break;
    case knownPartitionNotAvailable :
        res = "known Partition is not available";
        break;
    case tooManyWeightColumnDescription :
        res = "Too many WeightColumnDescription";
        break;
    case badDataDescription :
        res = "Bad Data Description";
        break;
    case badLabelDescription :
        res = "Bad Label Description";
        break;
    case errorInColumnDescription :
        res = "Bad size of Column Description";
        break;
    case errorInXEMInputSelector :
        res = "Bad size of Column Description";
        break;
    case wrongIndexInGetMethod :
        res = "wrong index in get method";
        break;
    case nullPointerError :
        res = "Internal error (Null pointer)";
        break;
    case wrongSortCallInXEMModelOutput :
        res = "Bad sort call method in a XEMModelOutput (creating without estimation)";
        break;
    case errorEstimationStrategyRun :
	res = "Bad Estimation in run" ;
	break;
    case badFormat :
	res = "Bad Format" ;
	break; 
    case ColumnTypeNotValid :
	res = "Bad Format" ;
	break;   
    case badInputType :
	res = "Bad Input type";
	break;
    }   
    
    return res;
}




string XEMKeywordToString(const XEMKeyword & keyword) {
    string res;
    switch (keyword) {
    case NbLines :
        res = "NbLines";
        break;
    case PbDimension :
        res = "PbDimension";
        break;
    case NbNbCluster :
        res = "NbNbCluster";
        break;
    case ListNbCluster :
        res = "ListNbCluster";
        break;
    case NbModality :
        res = "NbModality";
        break;
    case NbCriterion :
        res = "NbCriterion";
        break;
    case ListCriterion :
        res = "ListCriterion";
        break;
    case NbModel :
        res = "NbModel";
        break;
    case ListModel :
        res = "ListModel";
        break;
    case subDimensionEqual :
        res = "subDimensionEqual";
        break;
    case subDimensionFree :
        res = "subDimensionFree";
        break;
    case NbStrategy :
        res = "NbStrategy";
        break;
    case InitType :
        res = "InitType";
        break;
    case InitFile :
        res = "InitFile";
        break;
    case NbAlgorithm :
        res = "NbAlgorithm";
        break;
    case Algorithm :
        res = "Algorithm";
        break;
    case PartitionFile :
        res = "PartitionFile";
        break;
    case DataFile :
        res = "DataFile";
        break;
    case WeightFile :
        res = "WeightFile";
        break;
    case NbCVBlocks :
        res = "nbCVBlocks";
        break;
    case CVinitBlocks :
        res = "CVinitBlocks";
        break;
    case NbDCVBlocks :
        res = "NbDCVBlocks";
        break;
    case DCVinitBlocks :
        res = "DCVinitBlocks";
        break;
    }
    return res;
}

bool isKeyword(string  name) {
    bool res = false;
    for (int64_t i=0; i<SizeKeyword; ++i) {
        if (name.compare(XEMKeywordToString(XEMKeyword(i)))==0) {
            res = true;
        }
    }

    return res;
}







//XEMCriterionNameToString
string XEMCriterionNameToString(const XEMCriterionName & criterionName) {
    string res;
    switch (criterionName) {
    case UNKNOWN_CRITERION_NAME :
        res = "UNKNOWN_CRITERION_NAME" ;
        break ;
    case BIC :
        res = "BIC";
        break;
    case ICL :
        res = "ICL";
        break;
    case NEC :
        res = "NEC";
        break;
    case CV :
        res = "CV";
        break;
    case DCV :
        res = "DCV";
        break;
    }
    return res;
}

//StringtoXEMCriterionName
XEMCriterionName StringtoXEMCriterionName(const string & str) {
    XEMCriterionName res = UNKNOWN_CRITERION_NAME;
    if (str.compare("UNKNOWN_CRITERION_NAME") == 0)
        res = UNKNOWN_CRITERION_NAME;
    if (str.compare("BIC") == 0)
        res = BIC;
    if (str.compare("ICL") == 0)
        res = ICL;
    if (str.compare("NEC") == 0)
        res = NEC;
    if (str.compare("CV") == 0)
        res = CV;
    if (str.compare("DCV") == 0)
        res = DCV;

    if (res == UNKNOWN_CRITERION_NAME)
        throw badCriterion;

    return res;
}


// edit XEMCriterionName
void edit(const XEMCriterionName & criterionName) {
#ifdef DEBUG
    cout<<XEMCriterionNameToString(criterionName);
#endif
}


//XEMAlgoNameToString
string XEMAlgoNameToString(const XEMAlgoName & typeAlgo) {
    string res;
    switch (typeAlgo) {
    case UNKNOWN_ALGO_NAME :
        res = "UNKNOWN_ALGO_NAME";
        break;
    case EM :
        res = "EM";
        break;
    case CEM :
        res = "CEM";
        break;
    case SEM :
        res = "SEM";
        break;
    case MAP :
        res = "MAP";
        break;
    case M :
        res = "M";
        break;
    }
    return res;
}

//StringToAlgoName
XEMAlgoName StringToAlgoName(const string & str) {
    XEMAlgoName res = UNKNOWN_ALGO_NAME;
    if (str.compare("UNKNOWN_ALGO_NAME") == 0)
        res = UNKNOWN_ALGO_NAME;
    if (str.compare("EM") == 0)
        res = EM;
    if (str.compare("CEM") == 0)
        res = CEM;
    if (str.compare("SEM") == 0)
        res = SEM;
    if (str.compare("MAP") == 0)
        res = MAP;
    if (str.compare("M") == 0)
        res = M;
    if (res == UNKNOWN_ALGO_NAME)
        throw badAlgo;

    return res;
}

//XEMAlgoNameToString
string XEMAlgoStopNameToString(const XEMAlgoStopName & algoStopName) {
    string res;
    switch (algoStopName) {
    case NO_STOP_NAME :
        res = "NO_STOP_NAME";
        break;
    case NBITERATION :
        res = "NBITERATION";
        break;
    case EPSILON :
        res = "EPSILON";
	break;
    case NBITERATION_EPSILON :
        res = "NBITERATION_EPSILON";
        break;
    }
    return res;
}

//StringToAlgoStopName
XEMAlgoStopName StringToAlgoStopName(const string & str) {
    XEMAlgoStopName res = NO_STOP_NAME;
    if (str.compare("NO_STOP_NAME") == 0)
        res = NO_STOP_NAME;
    if (str.compare("NBITERATION") == 0)
        res = NBITERATION;
    if (str.compare("EPSILON") == 0)
        res = EPSILON;
    if (str.compare("NBITERATION_EPSILON") == 0)
        res = NBITERATION_EPSILON;
    if (res == NO_STOP_NAME)
        throw badAlgoStop;
    return res;
}

// edit typeAlgo
void edit(const XEMAlgoName & typeAlgo) {
#ifdef DEBUG
    cout<<XEMAlgoNameToString(typeAlgo);
#endif
}

//XEMFormatNumericToString
string XEMFormatNumericFileToString(const FormatNumeric::XEMFormatNumericFile & formatNumericFile){
    string res;
    switch (formatNumericFile) {
    case FormatNumeric::txt :
        res = "txt";
        break;
    case FormatNumeric::hdf5 :
        res = "hdf5";
        break;
    case FormatNumeric::XML :
        res = "XML";
        break;
    }
    return res;
}

//StringToXEMFormatFile
FormatNumeric::XEMFormatNumericFile StringToXEMFormatNumericFile(const string & strFormatNumericFile){
    FormatNumeric::XEMFormatNumericFile res ;
    if (strFormatNumericFile.compare("txt") == 0){
        res = FormatNumeric::txt;
    }else if (strFormatNumericFile.compare("hdf5") == 0){
      res = FormatNumeric::hdf5;
    }else if (strFormatNumericFile.compare("XML") == 0){
      res = FormatNumeric::XML;
    }else{
      throw badFormat;
    }
    return res;
}

//XEMTypePartitionToString
string XEMTypePartitionToString(const TypePartition::XEMTypePartition & typePartition){
  string res;
  switch(typePartition){
    case TypePartition::UNKNOWN_PARTITION :
      res = "UNKNOWN_PARTITION";
      break;
    case TypePartition::label :
      res = "label";
      break;
    case TypePartition::partition :
      res = "partition";
      break;
  }
  return res;
}
      

//StringToXEMTypePartition
TypePartition::XEMTypePartition StringToXEMTypePartition(const string & strTypePartition){
  TypePartition::XEMTypePartition res = TypePartition::UNKNOWN_PARTITION;
  if (strTypePartition.compare("UNKNOWN_PARTITION") == 0)
    res = TypePartition::UNKNOWN_PARTITION;
  if (strTypePartition.compare("label") == 0)
    res = TypePartition::label;
  if (strTypePartition.compare("partition") == 0)
    res = TypePartition::partition;
  
  return res;
}

// XEMStrategyInitNameToString
string XEMStrategyInitNameToString(const XEMStrategyInitName & strategyInitName) {
    string res;
    switch (strategyInitName) {
    case RANDOM :
        res = "RANDOM";
        break;
    case CEM_INIT :
        res = "CEM_INIT";
        break;
    case SEM_MAX :
        res = "SEM_MAX";
        break;
    case SMALL_EM :
        res = "SMALL_EM";
        break;
    case USER :
        res = "USER";
        break;
    case USER_PARTITION :
        res = "USER_PARTITION";
        break;
    }
    return res;
}

//StringToStrategyInitName
XEMStrategyInitName StringToStrategyInitName(const string & str){
    
  XEMStrategyInitName res;
    if (str.compare("RANDOM") == 0)
      res = RANDOM;
    if (str.compare("CEM_INIT") == 0)
      res = CEM_INIT;
    if (str.compare("SEM_MAX") == 0)
      res = SEM_MAX;
    if (str.compare("SMALL_EM") == 0)
      res = SMALL_EM;
    if (str.compare("PARAMETER") == 0)
      res = USER;
    if (str.compare("PARTITION") == 0)
      res = USER_PARTITION;

    return res;  
}

// edit strategyInitName
void edit(const XEMStrategyInitName & strategyInitName) {
#ifdef DEBUG
    cout<<XEMStrategyInitNameToString(strategyInitName);
#endif
}

//edit algoStopName
void edit(const XEMAlgoStopName & algoStopName) {
#ifdef DEBUG
    cout<<XEMAlgoStopNameToString(algoStopName);
#endif
}


//---------------
/// editModelType
//----------------
void editModelType(ofstream & oFile, XEMModelType * modelType) {

    oFile<<"\t\t\tModel Type : ";

    switch (modelType->_nameModel) {

        // Gaussian models //
    case (Gaussian_p_L_B) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<"p_L_B";
        break;
    case (Gaussian_p_Lk_B) :
        oFile<<"p_Lk_B";
        break;
        oFile<<"Gaussian Diagonal Model : ";
    case (Gaussian_p_L_Bk) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<"p_L_Bk";
        break;
    case (Gaussian_p_Lk_Bk) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<"p_Lk_Bk";
        break;
    case (Gaussian_pk_L_B) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<" pk_L_B";
        break;
    case (Gaussian_pk_Lk_B) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<" pk_Lk_B";
        break;
    case (Gaussian_pk_L_Bk) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<"pk_L_Bk";
        break;
    case (Gaussian_pk_Lk_Bk) :
        oFile<<"Gaussian Diagonal Model : ";
        oFile<<"pk_Lk_Bk";
        break;
    case (Gaussian_p_L_I) :
        oFile<<"Gaussian Spherical Model : ";
        oFile<<"p_L_I";
        break;
    case (Gaussian_p_Lk_I) :
        oFile<<"Gaussian Spherical Model : ";
        oFile<<"p_Lk_I";
        break;
    case (Gaussian_pk_L_I) :
        oFile<<"Gaussian Spherical Model : ";
        oFile<<"pk_L_I";
        break;
    case (Gaussian_pk_Lk_I) :
        oFile<<"Gaussian Spherical Model : ";
        oFile<<"pk_Lk_I";
        break;
    case (Gaussian_p_L_C):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_L_C";
        break;
    case (Gaussian_p_Lk_C):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_Lk_C";
        break;
    case (Gaussian_p_L_D_Ak_D):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_L_D_Ak_D";
        break;
    case (Gaussian_p_Lk_D_Ak_D):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_Lk_D_Ak_D";
        break;
    case (Gaussian_p_L_Dk_A_Dk):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_L_Dk_A_Dk";
        break;
    case (Gaussian_p_Lk_Dk_A_Dk):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_Lk_Dk_A_Dk";
        break;
    case (Gaussian_p_L_Ck):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_L_Ck";
        break;
    case (Gaussian_p_Lk_Ck):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"p_Lk_Ck";
        break;
    case (Gaussian_pk_L_C):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_L_C";
        break;
    case (Gaussian_pk_Lk_C):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_Lk_C";
        break;
    case (Gaussian_pk_L_D_Ak_D):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_L_D_Ak_D";
        break;
    case (Gaussian_pk_Lk_D_Ak_D):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_Lk_D_Ak_D";
        break;
    case (Gaussian_pk_L_Dk_A_Dk):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_L_Dk_A_Dk";
        break;
    case (Gaussian_pk_Lk_Dk_A_Dk):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_Lk_Dk_A_Dk";
        break;
    case (Gaussian_pk_L_Ck):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_L_Ck";
        break;
    case (Gaussian_pk_Lk_Ck):
        oFile<<"Gaussian Ellipsoidal Model : ";
        oFile<<"pk_Lk_Ck";
        break;

        // Binary models //
    case (Binary_p_E):
        oFile<<"Binary Model : ";
        oFile<<"Binary_p_E";
        break;
    case (Binary_p_Ek):
        oFile<<"Binary Model : ";
        oFile<<"Binary_p_Ek";
        break;
    case (Binary_p_Ej):
        oFile<<"Binary Model : ";
        oFile<<"Binary_p_Ej";
        break;
    case (Binary_p_Ekj):
        oFile<<"Binary Model : ";
        oFile<<"Binary_p_Ekj";
        break;
    case (Binary_p_Ekjh):
        oFile<<"Binary Model : ";
        oFile<<"Binary_p_Ekjh";
        break;
    case (Binary_pk_E):
        oFile<<"Binary Model : ";
        oFile<<"Binary_pk_E";
        break;
    case (Binary_pk_Ek):
        oFile<<"Binary Model : ";
        oFile<<"Binary_pk_Ek";
        break;
    case (Binary_pk_Ej):
        oFile<<"Binary Model : ";
        oFile<<"Binary_pk_Ej";
        break;
    case (Binary_pk_Ekj):
        oFile<<"Binary Model : ";
        oFile<<"Binary_pk_Ekj";
        break;
    case (Binary_pk_Ekjh):
        oFile<<"Binary Model : ";
        oFile<<"Binary_pk_Ekjh";
        break;

    case (Gaussian_HD_pk_AkjBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkjBkQkD";
        break;
    case (Gaussian_HD_pk_AkjBkQkDk) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkjBkQkDk";
        break;
    case (Gaussian_HD_pk_AkjBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkjBQkD";
        break;
    case (Gaussian_HD_pk_AjBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AjBkQkD";
        break;
    case (Gaussian_HD_pk_AjBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AjBQkD";
        break;
    case (Gaussian_HD_pk_AkBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkBkQkD";
        break;
    case (Gaussian_HD_pk_AkBkQkDk) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkBkQkDk";
        break;
    case (Gaussian_HD_pk_AkBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_pk_AkBQkD";
        break;
    case (Gaussian_HD_p_AkjBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkjBkQkD";
        break;
    case (Gaussian_HD_p_AkjBkQkDk) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkjBkQkDk";
        break;
    case (Gaussian_HD_p_AkjBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkjBQkD";
        break;
    case (Gaussian_HD_p_AjBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AjBkQkD";
        break;
    case (Gaussian_HD_p_AjBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AjBQkD";
        break;
    case (Gaussian_HD_p_AkBkQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkBkQkD";
        break;
    case (Gaussian_HD_p_AkBkQkDk) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkBkQkDk";
        break;
    case (Gaussian_HD_p_AkBQkD) :
        oFile<<"HD Model : ";
        oFile<<"Gaussian_HD_p_AkBQkD";
        break;



    default :
        oFile<<"Model Type Error";
    }
    oFile<<endl;
    oFile<<"\t\t\t----------"<<endl<<endl;

}




//---------------
// printModelType
//----------------
/*void printModelType(ostream & flux, const XEMModelName & modelName){

    switch (modelName) {

  // Gaussian models //
  case (Gaussian_p_L_B) :
  case (Gaussian_pk_L_B) :
    flux<<"Gaussian_L_B";break;
  case (Gaussian_p_Lk_B) :
  case (Gaussian_pk_Lk_B) :
    flux<<"Gaussian_Lk_B";      break;
  case (Gaussian_p_L_Bk) :
  case (Gaussian_pk_L_Bk) :
    flux<<"Gaussian_L_Bk";      break;
  case (Gaussian_p_Lk_Bk) :
  case (Gaussian_pk_Lk_Bk) :
    flux<<"Gaussian_Lk_Bk";     break;

  case (Gaussian_p_L_I) :
  case (Gaussian_pk_L_I) :
    flux<<"Gaussian_L_I";break;
  case (Gaussian_p_Lk_I) :
  case (Gaussian_pk_Lk_I) :
    flux<<"Gaussian_Lk_I";break;

  case (Gaussian_p_L_C) :
  case (Gaussian_pk_L_C):
    flux<<"Gaussian_L_C";break;
  case (Gaussian_p_Lk_C) :
  case (Gaussian_pk_Lk_C):
    flux<<"Gaussian_Lk_C";break;
  case (Gaussian_p_L_D_Ak_D) :
  case (Gaussian_pk_L_D_Ak_D):
    flux<<"Gaussian_L_D_Ak_D";break;
  case (Gaussian_p_Lk_D_Ak_D) :
  case (Gaussian_pk_Lk_D_Ak_D):
    flux<<"Gaussian_Lk_D_Ak_D";break;
  case (Gaussian_p_L_Dk_A_Dk) :
  case (Gaussian_pk_L_Dk_A_Dk ):
    flux<<"Gaussian_L_Dk_A_Dk";break;
  case (Gaussian_p_Lk_Dk_A_Dk) :
  case (Gaussian_pk_Lk_Dk_A_Dk):
    flux<<"Gaussian_Lk_Dk_A_Dk";break;
  case (Gaussian_p_L_Ck) :
  case (Gaussian_pk_L_Ck):
    flux<<"Gaussian_L_Ck";break;
  case (Gaussian_p_Lk_Ck) :
  case (Gaussian_pk_Lk_Ck):
    flux<<"Gaussian_Lk_Ck";break;


  // Binary models //
  case (Binary_p_E) :
  case (Binary_pk_E):
    flux<<"Binary_E"; break;
  case (Binary_p_Ek) :
  case (Binary_pk_Ek):
    flux<<"Binary_Ek"; break;
  case (Binary_p_Ej) :
  case (Binary_pk_Ej):
    flux<<"Binary_Ej"; break;
  case (Binary_p_Ekj) :
  case (Binary_pk_Ekj):
    flux<<"Binary_Ekj"; break;
  case (Binary_p_Ekjh) :
  case (Binary_pk_Ekjh):
    flux<<"Binary_Ekjh"; break;

  case (Gaussian_HD_p_AkjBkQkD) :
  case (Gaussian_HD_pk_AkjBkQkD) :
    flux<<"Gaussian_HD_AkjBkQkD"; break;
  case (Gaussian_HD_p_AkjBkQkDk) :
  case (Gaussian_HD_pk_AkjBkQkDk) :
    flux<<"Gaussian_HD_AkjBkQkDk"; break;
  case (Gaussian_HD_p_AkjBQkD) :
  case (Gaussian_HD_pk_AkjBQkD) :
    flux<<"Gaussian_HD_AkjBQkD"; break;
  case (Gaussian_HD_p_AjBkQkD) :
  case (Gaussian_HD_pk_AjBkQkD) :
    flux<<"Gaussian_HD_AjBkQkD"; break;
  case (Gaussian_HD_p_AjBQkD) :
  case (Gaussian_HD_pk_AjBQkD) :
    flux<<"Gaussian_HD_AjBQkD"; break;
  case (Gaussian_HD_p_AkBkQkD) :
  case(Gaussian_HD_pk_AkBkQkD ) :
    flux<<"Gaussian_HD_AkBkQkD"; break;
  case (Gaussian_HD_p_AkBkQkDk) :
  case (Gaussian_HD_pk_AkBkQkDk) :
    flux<<"Gaussian_HD_AkBkQkDk"; break;
  case (Gaussian_HD_p_AkBQkD) :
  case (Gaussian_HD_pk_AkBQkD) :
    flux<<"Gaussian_HD_AkBQkD"; break;

  default :
    flux<<"Model Type Error";
  }


}*/



/*///printAlgoType
void printTypeAlgo(ostream & flux, const XEMAlgoName & typeAlgo){
 if (typeAlgo == EM)
          flux<<"EM"<<endl;
  else if (typeAlgo == CEM)
          flux<<"CEM"<<endl;
  else if (typeAlgo == SEM)
          flux<<"SEM"<<endl;
  else if (typeAlgo == MAP)
          flux<<"MAP"<<endl;
  else if (typeAlgo == M)
          flux<<"M"<<endl;
}*/



//-------------------------------
// is modelName a spherical Model
//-------------------------------
bool isSpherical(XEMModelName modelName) {
    bool res = false;
    if ((modelName == Gaussian_p_L_I)
            || (modelName == Gaussian_p_Lk_I)
            || (modelName == Gaussian_pk_L_I)
            || (modelName == Gaussian_pk_Lk_I)) {
        res = true;
    }
    return res;
}


//------------------------------
// is modelName a diagonal Model
//------------------------------
bool isDiagonal(XEMModelName modelName) {
    bool res = false;
    if (   (modelName == Gaussian_p_L_B)
            || (modelName == Gaussian_p_Lk_B)
            || (modelName == Gaussian_p_L_Bk)
            || (modelName == Gaussian_p_Lk_Bk)
            || (modelName == Gaussian_pk_L_B)
            || (modelName == Gaussian_pk_Lk_B)
            || (modelName == Gaussian_pk_L_Bk)
            || (modelName == Gaussian_pk_Lk_Bk)
       ) {
        res = true;
    }
    return res;
}


//-----------------------------
// is modelName a general Model
//-----------------------------
bool isGeneral(XEMModelName modelName) {
    bool res = false;
    if ((modelName == Gaussian_p_L_C) ||
            (modelName == Gaussian_p_Lk_C) ||
            (modelName == Gaussian_p_L_D_Ak_D) ||
            (modelName == Gaussian_p_Lk_D_Ak_D) ||
            (modelName == Gaussian_p_L_Dk_A_Dk) ||
            (modelName == Gaussian_p_Lk_Dk_A_Dk) ||
            (modelName == Gaussian_p_L_Ck) ||
            (modelName == Gaussian_p_Lk_Ck) ||
            (modelName == Gaussian_pk_L_C) ||
            (modelName == Gaussian_pk_Lk_C) ||
            (modelName == Gaussian_pk_L_D_Ak_D) ||
            (modelName == Gaussian_pk_Lk_D_Ak_D) ||
            (modelName == Gaussian_pk_L_Dk_A_Dk) ||
            (modelName == Gaussian_pk_Lk_Dk_A_Dk) ||
            (modelName == Gaussian_pk_L_Ck) ||
            (modelName == Gaussian_pk_Lk_Ck))
    {
        res =   true;
    }
    return res;
}


//-----------------------------------------
// is modelName a EDDA (Classical Gaussian)
//-----------------------------------------
bool isEDDA(XEMModelName modelName) {
    bool res = false;
    if (isSpherical(modelName) || isDiagonal(modelName) || isGeneral(modelName)) {
        res = true;
    }
    return res;
}

//---------------
// HD (HD or HDk)
//---------------
bool isHD(XEMModelName modelName) {
    bool res = false;
    if (  (modelName == Gaussian_HD_p_AkjBkQkDk)
            ||(modelName == Gaussian_HD_p_AkBkQkDk)
            ||(modelName == Gaussian_HD_p_AkjBkQkD)
            ||(modelName == Gaussian_HD_p_AjBkQkD)
            ||(modelName == Gaussian_HD_p_AkjBQkD)
            ||(modelName == Gaussian_HD_p_AjBQkD)
            ||(modelName == Gaussian_HD_p_AkBkQkD)
            ||(modelName == Gaussian_HD_p_AkBQkD)
            ||(modelName == Gaussian_HD_pk_AkjBkQkDk)
            ||(modelName == Gaussian_HD_pk_AkBkQkDk)
            ||(modelName == Gaussian_HD_pk_AkjBkQkD)
            ||(modelName == Gaussian_HD_pk_AjBkQkD)
            ||(modelName == Gaussian_HD_pk_AkjBQkD)
            ||(modelName == Gaussian_HD_pk_AjBQkD)
            ||(modelName == Gaussian_HD_pk_AkBkQkD)
            ||(modelName == Gaussian_HD_pk_AkBQkD)) {
        res = true;
    }
    return res;
}




bool isFreeSubDimension(XEMModelName modelName) {
    bool res = false;
    if ( (modelName == Gaussian_HD_pk_AkjBkQkDk) ||
            (modelName == Gaussian_HD_p_AkjBkQkDk) ||
            (modelName == Gaussian_HD_pk_AkBkQkDk) ||
            (modelName == Gaussian_HD_p_AkBkQkDk) ) {
        res = true;
    }
    return res;
}




bool isBinary(XEMModelName modelName) {
    bool res = false;
    if ( (modelName == Binary_p_E) ||
            (modelName == Binary_pk_E) ||
            (modelName == Binary_p_Ej) ||
            (modelName == Binary_pk_Ej) ||
            (modelName == Binary_p_Ek) ||
            (modelName == Binary_pk_Ek) ||
            (modelName == Binary_p_Ekj) ||
            (modelName == Binary_pk_Ekj) ||
            (modelName == Binary_p_Ekjh) ||
            (modelName == Binary_pk_Ekjh) ) {
        res = true;
    }
    return res;
}



//---------------------------------
// is modelType has free proportion
//---------------------------------
bool hasFreeProportion(XEMModelName modelName) {
    bool res= true;
    if (		 (modelName == Gaussian_p_L_I)
            || (modelName == Gaussian_p_Lk_I)
            || (modelName == Gaussian_p_L_B)
            || (modelName == Gaussian_p_Lk_B)
            || (modelName == Gaussian_p_L_Bk)
            || (modelName == Gaussian_p_Lk_Bk)
            || (modelName == Gaussian_p_L_C)
            || (modelName == Gaussian_p_Lk_C)
            || (modelName == Gaussian_p_L_D_Ak_D)
            || (modelName == Gaussian_p_Lk_D_Ak_D)
            || (modelName == Gaussian_p_L_Dk_A_Dk)
            || (modelName == Gaussian_p_Lk_Dk_A_Dk)
            || (modelName == Gaussian_p_L_Ck)
            || (modelName == Gaussian_p_Lk_Ck)
            || (modelName == Binary_p_E)
            || (modelName == Binary_p_Ej)
            || (modelName == Binary_p_Ek)
            || (modelName == Binary_p_Ekj)
            || (modelName == Binary_p_Ekjh)
            || (modelName == Gaussian_HD_p_AkjBkQkDk)
            || (modelName == Gaussian_HD_p_AkBkQkDk)
            || (modelName == Gaussian_HD_p_AkjBkQkD)
            || (modelName == Gaussian_HD_p_AjBkQkD)
            || (modelName == Gaussian_HD_p_AkjBQkD)
            || (modelName == Gaussian_HD_p_AjBQkD)
            || (modelName == Gaussian_HD_p_AkBkQkD)
            || (modelName == Gaussian_HD_p_AkBQkD)) {
        res = false;
    }
    return res;
}



//--------------------
// is symmetric matrix
//--------------------
bool isSymmetric(Matrix * mat, int64_t n) {
    bool res = true;
    int64_t i,j;
    for (i=2; i<=n; i++) {
        for (j=1; j<=i-1; j++) {
            if ( (*(mat))(i,j) != (*(mat))(j,i)) {
                res = false;
            }
        }
    }
    return res;
}



//-------------------
// is Matrix Diagonal
//-------------------
bool isDiagonal(Matrix * mat, int64_t n) {
    bool res = true;
    int64_t i,j;
    for (i=1; i<=n; i++) {
        for (j=1; j<=n; j++) {
            if (i != j) {
                if ( (*(mat))(i,j) != 0) {
                    res = false;
                }
            }
        }
    }
    return res;
}





void editSimpleTab(double * tab, int64_t n, string sep, string before, ostream & flux) {
    int64_t i;
    flux << before;
    for (i=0;i<n;i++) {
        flux << tab[i] << sep;
    }
    flux << endl ;
}
void editSimpleTab(int64_t    * tab, int64_t n, ostream & flux ) {
    int64_t i;
    for (i=0;i<n;i++)
        flux << tab[i] << endl ;
}





void editMatrix(GeneralMatrix & mat, int64_t nRows,ostream & flux, string before) {

    int64_t i;
    for (i=1 ; i<=nRows ; i++) {
        flux << before << mat.Row(i);
    }

}

void editSphericalMatrix(double mat, int64_t nRows, ostream & flux, string before) {
    int64_t i;
    int64_t j;
    for (i=0;i<nRows;i++) {
        flux << before << flush;
        for (j=0;j<i;j++) {
            flux << "0.000000 " << flush;
        }
        flux << mat << " "<< flush;
        for (j=i+1;j<nRows;j++) {
            flux << "0.000000 " << flush;
        }
        flux << endl;
    }
}

void editDiagMatrix(double * mat, int64_t nRows, ostream & flux, string before) {
    int64_t i;
    int64_t j;
    for (i=0;i<nRows;i++) {
        flux << before << flush;
        for (j=0;j<i;j++) {
            flux << "0.000000 " << flush;
        }
        flux << mat[i] << " "<< flush;
        for (j=i+1;j<nRows;j++) {
            flux << "0.000000 " << flush;
        }
        flux << endl;
    }

}








//---------------
// Move on file fi until what is reached
// after using that function fi is just after the first time what appears
//---------------

void moveUntilReach(ifstream & fi, string  what) {
    string keyWord = "";
    ConvertBigtoLowString(what);
    // init reading at the beginning of file //
    fi.clear();
    fi.seekg(0, ios::beg);
    // read until finding *what* we are looking for
    do {
        fi >> keyWord ;
        ConvertBigtoLowString(keyWord);
    }

// while( !fi.eof() && strcmp(keyWord,what)!=0 ) ;
    while ( !fi.eof() && (keyWord.compare(what) !=0) ) ;
//  delete[] keyWord;
}




//-------------------
// read nbNbCluster file names (ex : titi ; toto;tutu)

void readTabFileName(ifstream & fi, int64_t nbNbCluster,string* tabFileName, string& keyWord) {
    int64_t k=0;

    string c = "";
    string c1 = "";
    string tmp = "";
    string strBeforePv = "";
    string strAfterPv = "";

    fi>>c;
// on ne converti pas tout le nom en minuscules sinon il y a des erreurs dans le nom des fichiers


    while (!isKeyword(c) && (!fi.eof())) {

        if (c.compare(";")==0) {
            k++;
            fi>>c;
        }
        else {
            if (c.find_first_of(';')==0) { // si c commence par ;
                k++;
                strAfterPv = c.substr(1,c.length());
            }
            else {
                strAfterPv = c;
            }
            while ((strAfterPv.find_first_of(';') != string::npos)) { // ; est dans la chaine de caracteres
                tmp = strAfterPv;
                strBeforePv = tmp.substr(0,tmp.find_first_of(';'));
                strAfterPv = tmp.substr(tmp.find_first_of(';')+1,tmp.length());


                if (tabFileName[k].length() == 0) {
                    tabFileName[k] = strBeforePv;
                }
                else {
                    tabFileName[k].append(" ");
                    tabFileName[k].append(strBeforePv);

                }
                k++;
            }

            if (tabFileName[k].length() == 0) {
                tabFileName[k] = strAfterPv;
            }
            else {
                tabFileName[k].append(" ");
                tabFileName[k].append(strBeforePv);
            }
            fi>>c;
        }
    }

    if ( (k != nbNbCluster-1) || (tabFileName[nbNbCluster-1].compare("")==0) || (tabFileName[nbNbCluster-1].compare(" ")==0))
        throw wrongPartitionFileName;

    keyWord = c;
}




















void initToZero(double* tab, int64_t n) {
    double * p_tab = tab;
    int64_t i;
    for (i=0 ; i<n ; i++,p_tab++) {
        *p_tab = 0.0;
    }
}

void printShortcutModelType(const XEMModelType * const modelType, ostream & flux) {
    switch (modelType->_nameModel) {
    case (Gaussian_p_L_B) :
    case (Gaussian_p_Lk_B) :
    case (Gaussian_p_L_Bk) :
    case (Gaussian_p_Lk_Bk) :
    case (Gaussian_pk_L_B) :
    case (Gaussian_pk_Lk_B) :
    case (Gaussian_pk_L_Bk) :
    case (Gaussian_pk_Lk_Bk) :
        flux << "D" << flush;
        break;

    case (Gaussian_p_L_C):
    case (Gaussian_p_Lk_C):
    case (Gaussian_p_L_D_Ak_D):
    case (Gaussian_p_Lk_D_Ak_D):
    case (Gaussian_p_L_Dk_A_Dk):
    case (Gaussian_p_Lk_Dk_A_Dk):
    case (Gaussian_p_L_Ck):
    case (Gaussian_p_Lk_Ck):
    case (Gaussian_pk_L_C):
    case (Gaussian_pk_Lk_C):
    case (Gaussian_pk_L_D_Ak_D):
    case (Gaussian_pk_Lk_D_Ak_D):
    case (Gaussian_pk_L_Dk_A_Dk):
    case (Gaussian_pk_Lk_Dk_A_Dk):
    case (Gaussian_pk_L_Ck):
    case (Gaussian_pk_Lk_Ck):
        flux << "G" << flush;
        break;

    case (Gaussian_p_L_I) :
    case (Gaussian_p_Lk_I) :
    case (Gaussian_pk_L_I) :
    case (Gaussian_pk_Lk_I) :
        flux << "S" << flush;
        break;

        // Binary models //
    case (Binary_p_E) :
    case (Binary_p_Ek) :
    case (Binary_p_Ej) :
    case (Binary_p_Ekj) :
    case (Binary_p_Ekjh) :
    case (Binary_pk_E) :
    case (Binary_pk_Ek) :
    case (Binary_pk_Ej) :
    case (Binary_pk_Ekj) :
    case (Binary_pk_Ekjh) :
        flux << "B" << flush;
        break;

    case (Gaussian_HD_pk_AkjBkQkD) :
    case (Gaussian_HD_pk_AkjBkQkDk) :
    case (Gaussian_HD_pk_AkjBQkD) :
    case (Gaussian_HD_pk_AjBkQkD) :
    case (Gaussian_HD_pk_AjBQkD) :
    case (Gaussian_HD_pk_AkBkQkD) :
    case (Gaussian_HD_pk_AkBkQkDk) :
    case (Gaussian_HD_pk_AkBQkD) :
    case (Gaussian_HD_p_AkjBkQkD) :
    case (Gaussian_HD_p_AkjBkQkDk) :
    case (Gaussian_HD_p_AkjBQkD) :
    case (Gaussian_HD_p_AjBkQkD) :
    case (Gaussian_HD_p_AjBQkD) :
    case (Gaussian_HD_p_AkBkQkD) :
    case (Gaussian_HD_p_AkBkQkDk) :
    case (Gaussian_HD_p_AkBQkD) :
        flux<< "H" << flush;
        break;
    default :
        throw internalMixmodError;
    }
}

void printModelType(const XEMModelType * const modelType,ostream & flux) {
    switch (modelType->_nameModel) {

        // Gaussian models //
    case (Gaussian_p_L_B) :
        flux<<"p_L_B         ";
        break;
    case (Gaussian_p_Lk_B) :
        flux<<"p_Lk_B        ";
        break;
    case (Gaussian_p_L_Bk) :
        flux<<"p_L_Bk        ";
        break;
    case (Gaussian_p_Lk_Bk) :
        flux<<"p_Lk_Bk       ";
        break;
    case (Gaussian_pk_L_B) :
        flux<<"pk_L_B        ";
        break;
    case (Gaussian_pk_Lk_B) :
        flux<<"pk_Lk_B       ";
        break;
    case (Gaussian_pk_L_Bk) :
        flux<<"pk_L_Bk       ";
        break;
    case (Gaussian_pk_Lk_Bk) :
        flux<<"pk_Lk_Bk      ";
        break;
    case (Gaussian_p_L_C):
        flux<<"p_L_C         ";
        break;
    case (Gaussian_p_Lk_C):
        flux<<"p_Lk_C        ";
        break;
    case (Gaussian_p_L_D_Ak_D):
        flux<<"p_L_D_Ak_D    ";
        break;
    case (Gaussian_p_Lk_D_Ak_D):
        flux<<"p_Lk_D_Ak_D   ";
        break;
    case (Gaussian_p_L_Dk_A_Dk):
        flux<<"p_L_Dk_A_Dk   ";
        break;
    case (Gaussian_p_Lk_Dk_A_Dk):
        flux<<"p_Lk_Dk_A_Dk  ";
        break;
    case (Gaussian_p_L_Ck):
        flux<<"p_L_Ck        ";
        break;
    case (Gaussian_p_Lk_Ck):
        flux<<"p_Lk_Ck       ";
        break;
    case (Gaussian_pk_L_C):
        flux<<"pk_L_C        ";
        break;
    case (Gaussian_pk_Lk_C):
        flux<<"pk_Lk_C       ";
        break;
    case (Gaussian_pk_L_D_Ak_D):
        flux<<"pk_L_D_Ak_D   ";
        break;
    case (Gaussian_pk_Lk_D_Ak_D):
        flux<<"pk_Lk_D_Ak_D  ";
        break;
    case (Gaussian_pk_L_Dk_A_Dk):
        flux<<"pk_L_Dk_A_Dk  ";
        break;
    case (Gaussian_pk_Lk_Dk_A_Dk):
        flux<<"pk_Lk_Dk_A_Dk ";
        break;
    case (Gaussian_pk_L_Ck):
        flux<<"pk_L_Ck       ";
        break;
    case (Gaussian_pk_Lk_Ck):
        flux<<"pk_Lk_Ck      ";
        break;
    case (Gaussian_p_L_I) :
        flux<<"p_L_I         ";
        break;
    case (Gaussian_p_Lk_I) :
        flux<<"p_Lk_I        ";
        break;
    case (Gaussian_pk_L_I) :
        flux<<"pk_L_I        ";
        break;
    case (Gaussian_pk_Lk_I) :
        flux<<"pk_Lk_I       ";
        break;

        // Binary models //
    case (Binary_p_E) :
        flux<<"Binary_p_E    ";
        break;
    case (Binary_p_Ek) :
        flux<<"Binary_p_Ek   ";
        break;
    case (Binary_p_Ej) :
        flux<<"Binary_p_Ej   ";
        break;
    case (Binary_p_Ekj) :
        flux<<"Binary_p_Ekj  ";
        break;
    case (Binary_p_Ekjh) :
        flux<<"Binary_p_Ekjh  ";
        break;
    case (Binary_pk_E) :
        flux<<"Binary_pk_E   ";
        break;
    case (Binary_pk_Ek) :
        flux<<"Binary_pk_Ek  ";
        break;
    case (Binary_pk_Ej) :
        flux<<"Binary_pk_Ej  ";
        break;
    case (Binary_pk_Ekj) :
        flux<<"Binary_pk_Ekj ";
        break;
    case (Binary_pk_Ekjh) :
        flux<<"Binary_pk_Ekjh ";
        break;

//HDDA models
    case (Gaussian_HD_pk_AkjBkQkD) :
        flux<<"HD_pk_AkjBkQkD   ";
        break;
    case (Gaussian_HD_pk_AkjBkQkDk) :
        flux<<"HD_pk_AkjBkQkDk   ";
        break;
    case (Gaussian_HD_pk_AkjBQkD) :
        flux<<"HD_pk_AkjBQkD    ";
        break;
    case (Gaussian_HD_pk_AjBkQkD) :
        flux<<"HD_pk_AjBkQkD    ";
        break;
    case (Gaussian_HD_pk_AjBQkD) :
        flux<<"HD_pk_AjBQkD     ";
        break;
    case (Gaussian_HD_pk_AkBkQkD) :
        flux<<"HD_pk_AkBkQkD    ";
        break;
    case (Gaussian_HD_pk_AkBkQkDk) :
        flux<<"HD_pk_AkBkQkDk    ";
        break;
    case (Gaussian_HD_pk_AkBQkD) :
        flux<<"HD_pk_AkBQkD     ";
        break;
    case (Gaussian_HD_p_AkjBkQkD) :
        flux<<"HD_p_AkjBkQkD    ";
        break;
    case (Gaussian_HD_p_AkjBkQkDk) :
        flux<<"HD_p_AkjBkQkDk     ";
        break;
    case (Gaussian_HD_p_AkjBQkD) :
        flux<<"HD_p_AkjBQkD     ";
        break;
    case (Gaussian_HD_p_AjBkQkD) :
        flux<<"HD_p_AjBkQkD     ";
        break;
    case (Gaussian_HD_p_AjBQkD) :
        flux<<"HD_p_AjBQkD      ";
        break;
    case (Gaussian_HD_p_AkBkQkD) :
        flux<<"HD_p_AkBkQkD     ";
        break;
    case (Gaussian_HD_p_AkBkQkDk) :
        flux<<"HD_p_AkBkQkDk     ";
        break;
    case (Gaussian_HD_p_AkBQkD) :
        flux<<"HD_p_AkBQkD      ";
        break;
    default :
        throw internalMixmodError;
    }
    flux << flush;

}


inline void echange(double * tab, int64_t i1, int64_t i2) {
    double tmp_double = tab[i1];
    tab[i1]           = tab[i2];
    tab[i2]           = tmp_double ;
}
inline void echange(int64_t * tab, int64_t i1, int64_t i2) {

    int64_t tmp   = tab[i1];
    tab[i1]         = tab[i2];
    tab[i2]         = tmp ;
}

void selectionSortWithOrder(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right) {
    int64_t i, j;
    int64_t min;

    for (i = left; i < right; i++) {
        min = i;
        for (j=i+1; j <= right; j++)
            if (tabRandom[j] < tabRandom[min])
                min = j;
        echange(tabRandom, min, i);
        echange(tabOrder, min, i);
    }
}

int64_t partition(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right) {

    double val = tabRandom[left];
    int64_t lm    = left-1;
    int64_t rm    = right+1;
    for (;;) {
        do
            rm--;
        while (tabRandom[rm] > val);

        do
            lm++;
        while ( tabRandom[lm] < val);

        if (lm < rm) {
            echange(tabRandom, rm, lm);
            echange(tabOrder , rm, lm);
        }
        else
            return rm;
    }

}

void quickSortWithOrder(double * tabRandom, int64_t * tabOrder, int64_t left, int64_t right) {
    if (left < (right - SMALL_ENOUGH_TO_USE_SELECTION_SORT)) {
        int64_t split_pt = partition(tabRandom, tabOrder, left, right);
        quickSortWithOrder(tabRandom, tabOrder, left      , split_pt);
        quickSortWithOrder(tabRandom, tabOrder, split_pt+1, right);
    }
    else selectionSortWithOrder(tabRandom, tabOrder, left, right);


}



//-------------------
//generateRandomIndex
//-------------------
int64_t generateRandomIndex(bool * tabIndividualCanBeUsedForInitRandom, double * weight, double totalWeight) {
    double rndWeight, sumWeight;
    int64_t idxSample;

    /* Generate a random integer between 0 and _nbSample-1 */
    bool IdxSampleCanBeUsed = false;  // idxSample can be used
    while (!IdxSampleCanBeUsed) {
        // get index of sample with weight //
        rndWeight = (int64_t)(totalWeight*rnd()+1);
        sumWeight = 0.0;
        idxSample = -1;
        while (sumWeight < rndWeight) {
            idxSample++;
            sumWeight += weight[idxSample];
        }
        //cout<<"index tire au hasard :"<<idxSample<<endl;
        IdxSampleCanBeUsed = tabIndividualCanBeUsedForInitRandom[idxSample];
    }
    // on indique que cet individu ne pourra pas �tre tir� au  hasard pour une autre classe
    tabIndividualCanBeUsedForInitRandom[idxSample] = false;
    //cout<<"choisi"<<endl;
    return idxSample;
}


void inputCriterion(ifstream & fi,XEMCriterionName & criterionName) {

    string a = "";
    fi>>a;
    if (a.compare("BIC")==0) {
        criterionName = BIC;
    }
    else if (a.compare("CV")==0) {
        criterionName = CV;
    }
    else if (a.compare("ICL")==0) {
        criterionName = ICL;
    }
    else if (a.compare("NEC")==0) {
        criterionName = NEC;
    }
    else if (a.compare("DCV")==0) {
        criterionName = DCV;
    }
    else {
        throw wrongCriterionName;
    }
}




void inputCVinitBlocks(ifstream & fi,XEMCVinitBlocks & CVinitBlocks) {
    string a = "";
    fi>>a;
    if (a.compare("CV_RANDOM")==0) {
        CVinitBlocks = CV_RANDOM;
    }
    else if (a.compare("DIAG")==0) {
        CVinitBlocks = CV_DIAG;
    }
    else {
        throw wrongCVinitType;
    }
}

void inputDCVinitBlocks(ifstream & fi,XEMDCVinitBlocks & DCVinitBlocks) {
    string a = "";
    fi>>a;
    if (a.compare("DCV_RANDOM")==0) {
        DCVinitBlocks = DCV_RANDOM;
    }
    else if (a.compare("DIAG")==0) {
        DCVinitBlocks = DCV_DIAG;
    }
    else {
        throw wrongDCVinitType;
    }
}



XEMAlgo * createDefaultAlgo() {
    XEMAlgo * result = NULL;
    if (defaultAlgoName == EM) {
        result = new XEMEMAlgo();
    }
    else {
        throw internalMixmodError;
    }
    return result;
}

