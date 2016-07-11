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

#ifndef mixmod_Armadillo_H
#define	mixmod_Armadillo_H

#include "mixmod/Util/types.h"
#include <armadillo>

namespace XEM {
namespace MATH {

class DiagonalMatrix {

public:

	DiagonalMatrix(int dim)
	{
		_value = new arma::Col<Real>(dim);
	}

	~DiagonalMatrix()
	{
		delete _value;
	}

	Real* Store()
	{
		return _value->memptr();
	}

	Real& operator[] (int i)
	{
		return (*_value)[i];
	}

	int Nrow()
	{
		return _value->n_rows;
	}

	arma::Col<Real>* _value;
};

class Matrix {

public:

	Matrix(int nrow, int ncol)
	{
		_value = new arma::Mat<Real>(nrow, ncol);
	}

	~Matrix()
	{
		delete _value;
	}

	Real* Store()
	{
		return _value->memptr();
	}

	Real& operator() (int i, int j)
	{
		return (*_value)(i,j);
	}

	Real* GetRow(int index)
	{
		auto row = _value->row(index);
		// TODO: check correctness
		return row.colptr(0);
	}

	int Nrow()
	{
		return _value->n_rows;
	}

	int Ncol()
	{
		return _value->n_cols;
	}

	arma::Mat<Real>* _value;
};

class SymmetricMatrix {

public:

	// directly assign _value
	SymmetricMatrix(arma::Mat<Real>* value)
	{
		_value = value;
	}

	// nrow == ncol
	SymmetricMatrix(int nrow)
	{
		_value = new arma::Mat<Real>(nrow, nrow);
	}

	// build from plain store (store nrow x nrow values)
	SymmetricMatrix(Real** plainStore, int nrow)
	: SymmetricMatrix(nrow)
	{
		for (int i=0; i<nrow; i++) {
			for (int j=0; j<nrow; j++)
				(*_value)(i,j) = plainStore[i][j];
		}
	}

	~SymmetricMatrix()
	{
		delete _value;
	}

	Real* Store()
	{
		return _value->memptr();
	}

	Real& operator() (int i, int j)
	{
		return (*_value)(i,j);
	}

	// return log(abs(det(M)))
	Real LogDeterminant()
	{
		arma::Mat<Real> L;
		arma::Mat<Real> U;
		arma::Mat<Real> P;
		arma::lu(L, U, P, *_value);
		Real logDet = 0.0;
		for (int i=0; i<_value->n_rows; i++)
			logDet += log(fabs(U(i,i)));
		return logDet;
	}

	// get inverse
	SymmetricMatrix* Inverse()
	{
		auto valueInv = new arma::Mat<Real>(_value->n_rows, _value->n_rows);
		arma::inv(*valueInv, *_value);
		return new SymmetricMatrix(valueInv);
	}

	// compute SVD (only matrices U and D, not V)
	void computeSVD(DiagonalMatrix* D, Matrix* U)
	{
		arma::Mat<Real> V;
		//TODO: tune 500
		if (_value->n_rows < 500) {
			// "small" matrix, standard method
			svd(*(U->_value), *(D->_value), V,*_value, "standard");
		}
		else {
			// (very) "large" matrix, divide & conquer method
			svd(*(U->_value), *(D->_value), V, *_value, "dc");
		}
	}

private:

	arma::Mat<Real>* _value;
};

}
}

#endif
