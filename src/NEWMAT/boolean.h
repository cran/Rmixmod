/***************************************************************************
                             SRC/NEWMAT/boolean.h  description
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
//$$ boolean.h                       bool class

// This is for compilers that do not have bool automatically defined

#ifndef bool_LIB
#define bool_LIB 0

#ifdef use_namespace
namespace RBD_COMMON {
#endif


class bool
{
	int value;
public:
	bool(const int b) { value = b ? 1 : 0; }
	bool(const void* b) { value = b ? 1 : 0; }
	bool() {}
	operator int() const { return value; }
	int operator!() const { return !value; }
};


const bool true = 1;
const bool false = 0;



// version for some older versions of gnu g++
//#define false 0
//#define true 1

#ifdef use_namespace
}
#endif



#endif
