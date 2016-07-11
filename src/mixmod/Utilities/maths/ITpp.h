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

#ifndef mixmod_ITpp_H
#define	mixmod_ITpp_H

#include "mixmod/Utilities/types.h"
#include <itpp/itbase.h>

namespace XEM {
namespace MATH {

class DiagonalMatrix {

public:

	DiagonalMatrix(int dim)
	{
		_value = new itpp::Vec<Real>(dim);
	}

	~DiagonalMatrix()
	{
		delete _value;
	}

	Real* Store()
	{
		return _value->_data();
	}

	Real& operator[] (int i)
	{
		return (*_value)[i];
	}

	int Nrow()
	{
		return _value->size();
	}

	itpp::Vec<Real>* _value;
};

class Matrix {
	
public:
	
	Matrix(int nrow, int ncol)
	{
		_value = new itpp::Mat<Real>(nrow, ncol);
	}
	
	~Matrix()
	{
		delete _value;
	}

	Real* Store()
	{
		return _value->_data();
	}
	
	Real& operator() (int i, int j)
	{
		return (*_value)(i,j);
	}

	Real* GetRow(int index)
	{
		return _value->get_row(index)._data();
	}
	
	int Nrow()
	{
		return _value->rows();
	}
	
	int Ncol()
	{
		return _value->cols();
	}
	
	itpp::Mat<Real>* _value;
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
		_value = new itpp::Mat<Real>(nrow, nrow);
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
		return _value->_data();
	}
	
	Real& operator() (int i, int j)
	{
		return (*_value)(i,j);
	}

	// return log(abs(det(M)))
	Real LogDeterminant()
	{
		itpp::Mat<Real> L(_value->rows(), _value->cols());
		itpp::Mat<Real> U(_value->rows(), _value->cols());
		itpp::ivec p(_value->rows());
		itpp::lu(*_value, L, U, p);
		Real logDet = 0.0;
		for (int i=0; i<_value->rows(); i++)
			logDet += log(fabs(U(i,i)));
		return logDet;
	}
	
	// get inverse
	SymmetricMatrix* Inverse()
	{
		auto valueInv = new itpp::Mat<Real>(_value->rows(), _value->rows());
		itpp::inv(*_value, *valueInv);
		return new SymmetricMatrix(valueInv);
	}
	
	// compute SVD (only matrices U and D, not V)
	void computeSVD(DiagonalMatrix* D, Matrix* U)
	{
		itpp::Mat<Real> V_itpp(U->Ncol(), U->Ncol());
		itpp::svd(*_value, *(U->_value), *(D->_value), V_itpp);
	}
	
private:
	
	itpp::Mat<Real>* _value;
};

}
}

#endif
