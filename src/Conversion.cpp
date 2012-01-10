/*
 * Conversion.cpp
 *
 *  Created on: 8 juin 2011
 *      Author: aude
 */

#include "Conversion.h"

//#include <sstream>
//#include <iostream>

namespace Conversion
{


//To spin off double** in Rcpp::NumericMatrix
Rcpp::NumericMatrix CMatrixToRcppMatrix(int64_t nbSample, int64_t pbDimension, double** matrix)
{
  //NumericMatrix for matrix
  Rcpp::NumericMatrix matrixOutput(nbSample,pbDimension);
  for(int i=0;i<nbSample;i++)
  {
    for(int j=0;j<pbDimension;j++)
    {
      matrixOutput(i,j) = matrix[i][j];
    }
  }
  return matrixOutput;
}
  
//To spin off double*** in Rcpp::NumericVector
Rcpp::GenericVector CMatrixToVectorOfRcppMatrix(int64_t nbCluster, int64_t pdDimension, int64_t* tabNbModality, double*** matrix)
{
  //VectorMatrix of NumericMatrix
  Rcpp::GenericVector vectorOutput(nbCluster);
  // get maximum number of modality
  int64_t max = *std::max_element(tabNbModality,tabNbModality+pdDimension);
  for (int k=0; k<nbCluster; k++) {
    //NumericMatrix for matrix
    Rcpp::NumericMatrix matrixOutput(pdDimension,max);
    for(int j=0; j<pdDimension; j++){
      for (int h=0; h<tabNbModality[j]; h++) {
        matrixOutput(j,h) = matrix[k][j][h];
      }
      vectorOutput(k) = matrixOutput;
    }
  }
  return vectorOutput;
}

//To spin off vector of vector in Rcpp::NumericMatrix
Rcpp::NumericMatrix XEMMatrixToRcppMatrix(vector<vector<double> > const& source)
{
  int64_t nbSample = source.size();
  int64_t pbDimension = source[0].size();

  //NumericMatrix for matrix
  Rcpp::NumericMatrix matrixOutput(nbSample,pbDimension);
  for (int64_t i=0; i<nbSample; i++)
  {
    for (int64_t j=0; j<pbDimension; j++)
    {
      matrixOutput(i,j) = source[i][j];
    }
  }
 return matrixOutput;
}

//To spin off int** in Rcpp::NumericMatrix
Rcpp::NumericMatrix CMatrixToRcppMatrixForInt(int64_t nbSample, int64_t pbDimension, int64_t** matrix)
{
    //NumericMatrix for matrix
    Rcpp::NumericMatrix matrixOutput(nbSample,pbDimension);
    for(int i=0;i<nbSample;i++)
    {
      for(int j=0;j<pbDimension;j++)
      {
        matrixOutput(i,j) = matrix[i][j];
      }
    }
    return matrixOutput;
}

//To spin off double* in Rcpp::NumericVector
Rcpp::NumericVector CVectorToRcppVector(int64_t dim, double* vector)
{
  // NumericVector for vector
  Rcpp::NumericVector vectorOutput(dim);
  for(int i=0;i<dim;i++)
  {
      vectorOutput(i) = vector[i];
  }
  return vectorOutput;
}

//To spin off int* in Rcpp::NumericVector
Rcpp::NumericVector CVectorToRcppVectorForInt(int64_t dim, int64_t* vector)
{
  // NumericVector for vector
  Rcpp::NumericVector vectorOutput(dim);
  for(int i=0;i<dim;i++)
  {
      vectorOutput(i) = vector[i];
  }
  return vectorOutput;
}

//To spin off int* in Rcpp::NumericVector
Rcpp::NumericVector VectorToRcppVectorForInt( std::vector<int64_t> const & data)
{
  // get dimension
  std::vector<int64_t>::size_type dim = data.size();
  //
  Rcpp::NumericVector vectorOutput(dim);
  for(int i=0;i<dim;i++)
  {
    vectorOutput[i] = (double)data[i];
  }
  return vectorOutput;
}

/*To spin off information for partition */
  Rcpp::NumericMatrix LabelToPartition( int64_t nbCluster, std::vector<int64_t> const & labels)
{
  int nbSample = labels.size() ;

  // Matrix for partition
  Rcpp::NumericMatrix partitionOutput(nbSample,(int)nbCluster);
  for(int i=0;i<nbSample;i++)
  {
    int ind = labels[i]-1;
    for(int j=0;j<nbCluster;j++)
    {
      (j == ind) ? partitionOutput(i,j) = 1 : partitionOutput(i,j) = 0;
    }
  }
  return partitionOutput;
}

/*
 *  convert a gaussian dataset to a Xem gaussian data set
 * @param data the input data set
 * @return a pointer on a XEMGaussianData set
 */
XEMGaussianData * DataToXemGaussianData(Rcpp::NumericMatrix& data)
{
  // wrap int variables to int64_t variables
  int64_t nbSample64 = data.nrow(), nbVariable64 = data.ncol();

  double ** matrix = new double * [nbSample64];
  for (int i=0; i<nbSample64; i++)
  {
   matrix[i] = new double[nbVariable64];
   for (int j=0; j<nbVariable64; j++)
     matrix[i][j] = data(i,j);
  }
  // create XEMGaussianData
  XEMGaussianData*  gData = new XEMGaussianData(nbSample64, nbVariable64, matrix);

  // release memory
  for (int64_t i=0; i<nbSample64; i++)
  { delete [] matrix[i];}
  delete [] matrix;
  matrix = 0;

  return gData;
}

/*
 *  convert a binary dataset to a Xem binary data set
 * @param data the input data set
 * @return a pointer on a XEMBinaryData set
 */
XEMBinaryData * DataToXemBinaryData(Rcpp::NumericMatrix& data)
{
  // wrap int variables to int64_t variables
  int64_t nbSample64 = data.nrow(), nbVariable64 = data.ncol();

  int64_t ** matrix = new int64_t * [nbSample64];
  for (int i=0; i<nbSample64; i++)
  {
   matrix[i] = new int64_t[nbVariable64];
   for (int j=0; j<nbVariable64; j++)
   {
     matrix[i][j] = (int64_t)data(i,j);
   }
  }

  // compute the modalities of the data
  vector<int64_t> nbModality;
  for(int j=0;j<nbVariable64;j++)
  {
    // vector with all the modalities
    vector<int64_t> modality;
    // add modality of the individual 0
    modality.push_back(matrix[0][j]);
    for(int i=1;i<nbSample64;i++)
    {
      vector<int64_t>::iterator it = modality.begin();
      bool find = false;
      int64_t val= matrix[i][j];
      for ( ;it != modality.end(); ++it)
      {
        if (val == *it) { find = true; break;}
      }
      if (!find)
      {
        modality.push_back(val);
      }
    }
    // add number of modality of the variable j
    nbModality.push_back(modality.size());
  }

  // create XEMBinaryData
  XEMBinaryData*  bData = new XEMBinaryData(nbSample64, nbVariable64, nbModality, matrix);

  // release memory
  for (int64_t i=0; i<nbSample64; i++)
  { delete [] matrix[i];}
  delete [] matrix;
  matrix = 0;

  return bData;
}


}


















