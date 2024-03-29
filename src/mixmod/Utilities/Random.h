/***************************************************************************
                             SRC/mixmod/Utilities/Random.h  description
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
/** @file Random.h
 *  @brief Random numbers generator.
 **/

#ifndef XEM_RANDOM_H
#define XEM_RANDOM_H

#include <stdint.h>

namespace XEM
{

extern double rnd();
extern int64_t flip(double x);

extern void initRandomize(int seed);

extern void randomize();

extern void antiRandomize(int seed);

}

#endif
