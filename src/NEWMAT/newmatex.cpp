/***************************************************************************
                             SRC/NEWMAT/newmatex.cpp  description
                             ------------------------
    copyright            : (C) MIXMOD Team - 2001-2003-2004-2005-2006-2007-2008-2009
    email                : mixmod@univ-fcomte.fr
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
                                                                          
 ***************************************************************************/
//$$ newmatex.cpp                    Exception handler

// Copyright (C) 1992,3,4,7: R B Davies

#define WANT_STREAM                  // include.h will get stream fns

#include "include.h"                 // include standard files
#include "newmat.h"

#ifdef use_namespace
namespace NEWMAT {
#endif

unsigned long OverflowException::Select;
unsigned long SingularException::Select;
unsigned long NPDException::Select;
unsigned long ConvergenceException::Select;
unsigned long ProgramException::Select;
unsigned long IndexException::Select;
unsigned long VectorException::Select;
unsigned long NotSquareException::Select;
unsigned long SubMatrixDimensionException::Select;
unsigned long IncompatibleDimensionsException::Select;
unsigned long NotDefinedException::Select;
unsigned long CannotBuildException::Select;
unsigned long InternalException::Select;



static void MatrixDetails(const GeneralMatrix& A)
// write matrix details to Exception buffer
{
   MatrixBandWidth bw = A.BandWidth(); int ubw = bw.upper; int lbw = bw.lower;
   Exception::AddMessage("MatrixType = ");
   Exception::AddMessage(A.Type().Value());
   Exception::AddMessage("  # Rows = "); Exception::AddInt(A.Nrows());
   Exception::AddMessage("; # Cols = "); Exception::AddInt(A.Ncols());
   if (lbw >=0)
      { Exception::AddMessage("; lower BW = "); Exception::AddInt(lbw); }
   if (ubw >=0)
      { Exception::AddMessage("; upper BW = "); Exception::AddInt(ubw); }
   Exception::AddMessage("\n");
}

NPDException::NPDException(const GeneralMatrix& A)
   : Runtime_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: matrix not positive definite\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

SingularException::SingularException(const GeneralMatrix& A)
   : Runtime_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: matrix is singular\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

ConvergenceException::ConvergenceException(const GeneralMatrix& A)
   : Runtime_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: process fails to converge\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

ConvergenceException::ConvergenceException(const char* c) : Runtime_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\n\n");
   if (c) Tracer::AddTrace();
}

OverflowException::OverflowException(const char* c) : Runtime_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\n\n");
   if (c) Tracer::AddTrace();
}

ProgramException::ProgramException(const char* c) : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\n\n");
   if (c) Tracer::AddTrace();
}

ProgramException::ProgramException(const char* c, const GeneralMatrix& A)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\n\n");
   MatrixDetails(A);
   if (c) Tracer::AddTrace();
}

ProgramException::ProgramException(const char* c, const GeneralMatrix& A,
   const GeneralMatrix& B) : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\n\n");
   MatrixDetails(A); MatrixDetails(B);
   if (c) Tracer::AddTrace();
}

ProgramException::ProgramException(const char* c, MatrixType a, MatrixType b)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(c); AddMessage("\nMatrixTypes = ");
   AddMessage(a.Value()); AddMessage("; ");
   AddMessage(b.Value()); AddMessage("\n\n");
   if (c) Tracer::AddTrace();
}

VectorException::VectorException() : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: cannot convert matrix to vector\n\n");
   Tracer::AddTrace();
}

VectorException::VectorException(const GeneralMatrix& A)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: cannot convert matrix to vector\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

NotSquareException::NotSquareException(const GeneralMatrix& A)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: matrix is not square\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

SubMatrixDimensionException::SubMatrixDimensionException()
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: incompatible submatrix dimension\n\n");
   Tracer::AddTrace();
}

IncompatibleDimensionsException::IncompatibleDimensionsException()
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: incompatible dimensions\n\n");
   Tracer::AddTrace();
}

