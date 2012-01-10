/***************************************************************************
                             SRC/NEWMAT/newmatio.h  description
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
//$$ newmatio.h           definition file for matrix package input/output

// Copyright (C) 1991,2,3,4: R B Davies

#ifndef NEWMATIO_LIB
#define NEWMATIO_LIB 0

#ifndef WANT_STREAM
#define WANT_STREAM
#endif

#include "newmat.h"

#ifdef use_namespace
namespace NEWMAT {
#endif



/**************************** input/output *****************************/

ostream& operator<<(ostream&, const BaseMatrix&);

ostream& operator<<(ostream&, const GeneralMatrix&);


/*  Use in some old versions of G++ without complete iomanipulators

class Omanip_precision
{
   int x;
public:
   Omanip_precision(int i) : x(i) {}
   friend ostream& operator<<(ostream& os, Omanip_precision i);
};


Omanip_precision setprecision(int i);

class Omanip_width
{
   int x;
public:
   Omanip_width(int i) : x(i) {}
   friend ostream& operator<<(ostream& os, Omanip_width i);
};

Omanip_width setw(int i);

*/

#ifdef use_namespace
}
#endif



#endif

// body file: newmat9.cpp

