/***************************************************************************
                             SRC/mixmod/Utilities/maths/SelectLibrary.h  description
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

#ifndef mixmod_SelectLibrary_H
#define mixmod_SelectLibrary_H

// Route to interface from any mathematical library to our code.
// Only the following include would change if we try another library.

// Current setting: NEWMAT [seg fault with Eigen, TOFIX]
#ifndef XEMmathLib
#define XEMmathLib 4
#endif

#if XEMmathLib == 0
#include "mixmod/Matrix/Libraries/Armadillo.h"

#elif XEMmathLib == 1
#include "mixmod/Matrix/Libraries/Eigen.h"

#elif XEMmathLib == 2
#include "mixmod/Matrix/Libraries/GSL.h"

#elif XEMmathLib == 3
#include "mixmod/Matrix/Libraries/ITpp.h"

#elif XEMmathLib == 4
#include "mixmod/Utilities/maths/NEWMAT.h"

#endif

#endif