IncompatibleDimensionsException::IncompatibleDimensionsException
   (const GeneralMatrix& A, const GeneralMatrix& B)
      : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: incompatible dimensions\n\n");
   MatrixDetails(A); MatrixDetails(B);
   Tracer::AddTrace();
}

NotDefinedException::NotDefinedException(const char* op, const char* matrix)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: ");
   AddMessage(op);
   AddMessage(" not defined for ");
   AddMessage(matrix);
   AddMessage("\n\n");
   Tracer::AddTrace();
}

CannotBuildException::CannotBuildException(const char* matrix)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: cannot build matrix type ");
   AddMessage(matrix); AddMessage("\n\n");
   Tracer::AddTrace();
}

IndexException::IndexException(int i, const GeneralMatrix& A)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: index error: requested index = ");
   AddInt(i); AddMessage("\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

IndexException::IndexException(int i, int j, const GeneralMatrix& A)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: index error: requested indices = ");
   AddInt(i); AddMessage(", "); AddInt(j);
   AddMessage("\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}


IndexException::IndexException(int i, const GeneralMatrix& A, bool)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage("detected by Newmat: element error: requested index (wrt 0) = ");
   AddInt(i);
   AddMessage("\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

IndexException::IndexException(int i, int j, const GeneralMatrix& A, bool)
   : Logic_error()
{
   Select = Exception::Select;
   AddMessage(
      "detected by Newmat: element error: requested indices (wrt 0) = ");
   AddInt(i); AddMessage(", "); AddInt(j);
   AddMessage("\n\n");
   MatrixDetails(A);
   Tracer::AddTrace();
}

InternalException::InternalException(const char* c) : Logic_error()
{
   Select = Exception::Select;
   AddMessage("internal error detected by Newmat: please inform author\n");
   AddMessage(c); AddMessage("\n\n");
   Tracer::AddTrace();
}




/************************* ExeCounter functions *****************************/

#ifdef DO_REPORT

int ExeCounter::nreports;                      // will be set to zero

ExeCounter::ExeCounter(int xl, int xf) : line(xl), fileid(xf), nexe(0) {}

ExeCounter::~ExeCounter()
{
   nreports++;
#ifdef VERBOSE
   cout << "REPORT  " << setw(6) << nreports << "  "
      << setw(6) << fileid << "  " << setw(6) << line
      << "  " << setw(6) << nexe << "\n";
#endif
}

#endif

/**************************** error handler *******************************/

void MatrixErrorNoSpace(void* v) { if (!v) Throw(Bad_alloc()); }
// throw exception if v is null




/************************* miscellanous errors ***************************/


void CroutMatrix::GetRow(MatrixRowCol&)
   { Throw(NotDefinedException("GetRow","Crout")); }
void CroutMatrix::GetCol(MatrixRowCol&)
   { Throw(NotDefinedException("GetCol","Crout")); }
void CroutMatrix::operator=(const BaseMatrix&)
   { Throw(NotDefinedException("=","Crout")); }
void BandLUMatrix::GetRow(MatrixRowCol&)
   { Throw(NotDefinedException("GetRow","BandLUMatrix")); }
void BandLUMatrix::GetCol(MatrixRowCol&)
   { Throw(NotDefinedException("GetCol","BandLUMatrix")); }
void BandLUMatrix::operator=(const BaseMatrix&)
   { Throw(NotDefinedException("=","BandLUMatrix")); }
void BaseMatrix::IEQND() const
   { Throw(NotDefinedException("inequalities", "matrices")); }
#ifdef TEMPS_DESTROYED_QUICKLY_R
   ReturnMatrixX::ReturnMatrixX(const ReturnMatrixX& tm)
     : gm(tm.gm) { Throw(ProgramException("ReturnMatrixX error")); }
#endif


#ifdef use_namespace
}
#endif

