#ifndef XEM_MATH_NEWMAT_H
#define XEM_MATH_NEWMAT_H

#include "newmatap.h"
#include "newmatio.h"

namespace XEM {
namespace MATH {

// TODO: copy constructors ?

class DiagonalMatrix {
	
public:
	
	DiagonalMatrix(int dim) {
		_value = new NEWMAT::DiagonalMatrix(dim);
	}
	
	~DiagonalMatrix() {
		if (_value)
			delete _value;
	}
	
	double* Store() {
		return _value->Store();
	}
	
	int Nrow() {
		return _value->Nrows();
	}
	
private:
	
	NEWMAT::DiagonalMatrix* _value;
};

class Matrix {
	
public:
	
	Matrix(int nrow, int ncol) {
		_value = new NEWMAT::Matrix(nrow, ncol);
	}
	
	~Matrix() {
		if (_value)
			delete _value;
	}

	double* Store() {
		return _value->Store();
	}
	
	double* GetRow(int index) {
		return _value->Store() + index * _value->Ncols();
	}
	
	int Nrow() {
		return _value->Nrows();
	}
	
	int Ncol() {
		return _value->Ncols();
	}
	
private:
	
	NEWMAT::Matrix* _value;
};

class SymmetricMatrix {
	
public:
	
	// nrow == ncol
	SymmetricMatrix(int nrow) {
		_value = new NEWMAT::SymmetricMatrix(nrow);
	}
	
	~SymmetricMatrix() {
		if (_value)
			delete _value;
	}
	
	double* Store() {
		return _value->Store();
	}
	
	// return log(abs(det(M)))
	// NOTE: NEWMAT also return the sign of det(M)
	double LogDeterminant() {
		return NEWMAT::LogDeterminant(*_value).Value();
	}
	
	// get inverse
	SymmetricMatrix* Inverse() {
		int _s_pbDimension = _value->Nrows();
		NEWMAT::SymmetricMatrix* value_Inv = new NEWMAT::SymmetricMatrix(_s_pbDimension);
		*value_Inv << _value->i();
		SymmetricMatrix* invMat = new SymmetricMatrix(_s_pbDimension);
		invMat->_value = value_Inv;
		return invMat;
	}
	
	// compute SVD (only matrices U and D, not V)
	void computeSVD(DiagonalMatrix* D, Matrix* U) {
		NEWMAT::DiagonalMatrix * D_NEWMAT = new NEWMAT::DiagonalMatrix(D->Nrow());
		for (int64_t i=0; i<D->Nrow(); i++) 
			D_NEWMAT->Store()[i] = D->Store()[i];
		NEWMAT::Matrix * U_NEWMAT = new NEWMAT::Matrix(U->Nrow(),U->Ncol());
		for (int64_t i=0; i<U->Nrow()*U->Ncol(); i++) 
			U_NEWMAT->Store()[i] = U->Store()[i];
		NEWMAT::SVD(*_value, *D_NEWMAT, *U_NEWMAT);
		for (int64_t i=0; i<D->Nrow(); i++) 
			D->Store()[i] = D_NEWMAT->Store()[i];
		for (int64_t i=0; i<U->Nrow()*U->Ncol(); i++) 
			U->Store()[i] = U_NEWMAT->Store()[i];
		delete D_NEWMAT;
		delete U_NEWMAT;
	}
	
private:
	
	NEWMAT::SymmetricMatrix* _value;
};

}
}

#endif
