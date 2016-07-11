/***************************************************************************
                             SRC/mixmod/Utilities/maths/Eigen.h  description
    copyright            : (C) MIXMOD Team - 2001-2016
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
#ifndef XEM_MATH_EIGEN_H
#define XEM_MATH_EIGEN_H

#include <Eigen/Dense>

namespace XEM {
namespace MATH {

// TODO: copy constructors ?
// TODO: fix this, especially inverse() and SVD() [segmentation fault on NR tests]

class DiagonalMatrix {
	
public:
	
	DiagonalMatrix(int dim) {
		_dim = dim;
		_value = new double[dim];
	}
	
	~DiagonalMatrix() {
		if (_value)
			delete [] _value;
	}
	
	double* Store() {
		return _value;
	}
	
	int Nrow() {
		return _dim;
	}
	
private:
	
	int64_t _dim;
	double* _value;
};

class Matrix {
	
public:
	
	Matrix(int nrow, int ncol) {
		_value = new Eigen::MatrixXd(nrow, ncol);
	}
	
	~Matrix() {
		if (_value)
			delete _value;
	}

	double* Store() {
		return _value->data();
	}
	
	double* GetRow(int index) {
		return _value->data() + index * _value->cols();
	}
	
	int Nrow() {
		return _value->rows();
	}
	
	int Ncol() {
		return _value->cols();
	}
	
private:
	
	Eigen::MatrixXd* _value;
};

class SymmetricMatrix {
	
public:
	
	// nrow == ncol
	SymmetricMatrix(int nrow) {
		_value = new Eigen::MatrixXd(nrow, nrow);
	}
	
	~SymmetricMatrix() {
		if (_value)
			delete _value;
	}
	
	double* Store() {
		return _value->data();
	}
	
	// return log(abs(det(M)))
	double LogDeterminant() {
		return log(_value->determinant());
	}
	
	// get inverse
	SymmetricMatrix* Inverse() {
		int _s_pbDimension = _value->rows();
		SymmetricMatrix* invMat = new SymmetricMatrix(_s_pbDimension);
		auto eigenInverse = (Eigen::MatrixXd)(_value->inverse());
		for (int64_t i=0; i<eigenInverse.rows()*eigenInverse.cols(); i++)
			invMat->Store()[i] = eigenInverse.data()[i];
		return invMat;
	}
	
	// compute SVD (only matrices U and D, not V)
	void computeSVD(DiagonalMatrix* D, Matrix* U) {
		// TODO: Eigen::ComputeThinU ?
		auto svd = new Eigen::JacobiSVD<Eigen::MatrixXd>(*_value, Eigen::ComputeFullU);
		auto eigenD = svd->singularValues();
		auto eigenU = svd->matrixU();
		for (int64_t i=0; i<eigenD.rows(); i++) 
			D->Store()[i] = eigenD.data()[i];
		for (int64_t i=0; i<eigenU.rows()*eigenU.cols(); i++)
			U->Store()[i] = eigenU.data()[i];
		delete svd;
	}
	
private:
	
	Eigen::MatrixXd* _value;
};

}
}

#endif
