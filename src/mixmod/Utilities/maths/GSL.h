//****************************************************************************************|
//	file      : mixmod / src/mixmod/Utilities/Maths/Eigen.h
//	copyright : (C) MIXMOD Team - 2001-2013
//	email     : contact@mixmod.org
//========================================================================================
//	This file is part of MIXMOD (see <http://www.mixmod.org>)
//
//	MIXMOD is free software: you can redistribute it and/or modify it under the terms of
//	the GNU General Public License as published by the Free Software Foundation,
//	either version 3 of the License, or (at your option) any later version.
//
//	MIXMOD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//	See the GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License along with MIXMOD.
//	If not, see <http://www.gnu.org/licenses/>.
//****************************************************************************************|

#ifndef mixmod_GSL_H
#define	mixmod_GSL_H

#include "mixmod/Util/types.h"
#include <gsl/gsl_linalg.h>

namespace XEM {
namespace MATH {

class DiagonalMatrix {

public:

	DiagonalMatrix(int dim)
	{
		_value = gsl_vector_alloc(dim);
	}

	~DiagonalMatrix()
	{
		gsl_vector_free(_value);
	}

	Real* Store()
	{
		return _value->data;
	}

	Real& operator[] (int i)
	{
		return _value->data[i];
		//return gsl_vector_get(_value, i); //Does not return a reference
	}

	int Nrow()
	{
		return _value->size;
	}

	gsl_vector* _value;
};

class Matrix {
	
public:
	
	Matrix(int nrow, int ncol)
	{
		_value = gsl_matrix_alloc(nrow, ncol);
	}
	
	~Matrix()
	{
		gsl_matrix_free(_value);
	}

	Real* Store()
	{
		return _value->data;
	}
	
	Real& operator() (int i, int j)
	{
		return _value->data[i*(_value->size2)+j];
		//return gsl_matrix_get(_value, i, j); //Does not return a reference
	}

	Real* GetRow(int index)
	{
		return gsl_matrix_row(_value, index).vector.data;
	}
	
	int Nrow()
	{
		return _value->size1;
	}
	
	int Ncol()
	{
		return _value->size2;
	}
	
	gsl_matrix* _value;
};

class SymmetricMatrix {
	
public:
	
	// directly assign _value
	SymmetricMatrix(itpp::Mat<Real>* value)
	{
		_value = value;
	}

	// nrow == ncol
	SymmetricMatrix(int nrow)
	{
		_value = gsl_matrix_alloc(nrow, nrow);
	}
	
	// build from plain store (store nrow x nrow values)
	SymmetricMatrix(Real** plainStore, int nrow)
	: SymmetricMatrix(nrow)
	{
		for (int i=0; i<nrow; i++) {
			for (int j=0; j<nrow; j++)
				gsl_matrix_set(_value,i,j,plainStore[i][j]);
		}
	}

	~SymmetricMatrix()
	{
		gsl_matrix_free(_value);
	}
	
	Real* Store()
	{
		return _value->data;
	}
	
	Real& operator() (int i, int j)
	{
		return _value->data[i*(_value->size2)+j];
		//return gsl_matrix_get(_value, i, j); //Does not return a reference
	}

	// return log(abs(det(M)))
	Real LogDeterminant()
	{
		int nrow = _value->size1;
		gsl_permutation* permutation = gsl_permutation_alloc(nrow);
		int signum;
		gsl_matrix* copyValue = gsl_matrix_alloc_from_matrix(_value, 0, 0, nrow, nrow);
		gsl_linalg_LU_decomp(copyValue, permutation, &signum);
		Real logDet = gsl_linalg_LU_lndet(copyValue);
		gsl_permutation_free(permutation);
		gsl_matrix_free(copyValue);
		return logDet;
	}
	
	// get inverse
	SymmetricMatrix* Inverse()
	{
		int nrow = _value->size1;
		gsl_permutation* permutation = gsl_permutation_alloc(nrow);
		int signum;
		gsl_matrix* LUdecomp = gsl_matrix_alloc_from_matrix(_value, 0, 0, nrow, nrow);
		gsl_linalg_LU_decomp(LUdecomp, permutation, &signum);
		auto inverse = new SymmetricMatrix(nrow);
		gsl_linalg_LU_invert(LUdecomp, permutation, inverse->_value);
		gsl_permutation_free(permutation);
		gsl_matrix_free(LUdecomp);
		return inverse;
	}
	
	// compute SVD (only matrices U and D, not V)
	void computeSVD(DiagonalMatrix* D, Matrix* U)
	{
		int nrow = _value->size1;
		// copy current matrix into U
		for (int i=0; i<nrow; i++) {
			for (int j=0; j<nrow; j++)
				gsl_matrix_set(U->_value,i,j,gsl_matrix_get(_value,i,j));
		}
		gsl_matrix* V = gsl_matrix_alloc(nrow, nrow);
		gsl_vector* work = gsl_vector_alloc(nrow);
		gsl_linalg_SV_decomp (U->_value, V, D->_value, work);
		gsl_matrix_free(V);
		gsl_vector_free(work);
	}
	
private:
	
	gsl_matrix* _value;
};

}
}

#endif
